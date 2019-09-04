#include "bsp_tim.h"
#include "includes.h"
//�����ж����ȼ�

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



// �ж����ȼ�����
static void GENERAL_TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // �����ж���Ϊ0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);		
		// �����ж���Դ
    NVIC_InitStructure.NVIC_IRQChannel = GENERAL_TIM_IRQ ;	
		// ���������ȼ�Ϊ 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
	  // ������ռ���ȼ�Ϊ3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

static void GENERAL_TIM_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;		
		// ������ʱ��ʱ��,���ڲ�ʱ��CK_INT=72M
    GENERAL_TIM_APBxClock_FUN(GENERAL_TIM_CLK, ENABLE);	
		// �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
    TIM_TimeBaseStructure.TIM_Period=GENERAL_TIM_Period;
	  // ʱ��Ԥ��Ƶ��
    TIM_TimeBaseStructure.TIM_Prescaler= GENERAL_TIM_Prescaler;	
		// ʱ�ӷ�Ƶ���� ��û�õ����ù�
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
		// ����������ģʽ������Ϊ���¼���
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Down; 		
		// �ظ���������ֵ��û�õ����ù�
		TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	  // ��ʼ����ʱ��
    TIM_TimeBaseInit(GENERAL_TIM, &TIM_TimeBaseStructure);
	
		// ����������жϱ�־λ
    TIM_ClearFlag(GENERAL_TIM, TIM_FLAG_Update);
	  	
		// ʹ�ܼ�����
		TIM_Cmd(GENERAL_TIM, ENABLE);
}

void GENERAL_TIM4_Init(void)
{
	GENERAL_TIM_NVIC_Config();
	GENERAL_TIM_Mode_Config();		
}


