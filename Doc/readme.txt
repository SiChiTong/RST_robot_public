������
APP1   WIFI������ͨѶ����
APP2   ʵʱ�����ϴ�����
APP3   ת���ٶȵ���
APP4   �������
APP5   485ͨ���շ�
APP6   ���Ź�

�ж���Դ��
   �¼�					   ��Դ                    ���ȼ�
1.WIFIͨ���ж�             USART3
2.�����жϳ���             EXTI_Line2/EXTI_Line4
3.����ж�                 EXTI_Line5
4.PWM�������ж�          TIM8
5.֡�����жϼ�⣨modbus�� TIM1
6.485���մ����ж�          USART2
7.ADC�������ж�          ADC2
8.PWM�������жϼ������    TIM3    

����        	 		����                      ����  
PB5,0,1         LED
PC7   			������������Ƶ����              tim3
PB13 			���Ʒ������ţ�ʹ���ж�            ����������
				΢�뼶��ʱʹ�ö�ʱ��              tim5
PE6				�¶ȼ��					        ds18b20
PA9,PA10		���ڴ�ӡ                         usart
PB8,9,10,11     wifiͨ��						 esp8266
PD2,PA4         ������Ӧ����                     D2���ұ� A4�����
PA5             ���׮����Ӧ
PC1				adc                             ��ѹ�ɼ�
PA2(tx),3(rx)	485ͨ��	                        USART2
PC9             485ͨ��ʹ��
PD9				���̵�������
PB14		    24V��Դ����
				ͨ�Ž������                     tim1
PC6				pwmռ�ձȡ�Ƶ�ʻ�ȡ              tim8

���ݱ���������
1.��ص���״̬   �ṹ�� Bettery  
struct{
        u16 Bettery_current_charge;  		//������
        u16 Bettery_current_discharge;      //�ŵ����
        u16 Bettery_current_voltage;  	    //���ڵ�ѹ
        u8 Bettery_current_temperature1;    //�¶�1
        u8 Bettery_current_temperature2;    //�¶�2
		u8 Bettery_current_temperature3;    //�¶�3
		u8 Bettery_current_temperature4;    //�¶�4
		u16 Bettery_work_status;            //����״̬
		u16 Bettery_charge_count;           //������
		u16 Bettery_discharge_count;        //�ŵ����
		float Bettery_Energy;               //ʣ�����
    } Bettery;
�����ļ���bsp_task.c

2.ͨ��״̬����



