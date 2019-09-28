/*******************************************************
2018中国教育机器大赛人双人擂台超声波小车程序
芯片：AT89S52
晶振：11.0592
外围器件：	超声波 			4
			红外循迹 		3
			输入或门		1
			稳压模块		1
						2018/10/2
						赵祥珅
********************************************************/
#include "uart.h"
#include "motor.h"
#include "HCSR04.h"
/*红外循迹引脚定义 1为黑线,0为白线*/
sbit INHL=P3^7;		//左前
sbit INHR=P3^4;		//右前
sbit INBM=P3^5;		//中后

void main()
{
	Uart_Init();
	InitTimer0();
	INT0_Init();
  	InitTimer1();
	while(1)
	{
		//由于红外寻迹的型号不同,有些高电平代表黑色有些相反,因此要根据情况来确定判断条件
		//检测过程中首先保证的是车在比赛场地内(寻迹检测优先),在此基础上检测敌人并发起进攻

		if(IN_counter==0)
		{
			//如果三个方向的寻迹都是黑色,确定了我军在战场内部,那就看周围有没有敌人,有就盘他
			//这里加个判断HCT_counter==0,因为我上次可能正准备盘对面的,还在转呢,这时候就等等,等转完了(盘了上一个,才能在盘下一个)
			if(INHR==1&&INHL==1&&INBM==1)
			{	  
				if(HCT_counter==0)    
				{
					HCT_flag=0;
					if(HCBM<DISTANCE) 		//后中 超声波检测到障碍物
					{
						if(IN_flag=='R')	//如果白线在右边左转
						{								
							Rv=-180;
							Lv=180;		
						}
						else				//否则右转
						{
							Rv=180;
							Lv=-180;
						}
						HCT_flag++;
						HCT_counter=250;
					}
																	  
					if(HCHR<DISTANCE)		//前右 超声波检测到障碍物 右转
					{
						Rv=180;
						Lv=-180;				
						HCT_counter=100;
						HCT_flag++;
					}
					if(HCHL<DISTANCE)		//前左 超声波检测到障碍物 左转
					{
						Rv=-180;
						Lv=180;					
						HCT_counter=100;
						HCT_flag++;
					}
					if(HCHM<DISTANCE)		//前中 超声波检测到障碍物 直走
					{
						Rv=-180;
						Lv=-180;						
						HCT_flag++;
					}
					if(HCT_flag==0)   		//如果都未能检测到 直走
					{
						Rv=-180;
						Lv=-180;						
					}
				}
				else
				{
					if(HCHM<DISTANCE)		//前方超声波检测到
					{
						HCT_counter=0; 		//停止超声波旋转延时
					}
				}
			}
			else 	/*  场地检测  */
			{
				if(INBM==0) 				//中后 红外传感器检测到白线 前进
				{
					Lv=-180;
					Rv=-180;							
					IN_counter=80;			
				}
				if(INHL==0)  				//左前 红外传感器检测到白线 右转
				{		   
					Lv=-180;
					Rv=180;						
					IN_flag='L';
					IN_counter=120;			
				}
				if(INHR==0) 				//右前 红外传感器检测到白线 左转
				{
					Lv=180;
					Rv=-180;							
					IN_flag='R';			
					IN_counter=120;		
				}
			}
		}
		else
		{
			//程序进入这里说明小车因为走到场地边缘正在进行旋转调整
			//如果旋转过程中前方超声波模块检测到敌人在正前方,别犹豫,盘它
			//旋转调整停止,开始盘对面的
			if(HCHM<DISTANCE)
			{
				IN_counter=0;
			}	
		}
	}
}










