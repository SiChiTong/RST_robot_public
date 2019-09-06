多任务：

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
