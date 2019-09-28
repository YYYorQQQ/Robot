

/************************************************************************
电机驱动程序

		伺服舵机的控制是通过一段高脉冲的长度来控制的， 1.5ms停止，1.7ms全速正
传，1.3ms全速反转，高脉冲之后为一段20ms左右的低电平。
		本程序通过变量speedleft，speedright来控制定时器定时宽度，周期性的执行
speedleft脉冲---低电平延时---speedright脉冲---低电平延时,从而实现不用延时
高精度的控制电机转速。
************************************************************************/

#include "motor.h"
#include "HCSR04.h"
sbit left  = P1^0;	//控制右轮
sbit right = P1^1;	//控制左轮

unsigned char Motor_counter=0;					//电机区间调制循环变量
unsigned char IN_counter=0;						//红外循迹计数器
unsigned int speedleft=1382,speedright=1382;	//1382*1.08507=1.5ms	(1382+180)*1.08507=1.7ms	(1382-180)*1.08507=1.3ms
unsigned char speed_H=0xff;						//TH1 temp
unsigned char speed_L=0x00;						//TL1 temp
unsigned int HCT_counter=0;						//超声波计数器
unsigned char IN_flag=0;						//红外循迹标记位 'R','L'
unsigned char HCT_flag=0;						//HCT_flag个超声波检测到障碍物
int Rv=0,Lv=0;
		
												//电机控制-185~185 反转-停止-正传
void InitTimer1(void)     
{
	TMOD |= 0x10;		 //0001 0000  方式1 16位定时器模式
    TH1 = 0xDC;			 //11.0592M晶振是10ms,10ms后定时器1溢出,自此之后定时器1的溢出时间将不再是10ms
    TL1 = 0x00;
    EA  = 1;
    ET1 = 1;
    TR1 = 1;
	PT1 = 1;			 //定时器1设置为高优先级
}
void Timer1Interrupt(void) interrupt 3
{
	speedleft = 1382 + Lv;
	speedright = 1382 - Rv;		//转换速度值
	TH1 = speed_H;				//使用上一次计算好的定时器宽度装初值
	TL1 = speed_L;
	if(IN_counter>0) IN_counter--;			//红外循迹延时
	if(HCT_counter>0) HCT_counter--;			//超声波延时

	/*舵机控制*/
	Motor_counter++;									
	if(Motor_counter > 7)
		Motor_counter = 0;


	//此处通过高脉冲长度控制舵机转速
	switch(Motor_counter)
	{
		case 0:
			left = 1;	 						//Left高脉冲
			speed_H = 0xf1;					//为下一个区间(低电平延时)装定时器初值
			speed_L = 0x9a;					//高脉冲计数值37114
			break;
		case 1:								//低电平延时
		case 2:								//为使Time1具有更准确的延时作用，将低电平延时分为多个区间
		case 3:
			left=0;
			speed_H = (0xffff - speedright) >> 8;		   //speed_H = speedright高8位	
			speed_L = (0xffff - speedright) & 0x00ff;	   //speed_L = speedright低8位	脉冲计数值为 speedright
			break;
		case 4:								//Right高脉冲
			speed_H = 0xf1;
			speed_L = 0x9a;
			right = 1;
			break;
		case 5:								//低电平延时
		case 6:	
		case 7:			
			speed_H = (0xffff - speedleft) >> 8;
			speed_L = (0xffff - speedleft) & 0x00ff;
			right=0;
			break;
		default:break;	
	}
}

