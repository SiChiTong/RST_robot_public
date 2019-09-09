#ifndef _H_VARIABLE_H_
#define	_H_VARIABLE_H_

#include "stm32f10x.h"
/*电池参数结构体变量*/
extern struct battery {
        u16 current_charge;  		//充电电流
        u16 current_discharge;  //放电电流
        u16 current_voltage;  	//单节电压
        u8 current_temperature1;//温度1
        u8 current_temperature2;//温度2
				u8 current_temperature3;//温度3
				u8 current_temperature4;//温度4
				u16 work_status;        //工作状态
				u16 charge_count;       //充电次数
				u16 discharge_count;    //放电次数
				float energy;           //剩余电量
 }Battery;

		/*MODBUS参数结构体*/
extern struct modbustx {
	u8 adress;           //地址
	u8 function;          //功能
	u16 register_adress; //寄存器
	u16 register_number; //寄存器数量
} Modbustx;


/*机器人状态参数结构体*/
extern struct robot{
	u32 tim_counter;     //中断计数
	u8 speed_now;    	   //实时速度
	u8 drection;         //方向,初始值1
	u8 grading;          //速度分档参数,初始值10
	u32 distance;        //距离值
	u8 interrupt_flag;   //中断标志
	u8 inspection_mode;  //巡检模式
	u8 running_status;   //运行状态
}Robot;

/*串口通信参数结构体*/
extern struct usart{
	u8 USART2_RX_BUF[128];    //接受数组
	u8 USART2_RX_CNT;    	   //数组计数值
}Usart485;


/*ADC采集参数结构体*/
extern struct adc{
	__IO uint16_t ConvertedValue;  //电压原始数据
	float ConvertedValueLocal;     //adc换算值
	float vi;                      //adc采集到电压值
}ADC_V;


//将eeprom进行分块
//#define EEP_IP_Adder    0x00     //IP地址
//#define EEP_Port_Adder    0x20     //端口地址
//#define EEP_ApSsid_Adder    0x28     //WIFI名称地址
//#define EEP_ApPwd_Adder   0x38    //WIFI密码地址

/*WIFI连接状态参数结构体*/
extern struct wifi{
	char   ApSsid_eeprom[32];         //要连接的热点的名称
	char   ApPwd_eeprom [16];         //要连接的热点的密钥
	char   TcpServer_IP_eeprom[16];   //要连接的服务器的IP
	char   TcpServer_Port_eeprom[8];  //要连接的服务器的端口
	
	volatile uint8_t ucTcpClosedFlag;//TCP断开连接状态标志位
	volatile uint8_t wifi_link_Status;//WIFI连接状态标志位
	volatile uint8_t tcp_link_Status;//TCP连接状态标志位
}WIFI;




#endif /* __VARIABLE_H */
		

