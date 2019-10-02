/************************************************************************
电机驱动程序

		伺服舵机的控制是通过一段高脉冲的长度来控制的， 1.5ms停止，1.7ms全速正
传，1.3ms全速反转，高脉冲之后为一段20ms左右的低电平。
		本程序通过变量speedleft，speedright来控制定时器定时宽度，周期性的执行
speedleft脉冲---低电平延时---speedright脉冲---低电平延时,从而实现不用延时
高精度的控制电机转速。
************************************************************************/
#include "motor.h"
#include "INRecv.h"
#define MAX_SPEED 200
sbit left=P1^0;	   //控制右轮
sbit right=P1^1;	 //控制左轮
/*红外循迹引脚定义 1为黑线，0为白线*/
sbit INHL=P3^6;
sbit INHR=P3^7;
sbit INBM=P3^5;

unsigned char Motor_counter=0;								//电机区间调制循环变量
unsigned char IN_counter=0;										//红外循迹计数器
unsigned int speedleft=1382,speedright=1382;	//1382*1.08507=1.5ms
unsigned char speed_H=0xff;										//TH1 temp
unsigned char speed_L=0x00;										//TL1 temp
int Rv=0,Lv=0;													//电机控制-185~185 反转-停止-正传
int Rv_temp,Lv_temp;											//Rv temp、Lv temp

void MotorControl(void)											//将接收到的数据处理成电机速度控制量
{
	//7表示停止,0和f表示全速转
	Lv_temp = ((Control/16) - 7) * 30;
	Rv_temp = ((Control&0x0f) - 7) * 30;

//	int X_temp,Y_temp;													
//	X_temp=Control/16;
//	Y_temp=Control&0x0f;				  //分离X、Y数据
//	X_temp-=7;
//	Y_temp-=7;
//	Lv_temp=(X_temp-Y_temp)*26;
//	Rv_temp=(X_temp+Y_temp)*26;									//转化速度
}
void InitTimer1(void)
{
	TMOD |= 0x10;
    TH1 = 0x0DC;
    TL1 = 0x00;
    EA = 1;
    ET1 = 1;
    TR1 = 1;
	PT1=1;
}
void Timer1Interrupt(void) interrupt 3
{
	speedleft=1382+Lv;
	speedright=1382-Rv;			//转换速度值
	TH1=speed_H;						//使用上一次计算好的定时器宽度装初值
	TL1=speed_L;
	if(IN_counter>0)IN_counter--;		//红外循迹延时
	Motor_counter++;									  
	if(Motor_counter>7)Motor_counter=0;
	Lost_counter++;						//遥控器超时计数

	if(IN_counter==0)					//红外循迹延时结束
	{
		if(INHR==1&&INHL==1&&INBM==1)
		{
			Rv=Rv_temp;
			Lv=Lv_temp;						//遥控速度
		}
		else
		{
			if(INHR==0)						//右
			{
				Lv=-MAX_SPEED;
				Rv=MAX_SPEED;				//左转
				IN_counter=80;
			}
			if(INHL==0)						//左
			{
				Lv=MAX_SPEED;
				Rv=-MAX_SPEED;				//右转
				IN_counter=80;
			}
			if(INBM==0)						//后
			{
				Lv=MAX_SPEED;
				Rv=MAX_SPEED;				//前进
				IN_counter=100;
			}
		}
	}	
	switch(Motor_counter)
	{
		case 0:								//Left高脉冲
			speed_H=0xf1;				//为下一个区间(低电平延时)装定时器初值
			speed_L=0x9a;
			left=1;
		break;
		case 1:								//低电平延时
		case 2:								//为使Time1具有更准确的延时作用，将低电平延时分为多个区间
		case 3:
			speed_H=(0xffff-speedright)>>8;
			speed_L=(0xffff-speedright)&0x00ff;
			left=0;
		break;
		case 4:								//Right高脉冲
			speed_H=0xf1;
			speed_L=0x9a;
			right=1;
		break;
		case 5:								//低电平延时
		case 6:	
		case 7:			
			speed_H=(0xffff-speedleft)>>8;
			speed_L=(0xffff-speedleft)&0x00ff;
			right=0;
		break;
		default:break;	
	}
}

