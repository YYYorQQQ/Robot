#include "uart.h"
void Uart_Init()
{
    TMOD |= 0x20;
    SCON |= 0x50;
    TH1 = 0xFD;
    TL1 = TH1;
    PCON = 0x00;
    EA = 1;
    ES = 1;
    TR1 = 1;
}
void Usart() interrupt 4
{
	unsigned char receiveData;
	receiveData=SBUF; 
	RI = 0;           //清除接收中断标志位
}
char putchar(char c)	//putchar重载，可直接调用printf，若使用scanf请重载getchar
{  
	hal_uart_putchar(c);  
	return c;  
}  
void hal_uart_putchar(char i) 
{  
	ES = 0;  
	TI = 0; 
	SBUF = i;   
	while(TI == 0); 
	TI = 0;   
	ES = 1;  
}  





