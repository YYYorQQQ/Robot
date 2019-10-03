#ifndef __ADC_H_
#define __ADC_H_
#include "STC12C5A60S2.h"
typedef unsigned char BYTE;
typedef unsigned int WORD;
void InitADC();
BYTE GetADCResult(BYTE ch);
void Delay1(WORD n);
#endif