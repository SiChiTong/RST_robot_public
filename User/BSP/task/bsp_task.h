#ifndef  __TEST_H
#define	 __TEST_H



#include "stm32f10x.h"



/********************************** 用户需要设置的参数**********************************/
#define      macUser_ESP8266_ApSsid                       "RST"                //要连接的热点的名称
#define      macUser_ESP8266_ApPwd                        "RST88888"           //要连接的热点的密钥

#define      macUser_ESP8266_TcpServer_IP                 "www.chengdurst.work"      //要连接的服务器的 IP
#define      macUser_ESP8266_TcpServer_Port               "7000"               //要连接的服务器的端口



/********************************** 外部全局变量 ***************************************/
extern volatile uint8_t ucTcpClosedFlag;
void HexToStr1(char *pbDest, char *pbSrc, int nLen);



/********************************** 函数声明 ***************************************/
void Link_start  ( void );
void real_time_data(void);
void Electricity_monitoring(void);
void Modbus_tx(void);
void modbus_rx(void);
void Battery_rx(void);

void                      dt_send2               (uint16_t a,uint8_t b);//a代表数据位b代表指令

#endif

