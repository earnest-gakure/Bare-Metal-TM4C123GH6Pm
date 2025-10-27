#include "TM4C123.h"                    // Device header

int main()
{
	SYSCTL->RCGCGPIO = 0x020;
	GPIOF->DIR = 0x02;
	GPIOF->DEN = 0x02;
	GPIOF->DATA = 0x02;
	
}