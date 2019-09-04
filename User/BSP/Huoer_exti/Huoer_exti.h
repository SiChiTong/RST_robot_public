#ifndef HUOER__EXTI_H
#define	HUOER__EXTI_H


#include "stm32f10x.h"


//引脚定义，定义引起中断的引脚，即高低电平
#define HUOER_INT_GPIO_PORT         GPIOA
#define HUOER_INT_GPIO_CLK          (RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO)//打开APB2使能时钟以及AFIO
#define HUOER_INT_GPIO_PIN          GPIO_Pin_2


//定义中断寄存器，中断源
#define HUOER_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOA//中断源寄存器A
#define HUOER_INT_EXTI_PINSOURCE    GPIO_PinSource2//GPIO端口0
#define HUOER_INT_EXTI_LINE         EXTI_Line2  //对应端口中断线
#define HUOER_INT_EXTI_IRQ          EXTI2_IRQn //nvic中断源

#define HUOER_IRQHandler            EXTI2_IRQHandler

#define HUOER1_INT_GPIO_PORT         GPIOA
#define HUOER1_INT_GPIO_CLK          (RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO)//打开APB2使能时钟以及AFIO
#define HUOER1_INT_GPIO_PIN          GPIO_Pin_4


#define HUOER1_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOA//中断源寄存器A
#define HUOER1_INT_EXTI_PINSOURCE    GPIO_PinSource4//GPIO端口0
#define HUOER1_INT_EXTI_LINE         EXTI_Line4  //对应端口中断线
#define HUOER1_INT_EXTI_IRQ          EXTI4_IRQn //nvic中断源
//中断执行函数


#define HUOER1_IRQHandler            EXTI4_IRQHandler
                    


void EXTI_HUOER_Config(void);


#endif /* __EXTI_H */
