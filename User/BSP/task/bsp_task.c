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
#include "stm32f10x_it.h"
#include "bsp_variable.h"
#include <stdlib.h>


uint8_t U_flag = 0;//�����Ƿ����flag����
uint8_t U = 0; //�����ٷֱȶ���


//ASCII��ת����Ӧ���ֺ���
u8 ascii_to_int(u8 ascii)
{
	if(ascii >= 65)
	{
		return ascii-65+10;
	}
	else
	{
		char *nptr = NULL;
		u8 int8 = 0;
		char char1;
		char1 = (char)(ascii);
		nptr = &char1;
		int8 = atoi(nptr);
		return int8;
	}
}

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
	
	printf ( "\r\n�������� ESP8266 ......\r\n" );

	macESP8266_CH_ENABLE();
	
	ESP8266_Net_Mode_Choose ( STA );
	printf("�ѿ���STAģʽ....\n");
	
	while (!ESP8266_JoinAP(macUser_ESP8266_ApSsid, macUser_ESP8266_ApPwd))
	{
		printf("����wifiʧ��,��������...\n");
		OSTimeDly ( 2000, OS_OPT_TIME_DLY, & err );//��ʱ2s
	}
	WIFI.wifi_link_Status = 1;
	macLED1_OFF();
	macLED3_OFF();
	macLED2_ON();		

	
	printf("����wifi�ɹ�....\n");
	ESP8266_Enable_MultipleId ( DISABLE );
	
	while ( !	ESP8266_Link_Server ( enumTCP, macUser_ESP8266_TcpServer_IP, macUser_ESP8266_TcpServer_Port, Single_ID_0 ) );
	WIFI.tcp_link_Status = 1;
	
	
	
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
		
		if ( WIFI.ucTcpClosedFlag )                                             //����Ƿ�ʧȥ����
		{
			WIFI.tcp_link_Status = 0;
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
				
				WIFI.tcp_link_Status = 1;
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
	uint8_t count =0; 
	while (1) 
	{ 
		if(WIFI.wifi_link_Status)//���wifi�Ƿ�����
			{
				while(1)
				{
					
				if (WIFI.tcp_link_Status == 0)//���tcp�Ƿ�����
					{
						printf("TCPδ����...\n");
						OSTimeDly ( 2000, OS_OPT_TIME_DLY, & err );    //TCPδ������ʱ2s
					}
				else
				{	
					count++;
					OSTimeDly ( 2000, OS_OPT_TIME_DLY, & err );  //1�뷢��һ�ξ�������
					sprintf(Data_str,"%s%s%s%s,%6d","01","0D","02","1S",Robot.distance);
					//10�뷢��һ��ʵʱ����
					if(count == 5)
					{
						count = 0;
						Temperature = DS18B20_GetTemp_SkipRom();
						ADC_V.ConvertedValueLocal =(float) ADC_V.ConvertedValue/4096*3.3; 			
						ADC_V.vi=ADC_V.ConvertedValueLocal*8.82;//�������ѹ��ʽ
						U = (int)((25*ADC_V.vi - 575)*0.744)*9-720;
						U_flag = 1;
						if(U>= 100)
						{
							U=100;
						}
						if(U<=0)
						{
							U=0;
						}
									
						sprintf(Data_str,"%s%s%s%s,%02d,%02d,%02d,%02d,%02d,%02d,%6d","01","0D","02","1C",Robot.running_status,Robot.speed_now,Robot.inspection_mode,Robot.drection,U,(int)Temperature,Robot.distance);//�ϴ�ʵʱ���ݣ�״̬���ٶȡ�Ѳ��ģʽ�����򡢵������¶ȡ�����	��	
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
	
	if(U_flag == 1)
	{
		if(10<U&&U<20)
		{
			OSTimeDly ( 5000, OS_OPT_TIME_DLY, & err );//�˲� ��ֹ��ѹ�ɼ�����ֵ
			if(10<ADC_V.vi&&ADC_V.vi<20)
			{
				EXTI->IMR &= ~(HUOER_INT_EXTI_LINE);//�����ⲿ�ж�
				EXTI->IMR &= ~(HUOER1_INT_EXTI_LINE);//�����ⲿ�ж�
				Motor_Speed_Set(3);
				cw_OFF();
				Robot.drection = 1;
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


void Modbus_tx(void)//modbusָ��ͺ���
{
	modbusrtu_send(&Modbustx.adress,&Modbustx.function,&Modbustx.register_adress,&Modbustx.register_number);//Modbusָ��
}

//modbus���ܺ���
void modbus_rx(void)
{
	u8 USART2_RX_DATA[64];
  u8 length =0;
	u8 i =0;

	while(1)	
	{			
			USART2_RX_DATA[length] = Usart485.USART2_RX_BUF[length];//��ֵ������
			length++;
			if(Usart485.USART2_RX_CNT == length)
			{
				break;
			}								
	}
	printf("���յ�Modbusָ��:");
	while(1)
	{
			printf("%02X ",USART2_RX_DATA[i]);	
			i++;
			Usart485.USART2_RX_CNT--;
			if(Usart485.USART2_RX_CNT == 0)
			{
				break;
			}
								
	}
	printf("\n");	
	memset(Usart485.USART2_RX_BUF,0,sizeof(Usart485.USART2_RX_BUF));//�������
					 
}

//���ͨѶ��ѯָ��
void Battery_tx(void)
{
	u8 i = 0;
	unsigned char str_buf[14];
	
	str_buf[0] =  0x3A;
	str_buf[1] =  0x30;
	str_buf[2] =  0x30;
	str_buf[3] =  0x30;
	str_buf[4] =  0x32;
	str_buf[5] =  0x35;
	str_buf[6] =  0x30;
	str_buf[7] =  0x30;
	str_buf[8] =  0x30;
	str_buf[9] =  0x30;
	str_buf[10] = 0x45;
	str_buf[11] = 0x30;
	str_buf[12] = 0x33;
	str_buf[13] = 0x7E;
	
	RS485_Usart_SendArray(RS485_USARTx,str_buf,14);
	printf("�������ݣ�");
	for(i=0; i<14; i++)
  {  
	    printf("%02X ",str_buf[i]);	
  }
	printf("\n");		
}


//���ͨѶ���ս�������
void Battery_rx(void)
{
	u8 USART2_RX_DATA[200];
  u8 j =0;
	u8 i =0;
	
	if(Usart485.USART2_RX_BUF[0] == 0x3A && Usart485.USART2_RX_BUF[1] == 0x30)
	{
		while(true)	
		{			
				USART2_RX_DATA[j] = Usart485.USART2_RX_BUF[j];//��ֵ������
				j++;
				if(Usart485.USART2_RX_CNT == j)
				{
					Usart485.USART2_RX_CNT = 0;
					break;
				}								
		}
//		printf("���յ�Modbusָ��:");
//		for(;i < j;i++)
//		{
//			printf("%02X ",USART2_RX_DATA[i]);			
//		}
//		printf("\n");	
		memset(&Usart485.USART2_RX_BUF,0,sizeof(Usart485.USART2_RX_BUF));//�������				
		//Vcell[]����86��87��88��89λ�������ڵ�ص�ѹ����λmV
		Battery.current_voltage = ascii_to_int(USART2_RX_DATA[25])*4096+ascii_to_int(USART2_RX_DATA[26])*256+ascii_to_int(USART2_RX_DATA[27])*16+ascii_to_int(USART2_RX_DATA[28]);
		//CURR[0],��90/91/92/93λ,�������������λ10mA
		Battery.current_charge = (ascii_to_int(USART2_RX_DATA[89])*4096+ascii_to_int(USART2_RX_DATA[90])*256+ascii_to_int(USART2_RX_DATA[91])*16+ascii_to_int(USART2_RX_DATA[92]))*10;
		
		//CURR[1],��94/95/96/97λ������ŵ��������λ10mA
		Battery.current_discharge = (ascii_to_int(USART2_RX_DATA[93])*4096+ascii_to_int(USART2_RX_DATA[94])*256+ascii_to_int(USART2_RX_DATA[95])*16+ascii_to_int(USART2_RX_DATA[96]))*10;
		
		//4���¶ȣ���98/99/100/101/102/103/104/105λ����λ���϶�
		Battery.current_temperature1 = ascii_to_int(USART2_RX_DATA[97])*16+ascii_to_int(USART2_RX_DATA[98])-40;	
		Battery.current_temperature2 = ascii_to_int(USART2_RX_DATA[99])*16+ascii_to_int(USART2_RX_DATA[100])-40;
		//����״̬����106/107/108/109λ   
		/*˵�� uint16 CING:1; //���״̬��1 ��� 0 �޳�� �����λ LSB��
				uint16 DING:1; //�ŵ�״̬ 1 �ŵ磬0 �޷ŵ�
				uint16 VoltH:1; //��ѹ������ 1�������� 0 Ϊ����
				uint16 VoltL:1; //���ű����� 1�������� 0 Ϊ����
				uint16 CurrC:1; //�������� 1�������� 0 Ϊ����
				uint16 CurrS:1; //��·������ 1�������� 0 Ϊ����
				uint16 CurrD1:1;//�ŵ����һ��1�������� 0 Ϊ����
				uint16 CurrD2:1;//�ŵ��������1�������� 0 Ϊ����
				uint16 TempCH:1;//�����£� 1�������� 0 Ϊ����
				uint16 TempCL:1;//�����£� 1�������� 0 Ϊ����
				uint16 TempDH:1;//�ŵ���� 1�������� 0 Ϊ����
				uint16 TempDL:1;//�ŵ���� 1�������� 0 Ϊ����
				uint16 DFET:1; //�ŵ翪��״̬�� 1���� 0 Ϊ�ر�
				uint16 CFET:1; //��翪��״̬�� 1���� 0 Ϊ�ر�
				uint16 resv1:1 //����λ 1
				uint16 resv2:1;//����λ 2 �����λ MSB�� */
		Battery.work_status = ascii_to_int(USART2_RX_DATA[105])*4096+ascii_to_int(USART2_RX_DATA[106])*256+ascii_to_int(USART2_RX_DATA[107])*16+ascii_to_int(USART2_RX_DATA[108]);
		
		//��116/117/118/119λ,������
		Battery.charge_count = ascii_to_int(USART2_RX_DATA[115])*4096+ascii_to_int(USART2_RX_DATA[116])*256+ascii_to_int(USART2_RX_DATA[117])*16+ascii_to_int(USART2_RX_DATA[118]);
	
		//��120/121/122/123λ���ŵ����
		Battery.discharge_count = ascii_to_int(USART2_RX_DATA[119])*4096+ascii_to_int(USART2_RX_DATA[120])*256+ascii_to_int(USART2_RX_DATA[121])*16+ascii_to_int(USART2_RX_DATA[122]);
		
		//��124/125λ��ʣ���������λ1%
		Battery.energy = (ascii_to_int(USART2_RX_DATA[123])*16+ascii_to_int(USART2_RX_DATA[124]));
		printf("���״̬���£�\n");
		printf("���ڵ�ѹ:��%dmV\n", Battery.current_voltage);
		printf("��������%dmA\n", Battery.current_charge);
		printf("�ŵ������%dmA\n", Battery.current_discharge);
		printf("�¶�1��%d��\n", Battery.current_temperature1);
		printf("�¶�2��%d��\n", Battery.current_temperature2);
		printf("����״̬��%d\n", Battery.work_status);
		printf("��������%d\n", Battery.charge_count);
		printf("�ŵ������%d\n", Battery.discharge_count);
		printf("ʣ�������%d%%\n", Battery.energy);
		
	}
}
		
		



