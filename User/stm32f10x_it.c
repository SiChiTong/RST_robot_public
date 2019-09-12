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
#include "includes.h"
#include "bsp_485.h"
#include "bsp_light_exit.h"
#include "bsp_variable.h"


//wifi中断函数
void macESP8266_USART_INT_FUN ( void )
{	
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
	
		WIFI.ucTcpClosedFlag = strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "CLOSED\r\n" ) ? 1 : 0;
		
  }	
}


//霍尔中断程序
void HUOER_IRQHandler(void)
{

  //确保是否产生了EXTI Line中断
 if(EXTI_GetITStatus(HUOER_INT_EXTI_LINE) != RESET) 
 {
	 Robot.interrupt_flag = 1;
	 EXTI->IMR &= ~(HUOER1_INT_EXTI_LINE);//屏蔽外部中断
	 EXTI->IMR &= ~(HUOER_INT_EXTI_LINE);//屏蔽外部中断
	 Robot.distance = 0;
	 EXTI_ClearITPendingBit(HUOER_INT_EXTI_LINE); //清除中断标志
	 
 }  
}


//EXTI->IMR &= ~(EXTI_Line4);//屏蔽外部中断
//EXTI->IMR &= ~(EXTI_Line2);
//EXTI->IMR |= EXTI_Line4;//开启中断
      

void HUOER1_IRQHandler(void)
{
	
  //确保是否产生了EXTI Line中断
 if(EXTI_GetITStatus(HUOER1_INT_EXTI_LINE) != RESET) 
 {
	 Robot.interrupt_flag = 1;
	 EXTI->IMR &= ~(HUOER_INT_EXTI_LINE);//屏蔽外部中断
	 EXTI->IMR &= ~(HUOER1_INT_EXTI_LINE);//屏蔽外部中断
	 Robot.distance = 680;
	 EXTI_ClearITPendingBit(HUOER1_INT_EXTI_LINE);     
 }  
}



//光电中断程序
void LIGHT_IRQHandler(void)
{
	OS_ERR      err;
  //确保是否产生了EXTI Line中断
 if(EXTI_GetITStatus(LIGHT_INT_EXTI_LINE) != RESET) 
 {
		printf("中断触发...\n");
		OSTimeDly ( 1000, OS_OPT_TIME_DLY, & err );//延时一秒再停止机器人
		TIM_Cmd(BRE_TIMx, DISABLE);
		Charge_OFF();
		EXTI_ClearITPendingBit(LIGHT_INT_EXTI_LINE); //清除中断标志
	 
 }  
}




/*
 * 如果是第一个上升沿中断，计数器会被复位，锁存到CCR1寄存器的值是0，CCR2寄存器的值也是0
 * 无法计算频率和占空比。当第二次上升沿到来的时候，CCR1和CCR2捕获到的才是有效的值。其中
 * CCR1对应的是周期，CCR2对应的是占空比。
 */
void ADVANCE_TIM_IRQHandler(void)
{
	u32 count_quan_value = 0;
	u16 count_value = 0;
  /* 清除中断标志位 */
  TIM_ClearITPendingBit(ADVANCE_TIM8, TIM_IT_CC1);
	count_value++;
	if(count_value == 675)
	{
		if(Robot.drection == 1)
		{
		count_quan_value++;
		count_value = 0;
		}
	}

}

//帧数据中断检测
void ADVANCE_TIM1_IRQHandler(void)
{
	if (TIM_GetITStatus(ADVANCE_TIM1, TIM_IT_Update) != RESET)
	{
		/*完成接收*/
		//modbus_rx();
		Battery_rx();
		TIM_Cmd(ADVANCE_TIM1, DISABLE);
		TIM_ClearITPendingBit(ADVANCE_TIM1, TIM_IT_Update);
	}	
}


// 串口中断服务函数
void RS485_USART_IRQHandler(void)
{
	 if(USART_GetITStatus(RS485_USARTx,USART_IT_RXNE) != RESET) 
			{ 
				TIM_Cmd(ADVANCE_TIM1, ENABLE);
				//计数器CNT->0
				ADVANCE_TIM1->CNT = 0;
				USART_ClearITPendingBit(RS485_USARTx,USART_IT_RXNE);				
				Usart485.USART2_RX_BUF[Usart485.USART2_RX_CNT] = USART_ReceiveData(RS485_USARTx); 
				Usart485.USART2_RX_CNT++; 			
			}		
}



//电压数据采集中断函数
void ADC_IRQHandler(void)
{	
	
	if (ADC_GetITStatus(ADCx,ADC_IT_EOC)==SET) 
	{
		// 读取ADC的转换值
		ADC_V.ConvertedValue = ADC_GetConversionValue(ADCx);
	}
	ADC_ClearITPendingBit(ADCx,ADC_IT_EOC);
}




//脉冲计数数，pwm发生中断函数
void  BRE_TIMx_IRQHandler (void)
{
	u32 Tim_counter=0;
	extern uint8_t cw;
	if ( TIM_GetITStatus( BRE_TIMx, TIM_IT_Update) != RESET ) 
	{	
		Tim_counter++;
		
			if(Tim_counter == 4000)//4000个脉冲
			{
				Tim_counter = 0;
				if(Robot.drection == 1)
				{
					Robot.distance += 2;
				}
				else
				{
					Robot.distance -= 2;
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
