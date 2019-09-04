#include "stm32f10x.h"
#include "bsp_tim_pwm.h"
#include "bsp_usart.h"
#include "includes.h"


uint32_t Motor_Speed_Counter = 0;
uint8_t Motor_Speed_Now = 0;


 /**
  * @brief  ����TIM�������PWMʱ�õ���I/O
  * @param  ��
  * @retval ��
  */
static void TIMx_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /*  clock enable */
  RCC_APB2PeriphClockCmd(BRE_TIM_GPIO_CLK, ENABLE); 
  BRE_TIM_GPIO_APBxClock_FUN  ( BRE_TIM_GPIO_CLK, ENABLE );
		
	GPIO_AFIODeInit();//��ȫ��ӳ��ǰ��ʼ���Ĵ���
	BRE_GPIO_REMAP_FUN(); //��ȫ��ӳ�� 

  /* ���ú������õ������� */
  GPIO_InitStructure.GPIO_Pin =  BRE_TIM_LED_PIN ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	// �����������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init( BRE_TIM_LED_PORT, &GPIO_InitStructure );
}


/**
  * @brief  ����Ƕ�������жϿ�����NVIC
  * @param  ��
  * @retval ��
  */
static void NVIC_Config_PWM(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  /* ����TIM3_IRQ�ж�Ϊ�ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = BRE_TIMx_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  ����TIM�����PWM�źŵ�ģʽ�������ڡ�����
  * @param  ��
  * @retval ��
  */

static void TIMx_Mode_Config()
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;																				
	
	/* ����TIM3CLK ʱ�� */
	BRE_TIM_APBxClock_FUN ( BRE_TIM_CLK, ENABLE ); 

  /* ������ʱ������ */		  
  TIM_TimeBaseStructure.TIM_Period = 1000-1;;       							  //����ʱ����0������ TIM_Period+1 ��Ϊһ����ʱ����
  TIM_TimeBaseStructure.TIM_Prescaler = 71;	    							//����Ԥ��Ƶ
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;			//����ʱ�ӷ�Ƶϵ��������Ƶ(�����ò���)
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  	//���ϼ���ģʽ
  TIM_TimeBaseInit(BRE_TIMx, &TIM_TimeBaseStructure);

  /* PWMģʽ���� */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    				//����ΪPWMģʽ1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//ʹ�����
  TIM_OCInitStructure.TIM_Pulse = 0;				 						  			//���ó�ʼPWM������Ϊ0	
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;  	  //����ʱ������ֵС��CCR1_ValʱΪ�͵�ƽ

  BRE_TIM_OCxInit ( BRE_TIMx, &TIM_OCInitStructure );	 									//ʹ��ͨ��

  BRE_TIM_OCxPreloadConfig ( BRE_TIMx, TIM_OCPreload_Enable );						//ʹ��Ԥװ��	

  TIM_ARRPreloadConfig(BRE_TIMx, ENABLE);			 										//ʹ��TIM���ؼĴ���ARR

  /* TIM3 enable counter */
  TIM_Cmd(BRE_TIMx, ENABLE);                   										//ʹ�ܶ�ʱ��	
	
	TIM_ITConfig(BRE_TIMx, TIM_IT_Update, ENABLE);										//ʹ��update�ж�
		
	NVIC_Config_PWM();																					//�����ж����ȼ�		
	BRE_TIMx->BRE_CCRx = 1000;
}

/**
  * @brief  TIM ��ʼ��
  *         ����PWMģʽ��GPIO
  * @param  ��
  * @retval ��
  */
void TIMx_Breathing_Init(void)
{
	TIMx_GPIO_Config();
	TIMx_Mode_Config();	
}



//�ٶ����ú���
void Motor_Speed_Set(uint32_t Motor_Speed)
{ 
	BRE_TIMx->BRE_CCRx = Motor_Speed*100;
	Motor_Speed_Now = Motor_Speed;
	printf("ռ�ձȣ�%d%%\n",Motor_Speed*10);
}

//����Ӽ��ٺ���
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
			TIM_Cmd(BRE_TIMx, DISABLE);  //�������ֹͣ
			//����
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
