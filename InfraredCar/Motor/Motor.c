/************************************************************************
电机驱动程序

	伺服舵机的控制是通过一段高脉冲的长度来控制的， 1.5ms停止，1.7ms全速正
传，1.3ms全速反转，高脉冲之后为一段20ms左右的低电平
	本程序通过变量speedleft，speedright来控制定时器定时宽度，周期性的执行
speedleft脉冲---低电平延时---speedright脉冲---低电平延时,从而实现不用延时
高精度的控制电机转速
************************************************************************/
#include "motor.h"
sbit left=P1^0;	   	//控制右轮
sbit right=P1^1;	//控制左轮
unsigned char Motor_counter=0;					//电机区间调制循环变量
unsigned int IN_counter=0;						//红外循迹计数器
unsigned int speedleft=1382,speedright=1382;	//1382*1.08507=1.5ms
unsigned char speed_H=0xff;						//TH1 temp
unsigned char speed_L=0x00;						//TL1 temp
unsigned int INT_counter=0;						//红外避障计数器
unsigned char IN_flag=0;						//红外循迹标记位 'R','L'
unsigned char INT_flag=0;						//INT_flag个红外避障检测到障碍物
int Rv=0,Lv=0;									//电机控制-185~185 反转-停止-正传
void InitTimer1(void)
{
	TMOD |= 0x10;
	TH1 = 0x0DC;
	TL1 = 0x00;
	EA  = 1;
	ET1 = 1;
	TR1 = 1;
	PT1 = 1;
	P1  = 0xff;
	P3  = 0xff;
	PT1=1;
}
void Timer1Interrupt(void) interrupt 3
{
	speedleft=1382+Lv;
	speedright=1382-Rv;	
	TH1=speed_H;					
	TL1=speed_L;
	if(IN_counter>0)IN_counter--;		
	if(INT_counter>0)INT_counter--;		
	/*舵机控制*/
	Motor_counter++;									
	if(Motor_counter>7)Motor_counter=0;
	switch(Motor_counter)
	{
		case 0:						
			speed_H=0xf1;			
			speed_L=0x9a;
			left=1;
		break;
		case 1:						
		case 2:						
		case 3:
			speed_H=(0xffff-speedright)>>8;
			speed_L=(0xffff-speedright)&0x00ff;
			left=0;
		break;
		case 4:							
			speed_H=0xf1;
			speed_L=0x9a;
			right=1;
		break;
		case 5:							
		case 6:	
		case 7:			
			speed_H=(0xffff-speedleft)>>8;
			speed_L=(0xffff-speedleft)&0x00ff;
			right=0;
		break;
		default:break;	
	}
}

