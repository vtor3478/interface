#ifndef _IIC_IF_H_
#define _IIC_IF_H_
#include "driver/delay.h"

extern unsigned short iic_error;

void iic_remap(void(*device_scl_write)(char),
                void(*device_sda_write)(char),
                char(*device_sda_read)());

void start();
void write_byte(unsigned char dat);
unsigned char wait_ack();
unsigned char read_byte();
void ack(void);
void nak();
void stop();

#endif