#include "tim.h"

void Timer1_Init(void)
{
	AUXR &= 0xBF;		
	TMOD &= 0x0F;		
	TL1 = 0x18;		
	TH1 = 0xFC;		
	TF1 = 0;		
	TR1 = 1;	//定时器1开启
	
	ET1 = 1; //定时器中断
	EA = 1; //总中断开启
}

void Cls_Peripheral(void)//临时寄存处，实在没地方去了，在此借宿一段时间，啥时候离开，不清楚
{
	P0 = 0XFF;  //关闭LED灯
	P2 = P2 & 0X1F | 0X80;
	P2 &= 0X1F; //	P2 = P2 & 0X1F;

	P0 = 0;  //关闭外设，蜂鸣器，继电器等
	P2 = P2 & 0X1F | 0XA0;
	P2 &= 0X1F; //	P2 = P2 & 0X1F;	
}