
#include <xc.h>
#include "timer0.h"
#include "clcd.h"
#include "timer1.h"
extern unsigned char flag;
extern unsigned char f;
extern unsigned long int duration_;
extern unsigned int T;
extern unsigned int F;
unsigned int seconds_2=0;
unsigned int seconds_5=0;
extern unsigned int event_count;
extern unsigned char view_event[10][18];
extern unsigned char time[9];
extern int match;
int seconds_flag=0;
extern unsigned  int p;
extern int stop;

void __interrupt() isr(void) {
    static unsigned long int count = 0;
    static unsigned long int sec_5_count=0;    //for 5 seconds
    static unsigned long int sec_2_count=0;   //for 2 seconds
  //  static unsigned long int p=0;
   
    if (TMR0IF) {
        TMR0 = TMR0 + 8; 
       
        if(seconds_5==0)
        {
          if(sec_5_count++==80000)
           {
            seconds_2=1;
            seconds_5=1;
           }
        }
        if(seconds_2==1)
        {
           if(sec_2_count++==40000)
           {
            sec_2_count=0;
            seconds_2=0;
            sec_5_count=0;
            seconds_5=0;
            seconds_flag=0;
            flag=0;
            f=0;
           }
           else
           {
               if(match==1)
               {
                if(p<event_count)
                {
                 p++;
                }
                else
                {
                   p=0;
                }
                match=0;
               }
               seconds_flag=1;
              }   
           }
        
        
        if (F == 1) {
            if (count++ != duration_) {
              //  RE0 = 1;
                TRISB0 = 0;
                RB0 = 1;      
            } else {
                TRISB0 = 0;
                RB0 = 0;
                F = 0;
                 count = 0;
                // RE0 = 1;
            }
        }
        TMR0IF = 0;
      }
    }
        
    
  

