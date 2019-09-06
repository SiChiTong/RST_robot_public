#include "bsp_tim.h"
#include "includes.h"
//配置中断优先级

void Delay_Tim5_Init(void)
{
      TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
     
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
      
      TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);
      TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
      TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Down;
			TIM_TimeBaseInitStruct.TIM_Period = 100-1;
			TIM_TimeBaseInitStruct.TIM_Prescaler = (72-1);
			TIM_TimeBaseInit(TIM5, &TIM_TimeBaseInitStruct);
     
      while((TIM5->SR & TIM_FLAG_Update)!=SET);
      TIM5->SR = (uint16_t)~TIM_FLAG_Update;
}



void Delay_us(uint32_t time)
{
		TIM5->CNT = time;
		TIM5->CR1 |= TIM_CR1_CEN;   
		while((TIM5->SR & TIM_FLAG_Update)!=SET);
		TIM5->SR = (uint16_t)~TIM_FLAG_Update;
		TIM5->CR1 &= ~TIM_CR1_CEN;
	
}


   


