#include "INSend.h"
#define k 2
sbit OUT=P2^1; 
static bit OP; //红外发射管的亮灭 
static unsigned int count; //延时计数器 
static unsigned int endcount; //终止延时计数 
static unsigned char flag; //红外发送标志 
char iraddr1; //十六位地址的第一个字节 
char iraddr2; //十六位地址的第二个字节 
char senddat[5]=0x55; 
void delay1ms(unsigned int i) 
{ 
	unsigned char j; 
	while(i--) 
	{ 
		for(j=0;j<115;j++); //1ms基准延时程序 
	} 
} 
void InitTimer0(void)
{
	TMOD = 0x01;
	TH0 = 0x0FF;
	TL0 = 0x0F4;
	EA = 1;
	ET0 = 1;
	TR0 = 1;
}
void INSend_Init(void)
{
	InitTimer0();
	count = 0;
	flag = 0; 
	OP = 0; 
	OUT = 0;  
	iraddr1=0x00; 
	iraddr2=0x01; 
}
void SendIRdata(char p_irdata) 
{ 
	int i; 
	char irdata; //发送9ms的起始码 
	endcount=346*k; 
	flag=1; 
	count=0; 
	do{}while(count<endcount); //发送4.5ms的结果码 
	endcount=173*k ; 
	flag=0; 
	count=0; 
	do{}while(count<endcount); 
	//发送十六位地址的前八位 
	irdata=iraddr1; 
	for(i=0;i<8;i++) 
	{ //先发送0.56ms的38KHZ红外波即编码中0.56ms的低电平 
		endcount=21*k; 
		flag=1; 
		count=0; 
		do
		{}while(count<endcount); //停止发送红外信号即编码中的高电平 
		//if(irdata&0X01) //判断二进制数个位为1还是0 
		if(irdata-(irdata/2)*2) 
		{ 
			endcount=65*k; //1为宽的高电平 
		} 
		else 
		{ 
			endcount=21.5*k; //0为窄的高电平 
		} 
		flag=0; 
		count=0; 
		do{}while(count<endcount); 
		irdata=irdata>>1; 
	} 
	//发送十六位地址的后八位 
	irdata=iraddr2; 
	for(i=0;i<8;i++) 
	{ 
		endcount=21.5*k; 
		flag=1; 
		count=0; 
		do{}while(count<endcount); 
		// if(irdata&0X01) 
		if(irdata-(irdata/2)*2) 
		{ 
			endcount=21.5*k; 
		} 
		else 
		{ 
			endcount=21.5*k; 
		} 
		flag=0; 
		count=0; 
		do{}while(count<endcount); 
		irdata=irdata>>1; 
	} 

	//发送八位数据 
	irdata=p_irdata; 
	for(i=0;i<8;i++) 
	{ 
		endcount=21.5*k; 
		flag=1; 
		count=0; 
		do{}while(count<endcount); 
		if(irdata-(irdata/2)*2) 
		//if(irdata&0X01) 
		{ 
			endcount=65*k; 
		} 
		else 
		{ 
			endcount=21.5*k; 
		} 
		flag=0; 
		count=0; 
		do{}while(count<endcount); 
		irdata=irdata>>1; 
	} 
	//发送后八位数据 
	irdata=~p_irdata; 
	for(i=0;i<8;i++) 
	{ 
		endcount=21.5*k; 
		flag=1; 
		count=0; 
		do{}while(count<endcount); 
		if(irdata-(irdata/2)*2) 
		{ 
		endcount=65.5*k; 
			} 
		else 
		{ 
			endcount=21.5*k; 
		} 
		flag=0; 
		count=0; 
		do{}while(count<endcount); 
		irdata=irdata>>1; 
	} 
	endcount=65*k; 
	flag=1; 
	count=0; 
	do{}while(count<endcount); 
	flag=0; 
} 
void DelayUs2x(unsigned char t) 
{ 
	while(--t); 
} 

void DelayMs(unsigned char t) 
{ 
	while(t--) 
	{ 
		//大致延时1mS 
		DelayUs2x(245*k); 
		DelayUs2x(245*k); 
	} 
}  
void timeint(void) interrupt 1 
{ 
	TH0 = 0x0FF;
	TL0 = 0x0F4;;
	count++; 
	if(flag) 
	OUT=~OUT; 
} 

/*void delay() 
{ 
	int i,j; 
	for(i=0;i<400;i++) 
		for(j=0;j<100;j++); 
	 
}*/ 
