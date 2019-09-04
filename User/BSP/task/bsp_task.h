#ifndef  __TEST_H
#define	 __TEST_H



#include "stm32f10x.h"



/********************************** �û���Ҫ���õĲ���**********************************/
#define      macUser_ESP8266_ApSsid                       "RST"                //Ҫ���ӵ��ȵ������
#define      macUser_ESP8266_ApPwd                        "RST88888"           //Ҫ���ӵ��ȵ����Կ

#define      macUser_ESP8266_TcpServer_IP                 "192.168.1.100"      //Ҫ���ӵķ������� IP
#define      macUser_ESP8266_TcpServer_Port               "7000"               //Ҫ���ӵķ������Ķ˿�



/********************************** �ⲿȫ�ֱ��� ***************************************/
extern volatile uint8_t ucTcpClosedFlag;
void HexToStr1(char *pbDest, char *pbSrc, int nLen);



/********************************** �������� ***************************************/
void Link_start  ( void );
void real_time_data(void);
void Electricity_monitoring(void);
void Data_485(void);
void modbus_rx(void);
void                      dt_send2               (uint16_t a,uint8_t b);//a��������λ�bb����ָ��

#endif
