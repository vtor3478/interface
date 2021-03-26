#include "driver/soft_iic_if.h"

unsigned short iic_error;

// 局部函数指针，实现函数的服用，尤其是在iic这个接口上面
void(*iic_scl_write)(char);
void(*iic_sda_write)(char);
char(*iic_sda_read)(void);

void iic_remap(void(*device_scl_write)(char),
                void(*device_sda_write)(char),
                char(*device_sda_read)())
{
    iic_scl_write = device_scl_write;
    iic_sda_write = device_sda_write;
    iic_sda_read = device_sda_read;
}

void start()
{
	iic_sda_write(1);
	Delay5us();
    iic_scl_write(1);
	Delay8us();
	iic_sda_write(0);
	Delay8us();
	iic_scl_write(0);
}

void stop()
{
	iic_scl_write(0);
	Delay5us();
	iic_sda_write(0);
	Delay8us();
	iic_scl_write(1);
	Delay8us();
	iic_sda_write(1);
	Delay8us();
}

unsigned char wait_ack()
{
	unsigned char ucErrTime=0;
	iic_sda_write(1);
	Delay5us();   
	iic_scl_write(1);
	Delay5us();
	while(iic_sda_read())
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
//			stop();
			iic_scl_write(0);//时钟输出0 
			Delay8us();
            iic_error = ucErrTime;
			return 1;
		}
	}
    iic_error = ucErrTime;
	iic_scl_write(0);//时钟输出0 
	Delay8us();
	return 0;  
} 

//产生ACK应答
void ack(void)
{
	iic_scl_write(0);
	iic_sda_write(0);
	Delay8us();
	iic_scl_write(1);
	Delay8us();
	iic_scl_write(0);
}

//不产生ACK应答	
void nak()
{
	iic_scl_write(0);
	iic_sda_write(1);
	Delay8us();
	iic_scl_write(1);
	Delay8us();
	iic_scl_write(0);
}


void write_byte(unsigned char dat)
{
	unsigned char i;
	iic_scl_write(0);
	for(i=0;i<8;i++)
    {
        if((dat&0x80)>>7)
            iic_sda_write(1);
        else
            iic_sda_write(0);
        dat<<=1;
        Delay2us();
        iic_scl_write(1);
        Delay8us();
        iic_scl_write(0);
        Delay2us();
    }
}

unsigned char read_byte()
{
	unsigned char i,dat=0;
    iic_sda_write(1);
	for(i=0;i<8;i++)
	{
		iic_scl_write(0);
		Delay8us();
		iic_scl_write(1);
		dat<<=1;
		Delay8us();
        dat |= iic_sda_read();
//		Delay8us();	
	}
    return dat;
}

