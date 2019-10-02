#ifndef __MOTOR_H_
#define __MOTOR_H_
#include <reg52.h>
#include "uart.h"
extern int Rv,Lv;
extern unsigned char speed;
void InitTimer1(void);
void MotorControl(void);
#endif