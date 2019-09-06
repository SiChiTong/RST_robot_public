#ifndef __BSP_ADVANCETIME_H
#define __BSP_ADVANCETIME_H


#include "stm32f10x.h"
#include "bsp_485.h"


/************高级定时器TIM参数定义，只限TIM1和TIM8************/
// 当使用不同的定时器的时候，对应的GPIO是不一样的，这点要注意
// 这里我们使用高级控制定时器TIM1

#define            ADVANCE_TIM8                   TIM8
#define            ADVANCE_TIM8_APBxClock_FUN     RCC_APB2PeriphClockCmd
#define            ADVANCE_TIM8_CLK               RCC_APB2Periph_TIM8

// 输入捕获能捕获到的最小的频率为 72M/{ (ARR+1)*(PSC+1) }
#define            ADVANCE_TIM8_PERIOD            (65535-1)
#define            ADVANCE_TIM8_PSC               (72-1)

// 中断相关宏定义
#define            ADVANCE_TIM8_IRQ               TIM8_CC_IRQn
#define            ADVANCE_TIM8_IRQHandler        TIM8_CC_IRQHandler

// TIM1 输入捕获通道1
#define            ADVANCE_TIM8_CH1_GPIO_CLK      RCC_APB2Periph_GPIOC
#define            ADVANCE_TIM8_CH1_PORT          GPIOC
#define            ADVANCE_TIM8_CH1_PIN           GPIO_Pin_6

#define            ADVANCE_TIM8_IC1PWM_CHANNEL    TIM_Channel_1
#define            ADVANCE_TIM8_IC2PWM_CHANNEL    TIM_Channel_2

#define            ADVANCE_TIM1                   TIM1
#define            ADVANCE_TIM1_APBxClock_FUN     RCC_APB2PeriphClockCmd
#define            ADVANCE_TIM1_CLK               RCC_APB2Periph_TIM1
#define            ADVANCE_TIM1_Period            ((int)((float)(38.5/RS485_USART_BAUDRATE)*1000000)-1)//多少us产生中断  38.5/9.6*1000  (int)((float)(38.5/RS485_USART_BAUDRATE)*1000000)
#define            ADVANCE_TIM1_Prescaler         71
#define            ADVANCE_TIM1_IRQ               TIM1_UP_IRQn
#define            ADVANCE_TIM1_IRQHandler        TIM1_UP_IRQHandler

/**************************函数声明********************************/

void ADVANCE_TIM8_Init(void);
void ADVANCE_TIM1_Init(void);


#endif	/* __BSP_ADVANCETIME_H */


