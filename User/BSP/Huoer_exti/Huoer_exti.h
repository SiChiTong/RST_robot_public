#ifndef HUOER__EXTI_H
#define	HUOER__EXTI_H


#include "stm32f10x.h"


//���Ŷ��壬���������жϵ����ţ����ߵ͵�ƽ
#define HUOER_INT_GPIO_PORT         GPIOA
#define HUOER_INT_GPIO_CLK          (RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO)//��APB2ʹ��ʱ���Լ�AFIO
#define HUOER_INT_GPIO_PIN          GPIO_Pin_2


//�����жϼĴ������ж�Դ
#define HUOER_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOA//�ж�Դ�Ĵ���A
#define HUOER_INT_EXTI_PINSOURCE    GPIO_PinSource2//GPIO�˿�0
#define HUOER_INT_EXTI_LINE         EXTI_Line2  //��Ӧ�˿��ж���
#define HUOER_INT_EXTI_IRQ          EXTI2_IRQn //nvic�ж�Դ

#define HUOER_IRQHandler            EXTI2_IRQHandler

#define HUOER1_INT_GPIO_PORT         GPIOA
#define HUOER1_INT_GPIO_CLK          (RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO)//��APB2ʹ��ʱ���Լ�AFIO
#define HUOER1_INT_GPIO_PIN          GPIO_Pin_4


#define HUOER1_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOA//�ж�Դ�Ĵ���A
#define HUOER1_INT_EXTI_PINSOURCE    GPIO_PinSource4//GPIO�˿�0
#define HUOER1_INT_EXTI_LINE         EXTI_Line4  //��Ӧ�˿��ж���
#define HUOER1_INT_EXTI_IRQ          EXTI4_IRQn //nvic�ж�Դ
//�ж�ִ�к���


#define HUOER1_IRQHandler            EXTI4_IRQHandler
                    


void EXTI_HUOER_Config(void);


#endif /* __EXTI_H */
