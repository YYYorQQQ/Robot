

#include "uart.h"
#include "motor.h"


/*
* 超声波调试时发送数据使用
* 定义定时器2（T2）作为波特率发生器
* 9600bps  11.0592MHz
*/
void Uart_Init()  //定时器2作为波特率发生器
{
	TCLK=1;		  //选择定时器2作为串行发送波特率发生器 
	RCLK=1; 	  //选择定时器2作为串行接受波特率发生器
				  //T2CON  T2定时器寄存器
	

	RCAP2H=0xFF;  
	RCAP2L=0xDC;  //9600bps  11.0592MHz

	SCON=0x50; 	  //串行口控制寄存器	0101 0000 10位异步收发(8位数据)	允许串行口接收数据
				  //D0 D1 发送/接受中断标志位  内部硬件值1  必需软件清零
				  //方式1   TXD-P3.1    RXD-P3.0

	PCON=0x00; 	  //电源管理寄存器

	TR2=1;		  //启动标志-启动T2
	EA=1;
	ET2=1;		  //使能
	PS=0;		  //串行口中断定义为低优先级中断
}


/*
* 中断服务函数
*/
void Usart() interrupt 4
{
	unsigned char receiveData;
	receiveData=SBUF; 
	RI = 0;       //清除接收中断标志位
}



/*
* 
*/
char putchar(char c)	//putchar重载，可直接调用printf，若使用scanf请重载getchar
{  
	hal_uart_putchar(c);  
	return c;  
}  


/*
*
*/
void hal_uart_putchar(char i) 
{  
	ES = 0;  		//关闭串口中断 
	TI = 0; 		//清除发送中断标志位
	SBUF = i;   
	while(TI == 0);	//等待中断发生，中断发生TI硬件置1 
	TI = 0;   		//清除发送中断标志位
	ES = 1;  		//开启串口中断
}  







