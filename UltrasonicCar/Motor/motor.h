#ifndef __MOTOR_H_
#define __MOTOR_H_
#include <reg52.h>
#include "uart.h"

extern int Rv,Lv;
extern unsigned char Motor_counter;
extern unsigned char IN_counter;
extern unsigned int speedleft,speedright;
extern unsigned char speed_H;
extern unsigned char speed_L;
extern unsigned int HCT_counter;
extern unsigned char IN_flag;
extern unsigned char HCT_flag;
void InitTimer1(void);


#endif