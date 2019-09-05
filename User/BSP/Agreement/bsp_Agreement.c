#include "bsp_Agreement.h"
#include "stm32f10x_it.h"
#include "includes.h"
#include "bsp_led.h"
#include "bsp_tim_pwm.h"

OS_ERR     err;


/*motor speed control function */

uint32_t S = 0;//����ֵ

char data_send[100] ={0};
uint8_t data;//���������ϴ�

uint8_t cw = 1;//����
uint8_t j =0;//�ٶȷֵ�����
uint8_t flag =0,flag1 =0;//Ѳ�����ͱ�־λ


volatile uint8_t ucTcpClosedFlag = 0;//TCP�Ͽ�����״̬��־λ
volatile uint8_t wifi_link_Status = 0;//WIFI����״̬��־λ
volatile uint8_t tcp_link_Status = 0;//TCP����״̬��־λ

extern __IO uint16_t ADC_ConvertedValue;//��ѹԭʼ����
extern float ADC_ConvertedValueLocal;//�����ѹ���� 
extern float vi;//��ѹ��

/*��������*/
void dt_send(uint16_t a,uint8_t b);//�ظ�010Aָ��
void dt_send1(uint16_t a,uint8_t b);//�ظ�010Bָ��
void dt_send2(uint16_t a,uint8_t b);//�ظ�010Dָ��



static void jiasu(u8 j)
{
	if(j >= 0)
	{
		if(j<=10)
		{
			data=10 - j;
		  dt_send(data,4);
			Motor_Speed_Set(j);
		}
	}
		
}


static void jiansu(u8 j)
{
	if(j >= 0)
	{
		if(j<=10)
		{
			data=10 - j;
		  dt_send(data,5);
			Motor_Speed_Set(j);
		}
	}
		
}




 
 void tcp_Agreement(char * pRecStr)
 {
	  char cStr [ 100 ] = { 0 };
		if(*(pRecStr+2)=='0'&&*(pRecStr+3)=='A')
			{ 
			 if(*(pRecStr+4)=='0'&&*(pRecStr+5)=='1')//�Զ�Ѳ��flag1=1   
				 {
					 if(flag1==1)
					 {
						 ;
					 }
					 else
						{
					 flag1 =1;
					 flag = 1;//�ж��Ƿ������˶�������ֹͣ  1��ʾ�˶�  0��ʾֹͣ
					 Motor_Speed_Set(3);
						data=3;
						dt_send(data,1);
					 }
					 
				 }
				 
         else if(*(pRecStr+4)=='0'&&*(pRecStr+5)=='2')//�˹�Ѳ��flag1 ==0
				 {
					 if(flag1 ==1&&flag == 1 )
					 {
						 j=0;
						 flag1 = 0;
						 TIM_Cmd(BRE_TIMx, DISABLE);
						 flag = 0;
						 data=2;
						dt_send(data,2);
					 }
						 else
						{
							;
						}
				 }
		
				else if(*(pRecStr+4)=='0'&&*(pRecStr+5)=='3'&&flag1== 0&&flag ==0)//����������
				 {	
					TIM_Cmd(BRE_TIMx, ENABLE); 
					flag=1;
					j=10;
					data=1;
					dt_send(data,3);//data��ʾ��������  3��ʾ����ָ��
					 
				 }

				 
				 else if(*(pRecStr+4)=='0'&&*(pRecStr+5)=='4'&&flag1 ==0&&flag==1)//����ָ��
				 {
					 j--;//j��ʾ���ٶ����Ӧ���Լ��趨����ֵ��
					 jiasu(j);
				 }
				 else if(*(pRecStr+4)=='0'&&*(pRecStr+5)=='5'&&flag1 ==0&&flag==1)//����ָ��
				 {
					 j++;
					 jiansu(j);														 
				 }
				 else if(*(pRecStr+4)=='0'&&*(pRecStr+5)=='6'&&flag1 ==0&&flag==1)//����ָ��
				 {
						cw=1;
					  Delay_us(10000);
					  cw_OFF();
					  data=1;
					  dt_send(data,6);
				 }
				 else if(*(pRecStr+4)=='0'&&*(pRecStr+5)=='7'&&flag1 ==0&&flag==1)//����ָ��
				 {
						cw=0;
					  Delay_us(10000);
					  cw_ON();
					  data=2;
					  dt_send(data,7);
				 }
				 else if(*(pRecStr+4)=='0'&&*(pRecStr+5)=='8'&&flag1 ==0&&flag==1)//ָֹͣ��
				 {
					 flag=0;
					 TIM_Cmd(BRE_TIMx, DISABLE); 
					 data=1;
					 dt_send(data,8);
 }
				 //��λ����ָ��
				 else if(*(pRecStr+4)=='0'&&*(pRecStr+5)=='9')
				 {
						 data=1;
						dt_send(data,9);
						OSTimeDly ( 1000, OS_OPT_TIME_DLY, & err );//��ʱ1000ms
						__set_FAULTMASK(1);//�ر������ж�
						NVIC_SystemReset();//��λ����	 
				 }
				 //���ָ��
				 else if(*(pRecStr+4)=='0'&&*(pRecStr+5)=='A')
				 {
					  EXTI->IMR &= ~(HUOER_INT_EXTI_LINE);//�����ⲿ�ж�
						EXTI->IMR &= ~(HUOER1_INT_EXTI_LINE);//�����ⲿ�ж�
						Motor_Speed_Set(3);
						cw_OFF();
						cw = 1; 
				 }
				else 
				 {
					 data=1;
					dt_send2(data,6);
				 }
			 }
		
			else
			{
				sprintf(cStr,"ָ������");
				data=2;
				dt_send(data,8);
				ESP8266_SendString ( ENABLE, cStr, 0, Single_ID_0 );		
			}
			
				
		
 }
		

	 
 


 
 //��16������ת���ɶ�Ӧ�ַ���
