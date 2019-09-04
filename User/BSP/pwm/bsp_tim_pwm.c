#include "stm32f10x.h"
#include "bsp_tim_pwm.h"
#include "bsp_usart.h"
#include "includes.h"


uint32_t Motor_Speed_Counter = 0;
uint8_t Motor_Speed_Now = 0;


 /**
  * @brief  配置TIM复用输出PWM时用到的I/O
  * @param  无
  * @retval 无
  */
static void TIMx_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /*  clock enable */
  RCC_APB2PeriphClockCmd(BRE_TIM_GPIO_CLK, ENABLE); 
  BRE_TIM_GPIO_APBxClock_FUN  ( BRE_TIM_GPIO_CLK, ENABLE );
		
	GPIO_AFIODeInit();//完全重映射前初始化寄存器
	BRE_GPIO_REMAP_FUN(); //完全重映射 

  /* 配置呼吸灯用到的引脚 */
  GPIO_InitStructure.GPIO_Pin =  BRE_TIM_LED_PIN ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	// 复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init( BRE_TIM_LED_PORT, &GPIO_InitStructure );
}


/**
  * @brief  配置嵌套向量中断控制器NVIC
  * @param  无
  * @retval 无
  */
static void NVIC_Config_PWM(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  /* 配置TIM3_IRQ中断为中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = BRE_TIMx_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  配置TIM输出的PWM信号的模式，如周期、极性
  * @param  无
  * @retval 无
  */

static void TIMx_Mode_Config()
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;																				
	
	/* 设置TIM3CLK 时钟 */
	BRE_TIM_APBxClock_FUN ( BRE_TIM_CLK, ENABLE ); 

  /* 基本定时器配置 */		  
  TIM_TimeBaseStructure.TIM_Period = 1000-1;;       							  //当定时器从0计数到 TIM_Period+1 ，为一个定时周期
  TIM_TimeBaseStructure.TIM_Prescaler = 71;	    							//设置预分频
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;			//设置时钟分频系数：不分频(这里用不到)
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  	//向上计数模式
  TIM_TimeBaseInit(BRE_TIMx, &TIM_TimeBaseStructure);

  /* PWM模式配置 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    				//配置为PWM模式1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//使能输出
  TIM_OCInitStructure.TIM_Pulse = 0;				 						  			//设置初始PWM脉冲宽度为0	
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;  	  //当定时器计数值小于CCR1_Val时为低电平

  BRE_TIM_OCxInit ( BRE_TIMx, &TIM_OCInitStructure );	 									//使能通道

  BRE_TIM_OCxPreloadConfig ( BRE_TIMx, TIM_OCPreload_Enable );						//使能预装载	

  TIM_ARRPreloadConfig(BRE_TIMx, ENABLE);			 										//使能TIM重载寄存器ARR

  /* TIM3 enable counter */
  TIM_Cmd(BRE_TIMx, ENABLE);                   										//使能定时器	
	
	TIM_ITConfig(BRE_TIMx, TIM_IT_Update, ENABLE);										//使能update中断
		
	NVIC_Config_PWM();																					//配置中断优先级		
	BRE_TIMx->BRE_CCRx = 1000;
}

/**
  * @brief  TIM 初始化
  *         配置PWM模式和GPIO
  * @param  无
  * @retval 无
  */
void TIMx_Breathing_Init(void)
{
	TIMx_GPIO_Config();
	TIMx_Mode_Config();	
}



//速度配置函数
void Motor_Speed_Set(uint32_t Motor_Speed)
{ 
	BRE_TIMx->BRE_CCRx = Motor_Speed*100;
	Motor_Speed_Now = Motor_Speed;
	printf("占空比：%d%%\n",Motor_Speed*10);
}

//缓冲加减速函数
void Speed_cache(void)
{
	OS_ERR      err;
	extern uint8_t j;
	extern uint8_t cw;
	extern uint32_t S;
	uint8_t i = 0;
	
	i = j;
	while(1)
	{
		j--;
		if(j==0)
		{
			OSTimeDly ( 1000, OS_OPT_TIME_DLY, & err );
			TIM_Cmd(BRE_TIMx, DISABLE);  //减速完毕停止
			//反向
			if(cw == 1)
			{
				cw_ON();
				cw = 0;
			}
			else
			{
				cw_OFF();
				cw = 1;
			}
			OSTimeDly ( 500, OS_OPT_TIME_DLY, & err );
			TIM_Cmd(BRE_TIMx, ENABLE);
			while(!(i==j))
			{
				j++;
				OSTimeDly ( 1000, OS_OPT_TIME_DLY, & err );
				Motor_Speed_Set(j);
			}
			break;
		}
		else
		{
			OSTimeDly ( 1000, OS_OPT_TIME_DLY, & err );
			Motor_Speed_Set(j);
		}
	}

}



/*********************************************END OF FILE**********************/
