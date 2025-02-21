
#include "main.h"
#include <string.h>
#include<stdlib.h>
unsigned char clock_reg[3];
unsigned char calender_reg[4];
unsigned char time[9];
unsigned char date[16];
unsigned char flag = 0;
unsigned char oper = 1;
unsigned char oper1 = 1;
unsigned char key;
unsigned char f = 0;
unsigned int hour = 1, minute = 0, sec = 0, AM_PM = 0;
unsigned int dur_hour = 0, dur_minute = 5, dur_sec = 0, w = 0;
unsigned int year = 0, month = 0, DATE = 0, day = 0;
unsigned char change_field = 1;
unsigned char event_field = 1;
unsigned char once = 1;
unsigned short wait = 0;
unsigned char HOURS[2], MINUTES[2], SECONDS[2];
unsigned char STORE_TIME[10];
unsigned char duration[16];
unsigned int event_count = 0;
unsigned char count = 0;
unsigned char view_event[10][18];
unsigned char INC = 0x01;
unsigned char bytes = 0x01;
unsigned char timer_flag=0;
unsigned char AM_PM2=0;//0 means AM 1 means PM
unsigned char duration_var=0;
unsigned int T=2;  //this flag for for every one second checking for the event
unsigned int F=0;  //this flag for enabling the count_duration
unsigned char temp_AM[10][18];
unsigned char temp_PM[10][18];
extern int seconds_flag;
unsigned int p=0;
int stop=0;
int m=0;
int n=0;
int match=0;

void strcopy(unsigned char *ptr,unsigned char *qtr);
/*
void init_config(void)
{
   
   PEIE=1; 
   
      PORTB = 0x00;
      
	TRISB = 0x00;

	
	ADCON1 = 0x0F;
    init_timer1();
    init_timer1();
   
    GIE=1;
	init_clcd();
	init_i2c();
	init_ds1307();
    init_matrix_keypad();
 
   
  
}*/
void init_config(void) {
    // Enable peripheral and global interrupts
    PEIE = 1;
    GIE = 1;

    // Configure PORTB as digital output
    PORTB = 0x00;
    TRISB = 0x00;
    ADCON1 = 0x0F; // All pins digital

    // Initialize peripherals
    init_timer0();
  //  init_timer1();
    init_i2c();
    init_ds1307();
    init_matrix_keypad();
    init_clcd();
    TRISE0 = 0;
    RE0 = 0;
}



void write(void) 
{
    if (event_count < 10)
    {
      
        for (int i = 0; i < 8; i++)
        {
            write_EEPROM(INC++, STORE_TIME[i]);
        }
        for (int i = 0; i < 10; i++)
        {
            write_EEPROM(INC++, duration[i]);
        }
        event_count++;
        write_EEPROM(0x00, event_count+48);
        for (int j = 0; j < 18; j++)
        {
            view_event[event_count][j] = read_EEPROM(bytes);
            write_EEPROM(bytes, view_event[event_count][j]);
            bytes++;
        }
         if(duration[9]=='W')
        {
            write_EEPROM(0x0A,(day%10)+'0');
        }
    } 
    else
    {
       // clcd_print("ALL LOGS COMPLET", LINE1(0));
    }
}


void display_date(void)
{
   clcd_print("DATE ",LINE1(0));
   clcd_print(date, LINE1(5));
   clcd_putch(' ',LINE1(15));
  
}

void display_time(void)
{
    clcd_print("TIME ",LINE2(0));
    clcd_print(time, LINE2(5));
    clcd_putch(' ',LINE2(13));
    if (clock_reg[0] & 0x40) 
    {
        if (clock_reg[0] & 0x20)
        {
          clcd_print("PM", LINE2(14));
           AM_PM2='P';
        } 
        else 
        {
           clcd_print("AM", LINE2(14));
           AM_PM2='A';
        }
    }
}

