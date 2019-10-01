#include "delay.h"
void delay_us(unsigned int t)
{
	 t=t/10;
  while(--t);
}

void delay_ms(unsigned int t)
{
	unsigned int i=0,j=0;
	for(i=0;i<t;i++) 
		for(j=0;j<121;j++);
}