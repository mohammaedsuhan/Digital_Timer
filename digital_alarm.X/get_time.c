
#include <string.h>
#include "get_time.h"
#include "main.h"
extern unsigned char AM_PM2;
extern unsigned char date[16];
extern unsigned char time[9];       // Stores current time in the format HH:MM:SS unsigned char date[16];       // Stores current date in the format DD-MM-YYYY
// Stores the event duration (e.g., "00:30:00" for 30 minutes)
unsigned int  event_day;      // Stores the day of the week the event is scheduled for (0 = Sunday, ..., 6 = Saturday)
extern unsigned int event_count;    // Keeps track of number of events stored
//extern unsigned char current_day;    // Holds the current day of the week
extern unsigned char view_event[10][18];
unsigned int hour,minute,sec;
unsigned int day,month,year;
unsigned long int duration_=0;
extern unsigned int F;
extern unsigned char f;
extern unsigned char flag;
extern unsigned char clock_reg[3];
extern int match;

void current_time(void)
{
    hour = (time[0] - '0')*10+(time[1] - '0');
    minute = (time[3] - '0') * 10 + (time[4] - '0');
    sec = (time[6] - '0') * 10 + (time[7] - '0');
}
void current_date(void)
{
    day = (date[0] - '0') * 10 + (date[1] - '0');
    month = (date[3] - '0') * 10 + (date[4] - '0');
    year = (date[6] - '0') * 1000 + (date[7] - '0') * 100 + (date[8] - '0') * 10 + (date[9] - '0');
}
unsigned int get_current_day(void)
{
    current_date();  
    unsigned int current_day = (date[0] - '0') * 10 + (date[1] - '0');
    return current_day;  
}
/*
unsigned char check_event_time(void)
{
    get_time();  
    unsigned char ptr[10];
    for(int i=0;i<10;i++)
    {
        
        for(int j=0;j<8;j++)
        {
         ptr[j]=view_event[i][j];
        }
         ptr[5]=':';
         ptr[8]='\0';
         if (time[0] & 0x40) 
         {
           if (time[0] & 0x20)
           {
           
                time[6]='P';
                time[7]='M';
           } 
           else 
           {
                time[6]='A';
                time[7]='M';
           }
             
        }
        time[8]='\0';

       if (strcmp(time,ptr) == 0)
       {
        return 1; 
       }
    }
    return 0; 
}
*/

signed int check_event_time()
{
    get_time();
    unsigned char ptr[10];
    for(unsigned int i=0;i<event_count;i++)
    {
        if(AM_PM2==view_event[i][6])
        {
             for(int j=0;j<8;j++)
             {
              ptr[j]=view_event[i][j];
             }
             ptr[5]=':';
             ptr[8]='\0';
             if (clock_reg[0] & 0x40) 
             {
             if (clock_reg[0] & 0x20)
             {
                time[6]='P';
                time[7]='M';
             } 
             else 
             {
                time[6]='A';
                time[7]='M';
             }  
             }
         time[8]='\0';
         CLEAR_DISP_SCREEN;
        // clcd_print(ptr,LINE1(0));
        // clcd_print(time,LINE2(0));
      
        // f=0;
        // flag=0;
          // clcd_print("hello",LINE1(0));
        if (strcmp(time,ptr) == 0)
          {
            CLEAR_DISP_SCREEN;
           //clcd_print("hello",LINE1(0));
           //clcd_print("hello",LINE1(0));
            calculate_duration(i);
            match=1;
             //return i; 
          }
        }
    }
    return -1;
}
unsigned char check_weekly_event(void)
{
    get_date();
    unsigned int current_day_of_week = get_current_day();
    event_day=read_EEPROM(0x0A)-'0';
    if (current_day_of_week == event_day)  
    {
       
        if(check_event_time())  
        {
            return 1;  
        }
    }
    return 0;  
}

unsigned char check_daily_event(void)
{
  
    if (check_event_time())  
    {
        return 1;  
    }
    return 0;  
}

unsigned char check_onetime_event(void)
{
   // get_date(); 
    if(check_event_time())
    {
        return 1;
    }
    return 0;  
}
void calculate_duration(unsigned int k)
{
    CLEAR_DISP_SCREEN; 
    unsigned long int temp_minute=(10*(view_event[k][11]-48))+(view_event[k][12]-48);
    unsigned long int temp_sec=(10*(view_event[k][14]-48))+(view_event[k][15]-48);
    duration_=(temp_minute*60*20000)+(temp_sec*20000);
    int j=0;
    if(view_event[k][17]=='O')
    {
        unsigned int i=k;
        for( i=k;i<event_count;i++)
        {
            if((i+1)<event_count)
            strcpy(view_event[i],view_event[i+1]);  
        }
        event_count=i-1;
    }
    for(int i=15;i>=0;i--)
    {
        if(duration_&(1<<i))
        {
            clcd_putch('1',LINE1(j));
            j++;
        }
        else 
        {
            clcd_putch('0',LINE1(j));
            j++;
        }
    }
    F=1;      
}
 
void check_event_trigger(void)
{
    for (unsigned int i = 0; i < event_count; i++)  
    {
        if (view_event[i][17]=='W')  
        {
            int ch;
          //  CLEAR_DISP_SCREEN;
           // clcd_print("haiee WORLD     ",LINE1(0));
            check_weekly_event();
           /* if ((ch=check_weekly_event()) != -1)  
            { 
                calculate_duration(ch); 
            }*/
        }
        else if (view_event[i][17]=='D')  
        {
            int ch;
             // CLEAR_DISP_SCREEN;
            check_daily_event();
           //clcd_print("hello WORLD     ",LINE1(0));
        /*   if ((ch=check_daily_event())!=-1)  
            {
                 calculate_duration(ch);
            }*/
        }
        else if (view_event[i][17]=='O')  
        {
            CLEAR_DISP_SCREEN;
           // clcd_putch(view_event[i][17],LINE1(0));
           // while(1);
            int ch;
             // CLEAR_DISP_SCREEN;
           // clcd_print("hmmmm WORLD     ",LINE1(0));
            ch=check_onetime_event();
         /*   if (ch!=-1)  
            {
                 calculate_duration(ch);
            }*/
        }
    }
}