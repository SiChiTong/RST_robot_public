#ifndef HUOER__EXTI_H
#define	HUOER__EXTI_H


#include "stm32f10x.h"


//���Ŷ��壬���������жϵ����ţ����ߵ͵�ƽ
#define Distance_INT_GPIO_PORT         GPIOC
#define Distance_INT_GPIO_CLK          (RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO)//��APB2ʹ��ʱ���Լ�AFIO
#define Distance_INT_GPIO_PIN          GPIO_Pin_7


//�����жϼĴ������ж�Դ
#define Distance_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOC//�ж�Դ�Ĵ���A
#define Distance_INT_EXTI_PINSOURCE    GPIO_PinSource7//GPIO�˿�0
#define Distance_INT_EXTI_LINE         EXTI_Line7  //��Ӧ�˿��ж���
#define Distance_INT_EXTI_IRQ          EXTI9_5_IRQn //nvic�ж�Դ

#define Distance_IRQHandler            EXTI9_5_IRQHandler

                    


void EXTI_Distance_Config(void);


#endif /* __EXTI_H */
