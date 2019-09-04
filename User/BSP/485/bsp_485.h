#ifndef __RS485_H
#define	__RS485_H


#include "stm32f10x.h"
#include "bsp_tim.h"
#include <stdio.h>

/** 
  * 串口宏定义，不同的串口挂载的总线和IO不一样，移植时需要修改这几个宏
	* 1-修改总线时钟的宏，uart1挂载到apb2总线，其他uart挂载到apb1总线
	* 2-修改GPIO的宏
  */
	
// 串口1-USART1
#define  RS485_USARTx                   USART2
#define  RS485_USART_CLK                RCC_APB1Periph_USART2
#define  RS485_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
#define  RS485_USART_BAUDRATE           9600

// USART GPIO 引脚宏定义
#define  RS485_USART_GPIO_CLK           (RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC)
#define  RS485_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define  RS485_USART_TX_GPIO_PORT       GPIOA  
#define  RS485_USART_TX_GPIO_PIN        GPIO_Pin_2

#define  RS485_USART_RX_GPIO_PORT       GPIOA
#define  RS485_USART_RX_GPIO_PIN        GPIO_Pin_3

#define  RS485_USART_EN_GPIO_PORT       GPIOC
#define  RS485_USART_EN_GPIO_PIN        GPIO_Pin_9

#define  RS485_USART_IRQ                USART2_IRQn
#define  RS485_USART_IRQHandler         USART2_IRQHandler



static void RS485_Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}

#define RS485_RX_ENABLE 		 	 RS485_Delay(1000);GPIO_ResetBits(RS485_USART_EN_GPIO_PORT,RS485_USART_EN_GPIO_PIN);RS485_Delay(1000);
#define RS485_TX_ENABLE			 	 GPIO_SetBits(RS485_USART_EN_GPIO_PORT,RS485_USART_EN_GPIO_PIN);RS485_Delay(1000);


// 串口2-USART2
//#define  DEBUG_USARTx                   USART2
//#define  DEBUG_USART_CLK                RCC_APB1Periph_USART2
//#define  DEBUG_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
//#define  DEBUG_USART_BAUDRATE           115200

//// USART GPIO 引脚宏定义
//#define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOA)
//#define  DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
//    
//#define  DEBUG_USART_TX_GPIO_PORT       GPIOA   
//#define  DEBUG_USART_TX_GPIO_PIN        GPIO_Pin_2
//#define  DEBUG_USART_RX_GPIO_PORT       GPIOA
//#define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_3

//#define  DEBUG_USART_IRQ                USART2_IRQn
//#define  DEBUG_USART_IRQHandler         USART2_IRQHandler

// 串口3-USART3
//#define  DEBUG_USARTx                   USART3
//#define  DEBUG_USART_CLK                RCC_APB1Periph_USART3
//#define  DEBUG_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
//#define  DEBUG_USART_BAUDRATE           115200

//// USART GPIO 引脚宏定义
//#define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOB)
//#define  DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
//    
//#define  DEBUG_USART_TX_GPIO_PORT       GPIOB   
//#define  DEBUG_USART_TX_GPIO_PIN        GPIO_Pin_10
//#define  DEBUG_USART_RX_GPIO_PORT       GPIOB
//#define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_11

//#define  DEBUG_USART_IRQ                USART3_IRQn
//#define  DEBUG_USART_IRQHandler         USART3_IRQHandler

// 串口4-UART4
//#define  DEBUG_USARTx                   UART4
//#define  DEBUG_USART_CLK                RCC_APB1Periph_UART4
//#define  DEBUG_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
//#define  DEBUG_USART_BAUDRATE           115200

//// USART GPIO 引脚宏定义
//#define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOC)
//#define  DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
//    
//#define  DEBUG_USART_TX_GPIO_PORT       GPIOC   
//#define  DEBUG_USART_TX_GPIO_PIN        GPIO_Pin_10
//#define  DEBUG_USART_RX_GPIO_PORT       GPIOC
//#define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_11

//#define  DEBUG_USART_IRQ                UART4_IRQn
//#define  DEBUG_USART_IRQHandler         UART4_IRQHandler


// 串口5-UART5
//#define  DEBUG_USARTx                   UART5
//#define  DEBUG_USART_CLK                RCC_APB1Periph_UART5
//#define  DEBUG_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
//#define  DEBUG_USART_BAUDRATE           115200

//// USART GPIO 引脚宏定义
//#define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD)
//#define  DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
//    
//#define  DEBUG_USART_TX_GPIO_PORT       GPIOC   
//#define  DEBUG_USART_TX_GPIO_PIN        GPIO_Pin_12
//#define  DEBUG_USART_RX_GPIO_PORT       GPIOD
//#define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_2

//#define  DEBUG_USART_IRQ                UART5_IRQn
//#define  DEBUG_USART_IRQHandler         UART5_IRQHandler

void RS485_USART_Config(void);
void RS485_Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
void RS485_Usart_SendArray( USART_TypeDef * pUSARTx, uint8_t *array, uint16_t num);

void RS485_Usart_SendString( USART_TypeDef * pUSARTx, char *str);
void RS485_Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch);

#endif /* __RS485_H */
