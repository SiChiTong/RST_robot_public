#ifndef __BSP_ADVANCETIME_H
#define __BSP_ADVANCETIME_H


#include "stm32f10x.h"
#include "bsp_485.h"


/************�߼���ʱ��TIM�������壬ֻ��TIM1��TIM8************/
// ��ʹ�ò�ͬ�Ķ�ʱ����ʱ�򣬶�Ӧ��GPIO�ǲ�һ���ģ����Ҫע��
// ��������ʹ�ø߼����ƶ�ʱ��TIM1

#define            ADVANCE_TIM8                   TIM8
#define            ADVANCE_TIM8_APBxClock_FUN     RCC_APB2PeriphClockCmd
#define            ADVANCE_TIM8_CLK               RCC_APB2Periph_TIM8

// ���벶���ܲ��񵽵���С��Ƶ��Ϊ 72M/{ (ARR+1)*(PSC+1) }
#define            ADVANCE_TIM8_PERIOD            (65535-1)
#define            ADVANCE_TIM8_PSC               (72-1)

// �ж���غ궨��
#define            ADVANCE_TIM8_IRQ               TIM8_CC_IRQn
#define            ADVANCE_TIM8_IRQHandler        TIM8_CC_IRQHandler

// TIM1 ���벶��ͨ��1
#define            ADVANCE_TIM8_CH1_GPIO_CLK      RCC_APB2Periph_GPIOC
#define            ADVANCE_TIM8_CH1_PORT          GPIOC
#define            ADVANCE_TIM8_CH1_PIN           GPIO_Pin_6

#define            ADVANCE_TIM8_IC1PWM_CHANNEL    TIM_Channel_1
#define            ADVANCE_TIM8_IC2PWM_CHANNEL    TIM_Channel_2

#define            ADVANCE_TIM1                   TIM1
#define            ADVANCE_TIM1_APBxClock_FUN     RCC_APB2PeriphClockCmd
#define            ADVANCE_TIM1_CLK               RCC_APB2Periph_TIM1
#define            ADVANCE_TIM1_Period            ((int)((float)(38.5/RS485_USART_BAUDRATE)*1000000)-1)//����us�����ж�  38.5/9.6*1000  (int)((float)(38.5/RS485_USART_BAUDRATE)*1000000)
#define            ADVANCE_TIM1_Prescaler         71
#define            ADVANCE_TIM1_IRQ               TIM1_UP_IRQn
#define            ADVANCE_TIM1_IRQHandler        TIM1_UP_IRQHandler

/**************************��������********************************/

void ADVANCE_TIM8_Init(void);
void ADVANCE_TIM1_Init(void);


#endif	/* __BSP_ADVANCETIME_H */