void view_ALARM(void)
{
   
    if(f==2)
    {
        if(key==MK_SW11)
        {
            if(count>0)
            {
                count--;
            }
        }
        else if(key==MK_SW12)
        {
            if(count<event_count-1)
            {
                count++;
            }
        }
        clcd_putch('#',LINE1(0));
        clcd_print(" T- ",LINE1(1));
        int line_variable=5;
        int k=0;
        for(k=0;k<8;k++)
        {
           clcd_putch(view_event[count][k],LINE1(line_variable));
            line_variable++;
        }
       clcd_print("  *",LINE1(line_variable));
        line_variable=0;
       clcd_putch(count+'0',LINE2(line_variable++));
        clcd_print(" D- ",LINE2(line_variable));
        line_variable=5;
        for( ;k<18;k++)
        {
           clcd_putch(view_event[count][k],LINE2(line_variable));
            line_variable++;
        }
        if(key==MK_SW2)
        {
        f=0;
        flag=0;
        stop=0;
        GIE=1;
        PEIE=1;
        }
        
      
    }
}


void set_EVENT(void)
{
    if(f==2)
    {   
        if(once)
        {
        hour=(time[0]-'0')*10+(time[1]-'0');
        minute=(time[3]-'0')*10+(time[4]-'0');
        sec=(time[6]-'0')*10+(time[7]-'0');
        AM_PM=(time[0] & (1 << 5)) ? 1 : 0;
        once=0;
        }
        
 
        if(event_field==1)
        {
         
          if(key==MK_SW11)
          {
          if(hour++>=11)
          {
            hour=1;
          }
          }
         }
      else  if(event_field==2)
      {    
          
        if(key==MK_SW11)
        {
        if(minute++==59)
        {
            minute=0;
        }
        }
        
       }
     
     else if(change_field==3)
     { 
        
         if(key==MK_SW11)
          AM_PM=!AM_PM;
     }
     else if(event_field==4)
     { 
         if(key==MK_SW11)
         {
             if(dur_hour++==12)
             {
                 dur_hour=1;
             }
         }     
     }
     else if(event_field==5)
     {
        
         if(key==MK_SW11)
         {
             if(dur_minute++==30)
             {
                 dur_minute=1;
             }
         }
     }else if(event_field==6)
     {
         
         if(key==MK_SW11)
         {
             if(dur_sec++==59)
             {
                 dur_sec=0;
             }
         }
     }
     else if(event_field==7)
     {
         if(key==MK_SW11)
         {
             if(w++==2)
             {
                 w=0;
             }
         }
     }
        if(w==0)
        {
            duration[9]='W';
        }else if(w==1)
        {
            duration[9]='D';
            
        }else if(w==2)
        {
            duration[9]='O';
        }
        
        
    STORE_TIME[0]=hour/10+'0';
    STORE_TIME[1]=hour%10+'0';
    STORE_TIME[2]=':';
    STORE_TIME[3]=minute/10+'0';
    STORE_TIME[4]=minute%10+'0';
    STORE_TIME[5]=' ';
    if(AM_PM)
    {
        STORE_TIME[6]='A';
        STORE_TIME[7]='M';
    }
    else
    {
        STORE_TIME[6]='P';
        STORE_TIME[7]='M';
    }
    STORE_TIME[8]='\0';
     duration[0]=dur_hour/10+'0';
     duration[1]=dur_hour%10+'0';
     duration[2]=':';
     duration[3]=dur_minute/10+'0';
     duration[4]=dur_minute%10+'0';
     duration[5]=':';
     duration[6]=dur_sec/10+'0';
     duration[7]=dur_sec%10+'0';
     duration[8]=' ';
     duration[10]='\0';
    if(key==MK_SW2)
    {
        if(event_count==10)
        {
            event_count=0;
        }
        write();
        
        //clcd_print("SAVE AND EXIT TI",LINE1(0));
        //clcd_print("                ",LINE2(0));
        f=0;
        flag=0;
        stop=0;
        GIE=1;
        PEIE=1;
    }
   clcd_print("TIME",LINE1(0));
   clcd_print(STORE_TIME,LINE1(4));
   clcd_print("   ",LINE1(12));
    clcd_putch('*',LINE1(15));
   clcd_print("DUR - ",LINE2(0));
   clcd_print(duration,LINE2(6));
   
    }
}

