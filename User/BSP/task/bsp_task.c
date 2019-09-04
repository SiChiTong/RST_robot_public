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


//////��eeprom���зֿ�
//#define EEP_IP_Adder    0x00     //IP��ַ
//#define EEP_Port_Adder    0x20     //�˿ڵ�ַ
//#define EEP_ApSsid_Adder    0x28     //WIFI���Ƶ�ַ
//#define EEP_ApPwd_Adder   0x38    //WIFI�����ַ

extern volatile uint8_t ucTcpClosedFlag;
extern uint8_t wifi_link_Status;
extern uint8_t tcp_link_Status;
uint8_t U = 0; //�����ٷֱȶ���
uint8_t U_flag = 0;//�����Ƿ����flag����

//char   macUser_ESP8266_ApSsid_eeprom[32]=  "";                     //Ҫ���ӵ��ȵ������
//char   macUser_ESP8266_ApPwd_eeprom [16] = "";                     //Ҫ���ӵ��ȵ����Կ
//char   macUser_ESP8266_TcpServer_IP_eeprom[16]= "";                 //Ҫ���ӵķ������� IP
//char   macUser_ESP8266_TcpServer_Port_eeprom[8]= "";                //Ҫ���ӵķ������Ķ˿�



/**
  * @brief  ESP8266 ��Sta Tcp Client��͸��
  * @param  ��
  * @retval ��
  */
void Link_start( void )
{
	uint8_t ucStatus;
	OS_ERR     err;
	char *pRecStr = NULL;
	char cStr[25];
	
//	//��ȡeeprom����
//	Read_eeprom(macUser_ESP8266_TcpServer_IP_eeprom,EEP_IP_Adder);//����IP,�˿ڼ�������
//		OSTimeDly ( 5000, OS_OPT_TIME_DLY, & err );//��ʱ2s
//	Read_eeprom(macUser_ESP8266_TcpServer_Port_eeprom,EEP_Port_Adder);
//		OSTimeDly ( 5000, OS_OPT_TIME_DLY, & err );//��ʱ2s
//	Read_eeprom( macUser_ESP8266_ApSsid_eeprom,EEP_ApSsid_Adder);
//		OSTimeDly ( 5000, OS_OPT_TIME_DLY, & err );//��ʱ2s
//	Read_eeprom( macUser_ESP8266_ApPwd_eeprom,EEP_ApPwd_Adder);
//		OSTimeDly ( 5000, OS_OPT_TIME_DLY, & err );//��ʱ2s
	
	printf ( "\r\n�������� ESP8266 ......\r\n" );

	macESP8266_CH_ENABLE();
	
	ESP8266_Net_Mode_Choose ( STA );
	printf("�ѿ���STAģʽ....\n");
	
	while (!ESP8266_JoinAP(macUser_ESP8266_ApSsid, macUser_ESP8266_ApPwd))
	{
		printf("����wifiʧ��,��������...\n");
		OSTimeDly ( 2000, OS_OPT_TIME_DLY, & err );//��ʱ2s
	}
	wifi_link_Status = 1;
	macLED1_OFF();
	macLED3_OFF();
	macLED2_ON();		

	
	printf("����wifi�ɹ�....\n");
	ESP8266_Enable_MultipleId ( DISABLE );
	
	while ( !	ESP8266_Link_Server ( enumTCP, macUser_ESP8266_TcpServer_IP, macUser_ESP8266_TcpServer_Port, Single_ID_0 ) );
	tcp_link_Status = 1;
	
	
	
	while ( ! ESP8266_UnvarnishSend () );	
	
	printf("���ӷ������ɹ�....\n");
	
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
			printf("���ܵ�������ָ��...\n");
			strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ] = '\0';
			pRecStr = strEsp8266_Fram_Record .Data_RX_BUF;
			
			strEsp8266_Fram_Record .InfBit .FramLength = 0;
			strEsp8266_Fram_Record .InfBit .FramFinishFlag = 0;
			
			tcp_Agreement(pRecStr);
		}
		else
		{		
			OSTimeDly ( 20, OS_OPT_TIME_DLY, & err );//��ʱ20ms,ucos�������
		}
		
		if ( ucTcpClosedFlag )                                             //����Ƿ�ʧȥ����
		{
			tcp_link_Status = 0;
			ESP8266_ExitUnvarnishSend ();                                    //�˳�͸��ģʽ
			
			do ucStatus = ESP8266_Get_LinkStatus ();                         //��ȡ����״̬
			while ( ! ucStatus );
			
			if ( ucStatus == 4 )                                             //ȷ��ʧȥ���Ӻ�����
			{
				macLED1_OFF();
				macLED2_OFF();
				macLED1_ON();					
				printf ( "\r\n���������ȵ�ͷ����� ......\r\n" );
				
				while ( ! ESP8266_JoinAP ( macUser_ESP8266_ApSsid, macUser_ESP8266_ApPwd ) );
				macLED1_OFF();
				macLED3_OFF();
				macLED2_ON();
				
				while ( !	ESP8266_Link_Server ( enumTCP, macUser_ESP8266_TcpServer_IP, macUser_ESP8266_TcpServer_Port, Single_ID_0 ) );
				
				tcp_link_Status = 1;
				macLED1_OFF();	
				macLED2_OFF();
				macLED3_ON();	
				
				printf ( "\r\n�����ȵ�ͷ������ɹ�\r\n" );
      
			}
			  
			while ( ! ESP8266_UnvarnishSend () );	
			
			printf ( "\r\n���� ESP8266 ���\r\n" );			
		}
		
	}

}		


