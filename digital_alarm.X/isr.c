/*
#include "main.h"
#include "timer0.h"

extern unsigned char flag;
static unsigned int count=0;
void __interrupt() isr(void)
{
   
    if(TMR0IF)
    {
        TMR0=TMR0+8;
        if(count++==20000*5)
        {
            count=0;
            flag=1; 
            TMR0IF=0;
        }
    }
}*/