void set_time(void)
{
    event_count=read_EEPROM(0x00)+'0';   
    if(f==2)
    {
        if(once)
        {
        hour=(time[0]-'0')*10+(time[1]-'0');
        minute=(time[3]-'0')*10+(time[4]-'0');
        sec=(time[6]-'0')*10+(time[7]-'0');
        AM_PM=(time[0] & (1 << 5)) ?1:0;
        
        once=0;
        }
        clcd_print(" SET TIME       ",LINE1(0));
        if(change_field==1)
        {
          if(key==MK_SW11)
          {
          if(hour++>=11)
          {
            hour=0;
          }
          }
         }
      else  if(change_field==2)
      {    
        if(key==MK_SW11)
        {
        if(minute++==59)
        {
            minute=0;
        }
        }
       }
      else if(change_field==3)
     {
         if(key==MK_SW11)
        if(sec++==59)
        {
            sec=0;
        }
     }
      else if(change_field==4)
     {
         if(key==MK_SW11)
          AM_PM=!AM_PM;
     }
    time[0]=hour/10+'0';
    time[1]=hour%10+'0';
    time[3]=minute/10+'0';
    time[4]=minute%10+'0';
    time[6]=sec/10+'0';
    time[7]=sec%10+'0';
    if(key==MK_SW2)
    {
        if(AM_PM)  //if AM_PM is 1 then 5 th bit is 0
        {
        write_ds1307(HOUR_ADDR,((0x00<<5)|((0x01)<<6)|(hour/10)<<4)|(hour%10));
        }
        else      //if AM_PM is 0 then 5th bit is 1
        {
           write_ds1307(HOUR_ADDR,((0x01<<5)|((0x01)<<6)|(hour/10)<<4)|(hour%10));
        }
        write_ds1307(MIN_ADDR,((minute/10)<<4)|(minute%10));
        write_ds1307(SEC_ADDR,((sec/10)<<4)|(sec%10));
       
        // clcd_print("SAVE AND EXIT TI",LINE1(0));
         //clcd_print("                ",LINE2(0));
         f=0;
        flag=0;
        stop=0;
        GIE=1;
        PEIE=1;
    }
    
    clcd_print(time,LINE2(0));
    clcd_print("      ",LINE2(8));
    if(AM_PM)
    {
       clcd_print("AM",LINE2(14));
    }
    else 
    {
        clcd_print("PM",LINE2(14));
      
    } 
    }
}
void set_date(void)
{
    if(f==2)
    {
        if(once)
        {
            year=(date[8]-'0')*10+(date[9]-'0');
            month=(date[3]-'0')*10+(date[4]-'0');
            DATE=(date[0]-'0')*10+(date[1]-'0');
            day=(date[10]-'0')*10+(date[11]-'0');
            once=0;
        }
         clcd_print(" SET DATE       ",LINE1(0));
         if(change_field==1)
         {
             if(key==MK_SW11)
             {
                 if(year++==99)
                 {
                     year=0;
                 }
             }
         }
         else if(change_field==2)
         {
             if(key==MK_SW11)
             {
                 if(month++==11)
                 {
                     month=0;
                 }
             }
         }
         else if(change_field==3)
         {
             if(key==MK_SW11)
             {
                 if(DATE++==30)
                 {
                     DATE=0;
                 }
             }
         }
         else if(change_field==4)
         {
             if(key==MK_SW11)
             {
                 if(day++==7)
                 {
                     day=0;
                 }
             }
         }

      date[0] = '0' + DATE/10;
	  date[1] = '0' + DATE%10;
	  date[2] = '-';
	  date[3] = '0' + month/10;
	  date[4] = '0' + month%10;
	  date[5] = '-';
      date[6] = '2';
	  date[7] = '0';
	  date[8] = '0' + year/10;
	  date[9] = '0' + year%10;
      date[10]='0'+ day/10;
      date[11]='0'+ day%10;
      date[12]='\0';
      if(key==MK_SW2)
      {
           write_ds1307(YEAR_ADDR,((year/10)<<4)|(year%10));
           write_ds1307(MONTH_ADDR,((month/10)<<4)|(month%10));
           write_ds1307(DATE_ADDR,((DATE/10)<<4)|(DATE%10));
           write_ds1307(DAY_ADDR,((day/10)<<4)|(day%10));
           f=0;
           flag=0;
           stop=0;
           GIE=1;
           PEIE=1;
      }
     clcd_print(date,LINE2(0));
     clcd_print("    ",LINE2(12));
      
         
             
    }
    
}


