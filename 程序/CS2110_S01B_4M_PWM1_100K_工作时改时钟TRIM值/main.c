#include "DP80390CPU.H"
#include "common.h"
#include "register.h"
#include "intrins.h"

//#define FIRE_ALARM_VALUE   0x40
//#define FIRE_ALARM_VALUE   10
//#define QUIT_ALARM_VALUE   (FIRE_ALARM_VALUE-10)
#define SLEF_CHECK_VALUE   0x00
#define SLIENCDE_VALUE     0xFF

#define SLIENCDE_TIME	   50		//����ʱ��8*50/60=6.667��
#define DETECT_TIME1       1		//8s
#define DETECT_TIME2       1		//2s
#define DETECT_TIME3       2		//1s
#define BZ_TIME			   1		//500ms
#define LED_TIME_NORMAL	   8		//����64����һ��
#define LED_TIME_SLIENCDE  1	   	//����ģʽ��8����һ��
#define LVD_TIME	       5		//��ѹ����ʱ��4����


#define LED_ON()	PCREG &= ~0x08
#define LED_OFF()	PCREG |= 0x08


//unsigned int code BZ_Freq[]={0x560,0x540,0x500,0x300,0x1E0};
//unsigned int code BZ_Freq[]={0x4C0,0x4A0,0x400,0x380,0x270}; 	//50%ռ�ձ�����
//unsigned int code BZ_Freq[]={0x09,0x08,0x07,0x06,0x05};
unsigned int code BZ_Freq[]={0x495,0x490,0x480,0x460,0x430,0x3c0,0x320,0x250}; 	//50%ռ�ձ�����
unsigned char code IRED_ADJ[]=
{
0x4f,0x4e,0x0f,0x0e,
0x4d,0x4c,0x0d,0x0c,
0x4b,0x4a,0x0b,0x0a,
0x49,0x48,0x09,0x08,
0x47,0x46,0x07,0x06,
0x45,0x44,0x05,0x04,
0x43,0x42,0x03,0x02,
};		 //��������ɴ��С
unsigned char Smoke_Value;
unsigned char Dark_Value;
unsigned char timer_flag;
unsigned char bz_timer;
unsigned char bz_step;
unsigned char alarm_flag;
unsigned char alarm_value;
unsigned char detect_time;
unsigned char detect_count;
unsigned char sleep_flag;
unsigned char key_flag;
unsigned char rxd_flag;
unsigned char silince_flag;
unsigned char led_falg;
unsigned char led_timer;
unsigned int  silince_time;
unsigned char self_check_flag;
unsigned char lvd_flag;
unsigned char lvd_time;
unsigned char FIRE_ALARM_VALUE;
unsigned char QUIT_ALARM_VALUE;
unsigned char delta_value;
unsigned char comm_step;
unsigned char Battery_Value_H;
unsigned char Battery_Value_L;
unsigned int voltage_bak;
unsigned char adjust_flag;
unsigned char silince_time_bak;
unsigned char IRED_Ptr;
unsigned char Charge_Flag;



void delay_20us(unsigned char N);
void Smoke_Detect();
void BZ_Process();
void Key_Process();
void Other_Process();
void Sleep();
void MCU_Init();

void main()
{
	MCU_Init();

	timer_flag = 0;
	bz_timer = 0;
	bz_step = 0;
	alarm_flag = 0;
	detect_time = DETECT_TIME1;		//8s
	detect_count = 0;
	sleep_flag = 1;
	key_flag = 0;
	silince_time = 0;
	silince_time_bak = 0;
	silince_flag = 0;
	led_falg = 0;
	self_check_flag = 0;
	lvd_flag = 0;
	lvd_time = 0;
	comm_step = 0;
	voltage_bak = 0;
	IRED_Ptr = 14;		//����ָ���ʼλ��
	Charge_Flag = 0;
	while(1)
	{
	  	WDCON |= 0x10;
		PWM_CTRL |= 0x02;
	 			  
	}	


}

void MCU_Init()
{
	unsigned char i,temp[2];
	SYS_REG_MASK = 0xA5;
	SYS_REG1 = 0x58; 	 //�򿪿��Ź�ʹ�ܣ��򿪵�ѹ��λ���򿪸�������
	SYS_REG3 = 0x10; 	 //	
	SYS_REG2 = 0x03;	 //����ʱ�Ӳ���Ƶ������ģʽ
	SYS_REG4 = 0x01;	 //PWM�ø���ʱ�Ӳ���Ƶ
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


	PWM_PERIOD1L = 0x28;		//pwm1����Ϊ100KHz   ʱ��4M
	PWM_PERIOD1H = 0x00;
	PWM_PULSE1L = 0x14;
	PWM_PULSE1H = 0x00;								   
	DZ_CTRL = 0x0a;			//pwm1�������,ȫNģʽ
	PWM_CTRL = 0x80;		//pwm1װ�أ������������ʼ��ƽΪ0  


	WAKEUP_EDGE_REG = 0x42;		//�½��ػ���


	IRED_CTRL = 0x09;		//���÷������Ϊ125��A
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
	INTML = 0XDF;
	INTSEL0L = 0X20;	 

	WDCNTM = 0x38;
	WDCNTH = 0x01;		//8Kʱ�ӣ�10s��
//	WDCNTM = 0x38;
//	WDCNTH = 0x00;
	WDCON = 0x28;		//�������Ź���λ����
	WDCON |= 0x10;	


	E2P_ADDRH=0x00;	//��ʼ��ַ
	E2P_ADDRL=0x00;
	E2P_CTRL = 0x10;
	for(i=0;i<2;i++)
	{
	   if(i<1)
	   	   E2P_CTRL=0x13;	//������
	   else
	   	   E2P_CTRL=0x11;	//������
	   while(!(E2P_FLAG&0x01));//�ж��Ƿ�д��
	   temp[i]=E2P_DATA;
	} 
	if(temp[0]!=(~temp[1]))
	{
		FIRE_ALARM_VALUE = 0x60;
		alarm_value = FIRE_ALARM_VALUE;
		QUIT_ALARM_VALUE = FIRE_ALARM_VALUE - 10;		
	}
	else					   //�ϵ��ȡ������ֵ
	{
		FIRE_ALARM_VALUE = temp[0];
		alarm_value = FIRE_ALARM_VALUE;
		QUIT_ALARM_VALUE = FIRE_ALARM_VALUE - 10;			  
	}					  
	alarm_value = 0xc0;
	FIRE_ALARM_VALUE = 0xc0;


/*	delay_20us(250);
	delay_20us(250);
	delay_20us(250);
	delay_20us(250);
	delay_20us(250);
	delay_20us(250);
	delay_20us(250);
	delay_20us(250);
	delay_20us(250);
	delay_20us(250);
	delay_20us(250);*/
	delay_20us(250);  
	
	XBYTE[0x0604] = 0xA5;
	XBYTE[0x0600] = 0xB0;
	XBYTE[0x0602] = 0x02;
	XBYTE[0x0603] = 0x01;
		 	
}



void delay_20us(unsigned char N)
{
    unsigned char i;
    while(N--)     //6cycle
    {
        for(i=5;i>0;i--);
    }

}



