#include "ADC.h"
#include "intrins.h"
#include "uart.h"

#define ADC_FLAG    0x10            //ADC complete flag
BYTE GetADCResult(BYTE ch)
{
    ADC_CONTR = 0x80 | 0x00 | ch | 0x08;
    _nop_();                        //Must wait before inquiry
    _nop_();
    _nop_();
    _nop_();
    while (!(ADC_CONTR & ADC_FLAG));//Wait complete flag
    ADC_CONTR &= ~ADC_FLAG;         //Close ADC

    return ADC_RES;                 //Return ADC result
}

void InitADC()
{
    P1ASF = 0xff;                   //Open 8 channels ADC function
    ADC_RES = 0;                    //Clear previous result
    ADC_CONTR = 0x80 | 0x08;
    Delay1(2);                       //ADC power-on and delay
}

void Delay1(WORD n)
{
    WORD x;

    while (n--)
    {
        x = 5000;
        while (x--);
    }
}

