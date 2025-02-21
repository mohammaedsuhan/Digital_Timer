# 1 "external_eeprom_1.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 288 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "C:/Program Files/Microchip/MPLABX/v6.00/packs/Microchip/PIC18Fxxxx_DFP/1.3.36/xc8\\pic\\include\\language_support.h" 1 3
# 2 "<built-in>" 2
# 1 "external_eeprom_1.c" 2
# 1 "./i2c.h" 1




void init_i2c(void);
void i2c_start(void);
void i2c_rep_start(void);
void i2c_stop(void);
void i2c_write(unsigned char data);
unsigned char i2c_read(void);
# 1 "external_eeprom_1.c" 2

# 1 "./external_EEPROM.h" 1




void write_external_eeprom(unsigned char address1, unsigned char data);
unsigned char read_external_eeprom(unsigned char address1);
# 2 "external_eeprom_1.c" 2

void write_EEPROM(unsigned char address1, unsigned char data)
{
 i2c_start();
 i2c_write(0xA0);
 i2c_write(address1);
 i2c_write(data);
 i2c_stop();
    for(int i=3000;i--;);
}

unsigned char read_EEPROM(unsigned char address1)
{
 unsigned char data;

 i2c_start();
 i2c_write(0xA0);
 i2c_write(address1);
 i2c_rep_start();
 i2c_write(0xA1);
 data = i2c_read();
 i2c_stop();

 return data;
}