void HexToStr(char *pbDest, char *pbSrc, int nLen)
{
	char	ddl,ddh;
	int i;

	for (i=0; i<nLen; i++)
	{
		ddh = 48 + pbSrc[i] / 16;
		ddl = 48 + pbSrc[i] % 16;
		if (ddh > 57) ddh = ddh + 7;
		if (ddl > 57) ddl = ddl + 7;
		pbDest[i*2] = ddh;
		pbDest[i*2+1] = ddl;
	}

		pbDest[nLen*2] = '\0';
		ESP8266_SendString ( ENABLE, pbDest,nLen*2 , Single_ID_0 );
		return;
}

void HexToStr1(char *pbDest, char *pbSrc, int nLen)
{
		char	ddl,ddh;
		int i;

	for (i=0; i<nLen; i++)
	{
		ddh = 48 + pbSrc[i] / 16;
		ddl = 48 + pbSrc[i] % 16;
		if (ddh > 57) ddh = ddh + 7;
		if (ddl > 57) ddl = ddl + 7;
		pbDest[i*2] = ddh;
		pbDest[i*2+1] = ddl;
	}

	pbDest[nLen*2] = '\0';
	return;
}

	//���������ϴ��Զ�����У��λ
void dt_send(uint16_t a,uint8_t b)//a��������λ�bb����ָ��
{
		char c[100]={0};
		char*str=c;
		
		uint8_t sum=0;
		uint8_t i;
		uint8_t cnt=0;
		data_send[cnt++]  = 0x01;
		data_send[cnt++] = 0x0A;
		data_send[cnt++] = b;
		
		data_send[cnt++] = 0;
		
		data_send[cnt++] = (a>>8)&0xff;
		data_send[cnt++] = a&0xff;
		data_send[3] = (cnt-4)+(cnt-4);
		for( i=0;i<cnt;i++)
		{
			sum+=data_send[i];
		
		}
		data_send[cnt++] = sum;
		 HexToStr(str,data_send,  cnt);
		//sstr=s_str(data_send,cnt,str);
		
	
}

//�Զ������ϴ�����У��λ1

void dt_send1(uint16_t a,uint8_t b)//a��������λ�bb����ָ��
{
	char c[100]={0};
	char*str=c;
  
	uint8_t sum=0;
	uint8_t i;
	uint8_t cnt=0;
	data_send[cnt++] = 0x01;
	data_send[cnt++] = 0x0B;
	data_send[cnt++] = b;
	
	data_send[cnt++] = 0;
	
	data_send[cnt++] = (a>>8)&0xff;
	data_send[cnt++] = a&0xff;
	data_send[3] = (cnt-4)+(cnt-4);
	for( i=0;i<cnt;i++)
	{
		sum+=data_send[i];
	
	}
	data_send[cnt++] = sum;
	 HexToStr(str,data_send,  cnt);
	//sstr=s_str(data_send,cnt,str);
	
	
}




void dt_send2(uint16_t a,uint8_t b)//a��������λ�bb����ָ��
{
	char c[100]={0};
	char*str=c;
  
	uint8_t sum=0;
	uint8_t i;
	uint8_t cnt=0;
	data_send[cnt++] = 0x01;
	data_send[cnt++] = 0x0D;
	data_send[cnt++] = b;
	
	data_send[cnt++] = 0;
	
	data_send[cnt++] = (a>>8)&0xff;
	data_send[cnt++] = a&0xff;
	data_send[3] = (cnt-4)+(cnt-4);
	for( i=0;i<cnt;i++)
	{
		sum+=data_send[i];
	
	}
	data_send[cnt++] = sum;
	 HexToStr(str,data_send,  cnt);
	//sstr=s_str(data_send,cnt,str);
}




