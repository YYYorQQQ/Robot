#include "uart.h"
#include "motor.h"
void Uart_Init()  //定时器2作为波特率发生器
{
	TCLK=1;
	RCLK=1; 
	RCAP2H=0xFF;  //9600bps  11.0592MHz
	RCAP2L=0xDC;
	SCON=0x50; 
	PCON=0x00; 
	TR2=1;
	EA=1;
	ET2=1;
	PS=0;
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




