#ifndef MAIN_H
#define MAIN_H

#include <xc.h>
#include "clcd.h"
#include "ds1307.h"
#include "i2c.h"
#include "matrix_keypad.h"
#include "external_EEPROM.h"
#include "timer0.h"

void current_time(void);
void current_date(void);
unsigned int get_current_day(void);
signed int check_event_time(void);
unsigned char check_weekly_event(void);
unsigned char check_daily_event(void);
void check_event_trigger(void);
void get_time(void);
void get_date(void); 
void calculate_duration(unsigned int k);
void write_EEPROM(unsigned char address1,  unsigned char data);
unsigned char read_EEPROM(unsigned char address1);
void init_timer0(void);

#endif