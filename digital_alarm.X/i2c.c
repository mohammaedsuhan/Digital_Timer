
#include <xc.h>

void init_i2c(void)
{
	/* Set SCL and SDA pins as inputs */
	TRISC3 = 1;
	TRISC4 = 1;
	/* Set I2C master mode *///page no 200
	SSPCON1 = 0x28;

	SSPADD = 0x31;
	/* Use I2C levels, worked also with '0' */
	CKE = 0;
	/* Disable slew rate control  worked also with '0' */
	SMP = 1;
	/* Clear SSPIF interrupt flag */
	SSPIF = 0;//serial port communication completed or not
	/* Clear bus collision flag */
	BCLIF = 0;//  if addresses are same bus collision will happen to check the collision
}

void i2c_idle(void)     
{
	while (!SSPIF);
	SSPIF = 0;                    //commm is completed or not 
}

void i2c_ack(void)
{
	if (ACKSTAT)
	{
		/* Do debug print here if required */
	}
}

void i2c_start(void)
{
	SEN = 1;          //put start condition bit to 1 initiall (control register 2)
	i2c_idle();         //call idle to check check data line is pull down or not
}

void i2c_stop(void)
{
	PEN = 1;
	i2c_idle();             //communication is done or not ,the bit is pulled down or not
}

void i2c_rep_start(void)
{
	RSEN = 1;                //repeat start enable bit
	i2c_idle();
}

void i2c_write(unsigned char data)
{
	SSPBUF = data;             //buffer register to store data or address ,to send to data line from the slave to master
	i2c_idle();
}
void i2c_rx_mode(void)                 //receive data from slave to master 
{
	RCEN = 1;                         //
	i2c_idle();
}
void i2c_no_ack(void)
{
	ACKDT = 1;               //ack dont care
	ACKEN = 1;                //ack enable  these two bits belong to controll 2 register.
}

unsigned char i2c_read(void)
{
	i2c_rx_mode();
	i2c_no_ack();                 //

	return SSPBUF;
}