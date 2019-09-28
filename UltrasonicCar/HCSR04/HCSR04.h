

#ifndef __HCSR04_H_
#define __HCSR04_H_


#include <reg52.h>
#include "uart.h"

void INT0_Init();
void HC_delay(void);
void HC_start(unsigned char n);
extern unsigned int distance[4];
extern unsigned int HC_counter;
void InitTimer0(void);

#define HCHR distance[0]
#define HCHL distance[1]
#define HCBM distance[2]
#define HCHM distance[3]

#define DISTANCE 3000				//改变该值可调整超声波检测的最远距离




#endif