/*******************************************************
2016中国教育机器大赛人双人擂台超声波小车程序
芯片：STC12C5A60S2
晶振：11.0592
外围器件：	0.4W红外发射管			6
			3W大功率红外发射管		1
			8050三极管				12
			降压模块				1
********************************************************/
#include "INSend.h"
#include "ADC.h"
#include "delay.h"
#include "uart.h"
void main(void) 
{  
	unsigned char Motor;
	unsigned int Motor_temp;
	INSend_Init();
	InitADC();
	Uart_Init();
	while(1) 
	{ 
		unsigned char i;
		Motor_temp=0;
		for(i=0;i<100;i++)
		{
			Motor_temp+=GetADCResult(0); 
		}
		Motor_temp/=1600;									//取平均值
		Motor_temp=15-Motor_temp;					//X轴取反
		Motor=(unsigned char)Motor_temp;
		Motor<<=4;												//装载高四位数据
		Motor_temp=0;
		for(i=0;i<100;i++)
		{
			Motor_temp+=(GetADCResult(1)); 
		}
		Motor_temp/=1600;									//Y轴
		Motor|=(unsigned char)Motor_temp; //装载低四位数据
		
		SendIRdata(Motor);
		printf("%x\n",Motor);
		delay_ms(110);
	} 
}



