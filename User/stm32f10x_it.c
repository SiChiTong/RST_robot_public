/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTI
  
  AL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "bsp_tim_pwm.h"
#include "bsp_esp8266.h"
#include <stdio.h>
#include <string.h> 
#include "Huoer_exti.h"
#include "bsp_led.h"
#include "bsp_adc.h"
#include "includes.h"
#include "bsp_485.h"
#include "bsp_light_exit.h"



uint8_t Speed_cache_flag =0;
extern uint32_t S;
extern __IO uint16_t ADC_ConvertedValue;
extern float ADC_ConvertedValueLocal;
extern uint8_t cw;
u8 USART2_RX_BUF[64];
u8 USART2_RX_CNT = 0;
u8 RX485_flag1 = 0;
u16 count_value = 0;
u32 count_quan_value = 0;
u32 Tim_counter=0;
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/


//wifi�жϺ���
void macESP8266_USART_INT_FUN ( void )
{	
	extern volatile uint8_t ucTcpClosedFlag;
	uint8_t ucCh;
	
	if ( USART_GetITStatus ( macESP8266_USARTx, USART_IT_RXNE ) != RESET )
	{
		ucCh  = USART_ReceiveData( macESP8266_USARTx );

		if ( strEsp8266_Fram_Record .InfBit .FramLength < ( RX_BUF_MAX_LEN - 1 ) )                      
			strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ++ ]  = ucCh;

	}
	 	 
	if ( USART_GetITStatus( macESP8266_USARTx, USART_IT_IDLE ) == SET )                                         
	{
    strEsp8266_Fram_Record .InfBit .FramFinishFlag = 1;
		
		ucCh = USART_ReceiveData( macESP8266_USARTx );                                                             
	
		ucTcpClosedFlag = strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "CLOSED\r\n" ) ? 1 : 0;
		
  }	
}


//�����жϳ���
void HUOER_IRQHandler(void)
{

  //ȷ���Ƿ������EXTI Line�ж�
 if(EXTI_GetITStatus(HUOER_INT_EXTI_LINE) != RESET) 
 {
	 Speed_cache_flag = 1;
	 EXTI->IMR &= ~(HUOER1_INT_EXTI_LINE);//�����ⲿ�ж�
	 EXTI->IMR &= ~(HUOER_INT_EXTI_LINE);//�����ⲿ�ж�
	 S = 0;
	EXTI_ClearITPendingBit(HUOER_INT_EXTI_LINE); //����жϱ�־
	 
 }  
}


//EXTI->IMR &= ~(EXTI_Line4);//�����ⲿ�ж�
//EXTI->IMR &= ~(EXTI_Line2);
//EXTI->IMR |= EXTI_Line4;//�����ж�
      

void HUOER1_IRQHandler(void)
{
	
  //ȷ���Ƿ������EXTI Line�ж�
 if(EXTI_GetITStatus(HUOER1_INT_EXTI_LINE) != RESET) 
 {
	 Speed_cache_flag = 1;
	 EXTI->IMR &= ~(HUOER_INT_EXTI_LINE);//�����ⲿ�ж�
	 EXTI->IMR &= ~(HUOER1_INT_EXTI_LINE);//�����ⲿ�ж�
	 S = 680;
	 EXTI_ClearITPendingBit(HUOER1_INT_EXTI_LINE);     
 }  
}



//����жϳ���
void LIGHT_IRQHandler(void)
{
	OS_ERR      err;
  //ȷ���Ƿ������EXTI Line�ж�
 if(EXTI_GetITStatus(LIGHT_INT_EXTI_LINE) != RESET) 
 {
		printf("�жϴ���...\n");
		OSTimeDly ( 1000, OS_OPT_TIME_DLY, & err );//��ʱһ����ֹͣ������
		TIM_Cmd(BRE_TIMx, DISABLE);
		Charge_OFF();
		EXTI_ClearITPendingBit(LIGHT_INT_EXTI_LINE); //����жϱ�־
	 
 }  
}




/*
 * ����ǵ�һ���������жϣ��������ᱻ��λ�����浽CCR1�Ĵ�����ֵ��0��CCR2�Ĵ�����ֵҲ��0
 * �޷�����Ƶ�ʺ�ռ�ձȡ����ڶ��������ص�����ʱ��CCR1��CCR2���񵽵Ĳ�����Ч��ֵ������
 * CCR1��Ӧ�������ڣ�CCR2��Ӧ����ռ�ձȡ�
 */
void ADVANCE_TIM_IRQHandler(void)
{
  /* ����жϱ�־λ */
  TIM_ClearITPendingBit(ADVANCE_TIM, TIM_IT_CC1);
	count_value++;
	if(count_value == 675)
	{
		if(cw == 1)
		{
		count_quan_value++;
		count_value = 0;
		}
	}

}









// �����жϷ�����
void RS485_USART_IRQHandler(void)
{
	 if(USART_GetITStatus(RS485_USARTx,USART_IT_RXNE) != RESET) 
			{ 
				USART_ClearITPendingBit(RS485_USARTx,USART_IT_RXNE);				
				RX485_flag1 = 1;
				USART2_RX_BUF[USART2_RX_CNT] = USART_ReceiveData(RS485_USARTx); 
				USART2_RX_CNT++; 			
				
			}		
}



//��ѹ���ݲɼ��жϺ���
void ADC_IRQHandler(void)
{	
	
	if (ADC_GetITStatus(ADCx,ADC_IT_EOC)==SET) 
	{
		// ��ȡADC��ת��ֵ
		ADC_ConvertedValue = ADC_GetConversionValue(ADCx);
	}
	ADC_ClearITPendingBit(ADCx,ADC_IT_EOC);
}




//�����������pwm�����жϺ���
void  BRE_TIMx_IRQHandler (void)
{
	extern uint8_t cw;
	if ( TIM_GetITStatus( BRE_TIMx, TIM_IT_Update) != RESET ) 
	{	
		Tim_counter++;
		
			if(Tim_counter == 4000)//4000������
			{
				Tim_counter = 0;
				if(cw == 1)
				{
					S += 2;
				}
				else
				{
					S -= 2;
				}
			}
		
		TIM_ClearITPendingBit(BRE_TIMx , TIM_FLAG_Update);  		 
	}
}







/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
