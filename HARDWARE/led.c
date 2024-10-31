#include "led.h"

void Led_Disp( unsigned char ucLed )
{
	P0 = ~ucLed;
	P2 = P2 & 0X1F | 0X80;
	P2 &= 0X1F; //	P2 = P2 & 0X1F;	
}

