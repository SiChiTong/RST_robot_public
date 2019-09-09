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
1.电池电量状态   结构体 Bettery  
struct{
        u16 Bettery_current_charge;  		//充电电流
        u16 Bettery_current_discharge;      //放电电流
        u16 Bettery_current_voltage;  	    //单节电压
        u8 Bettery_current_temperature1;    //温度1
        u8 Bettery_current_temperature2;    //温度2
		u8 Bettery_current_temperature3;    //温度3
		u8 Bettery_current_temperature4;    //温度4
		u16 Bettery_work_status;            //工作状态
		u16 Bettery_charge_count;           //充电次数
		u16 Bettery_discharge_count;        //放电次数
		float Bettery_Energy;               //剩余电量
    } Bettery;
定义文件：bsp_task.c

2.通信状态定义



