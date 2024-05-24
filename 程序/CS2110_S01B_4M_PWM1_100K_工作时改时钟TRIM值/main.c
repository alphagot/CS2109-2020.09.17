#include "DP80390CPU.H"
#include "common.h"
#include "register.h"
#include "intrins.h"

//#define FIRE_ALARM_VALUE   0x40
//#define FIRE_ALARM_VALUE   10
//#define QUIT_ALARM_VALUE   (FIRE_ALARM_VALUE-10)
#define SLEF_CHECK_VALUE   0x00
#define SLIENCDE_VALUE     0xFF

#define SLIENCDE_TIME	   50		//静音时间8*50/60=6.667ｓ
#define DETECT_TIME1       1		//8s
#define DETECT_TIME2       1		//2s
#define DETECT_TIME3       2		//1s
#define BZ_TIME			   1		//500ms
#define LED_TIME_NORMAL	   8		//正常64ｓ闪一次
#define LED_TIME_SLIENCDE  1	   	//静音模式下8ｓ闪一次
#define LVD_TIME	       5		//低压报警时间4０ｓ


#define LED_ON()	PCREG &= ~0x08
#define LED_OFF()	PCREG |= 0x08


//unsigned int code BZ_Freq[]={0x560,0x540,0x500,0x300,0x1E0};
//unsigned int code BZ_Freq[]={0x4C0,0x4A0,0x400,0x380,0x270}; 	//50%占空比最响
//unsigned int code BZ_Freq[]={0x09,0x08,0x07,0x06,0x05};
unsigned int code BZ_Freq[]={0x495,0x490,0x480,0x460,0x430,0x3c0,0x320,0x250}; 	//50%占空比最响
unsigned char code IRED_ADJ[]=
{
0x4f,0x4e,0x0f,0x0e,
0x4d,0x4c,0x0d,0x0c,
0x4b,0x4a,0x0b,0x0a,
0x49,0x48,0x09,0x08,
0x47,0x46,0x07,0x06,
0x45,0x44,0x05,0x04,
0x43,0x42,0x03,0x02,
};		 //发射电流由大变小
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
	IRED_Ptr = 14;		//设置指针初始位置
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
	SYS_REG1 = 0x58; 	 //打开看门狗使能，打开低压复位，打开高速振荡器
	SYS_REG3 = 0x10; 	 //	
	SYS_REG2 = 0x03;	 //高速时钟不分频，空闲模式
	SYS_REG4 = 0x01;	 //PWM用高速时钟不分频
	LXT_START = 0X00;

	IE = 0x85;			       		//全局中断，int1(),int0()中断使能
 	IP = 0x01;						//int0优先
	TCON = 0x05;					//int0,int1为沿触发	
	  
	
	BANDGAP_TEMP = 0x05;
	LDO2_CTRL = 0X03;

	
	PADIR = 0x01;
	PAPU = 0XFe;
	PAREG = 0x00;

	PCDIR = 0x08;	 //红色led灯pc3
	PCPU = 0XF7;
	PCREG = 0x08;
	ADPCR = 0X30;

	PBDIR = 0x08;			//pb3输出低电平
	PBPU = ~0x08;			//pb3不上拉
	PBREG = 0x00;


	PWM_PERIOD1L = 0x28;		//pwm1周期为100KHz   时钟4M
	PWM_PERIOD1H = 0x00;
	PWM_PULSE1L = 0x14;
	PWM_PULSE1H = 0x00;								   
	DZ_CTRL = 0x0a;			//pwm1互补输出,全N模式
	PWM_CTRL = 0x80;		//pwm1装载，连续输出，初始电平为0  


	WAKEUP_EDGE_REG = 0x42;		//下降沿唤醒


	IRED_CTRL = 0x09;		//设置发射电流为125ｍA
	MISC = 0x40;	  		//pb6复用IRED  

/******************uart*********************/
/*	MISC |= 0x80;	//复用uart口
	SCON = 	0X40;	//方式1,溢出率／３２
	TTMOD2 = 0x00;	//使用timer1产生波特率，16位计数
	TMOD = 0x00;	//不分频，４Ｍ
	TH1 =  0X00;
	TL1 = 0X0c;		//104ｕｓ，9600ｂｐｓ
	TTCON = 0x2a;  	   */
/*******************************************/


	OPA1C0 = 0x23;		//积分增益为８倍
	OPA1C1 = 0x0c;		//OPA1连接红外接收管
	OPA1MODE = 0x04;	//设置成开环 	

	LVDC = 0x05;		//2.5V
	
//	TBC = 0xa6;		   //TBC开启2s定时
//	TBC = 0x08;		   //TBC0开启4s定时,TBC1关闭
//	TBC = 0X40;
//	TBC = 0x84;		   //TBC开启0.5s定时
//	INTML = 0xf7;	   //TBC中断使能
//	INTSEL0L = 0x08;   //TBC中断从int0进入	*/ 
	 
	TMR3C = 0X58;		//timer3使用低速时钟,并使能
	TMR3L = 0X00;
	TMR3H = 0X06;		//0.125ms*64000=8s
	INTML = 0XDF;
	INTSEL0L = 0X20;	 

	WDCNTM = 0x38;
	WDCNTH = 0x01;		//8K时钟，10s钟
//	WDCNTM = 0x38;
//	WDCNTH = 0x00;
	WDCON = 0x28;		//开启看门狗复位功能
	WDCON |= 0x10;	


	E2P_ADDRH=0x00;	//起始地址
	E2P_ADDRL=0x00;
	E2P_CTRL = 0x10;
	for(i=0;i<2;i++)
	{
	   if(i<1)
	   	   E2P_CTRL=0x13;	//启动读
	   else
	   	   E2P_CTRL=0x11;	//启动度
	   while(!(E2P_FLAG&0x01));//判断是否写完
	   temp[i]=E2P_DATA;
	} 
	if(temp[0]!=(~temp[1]))
	{
		FIRE_ALARM_VALUE = 0x60;
		alarm_value = FIRE_ALARM_VALUE;
		QUIT_ALARM_VALUE = FIRE_ALARM_VALUE - 10;		
	}
	else					   //上电读取报警阈值
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