int  GOTO_SET_TIMEDATA(void)
{
   
    if(f==1)
    {
          if(oper1==1)
    {
        clcd_print("->",LINE1(0));
        clcd_print("  ",LINE2(0));
    }
    else if(oper1==2)
    {
        clcd_print("  ",LINE1(0));
        clcd_print("->",LINE2(0));
    }
     clcd_print("SET TIME      ",LINE1(2));
     clcd_print("SET DATE      ",LINE2(2));
   
    if(key==MK_SW11)
    {
        if(f==1)
        {
        if(oper1>1)
        {
            oper1--;
        }
        }
       
    }
    else if(key==MK_SW12)
     {
         if(f==1)
         {
         if(oper1<2)
         {
             oper1++;
         }
         }   
     }
    }
    
    
    if(oper1==1)
    {
        if(f==2)
        {
            set_time();
        }    
        
    }
    else if(oper1==2)
     {
         if(f==2)
         {
             set_date();
         }
         
     }
        
    
}
void GOTO_SET_EVENT(void)
{
    
    if(f==1)
    {
        CLEAR_DISP_SCREEN;
     clcd_print("VIEW EVENT    ",LINE1(2));
     clcd_print("SET EVENT     ",LINE2(2));
    
    if(oper1==1)
    {
        clcd_print("->",LINE1(0));
        clcd_print("  ",LINE2(0));
    }
    else if(oper1==2)
    {
        clcd_print("->",LINE2(0));
        clcd_print("  ",LINE1(0));
    }
    if(key==MK_SW11)
    {
        if(f==1)
        {
        if(oper1>1)
        {
            oper1--;
        }
        }
        
    }
    else if(key==MK_SW12)
     {
         if(f==1)
         {
         if(oper1<2)
         {
             oper1++;
         } 
         }
       
       
     }
    }
    if(oper1==1)
    {
        if(f==2)
        { 
             view_ALARM();   
        }    
        
    }
     if(oper1==2)
     {
         if(f==2)
         {
             set_EVENT();
         }
         
     }   
}

void get_time(void)
{
	clock_reg[0] = read_ds1307(HOUR_ADDR);
	clock_reg[1] = read_ds1307(MIN_ADDR);
	clock_reg[2] = read_ds1307(SEC_ADDR);

	if (clock_reg[0] & 0x40)
	{
		time[0] = '0' + ((clock_reg[0] >> 4) & 0x01);
		time[1] = '0' + (clock_reg[0] & 0x0F);
	}
	else
	{
		time[0] = '0' + ((clock_reg[0] >> 4) & 0x03);
		time[1] = '0' + (clock_reg[0] & 0x0F);
	}
	time[2] = ':';
	time[3] = '0' + ((clock_reg[1] >> 4) & 0x0F);
	time[4] = '0' + (clock_reg[1] & 0x0F);
	time[5] = ':';
	time[6] = '0' + ((clock_reg[2] >> 4) & 0x0F);
	time[7] = '0' + (clock_reg[2] & 0x0F);
	time[8] = '\0';
}

 void get_date(void)
{
	calender_reg[0] = read_ds1307(YEAR_ADDR);
	calender_reg[1] = read_ds1307(MONTH_ADDR);
	calender_reg[2] = read_ds1307(DATE_ADDR);
	calender_reg[3] = read_ds1307(DAY_ADDR);

   
	date[0] = '0' + ((calender_reg[2] >> 4) & 0x0F);
	date[1] = '0' + (calender_reg[2] & 0x0F);
	date[2] = '-';
	date[3] = '0' + ((calender_reg[1] >> 4) & 0x0F);
	date[4] = '0' + (calender_reg[1] & 0x0F);
	date[5] = '-';
    date[6] = '2';
	date[7] = '0';
	date[8] = '0' + ((calender_reg[0] >> 4) & 0x0F);
	date[9] = '0' + (calender_reg[0] & 0x0F);
    date[10]='\0';
   // date[10]='0'+((calender_reg[3]>>4)&0x0F);
   // date[11]='0'+(calender_reg[3]&0x0F);

    date[12]='\0';
 }

