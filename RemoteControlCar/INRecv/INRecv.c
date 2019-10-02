#include "INRecv.h"
#include "uart.h"
#include "motor.h"
unsigned short IrValue[6];
unsigned char Time;
unsigned char Control=0x77;
unsigned int Lost_counter=0;   	//遥控器连接超时计数器
bit INRecv_flag=0;				//接收遥控器信号标志位,1代表接收到,0代表未接收到
sbit IRIN=P3^2;
void DelayMs(unsigned int x)   	//0.014ms
{
	unsigned char i;
	while(x--)
	{
		for (i = 0; i<13; i++);
	}
}
void INT0_Init()
{
	IT0=1;       	//下降沿触发
	EX0=1;
	PX0=0;
	EA=1;
}

void INT_0() interrupt 0
{
	INRecv_flag=1;	//接收标记
}
void INRecv(void)
{
	unsigned int j,k;
	unsigned int err;
	EX0=0;
	INRecv_flag=0;
	Time=0;	
//	DelayMs(70);
//	if(Lost_counter>2000)
//		Control=0xee;
	
	if(IRIN==0)			//确认是否真的接收到正确的信号
	{	 
		printf("IRAN begin!!\r\n");
		err=850;				//1000*10us=10ms,超过说明接收到错误的信号
		/*当两个条件都为真是循环，如果有一个条件为假的时候跳出循环，免得程序出错的时
		侯，程序死在这里*/	
		while((IRIN==0)&&(err>0))	//等待前面9ms的低电平过去  		
		{			
			DelayMs(1);	//0.014ms
			err--;
		} 
		if(IRIN==1)					//如果正确等到9ms低电平
		{
			err=500;
			while((IRIN==1)&&(err>0))	//等待4.5ms的起始高电平过去
			{
				DelayMs(1);
				err--;
			}
			for(k=0;k<1;k++)		//共有4组数据
			{				
				for(j=0;j<8;j++)	//接收一组数据
				{

					err=60;		
					while((IRIN==0)&&(err>0))	//等待信号前面的560us低电平过去
					{
						DelayMs(1);
						err--;
					}
					err=500;
					while((IRIN==1)&&(err>0))	//计算高电平的时间长度。
					{
						DelayMs(1);//0.014ms
						Time++;
						err--;
						if(Time>300)	  
						{
							printf("Time is too long !!\r\n");
							EX0=1;
							return;
						}
					}
					IrValue[k]>>=1;	 	//k表示第几组数据
					if(Time>=8)			//如果高电平出现大于565us，那么是1
					{
						IrValue[k]|=0x80;
					}
					Time=0;				//用完时间要重新赋值							
				}
				//printf("======== the %d data %x ======= \r\n", k, IrValue[k]);
			}
		}
//		if(IrValue[2]!=~IrValue[3])
//		{
//			printf("Data is error !!\r\n");
//			EX0=1;
//			return;
//		}
		printf("IrValue[0]: %x\n", IrValue[0]);
		if (IrValue[0] == 0x8d) {
			Control = 0xee; //前进
		} else if (IrValue[0] == 0x2b) {
			Control = 0x00;//后退
		} else if (IrValue[0] == 0x87) {
			Control = 0xe0;//右转
		} else if(IrValue[0] == 0x89) {
			Control = 0x0e;//左转
		} else if(IrValue[0] == 0x81) {
			Control = 0x77;//停止
		} else {
		   	Control = 0xee;//按错键-前进
		}

		//Control=IrValue[0];		 //将接收到的数据的数据码存储起来,用来调整舵机
		Lost_counter=0;
		printf("Control : %x\r\n",Control);
	}
	MotorControl();
	EX0=1;
}