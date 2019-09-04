#include "bsp_task.h"
#include "bsp_esp8266.h"
#include <stdio.h>  
#include <stdlib.h>
#include <string.h>  
#include <stdbool.h>
#include "stdint.h"
#include "bsp_Agreement.h"
#include "bsp_i2c_ee.h"
#include "includes.h"
#include "bsp_led.h"
#include "crc16.h"
#include "bsp_485.h"


//////将eeprom进行分块
//#define EEP_IP_Adder    0x00     //IP地址
//#define EEP_Port_Adder    0x20     //端口地址
//#define EEP_ApSsid_Adder    0x28     //WIFI名称地址
//#define EEP_ApPwd_Adder   0x38    //WIFI密码地址

extern volatile uint8_t ucTcpClosedFlag;
extern uint8_t wifi_link_Status;
extern uint8_t tcp_link_Status;
uint8_t U = 0; //电量百分比定义
uint8_t U_flag = 0;//电量是否计算flag定义

//char   macUser_ESP8266_ApSsid_eeprom[32]=  "";                     //要连接的热点的名称
//char   macUser_ESP8266_ApPwd_eeprom [16] = "";                     //要连接的热点的密钥
//char   macUser_ESP8266_TcpServer_IP_eeprom[16]= "";                 //要连接的服务器的 IP
//char   macUser_ESP8266_TcpServer_Port_eeprom[8]= "";                //要连接的服务器的端口



/**
  * @brief  ESP8266 （Sta Tcp Client）透传
  * @param  无
  * @retval 无
  */
void Link_start( void )
{
	uint8_t ucStatus;
	OS_ERR     err;
	char *pRecStr = NULL;
	char cStr[25];
	
//	//读取eeprom参数
//	Read_eeprom(macUser_ESP8266_TcpServer_IP_eeprom,EEP_IP_Adder);//读出IP,端口及其密码
//		OSTimeDly ( 5000, OS_OPT_TIME_DLY, & err );//延时2s
//	Read_eeprom(macUser_ESP8266_TcpServer_Port_eeprom,EEP_Port_Adder);
//		OSTimeDly ( 5000, OS_OPT_TIME_DLY, & err );//延时2s
//	Read_eeprom( macUser_ESP8266_ApSsid_eeprom,EEP_ApSsid_Adder);
//		OSTimeDly ( 5000, OS_OPT_TIME_DLY, & err );//延时2s
//	Read_eeprom( macUser_ESP8266_ApPwd_eeprom,EEP_ApPwd_Adder);
//		OSTimeDly ( 5000, OS_OPT_TIME_DLY, & err );//延时2s
	
	printf ( "\r\n正在配置 ESP8266 ......\r\n" );

	macESP8266_CH_ENABLE();
	
	ESP8266_Net_Mode_Choose ( STA );
	printf("已开启STA模式....\n");
	
	while (!ESP8266_JoinAP(macUser_ESP8266_ApSsid, macUser_ESP8266_ApPwd))
	{
		printf("连接wifi失败,正在重试...\n");
		OSTimeDly ( 2000, OS_OPT_TIME_DLY, & err );//延时2s
	}
	wifi_link_Status = 1;
	macLED1_OFF();
	macLED3_OFF();
	macLED2_ON();		

	
	printf("连接wifi成功....\n");
	ESP8266_Enable_MultipleId ( DISABLE );
	
	while ( !	ESP8266_Link_Server ( enumTCP, macUser_ESP8266_TcpServer_IP, macUser_ESP8266_TcpServer_Port, Single_ID_0 ) );
	tcp_link_Status = 1;
	
	
	
	while ( ! ESP8266_UnvarnishSend () );	
	
	printf("连接服务器成功....\n");
	
	strEsp8266_Fram_Record .InfBit .FramLength = 0;
	strEsp8266_Fram_Record .InfBit .FramFinishFlag = 0;
	
	macLED1_OFF();
	macLED2_OFF();
	macLED3_ON();		

	sprintf(cStr,"010A00");
	ESP8266_SendString(ENABLE, cStr,0 , Single_ID_0);
		
	while (1)
	{	
		if (strEsp8266_Fram_Record .InfBit .FramFinishFlag)
		{
			printf("接受到服务器指令...\n");
			strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ] = '\0';
			pRecStr = strEsp8266_Fram_Record .Data_RX_BUF;
			
			strEsp8266_Fram_Record .InfBit .FramLength = 0;
			strEsp8266_Fram_Record .InfBit .FramFinishFlag = 0;
			
			tcp_Agreement(pRecStr);
		}
		else
		{		
			OSTimeDly ( 20, OS_OPT_TIME_DLY, & err );//延时20ms,ucos任务调度
		}
		
		if ( ucTcpClosedFlag )                                             //检测是否失去连接
		{
			tcp_link_Status = 0;
			ESP8266_ExitUnvarnishSend ();                                    //退出透传模式
			
			do ucStatus = ESP8266_Get_LinkStatus ();                         //获取连接状态
			while ( ! ucStatus );
			
			if ( ucStatus == 4 )                                             //确认失去连接后重连
			{
				macLED1_OFF();
				macLED2_OFF();
				macLED1_ON();					
				printf ( "\r\n正在重连热点和服务器 ......\r\n" );
				
				while ( ! ESP8266_JoinAP ( macUser_ESP8266_ApSsid, macUser_ESP8266_ApPwd ) );
				macLED1_OFF();
				macLED3_OFF();
				macLED2_ON();
				
				while ( !	ESP8266_Link_Server ( enumTCP, macUser_ESP8266_TcpServer_IP, macUser_ESP8266_TcpServer_Port, Single_ID_0 ) );
				
				tcp_link_Status = 1;
				macLED1_OFF();	
				macLED2_OFF();
				macLED3_ON();	
				
				printf ( "\r\n重连热点和服务器成功\r\n" );
      
			}
			  
			while ( ! ESP8266_UnvarnishSend () );	
			
			printf ( "\r\n配置 ESP8266 完毕\r\n" );			
		}
		
	}

}		


