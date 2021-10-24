#include <reg51.h>
#include "lcd.h"
#include "delay.h"
#define lcd P0
sbit rs=P2^0;
sbit rw=P2^1;
sbit en=P2^2;
void lcd_initial()
{
	delay_ms(15);
	wrt_command(0x03);
	delay_ms(15);
	wrt_command(0x03);
	delay_ms(2);
	wrt_command(0x03);
	delay_ms(2);
	wrt_command(0x02);
	wrt_command(0x28);//set interfece
	wrt_command(0x00);//clear display
	wrt_command(0x06);//cursor move direction
	wrt_command(0x0c);//DCB set
}
void wrt_command(char cm)
{
	rs=0;
	rw=0;
	lcd=cm>>4;
	en=1;
	en=0;
	delay_ms(2);
	rs=0;
	rw=0;
	lcd=cm&0x0f;
	en=1;
	en=0;
	delay_ms(2);
}
void wrt_data(char d)
{
	rs=1;
	rw=0;
	lcd=d>>4;
	en=1;
	en=0;
	delay_ms(2);
	rs=1;
	rw=0;
	lcd=d&0x0f;
	en=1;
	en=0;
	delay_ms(2);
}
void str_data(char *p)
{
	while(*p!='\0')
	wrt_data(*p++);
}