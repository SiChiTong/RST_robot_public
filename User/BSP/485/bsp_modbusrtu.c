#include "bsp_modbusrtu.h"
#include "bsp_485.h"
#include "bsp_tim.h"
#include "crc16.h"



void modbusrtu_send(uint8_t *rtu_adress,uint8_t *rtu_function,uint16_t *rtu_register_adress,uint16_t *rtu_register_number)
{
	uint8_t rtu_register_adress1 = 0x00;
	uint8_t rtu_register_adress2 = 0x00;
	uint8_t rtu_register_number1 = 0x00;
	uint8_t rtu_register_number2 = 0x00;
	uint8_t rtu_crc16_1 = 0x00;
	uint8_t rtu_crc16_2 = 0x00;
	uint16_t rtu_crc16 = 0x0000;//crc校验码
	uint8_t i =0;
	unsigned char str_buf[8];
	
	rtu_register_adress1 = ((*rtu_register_adress)&0XFF00)>>8;//取出高八位
	rtu_register_adress2 = (*rtu_register_adress)&0X00FF;//取出低八位
	
	rtu_register_number1 = ((*rtu_register_number)&0XFF00)>>8;
	rtu_register_number2 = (*rtu_register_number)&0X00FF;
	
	str_buf[0] = *rtu_adress;
	str_buf[1] = *rtu_function;
	str_buf[2] = rtu_register_adress1;
	str_buf[3] = rtu_register_adress2;
	str_buf[4] = rtu_register_number1;
	str_buf[5] = rtu_register_number2;
	
	//生成校验码
	rtu_crc16 = crc16(str_buf,6);
	
	rtu_crc16_2 = (rtu_crc16&0XFF00)>>8;
	rtu_crc16_1 = (rtu_crc16&0X00FF);
	
	str_buf[6] = rtu_crc16_1;
	str_buf[7] = rtu_crc16_2;
	
	RS485_Usart_SendArray(RS485_USARTx,str_buf,8);
	printf("发送数据：");
	for(i=0; i<8; i++)
  {  
	    printf("%02X ",str_buf[i]);	
  }
	printf("\n");	
	
}
