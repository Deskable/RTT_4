#include "key.h"


unsigned char Key_Read(void)
{
	unsigned int Key_New;//16位的一个int型标志位，用来标志是哪个位被按下了
	unsigned char Key_Val;//按键扫描函数的返回值
	
	P44 = 0; P42 = 1; P35 = 1; P34 = 1; //扫描第一列
	Key_New = (P3 & 0x0f);
	P44 = 1; P42 = 0; P35 = 1; P34 = 1; //扫描第二列
	Key_New = ((Key_New<<4)|(P3 & 0x0f));	
	P44 = 1; P42 = 1; P35 = 0; P34 = 1; //扫描第三列
	Key_New = ((Key_New<<4)|(P3 & 0x0f));		
	P44 = 1; P42 = 1; P35 = 1; P34 = 0; //扫描第四列
	Key_New = ((Key_New<<4)|(P3 & 0x0f));		

  switch( ~Key_New )
	{
		case 0x8000: Key_Val = 4; break;
		case 0x4000: Key_Val = 5; break;
		case 0x2000: Key_Val = 6; break;
		case 0x1000: Key_Val = 7; break;	
		case 0x0800: Key_Val = 8; break;
		case 0x0400: Key_Val = 9; break;
		case 0x0200: Key_Val = 10; break;
		case 0x0100: Key_Val = 11; break;		
		case 0x0080: Key_Val = 12; break;
		case 0x0040: Key_Val = 13; break;
		case 0x0020: Key_Val = 14; break;
		case 0x0010: Key_Val = 15; break;		
		case 0x0008: Key_Val = 16; break;
		case 0x0004: Key_Val = 17; break;
		case 0x0002: Key_Val = 18; break;
		case 0x0001: Key_Val = 19; break;		
		default : Key_Val = 0;
	}

	return Key_Val;
	
}	











