/*******************************************************
2018中国教育机器大赛人双人擂台红外遥控小车程序
芯片：AT89S52
晶振：11.0592
外围器件：红外接收管 	6	
		  红外循迹 		3
		  升降压模块	1
		  6输入与门		1
********************************************************/
#include "uart.h"
#include "motor.h"
#include "INRecv.h"
void main()
{
	InitTimer1();
	INT0_Init();
	Uart_Init();
	Control = 0xee;
	//printf("while begin~~\r\n");
	while(1)
	{
//		while(!INRecv_flag)			//没有收到遥控器的信号通知则等待
//		{
//			if(Lost_counter>2000)	//长时间没有接收到遥控器信号
//			{	
//				Lost_counter = 0;
//				Control=0x0e;	 
//				MotorControl();
//			}
//		}
		INRecv();
	}
}
