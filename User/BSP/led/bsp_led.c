#include "bsp_led.h"   



static void                         LED_GPIO_Config                  ( void );



 /**
  * @brief  ���� LED �� GPIO ����
  * @param  ��
  * @retval ��
  */
static void LED_GPIO_Config ( void )
{		
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure;


	/* ���� LED1 ���� */
	RCC_APB2PeriphClockCmd ( macLED1_GPIO_CLK, ENABLE ); 															   
	GPIO_InitStructure.GPIO_Pin = macLED1_GPIO_PIN;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init ( macLED1_GPIO_PORT, & GPIO_InitStructure );	

	/* ���� LED2 ���� */
	RCC_APB2PeriphClockCmd ( macLED2_GPIO_CLK, ENABLE ); 															   
	GPIO_InitStructure.GPIO_Pin = macLED2_GPIO_PIN;	
	GPIO_Init ( macLED2_GPIO_PORT, & GPIO_InitStructure );	

	/* ���� LED3 ���� */
	RCC_APB2PeriphClockCmd ( macLED3_GPIO_CLK, ENABLE ); 															   
	GPIO_InitStructure.GPIO_Pin = macLED3_GPIO_PIN;	
	GPIO_Init ( macLED3_GPIO_PORT, & GPIO_InitStructure );		
	
	//�������PB13����
	RCC_APB2PeriphClockCmd ( cw_GPIO_CLK, ENABLE ); 															   
	GPIO_InitStructure.GPIO_Pin = cw_GPIO_PIN;	
	GPIO_Init ( cw_GPIO_PORT, & GPIO_InitStructure );	
	
	//24V����PB14����
	RCC_APB2PeriphClockCmd ( V24_GPIO_CLK, ENABLE ); 															   
	GPIO_InitStructure.GPIO_Pin = V24_GPIO_PIN;	
	GPIO_Init ( V24_GPIO_PORT, & GPIO_InitStructure );	
	  		
	//������PD9����
	RCC_APB2PeriphClockCmd ( Charge_GPIO_CLK, ENABLE ); 															   
	GPIO_InitStructure.GPIO_Pin = Charge_GPIO_PIN;	
	GPIO_Init ( Charge_GPIO_PORT, & GPIO_InitStructure );	
}


 /**
  * @brief  LED ��ʼ������
  * @param  ��
  * @retval ��
  */
void LED_Init ( void )
{
  LED_GPIO_Config ();
	V24_OFF();
	macLED1_OFF();
	macLED2_OFF();
	macLED3_OFF();
	cw_OFF();
	Charge_ON();//���̵���ʹ�ܵ͵�ƽ
}



/*********************************************END OF FILE**********************/
