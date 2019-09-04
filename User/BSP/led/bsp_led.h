#ifndef __LED_H_
#define	__LED_H_



#include "stm32f10x.h"
 
 
 
/****************************** LED 引脚配置参数定义***************************************/
#define             macLED1_GPIO_APBxClock_FUN              RCC_APB2PeriphClockCmd
#define             macLED1_GPIO_CLK                        RCC_APB2Periph_GPIOB
#define             macLED1_GPIO_PORT                       GPIOB
#define             macLED1_GPIO_PIN                        GPIO_Pin_5

#define             macLED2_GPIO_APBxClock_FUN              RCC_APB2PeriphClockCmd
#define             macLED2_GPIO_CLK                        RCC_APB2Periph_GPIOB
#define             macLED2_GPIO_PORT                       GPIOB
#define             macLED2_GPIO_PIN                        GPIO_Pin_0

#define             macLED3_GPIO_APBxClock_FUN              RCC_APB2PeriphClockCmd
#define             macLED3_GPIO_CLK                        RCC_APB2Periph_GPIOB
#define             macLED3_GPIO_PORT                       GPIOB
#define             macLED3_GPIO_PIN                        GPIO_Pin_1


#define             cw_GPIO_APBxClock_FUN              RCC_APB2PeriphClockCmd
#define             cw_GPIO_CLK                        RCC_APB2Periph_GPIOB
#define             cw_GPIO_PORT                       GPIOB
#define             cw_GPIO_PIN                        GPIO_Pin_13


#define             V24_GPIO_APBxClock_FUN              RCC_APB2PeriphClockCmd
#define             V24_GPIO_CLK                        RCC_APB2Periph_GPIOB
#define             V24_GPIO_PORT                       GPIOB
#define             V24_GPIO_PIN                        GPIO_Pin_14

#define             Charge_APBxClock_FUN              RCC_APB2PeriphClockCmd
#define             Charge_GPIO_CLK                    RCC_APB2Periph_GPIOD
#define             Charge_GPIO_PORT                     GPIOD
#define             Charge_GPIO_PIN                       GPIO_Pin_9


/****************************** LED 函数宏定义***************************************/
#define             macLED1_ON()                            GPIO_ResetBits ( macLED1_GPIO_PORT, macLED1_GPIO_PIN )
#define             macLED1_OFF()                           GPIO_SetBits ( macLED1_GPIO_PORT, macLED1_GPIO_PIN )
#define             macLED1_TOGGLE()                        GPIO_ReadOutputDataBit ( macLED1_GPIO_PORT, macLED1_GPIO_PIN ) ? \
                                                            GPIO_ResetBits ( macLED1_GPIO_PORT, macLED1_GPIO_PIN ) : GPIO_SetBits ( macLED1_GPIO_PORT, macLED1_GPIO_PIN )

#define             macLED2_ON()                            GPIO_ResetBits ( macLED2_GPIO_PORT, macLED2_GPIO_PIN )
#define             macLED2_OFF()                           GPIO_SetBits ( macLED2_GPIO_PORT, macLED2_GPIO_PIN )
#define             macLED2_TOGGLE()                        GPIO_ReadOutputDataBit ( macLED2_GPIO_PORT, macLED2_GPIO_PIN ) ? \
                                                            GPIO_ResetBits ( macLED2_GPIO_PORT, macLED2_GPIO_PIN ) : GPIO_SetBits ( macLED2_GPIO_PORT, macLED2_GPIO_PIN )

#define             macLED3_ON()                            GPIO_ResetBits ( macLED3_GPIO_PORT, macLED3_GPIO_PIN )
#define             macLED3_OFF()                           GPIO_SetBits ( macLED3_GPIO_PORT, macLED3_GPIO_PIN )
#define             macLED3_TOGGLE()                        GPIO_ReadOutputDataBit ( macLED3_GPIO_PORT, macLED3_GPIO_PIN ) ? \
                                                            GPIO_ResetBits ( macLED3_GPIO_PORT, macLED3_GPIO_PIN ) : GPIO_SetBits ( macLED3_GPIO_PORT, macLED3_GPIO_PIN )


#define             cw_ON()                            		GPIO_ResetBits ( cw_GPIO_PORT, cw_GPIO_PIN )
#define             cw_OFF()                          	 GPIO_SetBits ( cw_GPIO_PORT, cw_GPIO_PIN )
#define             cw_TOGGLE()                       	 GPIO_ReadOutputDataBit ( cw_GPIO_PORT, cw_GPIO_PIN ) ? \
																													GPIO_ResetBits ( cw_GPIO_PORT, cw_GPIO_PIN ) : GPIO_SetBits ( cw_GPIO_PORT, cw_GPIO_PIN )
																											 
#define             V24_ON()                            	GPIO_ResetBits ( V24_GPIO_PORT, V24_GPIO_PIN )
#define             V24_OFF()                           	GPIO_SetBits ( V24_GPIO_PORT, V24_GPIO_PIN )
#define             V24_TOGGLE()                        	GPIO_ReadOutputDataBit ( V24_GPIO_PORT, V24_GPIO_PIN ) ? \
																													GPIO_ResetBits ( V24_GPIO_PORT, V24_GPIO_PIN ) : GPIO_SetBits ( V24_GPIO_PORT, V24_GPIO_PIN )																											 

#define             Charge_ON()                           GPIO_ResetBits ( Charge_GPIO_PORT, Charge_GPIO_PIN )
#define             Charge_OFF()                          GPIO_SetBits ( Charge_GPIO_PORT, Charge_GPIO_PIN )
#define             Charge_TOGGLE()                       GPIO_ReadOutputDataBit ( Charge_GPIO_PORT, Charge_GPIO_PIN ) ? \
																													GPIO_ResetBits ( Charge_GPIO_PORT, Charge_GPIO_PIN ) : GPIO_SetBits ( Charge_GPIO_PORT, Charge_GPIO_PIN )	

/************************** PAD 函数声明********************************/
void                               LED_Init                         ( void );
 
 

#endif /* __LED_H_ */

