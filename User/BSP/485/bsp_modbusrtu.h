#include "stm32f10x.h"


void modbusrtu_send(uint8_t *rtu_adress,uint8_t *rtu_function,uint16_t *rtu_register_adress,uint16_t *rtu_register_number);//申明modbus发送信息函数
