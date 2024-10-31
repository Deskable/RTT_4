#include "delay.h"

void Delay1ms()		//@12.000MHz
{
	unsigned char i, j;

	i = 12;
	j = 169;
	do
	{
		while (--j);
	} while (--i);
}

void Delaynms( unsigned int n )
{
	unsigned int i;
	for ( i=1;i<=n;i++ )
		Delay1ms();
}

