/*******************************************************
2018中国教育机器大赛人双人擂台红外小车程序
芯片：AT89S52
晶振：11.0592
外围器件：	红外避障 		4	
			红外循迹 		3
			升降压模块	    1
********************************************************/
#include "uart.h"
#include "motor.h"
/*红外循迹引脚定义*/
sbit INHL=P3^7;
sbit INHR=P3^6;
sbit INBM=P3^5;
/*红外避障引脚定义*/
sbit INTHM=P3^2;
sbit INTHL=P1^2;
sbit INTHR=P1^3;
sbit INTBM=P1^4;

#define MAX_SPEED 200

void main()
{
	Uart_Init();
	InitTimer1();
	while(1)
	{	
		//红外相比于超声波来说不用测距,更加简单
		if(IN_counter==0)				//红外循迹延时结束
		{
			if(INHR==1&&INHL==1&&INBM==1)
			{
				if(INT_counter==0)      //敌方检测，红外避障延时结束
				{
					INT_flag=0;
					if(INTBM==0)    		//后方检测到敌人存在
					{
						if(IN_flag=='R')	//白线在右边向左旋转
						{
							Rv=MAX_SPEED;
							Lv=-MAX_SPEED;		//白线在左边向右旋转
						}
						else
						{
							Rv=-MAX_SPEED;
							Lv=MAX_SPEED;
						}
						INT_counter=400;
						INT_flag++;
					}
					if(INTHR==0)  			//右方检测到敌人存在
					{
						Rv=-MAX_SPEED;
						Lv=MAX_SPEED;
						INT_counter=200;
						INT_flag++;
					}
					if(INTHL==0)  			//左放检测到敌人存在
					{
						Rv=MAX_SPEED;
						Lv=-MAX_SPEED;
						INT_counter=200;
						INT_flag++;
					}
					if(INTHM==0)			//前方检测到敌人存在
					{
						Rv=MAX_SPEED;
						Lv=MAX_SPEED;
						INT_flag++;
					}
					if(INT_flag==0) 		//如果都未能检测到,全速直行
					{
						Rv=MAX_SPEED;
						Lv=MAX_SPEED;
					}
				}
				else
				{
					if(INTHM==0)			//前方红外避障检测到
					{
						INT_counter=0;		//停止红外避障旋转延时	
					}
				}
			}
			else 										//场地检测
			{
				if(INBM == 0) 					//后
				{
					Lv=MAX_SPEED;		
					Rv=MAX_SPEED;						//前进
					IN_counter=200;
				}
				if(INHL == 0)  						//左
				{
					Lv=MAX_SPEED;
					Rv=-MAX_SPEED;					//右转
					IN_flag='L';
					IN_counter=200;	
				}
				if(INHR == 0) 						//右
				{
					Lv=-MAX_SPEED;
					Rv=MAX_SPEED;						//左转
					IN_flag='R';
					IN_counter=200;
				}

			}
		}
		else
		{
			if(INTHM==0)						//前方红外避障检测到
			{
				IN_counter=0;					//停止红外循迹旋转延时
			}	
		}
	}
}
