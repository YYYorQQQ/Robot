#ifndef __INRECV_H_
#define __INRECV_H_
#include "reg52.h"
#include "uart.h"
void INT0_Init();
void INT1_Init();
void INRecv(void);
extern bit INRecv_flag;
extern unsigned char Control;
extern unsigned int Lost_counter;
#endif