void real_time_data(void)
{
	OS_ERR     err;
	char  Data_str[200];//上传实时数据字符串
	float Temperature = 0;//温度
	extern __IO uint16_t ADC_ConvertedValue;//电压原始数据
	extern uint8_t wifi_link_Status;//wifi连接状态
	extern uint8_t tcp_link_Status;//tcp连接状态
	extern uint8_t flag,flag1;//巡检类型状态
	extern float ADC_ConvertedValueLocal;//浮点电压数据 
	extern float vi;//电压量
	extern uint8_t cw;
	extern int8_t j; 
	extern uint32_t S;//行走距离
	extern uint8_t Motor_Speed_Now;//实时速度
	uint8_t count1 =0; 
	
	
	while (1) 
	{ 
		if(wifi_link_Status)//检测wifi是否连接
			{
				while(1)
				{
					
				if (tcp_link_Status == 0)//检测tcp是否连接
					{
						printf("TCP未连接...\n");
						OSTimeDly ( 2000, OS_OPT_TIME_DLY, & err );    //TCP未连接延时2s
					}
				else
				{	
					count1++;
					OSTimeDly ( 2000, OS_OPT_TIME_DLY, & err );  //1秒发送一次距离数据
					sprintf(Data_str,"%s%s%s%s,%6d","01","0D","02","1S",S);
					//10秒发送一次实时数据
					if(count1 == 5)
					{
						count1 = 0;
						Temperature = DS18B20_GetTemp_SkipRom();

						ADC_ConvertedValueLocal =(float) ADC_ConvertedValue/4096*3.3; 			
						vi=ADC_ConvertedValueLocal*8.82;//计算出电压公式
						U = (int)((25*vi - 575)*0.744)*9-720;
						U_flag = 1;
						if(U>= 100)
						{
							U=100;
						}
						if(U<=0)
						{
							U=0;
						}
									
						sprintf(Data_str,"%s%s%s%s,%02d,%02d,%02d,%02d,%02d,%02d,%6d","01","0D","02","1C",flag,Motor_Speed_Now,flag1,cw,U,(int)Temperature,S);//上传实时数据（状态、速度、巡检模式、方向、电量、温度、距离	）	
						printf("%s\n",Data_str);								
						
				}					
						ESP8266_SendString ( ENABLE, Data_str, 0, Single_ID_0 );
						printf("心跳数据上传完毕...\n");
				}
			}	
		}
		else
		{
		OSTimeDly ( 2000, OS_OPT_TIME_DLY, & err );//wifi未连接，等待2s后重检
		}
	}
}

//电量监测
void Electricity_monitoring(void)
{
	OS_ERR      err;
	extern uint8_t data;
	extern uint8_t cw;
	extern uint32_t S;
	extern float vi;
	extern float ADC_ConvertedValueLocal;//浮点电压数据
	extern __IO uint16_t ADC_ConvertedValue;//电压原始数据	
	
	if(U_flag == 1)
	{
		if(10<U&&U<20)
		{
			OSTimeDly ( 5000, OS_OPT_TIME_DLY, & err );//滤波 防止电压采集波动值
			if(10<vi&&vi<20)
			{
				EXTI->IMR &= ~(HUOER_INT_EXTI_LINE);//屏蔽外部中断
				EXTI->IMR &= ~(HUOER1_INT_EXTI_LINE);//屏蔽外部中断
				Motor_Speed_Set(3);
				cw_OFF();
				cw = 1;
			}
		}
		else
		{
			OSTimeDly ( 20000, OS_OPT_TIME_DLY, & err );//延时20s，系统调度
		}

	}
	else
	{
		OSTimeDly ( 10000, OS_OPT_TIME_DLY, & err );//延时10s，系统调度
	}
}


void Data_485(void)//modbus指令发送函数
{
	OS_ERR      err;
	
	
	uint8_t rtu_adress = 01;//地址
	uint8_t rtu_function =03;//功能
	uint16_t rtu_register_adress = 14;//寄存器
	uint16_t rtu_register_number = 01;//寄存器数量
	
	
	modbusrtu_send(&rtu_adress,&rtu_function,&rtu_register_adress,&rtu_register_number);//485采集

	OSTimeDly ( 10000, OS_OPT_TIME_DLY, & err );//延时10s调度	
}

//modbus接受检测函数
void modbus_rx(void)
{
	
	OS_ERR      err;
	
	extern u8 RX485_flag1;
	u8 USART2_RX_DATA[64];
	extern u8 USART2_RX_BUF[64];
  u8 length =0;
	u8 i =0;
	extern u8 RX485_flag1;
	extern u8 USART2_RX_CNT;
		
	if(RX485_flag1 == 1)
	{
				OSTimeDly ( 20, OS_OPT_TIME_DLY, & err );
				while(1)	
				{			
						USART2_RX_DATA[length] = USART2_RX_BUF[length];//赋值到数组
						length++;
						if(USART2_RX_CNT == length)
						{
							break;
						}								
				}
				printf("接收到数据:");
				while(1)
				{
						printf("%02X ",USART2_RX_DATA[i]);	
						i++;
						USART2_RX_CNT--;
						if(USART2_RX_CNT == 0)
						{
								break;
						}
								
				}
				printf("\n");	
				memset(USART2_RX_BUF,0,sizeof(USART2_RX_BUF));//清空数组
				RX485_flag1 = 0;//接收标志位置零	
		}	
  
}


