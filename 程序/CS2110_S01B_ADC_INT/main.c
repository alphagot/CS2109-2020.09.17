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
	    //PAREG=~PAREG;
	 	//LED_ON();
	  	WDCON |= 0x10;
		//LDO2_CTRL = 0x00;
	//	Key_Process();
	  	//Smoke_Detect();
    	//BZ_Process();
	   // Other_Process();
	  // Sleep();
	
		  
	}	


}

void MCU_Init()
{
	unsigned char i,temp[2];
	SYS_REG_MASK = 0xA5;
	SYS_REG1 = 0x18; 	 //�رտ��Ź����򿪵�ѹ��λ���򿪸�������
	SYS_REG3 = 0x10; 	 //	
//	SYS_REG2 = 0xC2;  	 //����ʱ��4��Ƶ������ģʽ		
//	SYS_REG4 = 0x0c;	 //PWM�ø���ʱ��4��Ƶ
	SYS_REG2 = 0x03;
	SYS_REG4 = 0x01;	 //PWM�ø���ʱ��
	LXT_START = 0X00;

	IE = 0x85;			       		//ȫ���жϣ�int1(),int0()�ж�ʹ��
 	IP = 0x01;						//int0����
	TCON = 0x05;					//int0,int1Ϊ�ش���	
	  
	

	BANDGAP_TEMP = 0x05;
	LDO2_CTRL = 0X03;

	
	PADIR = 0xFF;
	PAPU = 0X00;
	PAREG = 0x00;

	PCDIR = 0x08;	 //��ɫled��pc3
	PCPU = 0XF7;
   	PCREG = 0x08;
	ADPCR = 0X30;



	PBDIR = 0x08;			//pb1����͵�ƽ
	PBPU = ~0x08;			//pb1������
	PBREG = 0x00;
/* 	PWM_PERIOD0L = 0x98;		//pwm����Ϊ3.4KHz   ʱ�ӣ���
	PWM_PERIOD0H = 0x04;
	//PWM_PERIOD0L = 0xe2;		//pwm����Ϊ3.2KHz   ʱ�ӣ���
//	PWM_PERIOD0H = 0x04;							   
	DZ_CTRL = 0x05;			//pwm0�������,ȫNģʽ
	PWM_CTRL = 0x40;		//pwm0װ�أ������������ʼ��ƽΪ0  

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
	 
/*   TMR3C = 0X58;		//timer3ʹ�õ���ʱ��,��ʹ��
	TMR3L = 0X00;
	TMR3H = 0X06;		//0.125ms*64000=8s
	INTML = 0XDF;
	INTSEL0L = 0X20;*/	 

			
	WDCNTM = 0x38;
	WDCNTH = 0x00;
	WDCON = 0x28;		//�������Ź���λ����
	WDCON |= 0x10;	   
	
/*	 WDCON |= 0x24;	 //�������Ź������ж�
	 WDCNTM = 0x11;
	 WDCNTH = 0x11;
	 INTMH &=~0X01;
	INTSEL0H = 0X01;  */

	 INTML&=~0X02;	//��ADC�ж�
	 INTSEL0L=0x02;	 //ADC�жϴ�IN0���
	 LDOC = 0x46;
	 LDOC |= 0x20;
	 ADCR = 0x36;		//��ADC��Դ��ѡ���ڲ�2.4VΪ��׼
	 ADCR |= 0x80;
	 ADCR &= ~0x80;	


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


void Smoke_Detect()
{	 
   
	unsigned char i,temp[2];
	unsigned int vol;
	if(timer_flag>=detect_time)				  //8s���һ��
	{
		timer_flag = 0;
		WDCON |= 0x10;
		BANDGAP_TEMP = 0x05;
		LDO2_CTRL = 0x03;

/*		SYS_REG2 = 0x02;	//�л��ɵ�Ƶ
		_nop_();
		_nop_();
		_nop_();
		_nop_(); 		
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();

		OPA1MODE = 0x10;	//���óɻ���ģʽ,����200mvƫ��
		OPA1C1 |= 0x01;		//�����˷�		
		LDOC = 0x46;
		_nop_();
		_nop_();
		_nop_();
		_nop_();

		SYS_REG2 = 0xc2;	//�л��ɸ�Ƶ
		_nop_();
		_nop_();
		_nop_();
		_nop_();
//		delay_20us(100);		//�˷��ȶ�ʱ��
*/
		TMR3C &= ~0x08;		//�رռ�����
		delay_20us(20);
		TMR3L = 0xc0;		//����8ms
		TMR3H = 0xff; 
		TMR3C |= 0x08;		//����������
		Charge_Flag = 1;
		PCON |= 0X02;	//mcu_stop,�����ݳ��			
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		TMR3C &= ~0x08;		//�رռ�����,������������ٿ���
		LDO2_CTRL = 0x00;//�ر�LDO2
		Charge_Flag = 0;
			  

		OPA1MODE = 0x10;	//���óɻ���ģʽ,����200mvƫ��
		OPA1C1 |= 0x01;		//�����˷�		
		LDOC = 0x46;
		delay_20us(20);		//�˷��ȶ�ʱ��
/******************************��Դ��ѹ���*/
		LDOC |= 0x20;		//��LDO��ADC��׼
		LCDC |= 0X70;
		LDOC |= 0X10;
		ADCR = 0x3b;		//��ADC��Դ��ѡ���ڲ�2.4VΪ��׼,����AVDD��ѹ
//		ADCR = 0x2b;		//��ADC��Դ��ѡ���ڲ�2.4VΪ��׼,����AVDD��ѹ
		for(i=0;i<5;i++)
		{
			ADCR |= 0x80;
			ADCR &= ~0x80;		//����ADC���
			while(ADCR&0x40);	//�ȴ�ADC������
		}
		Battery_Value_H = ADRH;
		Battery_Value_L = ADRL;
		ADCR = 0x00;
		LDOC &= ~0x20;		
		LDOC &= ~0X10;
		LCDC &= ~0X70;

	//	4096*2.4*2;//2560Ϊ3V,ÿ85����Ϊ0.1V
		vol = (((unsigned int)Battery_Value_H)<<4);
		vol += (unsigned int)Battery_Value_L>>4;


 
		if(voltage_bak==0x00)	//�״μ��
		{
			adjust_flag = 1;
			voltage_bak = vol;  	//������һ��У׼ʱ�ĵ�ѹ
		}
		else
		{
			if((vol>=(voltage_bak+8))||(vol<=(voltage_bak-8)))	//����10mv
			{
				adjust_flag = 1;
				voltage_bak = vol; 	//������һ��У׼ʱ�ĵ�ѹ
			}	
		}

		if(adjust_flag)
		{
			adjust_flag = 0;
			temp[0] = 24;

			if(vol>2688)		//����3.15V
			IRED_CTRL = IRED_ADJ[IRED_Ptr+1];
			else if((vol>2474)&&(vol<=2688))		//����2.9V
			IRED_CTRL = IRED_ADJ[IRED_Ptr];
			else if((vol>2389)&&(vol<=2474))		//����2.8V
			IRED_CTRL = IRED_ADJ[IRED_Ptr-1];
			else if((vol>2304)&&(vol<=2389))		//����2.7V
			IRED_CTRL = IRED_ADJ[IRED_Ptr-2];
			else if((vol>2218)&&(vol<=2304))		//����2.6V
			IRED_CTRL = IRED_ADJ[IRED_Ptr-3];
			else if((vol>2176)&&(vol<=2218))		//����2.55V
			IRED_CTRL = IRED_ADJ[IRED_Ptr-4];
			else if((vol>2133)&&(vol<=2176))		//����2.5V
			IRED_CTRL = IRED_ADJ[IRED_Ptr-5];
			else if((vol>2090)&&(vol<=2133))		//����2.45V
			IRED_CTRL = IRED_ADJ[IRED_Ptr-7]; 
			else if((vol>2048)&&(vol<=2090))		//����2.4V
			IRED_CTRL = IRED_ADJ[IRED_Ptr-9];
			else if((vol>2005)&&(vol<=2048))		//����2.35V
			IRED_CTRL = IRED_ADJ[IRED_Ptr-11];

		}	
		


/****************************************************/
//		temp[0] = 10;

		OPA1MODE |= 0x01;	//Sw_integ

//		for(i=temp[0];i>0;i--);
		delay_20us(5);

		OPA1MODE |= 0x02;
		IRED_CTRL &= ~0x20;	//IRED�رգ����ֱ���
		LDOC |= 0x20;		//��LDO��ADC��׼	 
		ADCR = 0x36;		//��ADC��Դ��ѡ���ڲ�2.4VΪ��׼
		ADCR |= 0x80;
		ADCR &= ~0x80;		//����ADC���
		while(ADCR&0x40);	//�ȴ�ADC������
		Dark_Value = ADRH;	//��������⣬ȡADC��8λ
		ADCR = 0x00;
		LDOC &= ~0x20;
		OPA1MODE &= ~0x03;

/*****************���밴�ո�ʱ��**************/		
		OPA1MODE |= 0x02;
		IRED_CTRL |= 0x20;	//IREDʹ��
		OPA1MODE &= ~0x02;
		OPA1MODE |= 0x01;	//Sw_integ

//		for(i=temp[0];i>0;i--);
		delay_20us(5);

		OPA1MODE |= 0x02;	 
		IRED_CTRL &= ~0x20;	//IRED�رգ����ֱ���
/********************************************/

		LVDC |= 0x10;	//������ѹ���,����ǰ����

		LDOC |= 0x20;
		ADCR = 0x36;		//��ADC��Դ��ѡ���ڲ�2.4VΪ��׼
		ADCR |= 0x80;
		ADCR &= ~0x80;		//����ADC���
		while(ADCR&0x40);	//�ȴ�ADC������
		Smoke_Value = ADRH;	//�������⣬ȡADC��8λ
		ADCR = 0x00;
		LDOC = 0x00;
		//LED_ON();
		if(LVDC&0x20)
		{
		    
			lvd_flag = 1;
		}
		else
		{
			lvd_flag = 0;
			lvd_time = 0;
		}
		LVDC &= ~0x10;	//�رյ�ѹ���

		OPA1MODE = 0x04;	//�ر����п��ز����óɿ���ģʽ
		BANDGAP_TEMP = 0x04;
	//	LDO2_CTRL = 0x00;//�ر�LDO2
		OPA1C1 &= ~0x01;	//�ر��˷�
		

		if(Smoke_Value>=Dark_Value)
		Smoke_Value = Smoke_Value - Dark_Value;
		else
		Smoke_Value = 0;


//		Smoke_Value = 0xff;

		if(rxd_flag==2)		//������ֵ
		{
		   
			SBUF =0x11;
			//SBUF = Smoke_Value;
		/*	SBUF = Battery_Value_H;
			while(rxd_flag);
			rxd_flag = 1;
			SBUF = Battery_Value_L;*/	
			while(rxd_flag);
			INTMH |= 0x02;
			MISC &= ~0x80;	//����IO��
		}
		else if(rxd_flag==3)	  //�궨
		{
			temp[0] = Smoke_Value + delta_value;
			temp[1] = ~temp[0];
			E2P_ADDRH=0x00;	//E2��ַ
			E2P_ADDRL=0x00;
			E2P_CTRL = 0x20;
			for(i=0;i<2;i++)
			{
			   E2P_DATA=temp[i];//����
			   if(i<1)
			   	   E2P_CTRL=0x23;	//����д
			   else
			   	   E2P_CTRL=0x21;	//����д���һ���ֽ�
			   while(!(E2P_FLAG&0x02));//�ж��Ƿ�д��	
			} 
	//		FIRE_ALARM_VALUE = Smoke_Value + delta_value;
	//		QUIT_ALARM_VALUE = FIRE_ALARM_VALUE - 10;
	//		alarm_value = FIRE_ALARM_VALUE;

			IRED_CTRL = delta_value;

			SBUF = Smoke_Value + delta_value;
			while(rxd_flag);
			INTMH |= 0x02;
			MISC &= ~0x80;	//����IO��
		}

		if((!silince_flag)||(self_check_flag))		//������״̬�����Լ�
		{
			if(Smoke_Value>alarm_value)
			{
			//	PAREG = ~PAREG;
				detect_count++;
				if(detect_count>3)		//��ֹ���
				detect_count = 3;
				if(detect_count==1)
				{
					detect_time = DETECT_TIME1;
					TMR3L = 0x80;		//���2s
					TMR3H = 0xc1; 
					TMR3C |= 0x08;		//��������
				}
				else if(detect_count==2)
				{
					detect_time = DETECT_TIME2;
					TMR3L = 0xc0;		//���1s
					TMR3H = 0xe0; 
					TMR3C |= 0x08;		//��������
				}
				else
				{
					detect_time = DETECT_TIME3;
			//		if(alarm_flag==0)
					{
						TMR3L = 0x60;		//���0.5
						TMR3H = 0xf0; 
						TMR3C |= 0x08;		//��������
					}
					alarm_flag = 1;
					bz_timer = BZ_TIME;
//					PAREG = ~PAREG;
				}
				led_falg = 1;
				if(!self_check_flag)
				alarm_value = QUIT_ALARM_VALUE;

				
			}
			else 	 //��ȡ�����ָ���ʼ
			{
				detect_count = 0;
				detect_time = DETECT_TIME1;
				alarm_value = FIRE_ALARM_VALUE;
				alarm_flag = 0;
				bz_timer = 0;
				PWM_CTRL &= ~0x01;
				sleep_flag = 1;
				bz_step = 0;
				TMR3L = 0x00;		//����8s
				TMR3H = 0x06; 
				TMR3C |= 0x08;		//��������
			}
		}
		else		 //����״̬��,����8s��ʱ��
		{
			TMR3L = 0x00;		//����8s
			TMR3H = 0x06; 
			TMR3C |= 0x08;		//��������
		}
		led_timer++;
		if(!silince_flag)
		{
			if(led_timer>=LED_TIME_NORMAL)
			{
				led_falg = 1;
				led_timer = 0;
			}
		}
		else
		{
			if(led_timer>=LED_TIME_SLIENCDE)
			{
				led_falg = 1;
				led_timer = 0;
			}
		} 			 
	} 
}


void BZ_Process()
{
	if((bz_timer>=BZ_TIME)&&(alarm_flag))			  //������������
	{
		led_falg = 1;
		bz_timer = 0;
		lvd_time = 0;			  //����ʱ��ѹ����ʱ������
		if(PWM_CTRL&0x01)
		{
			PWM_CTRL &= ~0x01;
			sleep_flag = 1;
		}
		else
		{
			if(bz_step>=8)
			bz_step = 7;
			PWM_PULSE0L = BZ_Freq[bz_step]&0x00ff;
			PWM_PULSE0H = BZ_Freq[bz_step]>>8;
			PWM_CTRL |= 0x40;
			PWM_CTRL |= 0x01;
			bz_step++;
			sleep_flag = 0;
		}
	}
}



void Key_Process()
{
	if(((PAREG&0x40)==0x00)&&(!key_flag))		 //��������
	{
		delay_20us(250);
		if(((PAREG&0x40)==0x00)&&(!key_flag))		 //��������
		{
			key_flag = 1;
			
			if(alarm_flag==1)		  //����
			{
				silince_flag = 1;
				alarm_value = SLIENCDE_VALUE;
				detect_count = 0;
				detect_time = DETECT_TIME1;
				timer_flag = 0;
				alarm_flag = 0;
				PWM_CTRL &= ~0x01;		   //�رշ�����
				sleep_flag = 1;
				bz_timer = 0;
				TMR3C &= ~0x08;		//�رռ�����
				delay_20us(20);
				TMR3L = 0x00;		//���8s
				TMR3H = 0x06; 
				TMR3C |= 0x08;

			}
			else					  //�Լ�
			{
				self_check_flag = 1;
				alarm_value = SLEF_CHECK_VALUE;
				detect_count = 2;
				detect_time = DETECT_TIME3;
				timer_flag = 1;

				if(silince_flag)
				silince_time_bak = silince_time;

				TMR3C &= ~0x08;		//�رռ�����
				delay_20us(20);
				TMR3L = 0x60;		//���0.5s
				TMR3H = 0xf0; 
				TMR3C |= 0x08;

			}
		}	
	}
	else if(((PAREG&0x40))&&(key_flag))
	{
		delay_20us(250);
		if(((PAREG&0x40))&&(key_flag))	  //�����ɿ�
		{
			key_flag = 0;
			alarm_flag = 0;
			PWM_CTRL &= ~0x01;
			bz_step = 0;
			if(!silince_flag)
			alarm_value = FIRE_ALARM_VALUE;
			detect_count = 0;
			detect_time = DETECT_TIME1;
			sleep_flag = 1;
			self_check_flag = 0;
			lvd_time = 0;

			if(silince_flag)
			silince_time = silince_time_bak;	

			TMR3C &= ~0x08;		//�رռ�����
			delay_20us(20);
			TMR3L = 0x00;		//���8s
			TMR3H = 0x06; 
			TMR3C |= 0x08;
		}
	}		

	if(((PAREG&0x02)==0x00)&&(!rxd_flag))
	{
		delay_20us(1);
		if(((PAREG&0x02)==0x00)&&(!rxd_flag))
		{
			rxd_flag = 1;
			
			MISC |= 0x80;	//����uart��
			SCON = 	0X50;	//��ʽ1,����ʣ�����
			TTMOD2 = 0x00;	//ʹ��timer1���������ʣ�16λ����
			TMOD = 0x00;	//����Ƶ������
			TH1 =  0X00;
			TL1 = 0X0c;		//104����9600����
			TTCON = 0x2a;
			INTMH &= ~0x02;
			comm_step = 0;
			timer_flag = 0xaa;	//����һ�β���
			LED_ON();
			while(rxd_flag==1);
			if(rxd_flag==4)		  //��������ֵ	
			{
				timer_flag = 0;
				SBUF = alarm_value;
				while(rxd_flag);
				INTMH |= 0x02;
				MISC &= ~0x80;	//����IO��
			}
			
		}
	} 
}


void Other_Process()
{
	if(silince_time>=SLIENCDE_TIME)		  //����ʱ�䵽
	{
		silince_flag = 0;
		silince_time = 0;
		silince_time_bak = 0;
		alarm_value = FIRE_ALARM_VALUE;	
		lvd_time = 0;
	}

	if(led_falg)
	{
		led_falg = 0;
		LED_ON();
		delay_20us(250);
		LED_OFF();
	} 	
//	if((!silince_flag)&&(!alarm_flag)&&(lvd_time>=LVD_TIME))	   //��ѹ����
	if((!alarm_flag)&&(lvd_time>=LVD_TIME))
	{
		lvd_time = 0;
		PWM_PULSE0L = 0x50;
		PWM_PULSE0H = 0x02;
		PWM_CTRL |= 0x40;
		PWM_CTRL |= 0x01;
		LED_ON();
		delay_20us(250);
		LED_OFF();
		delay_20us(250);
		PWM_CTRL &= ~0x01;
	}
}


void Sleep()
{
	if(((SYS_REG4&0x10)==0x00)&&(sleep_flag))
	{
//		sleep_flag = 0;
		
		BANDGAP_TEMP = 0X04; //�ر�bandgap
		LDO2_CTRL = 0x00;//�ر�LDO2		
//		delay_20us(20);
		if(PAREG&0x40)		  //δ����������pA6,pA1����
		WAKE_EN = 0x42;		  //
		PCON |= 0X02;	//mcu_stop			
		_nop_();
		_nop_();
		_nop_();
		_nop_(); 
	    WAKE_EN = 0x00;	//�ر�pA7����
//		PAREG = ~PAREG;
	
	} 

}
void delay_20us(unsigned char N)
{
    unsigned char i;
    while(N--)     //6cycle
    {
        for(i=5;i>0;i--);
    }

}


void isr0() 	using 0	   interrupt 0
{
	PAREG=~PAREG;
	if(ADIFG&0X01)
    {
	 ADIFG &=~0X01;
	 ADCR |= 0x80;
	 ADCR &= ~0x80;
	// PAREG=~PAREG;

	 }
/*	if(TBC_INT&0x01)	//TB0�жϹ̶�8s
	{
		if(!fast_flag)
		{
			TBC_INT = 0x00;
			timer_flag += 8;
			if(silince_flag)
			silince_time += 8;
			if(lvd_flag)
		    lvd_time += 8;
		}
		else
		TBC_INT &= ~0x01;
	}
	if(TBC_INT&0x02)	//���Ѽӿ�ʱ������TB1�ж�0.5s 
	{
		TBC_INT &= ~0x02;
		timer_flag++;
		bz_timer++;
		if(silince_flag)
		silince_time++;
		if(lvd_flag)
	    lvd_time++; 


	} */
	if(TMRINT&0X02)
	{
		TMRINT &= ~0X02;
		if(Charge_Flag==0)	   //�ǵ��ݳ�绽��
		{	
			timer_flag++;
			bz_timer++;
			if(silince_flag)
			silince_time++;
			if(lvd_flag)
		    lvd_time++;
		}
//		PAREG = ~PAREG;
	}
}




void isr1()	using 1	   interrupt 2		  //uart�ж�
{
	unsigned char temp;
	if(SCON&0X01)	  //�������
	{
		SCON &= ~0X01;
		temp = SBUF;
		if(temp==0xaa)		//������ֵ
		rxd_flag = 2;
		else if(temp==0xa0)		//��������ֵ
		rxd_flag = 4;
		else if(temp==0x55)		//�궨
		comm_step = 1;
		else if	(comm_step==1)
		{
			delta_value =  temp;
			rxd_flag = 3;
		}	
	}
	if(SCON&0X02)	 //�������
	{
	 	SCON &= ~0X02;
		rxd_flag = 0;
	}
}
