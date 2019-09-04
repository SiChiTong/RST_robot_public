#ifndef HUOER__EXTI_H
#define	HUOER__EXTI_H


#include "stm32f10x.h"


//引脚定义，定义引起中断的引脚，即高低电平
#define Distance_INT_GPIO_PORT         GPIOC
#define Distance_INT_GPIO_CLK          (RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO)//打开APB2使能时钟以及AFIO
#define Distance_INT_GPIO_PIN          GPIO_Pin_7


//定义中断寄存器，中断源
#define Distance_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOC//中断源寄存器A
#define Distance_INT_EXTI_PINSOURCE    GPIO_PinSource7//GPIO端口0
#define Distance_INT_EXTI_LINE         EXTI_Line7  //对应端口中断线
#define Distance_INT_EXTI_IRQ          EXTI9_5_IRQn //nvic中断源

#define Distance_IRQHandler            EXTI9_5_IRQHandler

                    


void EXTI_Distance_Config(void);


#endif /* __EXTI_H */
