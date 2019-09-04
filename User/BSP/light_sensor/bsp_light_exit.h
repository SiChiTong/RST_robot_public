#ifndef LIGHT__EXTI_H
#define	LIGHT__EXTI_H


#include "stm32f10x.h"


//引脚定义，定义引起中断的引脚，即高低电平
#define LIGHT_INT_GPIO_PORT         GPIOA
#define LIGHT_INT_GPIO_CLK          (RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO)//打开APB2使能时钟以及AFIO
#define LIGHT_INT_GPIO_PIN          GPIO_Pin_5


//定义中断寄存器，中断源
#define LIGHT_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOA//中断源寄存器A
#define LIGHT_INT_EXTI_PINSOURCE    GPIO_PinSource5//GPIO端口0
#define LIGHT_INT_EXTI_LINE         EXTI_Line5  //对应端口中断线
#define LIGHT_INT_EXTI_IRQ          EXTI9_5_IRQn //nvic中断源

//中断执行函数

#define LIGHT_IRQHandler            EXTI9_5_IRQHandler
                    


void EXTI_LIGHT_Config(void);


#endif /* __LIGHT_H */
