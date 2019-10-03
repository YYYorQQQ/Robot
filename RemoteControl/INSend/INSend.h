#ifndef __INSEND_H_
#define __INSEND_H_
#include "STC12C5A60S2.h"
#include "stdio.h"
void SendIRdata(char p_irdata); 
void DelayUs2x(unsigned char t); 
void DelayMs(unsigned char t); 
void delay(); 
void IT0_Init(void);
void INSend_Init(void);
#endif