void real_time_data(void)
{
	OS_ERR     err;
	char  Data_str[200];//�ϴ�ʵʱ�����ַ���
	float Temperature = 0;//�¶�
	extern __IO uint16_t ADC_ConvertedValue;//��ѹԭʼ����
	extern uint8_t wifi_link_Status;//wifi����״̬
	extern uint8_t tcp_link_Status;//tcp����״̬
	extern uint8_t flag,flag1;//Ѳ������״̬
	extern float ADC_ConvertedValueLocal;//�����ѹ���� 
	extern float vi;//��ѹ��
	extern uint8_t cw;
	extern int8_t j; 
	extern uint32_t S;//���߾���
	extern uint8_t Motor_Speed_Now;//ʵʱ�ٶ�
	uint8_t count1 =0; 
	
	
	while (1) 
	{ 
		if(wifi_link_Status)//���wifi�Ƿ�����
			{
				while(1)
				{
					
				if (tcp_link_Status == 0)//���tcp�Ƿ�����
					{
						printf("TCPδ����...\n");
						OSTimeDly ( 2000, OS_OPT_TIME_DLY, & err );    //TCPδ������ʱ2s
					}
				else
				{	
					count1++;
					OSTimeDly ( 2000, OS_OPT_TIME_DLY, & err );  //1�뷢��һ�ξ�������
					sprintf(Data_str,"%s%s%s%s,%6d","01","0D","02","1S",S);
					//10�뷢��һ��ʵʱ����
					if(count1 == 5)
					{
						count1 = 0;
						Temperature = DS18B20_GetTemp_SkipRom();

						ADC_ConvertedValueLocal =(float) ADC_ConvertedValue/4096*3.3; 			
						vi=ADC_ConvertedValueLocal*8.82;//�������ѹ��ʽ
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
									
						sprintf(Data_str,"%s%s%s%s,%02d,%02d,%02d,%02d,%02d,%02d,%6d","01","0D","02","1C",flag,Motor_Speed_Now,flag1,cw,U,(int)Temperature,S);//�ϴ�ʵʱ���ݣ�״̬���ٶȡ�Ѳ��ģʽ�����򡢵������¶ȡ�����	��	
						printf("%s\n",Data_str);								
						
				}					
						ESP8266_SendString ( ENABLE, Data_str, 0, Single_ID_0 );
						printf("���������ϴ����...\n");
				}
			}	
		}
		else
		{
		OSTimeDly ( 2000, OS_OPT_TIME_DLY, & err );//wifiδ���ӣ��ȴ�2s���ؼ�
		}
	}
}

//�������
void Electricity_monitoring(void)
{
	OS_ERR      err;
	extern uint8_t data;
	extern uint8_t cw;
	extern uint32_t S;
	extern float vi;
	extern float ADC_ConvertedValueLocal;//�����ѹ����
	extern __IO uint16_t ADC_ConvertedValue;//��ѹԭʼ����	
	
	if(U_flag == 1)
	{
		if(10<U&&U<20)
		{
			OSTimeDly ( 5000, OS_OPT_TIME_DLY, & err );//�˲� ��ֹ��ѹ�ɼ�����ֵ
			if(10<vi&&vi<20)
			{
				EXTI->IMR &= ~(HUOER_INT_EXTI_LINE);//�����ⲿ�ж�
				EXTI->IMR &= ~(HUOER1_INT_EXTI_LINE);//�����ⲿ�ж�
				Motor_Speed_Set(3);
				cw_OFF();
				cw = 1;
			}
		}
		else
		{
			OSTimeDly ( 20000, OS_OPT_TIME_DLY, & err );//��ʱ20s��ϵͳ����
		}

	}
	else
	{
		OSTimeDly ( 10000, OS_OPT_TIME_DLY, & err );//��ʱ10s��ϵͳ����
	}
}


void Data_485(void)//modbusָ��ͺ���
{
	OS_ERR      err;
	
	
	uint8_t rtu_adress = 01;//��ַ
	uint8_t rtu_function =03;//����
	uint16_t rtu_register_adress = 14;//�Ĵ���
	uint16_t rtu_register_number = 01;//�Ĵ�������
	
	
	modbusrtu_send(&rtu_adress,&rtu_function,&rtu_register_adress,&rtu_register_number);//485�ɼ�

	OSTimeDly ( 10000, OS_OPT_TIME_DLY, & err );//��ʱ10s����	
}

//modbus���ܼ�⺯��
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
						USART2_RX_DATA[length] = USART2_RX_BUF[length];//��ֵ������
						length++;
						if(USART2_RX_CNT == length)
						{
							break;
						}								
				}
				printf("���յ�����:");
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
				memset(USART2_RX_BUF,0,sizeof(USART2_RX_BUF));//�������
				RX485_flag1 = 0;//���ձ�־λ����	
		}	
  
}


