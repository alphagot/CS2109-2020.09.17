#include "DP80390CPU.H"
#include "common.h"
#include "register.h"
#include "intrins.h"

#define LED_ON()	PCREG &= ~0x08
#define LED_OFF()	PCREG |= 0x08


unsigned char uartr_flag,uartadd,uartdate;
unsigned char ADC_VALUE1;
unsigned char ADC_VALUE2;


void delay_20us(unsigned char N);
void MCU_Init();
void uart();
void ser_int();


void main()
{
	MCU_Init();
	ser_int();

	uartr_flag = 0;
	while(1)
	{
	  	WDCON |= 0x10;
		uart();
		delay_20us(250);
		delay_20us(250);
		delay_20us(250);
		delay_20us(250);
		delay_20us(250);
		delay_20us(250);
		delay_20us(250);
		delay_20us(250);
		delay_20us(250);
		delay_20us(250);
		delay_20us(250);
		delay_20us(250);
		  
	}	


}

void MCU_Init()
{
	SYS_REG_MASK = 0xA5;
	SYS_REG1 = 0x58; 	 //�򿪿��Ź�ʹ�ܣ��򿪵�ѹ��λ���򿪸�������
	SYS_REG3 = 0x10; 	 	
//	SYS_REG2 = 0xC2;  	 //����ʱ��4��Ƶ������ģʽ		
//	SYS_REG4 = 0x0c;	 //PWM�ø���ʱ��4��Ƶ
	SYS_REG2 = 0x03;	 //����ʱ�ӣ�����ģʽ
	SYS_REG4 = 0x01;	 //PWM�ø���ʱ��
	LXT_START = 0X00;

	IE = 0x85;			       		//ȫ���жϣ�int1(),int0()�ж�ʹ��
 	IP = 0x01;						//int0����
	TCON = 0x05;					//int0,int1Ϊ�ش���	
	  
	
	BANDGAP_TEMP = 0x05;
	LDO2_CTRL = 0X03;

	
	PADIR = 0x01;
	PAPU = 0XFe;
	PAREG = 0x00;

	PCDIR = 0x08;	 //��ɫled��pc3
	PCPU = 0XF7;
	PCREG = 0x08;
	ADPCR = 0X30;

	ADC_DIV= 0x01;

	PBDIR = 0x08;			//pb3����͵�ƽ
	PBPU = ~0x08;			//pb3������
	PBREG = 0x00;


	PWM_PERIOD0L = 0x98;		//pwm����Ϊ3.4KHz   ʱ�ӣ���
	PWM_PERIOD0H = 0x04;
//	PWM_PERIOD0L = 0xe2;		//pwm����Ϊ3.2KHz   ʱ�ӣ���
//	PWM_PERIOD0H = 0x04;							   
	DZ_CTRL = 0x05;			//pwm0�������,ȫNģʽ
	PWM_CTRL = 0x40;		//pwm0װ�أ������������ʼ��ƽΪ0
	  

	WAKEUP_EDGE_REG = 0x42;		//�½��ػ���


	IRED_CTRL = 0x09;		//���÷������
	MISC = 0x80;	  		//pb6������IRED��pa0����TXD��pa1����RXD  

/******************uart*********************/
/*	MISC |= 0x80;	//����uart��
	SCON = 	0X40;	//��ʽ1,����ʣ�����
	TTMOD2 = 0x00;	//ʹ��timer1���������ʣ�16λ����
	TMOD = 0x00;	//����Ƶ������
	TH1 =  0X00;
	TL1 = 0X0c;		//104����9600����
	TTCON = 0x2a;  	   */
/*******************************************/


	OPA1C0 = 0x23;		//��������Ϊ����
	OPA1C1 = 0x0c;		//OPA1���Ӻ�����չ�
	OPA1MODE = 0x04;	//���óɿ��� 	

	LVDC = 0x05;		//2.5V
	
//	TBC = 0xa6;		   //TBC����2s��ʱ
//	TBC = 0x08;		   //TBC0����4s��ʱ,TBC1�ر�
//	TBC = 0X40;
//	TBC = 0x84;		   //TBC����0.5s��ʱ
//	INTML = 0xf7;	   //TBC�ж�ʹ��
//	INTSEL0L = 0x08;   //TBC�жϴ�int0����	*/ 
	 
	TMR3C = 0X58;		//timer3ʹ�õ���ʱ��,��ʹ��
	TMR3L = 0X00;
	TMR3H = 0X06;		//0.125ms*64000=8s
//	INTML = 0XDF;		//TIMER23�жϿ���
	INTMH = 0x0D;       //uart�ж�ʹ��
	INTSEL0L = 0X20;	//TIMER23�ж�ѡ��int0��� 

	WDCNTM = 0x38;
	WDCNTH = 0x01;		//8Kʱ�ӣ�10s��
	WDCON = 0x28;		//�������Ź���λ����
	WDCON |= 0x10;
	
	ADPCR |= 0x08;       //PB6ΪAN3����
	BANDGAP_TEMP |= 0x02; //���¶ȴ�����
	LDOC=0X66;	//LDO 2.4v	


	delay_20us(250);   
	
}


void delay_20us(unsigned char N)
{
    unsigned char i;
    while(N--)     //6cycle
    {
        for(i=5;i>0;i--);
    }

}

void ser_int()
{	
	TH1 = 0x00;
	TL1 = 0x33;		  //2400������
	TTCON |= 0x02;	  //T1��ʱ����������Ч
	TMOD = 0x00;      //T1Ԥ��Ƶϵ��1
	TTCON = 0x28;     //T1ʹ�ܣ�T1����ʹ��	
	TTMOD2 = 0x00;	  //T1���������ʣ�16λ����
	SCON = 0x50;	  //�����������,���ڹ���ģʽ1
	SMOD = 0x00;
}

void uart()
{
	if(uartr_flag == 0)
	{
	 
	ADCR = 0x2A;	      //��ADC��Դ��ѡ���Դ����׼,ѡ���ڲ��¶ȼ��
	delay_20us(5);
	ADCR |= 0x80;
	delay_20us(1);
	ADCR &= ~0x80;		  //����ADC���
	LED_ON();
	while(ADCR&0x40);	  //�ȴ�ADC������
	LED_OFF();
	delay_20us(1);
	IE = 0X00;
	delay_20us(1);
	ADC_VALUE1 = ADRH;	  //ȡADC��8λ
	delay_20us(1);
	ADC_VALUE2 = ADRL>>4;	//ȡADC��4λ
	delay_20us(1);
	IE = 0X85;
	ADCR = 0x00;


		if(uartadd==0x68)
		{
			if(uartdate==0x81)
			{
				delay_20us(1);
				SBUF = ADC_VALUE1;
				uartr_flag = 3;
			}
			else
			{	
				uartr_flag = 0;
			}
		}
		else
		{	
			uartr_flag = 0;
		}		
	}
}



void ser_uart() interrupt 2
{
    //LED_ON()	;
	if(SCON&0x01)
	{	
	   
		SCON &= 0xfe;
		uartdate = SBUF;
		if(uartr_flag == 0)
		{	 		
			uartadd = uartdate;
			uartr_flag++;
		}
		else if(uartr_flag == 1)
		{	
			uartr_flag++;
		}
	}
	else if(SCON&0x02)
	{		
		SCON &= 0xfd;
		if(uartr_flag == 3)
		{
			SBUF = ADC_VALUE2;
			uartr_flag = 0;
		}					
	}
}


