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



// 中断优先级配置
static void GENERAL_TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // 设置中断组为0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);		
		// 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = GENERAL_TIM_IRQ ;	
		// 设置主优先级为 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
	  // 设置抢占优先级为3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

static void GENERAL_TIM_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;		
		// 开启定时器时钟,即内部时钟CK_INT=72M
    GENERAL_TIM_APBxClock_FUN(GENERAL_TIM_CLK, ENABLE);	
		// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
    TIM_TimeBaseStructure.TIM_Period=GENERAL_TIM_Period;
	  // 时钟预分频数
    TIM_TimeBaseStructure.TIM_Prescaler= GENERAL_TIM_Prescaler;	
		// 时钟分频因子 ，没用到不用管
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
		// 计数器计数模式，设置为向下计数
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Down; 		
		// 重复计数器的值，没用到不用管
		TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	  // 初始化定时器
    TIM_TimeBaseInit(GENERAL_TIM, &TIM_TimeBaseStructure);
	
		// 清除计数器中断标志位
    TIM_ClearFlag(GENERAL_TIM, TIM_FLAG_Update);
	  	
		// 使能计数器
		TIM_Cmd(GENERAL_TIM, ENABLE);
}

void GENERAL_TIM4_Init(void)
{
	GENERAL_TIM_NVIC_Config();
	GENERAL_TIM_Mode_Config();		
}


