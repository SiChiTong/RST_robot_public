#ifndef LIGHT__EXTI_H
#define	LIGHT__EXTI_H


#include "stm32f10x.h"


//���Ŷ��壬���������жϵ����ţ����ߵ͵�ƽ
#define LIGHT_INT_GPIO_PORT         GPIOA
#define LIGHT_INT_GPIO_CLK          (RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO)//��APB2ʹ��ʱ���Լ�AFIO
#define LIGHT_INT_GPIO_PIN          GPIO_Pin_5


//�����жϼĴ������ж�Դ
#define LIGHT_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOA//�ж�Դ�Ĵ���A
#define LIGHT_INT_EXTI_PINSOURCE    GPIO_PinSource5//GPIO�˿�0
#define LIGHT_INT_EXTI_LINE         EXTI_Line5  //��Ӧ�˿��ж���
#define LIGHT_INT_EXTI_IRQ          EXTI9_5_IRQn //nvic�ж�Դ

//�ж�ִ�к���

#define LIGHT_IRQHandler            EXTI9_5_IRQHandler
                    


void EXTI_LIGHT_Config(void);


#endif /* __LIGHT_H */