void compare(int a,unsigned char temp[][18])
{
    

 for( int i=0;i<a-1;i++)
    {
        for( int j=0;j<a-1-i;j++)
        {
            if((strcmp(temp[j],temp[j+1]))>0)
            {
             unsigned char temp2[18];
             strcopy(temp2,temp[j]);
             strcopy(temp[j],temp[j+1]);
             strcopy(temp[j+1],temp2);
            }
        }  
    }
}
void strcopy(unsigned char *ptr,unsigned char *qtr)
{
    for(int i=0;i<18;i++)
    {
        ptr[i]=qtr[i];
    }
}
void main(void)
{
	init_config();
    TRISB0=0;
    RB0=0;  
   
    //unsigned int u;
   //write_EEPROM(0x00,48);
    
  event_count=read_EEPROM(0x00)-'0';
    
    for(unsigned int i=0;i<event_count;i++)
    {
        for(int j=0;j<18;j++)
        {
            view_event[i][j]=read_EEPROM(bytes);
            bytes=bytes+0x01;  
        }
    } 
   
   
    for(unsigned int i=0;i<event_count;i++)
    {
        if(view_event[i][6]=='A')
        {
            strcopy(temp_AM[m],view_event[i]);
            m++;
            
        }
        else if(view_event[i][6]=='P')
        {
            strcopy(temp_PM[n],view_event[i]);
            
            
            
             n++;
                   
        }
    }
   
    compare(m,temp_AM); 
    compare(n,temp_PM); 
     int x;
    for( x=0;x<m;x++)
    {
        strcopy(view_event[x],temp_AM[x]);
    }
     int l=0;
    for( ;x<m+n;x++)
    {
        strcopy(view_event[x],temp_PM[l]);
        l++;
    }
  
   
    
  
    INC=bytes;
    unsigned int count2=0;
   // event_count=u;
	while (1)
	{
            key=read_switches(STATE_CHANGE);
            if(key==ALL_RELEASED)
            {
            T=(10*(time[6]-'0'))+(time[7]-'0');  
            if(T==0)
            { 
            f=2;
            flag=2;
            stop=1;
             check_event_trigger();
             }
            }
          
        if(seconds_flag==1)
        {
            CLEAR_DISP_SCREEN;
            flag=2;
            f=2;
              int j=0;
               int i=0;
               for(i=0;i<8;i++)
               {
                   clcd_putch(view_event[p][i],LINE1(j));
                   j++;
               }
               j=0;
               for(;i<18;i++)
               {
                   clcd_putch(view_event[p][i],LINE2(j));
                   j++;
               }
               
        }
        
            
        
      
      
        if(key==MK_SW12)
        {
            event_field++;
            if(event_field==8)
            {
              event_field=1;
           }
       }
       if(key==MK_SW12)
        {
         if(f==2)        //for changing the field
        {
           change_field++;
           if(change_field==5)
            {
              change_field=1;
            }  
          }
         }
       if(flag==0)
        {
        if(key==MK_SW11||key==MK_SW12)
        {
            flag=1; 
            stop=1;
            GIE=0;
            PEIE=0;
        }
       }
        if(flag==0)
        {
           get_date();
           display_date();
		    get_time();
		    display_time();
       }
       else if(flag==1)
       {
          if(key==MK_SW1)
            {
                if(f==2)
                {
                    f=0;
                    flag=0;
                    once=1;
                }
                else if(f==1)
                {
                 f=2;
                }
                else if(f==0)
                {
                f=1;
                }
           }
            if(f==0)
            {
               CLEAR_DISP_SCREEN;
            if(oper==1)
           {
              clcd_print("->",LINE1(0));
               clcd_print("  ",LINE2(0));
            }
            else if(oper==2)
           {
              clcd_print("->",LINE2(0));  
                clcd_print("  ",LINE1(0));
            }
            
          clcd_print("SET_TIME/DATE ",LINE1(2));
            clcd_print("SET/VIEW EVENT",LINE2(2));
           
           if(key==MK_SW11)
           {
                if(f==0)
               if(oper>1)
                oper--;
           }
            if(key==MK_SW12)
           {
                if(f==0)
               if(oper<2)
                {
                    oper++;
                }
           }
            }
            if(oper==1)
           {
                if(f==1||f==2)
                {
                  GOTO_SET_TIMEDATA(); 
                }
            }
            else if(oper==2)
            {
                if(f==1||f==2)
                {
                    GOTO_SET_EVENT();
               }
            }   
       }
       
        //delay(); 
       
	}
 }
