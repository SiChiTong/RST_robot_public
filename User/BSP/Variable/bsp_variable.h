#ifndef _H_VARIABLE_H_
#define	_H_VARIABLE_H_

#include "stm32f10x.h"
/*��ز����ṹ�����*/
extern struct battery {
        u16 current_charge;  		//������
        u16 current_discharge;  //�ŵ����
        u16 current_voltage;  	//���ڵ�ѹ
        u8 current_temperature1;//�¶�1
        u8 current_temperature2;//�¶�2
				u8 current_temperature3;//�¶�3
				u8 current_temperature4;//�¶�4
				u16 work_status;        //����״̬
				u16 charge_count;       //������
				u16 discharge_count;    //�ŵ����
				float energy;           //ʣ�����
 }Battery;

		/*MODBUS�����ṹ��*/
extern struct modbustx {
	u8 adress;           //��ַ
	u8 function;          //����
	u16 register_adress; //�Ĵ���
	u16 register_number; //�Ĵ�������
} Modbustx;


/*������״̬�����ṹ��*/
extern struct robot{
	u32 tim_counter;     //�жϼ���
	u8 speed_now;    	   //ʵʱ�ٶ�
	u8 drection;         //����,��ʼֵ1
	u8 grading;          //�ٶȷֵ�����,��ʼֵ10
	u32 distance;        //����ֵ
	u8 interrupt_flag;   //�жϱ�־
	u8 inspection_mode;  //Ѳ��ģʽ
	u8 running_status;   //����״̬
}Robot;

/*����ͨ�Ų����ṹ��*/
extern struct usart{
	u8 USART2_RX_BUF[128];    //��������
	u8 USART2_RX_CNT;    	   //�������ֵ
}Usart485;


/*ADC�ɼ������ṹ��*/
extern struct adc{
	__IO uint16_t ConvertedValue;  //��ѹԭʼ����
	float ConvertedValueLocal;     //adc����ֵ
	float vi;                      //adc�ɼ�����ѹֵ
}ADC_V;


//��eeprom���зֿ�
//#define EEP_IP_Adder    0x00     //IP��ַ
//#define EEP_Port_Adder    0x20     //�˿ڵ�ַ
//#define EEP_ApSsid_Adder    0x28     //WIFI���Ƶ�ַ
//#define EEP_ApPwd_Adder   0x38    //WIFI�����ַ

/*WIFI����״̬�����ṹ��*/
extern struct wifi{
	char   ApSsid_eeprom[32];         //Ҫ���ӵ��ȵ������
	char   ApPwd_eeprom [16];         //Ҫ���ӵ��ȵ����Կ
	char   TcpServer_IP_eeprom[16];   //Ҫ���ӵķ�������IP
	char   TcpServer_Port_eeprom[8];  //Ҫ���ӵķ������Ķ˿�
	
	volatile uint8_t ucTcpClosedFlag;//TCP�Ͽ�����״̬��־λ
	volatile uint8_t wifi_link_Status;//WIFI����״̬��־λ
	volatile uint8_t tcp_link_Status;//TCP����״̬��־λ
}WIFI;




#endif /* __VARIABLE_H */
		

