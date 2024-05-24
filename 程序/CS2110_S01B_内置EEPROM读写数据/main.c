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
	SYS_REG1 = 0x58; 	 //打开看门狗使能，打开低压复位，打开高速振荡器
	SYS_REG3 = 0x10; 	 //	
//	SYS_REG2 = 0xC2;  	 //高速时钟4分频，空闲模式		
//	SYS_REG4 = 0x0c;	 //PWM用高速时钟4分频
	SYS_REG2 = 0x03;	 //高速时钟，空闲模式
	SYS_REG4 = 0x01;	 //PWM用高速时钟
	LXT_START = 0X00;

	IE = 0x85;			       		//全局中断，int1(),int0()中断使能
 	IP = 0x01;						//int0优先
	TCON = 0x05;					//int0,int1为沿触发	
	  
	

	BANDGAP_TEMP = 0x05;
	LDO2_CTRL = 0X03;

	
	PADIR = 0x01;
	PAPU = 0XFe;
	PAREG = 0x00;

	PCDIR = 0x08;	 //红色led等pc3
	PCPU = 0XF7;
	PCREG = 0x08;
	ADPCR = 0X30;



	PBDIR = 0x08;			//pb3输出低电平
	PBPU = ~0x08;			//pb3不上拉
	PBREG = 0x00;
	PWM_PERIOD0L = 0x98;		//pwm周期为3.4KHz   时钟４Ｍ
	PWM_PERIOD0H = 0x04;
//	PWM_PERIOD0L = 0xe2;		//pwm周期为3.2KHz   时钟４Ｍ
//	PWM_PERIOD0H = 0x04;							   
	DZ_CTRL = 0x05;			//pwm0互补输出,全N模式
	PWM_CTRL = 0x40;		//pwm0装载，连续输出，初始电平为0  

	WAKEUP_EDGE_REG = 0x42;		//下降沿唤醒


	IRED_CTRL = 0x09;		//设置发射电流
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
//	INTML = 0XDF;		//TIMER23中断开启
	INTSEL0L = 0X20;	//TIMER23中断选择int0输	 

	WDCNTM = 0x38;
	WDCNTH = 0x01;		//8K时钟，10s钟
	WDCON = 0x28;		//开启看门狗复位功能
	WDCON |= 0x10;	



	MTP_PROTECT = 0xa5;
	MTP_CTRL = 0x20; 	//读操作
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
	MTP_CTRL = 0x10; 	//写操作
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

