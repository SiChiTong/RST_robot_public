#ifndef __dsp_TIM5
#define    __dsp_TIM5
#include "stm32f10x.h"


#define            GENERAL_TIM                   TIM4
#define            GENERAL_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            GENERAL_TIM_CLK               RCC_APB1Periph_TIM4
#define            GENERAL_TIM_Period            (65535-1)//多少us产生中断  38.5/9.6*1000
#define            GENERAL_TIM_Prescaler         71  //72分频
#define            GENERAL_TIM_IRQ               TIM4_IRQn
#define            GENERAL_TIM_IRQHandler        TIM4_IRQHandler


//函数声明
void Delay_Tim5_Init(void);
void Delay_us(uint32_t time);
void GENERAL_TIM4_Init(void);
#endif
