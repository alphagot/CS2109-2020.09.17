#include "DP80390CPU.H"
#include "common.h"
#include "register.h"
#include "intrins.h"


#define LED_ON()	PCREG &= ~0x08
#define LED_OFF()	PCREG |= 0x08


void delay_20us(unsigned char N);
void MCU_Init();

void main()
{
	MCU_Init();

	while(1)
	{
	
		  
	}	


}

void MCU_Init()
{
	SYS_REG_MASK = 0xA5;
	SYS_REG1 = 0x58; 	 //�򿪿��Ź�ʹ�ܣ��򿪵�ѹ��λ���򿪸�������
	SYS_REG3 = 0x10; 	 //	
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
	MISC = 0x40;	  		//pb6����IRED  

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
	INTSEL0L = 0X20;	//TIMER23�ж�ѡ��int0��	 

	WDCNTM = 0x38;
	WDCNTH = 0x01;		//8Kʱ�ӣ�10s��
	WDCON = 0x28;		//�������Ź���λ����
	WDCON |= 0x10;	



	MTP_PROTECT = 0xa5;
	MTP_CTRL = 0x20; 	//������
	DRAM_WR_REV_TM = 0x02;
	PCON |= 0x02;
	_nop_(); 
	_nop_();
	_nop_();
	_nop_();
	MTP_CTRL = 0x00;
	MTP_PROTECT = 0x00;




	XBYTE[0] = 0x00;
	XBYTE[1] = 0x01;
	XBYTE[2] = 0x02;
	XBYTE[3] = 0x03;


	MTP_PROTECT = 0xa5;
	MTP_CTRL = 0x10; 	//д����
	DRAM_WR_REV_TM = 0x02;
	PCON |= 0x02;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	MTP_CTRL = 0x00;
	MTP_PROTECT = 0x00;


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

void delay_20us(unsigned char N)
{
    unsigned char i;
    while(N--)     //6cycle
    {
        for(i=5;i>0;i--);
    }

}

