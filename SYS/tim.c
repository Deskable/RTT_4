#include "tim.h"

void Timer1_Init(void)
{
	AUXR &= 0xBF;		
	TMOD &= 0x0F;		
	TL1 = 0x18;		
	TH1 = 0xFC;		
	TF1 = 0;		
	TR1 = 1;	//��ʱ��1����
	
	ET1 = 1; //��ʱ���ж�
	EA = 1; //���жϿ���
}

void Cls_Peripheral(void)//��ʱ�Ĵ洦��ʵ��û�ط�ȥ�ˣ��ڴ˽���һ��ʱ�䣬ɶʱ���뿪�������
{
	P0 = 0XFF;  //�ر�LED��
	P2 = P2 & 0X1F | 0X80;
	P2 &= 0X1F; //	P2 = P2 & 0X1F;

	P0 = 0;  //�ر����裬���������̵�����
	P2 = P2 & 0X1F | 0XA0;
	P2 &= 0X1F; //	P2 = P2 & 0X1F;	
}