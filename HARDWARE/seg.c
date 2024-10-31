#include "seg.h"


//ת�����������ַ���ת��Ϊ���麯���������ڲ���Ϊ����
//pucSeg_BufΪһ�����飬�ڲ���ŵ����ַ���,�Ϊ8�����硰12.345678��
//pucSeg_CodeΪһ�����飬�ڲ���ŵ����ַ���ת��֮��Ķ��룬�Ҳ��8�������磬����ת��ΪF9 A4 B0....

void Seg_Tran(unsigned char *pucSeg_Buf , unsigned char *pucSeg_Code )
{
	unsigned char i, j=0, temp;
	
	for(i=0; i<8; i++, j++)
	{
		switch(pucSeg_Buf[j])
		{
			case '0': temp= 0xC0;  break;
			case '1': temp= 0xF9;  break;			
			case '2': temp= 0xA4;  break;
			case '3': temp= 0xB0;  break;	
			case '4': temp= 0x99;  break;
			case '5': temp= 0x92;  break;			
			case '6': temp= 0x82;  break;
			case '7': temp= 0xF8;  break;	
			case '8': temp= 0x80;  break;			
			case '9': temp= 0x90;  break;
			
			case 'A': temp= 0x88;  break;	
			case 'B': temp= 0x83;  break;
			case 'C': temp= 0xC6;  break;	
			case 'D': temp= 0xA1;  break;			
			case 'E': temp= 0x86;  break;
			case 'F': temp= 0x8E;  break;	
			
			case 'T': temp= 0x87;  break;	
			case 'Y': temp= 0x91;  break;	
			
			case 'H': temp= 0x89;  break;	
			case 'L': temp= 0xC7;  break;
			case 'N': temp= 0xC8;  break;	
			case 'P': temp= 0x8C;  break;			
			case 'U': temp= 0xC1;  break;
			case '-': temp= 0xBF;  break;		
			case ' ': temp= 0xFF;  break;
			default: temp= 0xFF; 						
		}
		
		if(pucSeg_Buf[j+1] == '.')
		{
			temp= temp & 0x7F;
			j++;	
		}
		
		pucSeg_Code[i] = temp;
	}
}

//����������ʾ�������������е���ֵ��ʾ����
//pucSeg_CodeΪҪ��ʾ��������ֵ
//ucSeg_PosΪҪ������ܵڼ���λ����ʾ

void Seg_Disp( unsigned char *pucSeg_Code, unsigned char ucSeg_Pos)
{
	P0 = 0XFF;  //��ѡ����������Ķ�������
	P2 = P2 & 0X1F | 0XE0;
	P2 &= 0X1F; //	P2 = P2 & 0X1F;		
	
	P0 = 1<<ucSeg_Pos;  //λѡ�����Ƶڼ��������λѡͨ
	P2 = P2 & 0X1F | 0XC0;
	P2 &= 0X1F; //	P2 = P2 & 0X1F;
	
	P0 = pucSeg_Code[ucSeg_Pos];  //��ѡ����������Ķ�������
	P2 = P2 & 0X1F | 0XE0;
	P2 &= 0X1F; //	P2 = P2 & 0X1F;	
}










