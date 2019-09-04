#ifndef __PWM_BREATHING_H
#define	__PWM_BREATHING_H

#include "stm32f10x.h"




#define Motor_Perimeter_Tyre 16.4         //轮子周长，单位：cm


/********************定时器通道**************************/

/***************************/
#define   BRE_TIMx                     TIM3

#define   BRE_TIM_APBxClock_FUN        RCC_APB1PeriphClockCmd
#define   BRE_TIM_CLK                  RCC_APB1Periph_TIM3
#define   BRE_TIM_GPIO_APBxClock_FUN   RCC_APB2PeriphClockCmd
#define   BRE_TIM_GPIO_CLK             (RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO)

#define  BRE_GPIO_REMAP_FUN()					 GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE); //全映射				

#define  BRE_TIM_LED_PORT               GPIOC
#define  BRE_TIM_LED_PIN                GPIO_Pin_7

#define  BRE_TIM_OCxInit                TIM_OC2Init            //通道选择，1~4
#define  BRE_TIM_OCxPreloadConfig       TIM_OC2PreloadConfig 
#define  BRE_CCRx                       CCR2

#define  BRE_TIMx_IRQn                 TIM3_IRQn              //中断
#define  BRE_TIMx_IRQHandler           TIM3_IRQHandler



void Motor_Speed_Set(uint32_t Motor_Speed);
void Speed_cache(void);
void TIMx_Breathing_Init(void);




#endif /* __PWM_BREATHING_H */
