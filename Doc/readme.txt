多任务：
APP1   WIFI启动、通讯部分
APP2   实时数据上传部分
APP3   转向速度调节
APP4   电量监测
APP5   485通信收发
APP6   看门狗

中断资源：
   事件					   资源                    优先级
1.WIFI通信中断             USART3
2.霍尔中断程序             EXTI_Line2/EXTI_Line4
3.光电中断                 EXTI_Line5
4.PWM输入监测中断          TIM8
5.帧数据中断检测（modbus） TIM1
6.485接收串口中断          USART2
7.ADC输入监测中断          ADC2
8.PWM波发生中断计数检测    TIM3    

引脚        	 		功能                      外设  
PB5,0,1         LED
PC7   			脉冲输出（控制电机）              tim3
PB13 			控制方向引脚，使用中断            霍尔传感器
				微秒级延时使用定时器              tim5
PE6				温度监测					        ds18b20
PA9,PA10		串口打印                         usart
PB8,9,10,11     wifi通信						 esp8266
PD2,PA4         霍尔感应引脚                     D2接右边 A4接左边
PA5             充电桩光电感应
PC1				adc                             电压采集
PA2(tx),3(rx)	485通信	                        USART2
PC9             485通信使能
PD9				充电继电器控制
PB14		    24V电源控制
				通信结束检测                     tim1
PC6				pwm占空比、频率获取              tim8

数据变量申明：
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




