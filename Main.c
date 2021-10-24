#include <reg51.h>
#include "keypad.h"
#include "lcd.h"
#include "delay.h"
#include "delays.h"
#include "vtype.h"
#include "i2c.h"
#include "EEPROM.h"
sbit ext=P3^2;
sbit Buzzer=P1^7;
sbit IN1=P2^6;
sbit IN2=P2^7;
#define I2C_EEPROM1_SA 0xA0 //slave address + w
u8 user[][4]={"1144","2255","3366"},password[5],p[5];
u16 user_status[3];
bit flag=0;
main()
{
	u8 cnt=0,t,reset=0;
	IE=0x81;
	IT0=1;
	Row_Init();
	lcd_initial();
	i2cPageWrite(I2C_EEPROM1_SA,0x00,"1144",4);
	i2cPageWrite(I2C_EEPROM1_SA,0x05,"2255",4);
	i2cPageWrite(I2C_EEPROM1_SA,0x0a,"3366",4);
	wrt_command(0x81);
	str_data("EMBEDDED BASED");
	wrt_command(0xC0);
	str_data("DOOR LOCK SYSTEM");
	delay_s(15);
	while(1)
		{
			USER:t=user_check(user);
			if(t=='@')
				{
					flag=0;
					goto USER;
				}
				psw_rd(t);
				if(user_sts_rd(t)==1)
					goto USER;
				IN:wrt_command(0x01);
				wrt_command(0x81);
				str_data("ENTER PASSWORD");
				if(password_check()==1)
					{
						wrt_command(0x01);
						wrt_command(0x81);
						str_data("PASSWORD OK");
						delay_s(5);
						cnt=0;
						IN1=1;
						IN2=0;
						wrt_command(0x01);
						wrt_command(0x81);
						str_data("DOOR OPENED");
						delay_s(20);
						IN1=0;
						IN2=1;
						wrt_command(0x01);
						wrt_command(0x81);
						str_data("DOOR CLOSED");
						delay_s(20);
						wrt_command(0x01);
						IN1=IN2=0;
					}
					else
						{
							cnt++;
							if(cnt<3)
								{
									wrt_command(0x01);
									wrt_command(0x81);
									str_data("WRONG PASSWORD");
									Buzzer=0;
									delay_s(10);
									Buzzer=1;
									goto IN;
							  }
					else
						{
							reset++;
							if(reset<3)
								{
									wrt_command(0x01);
									wrt_command(0x81);
									str_data("ID BLOCK");
									cnt=0;
									Buzzer=0;
									delay_s(15);
									Buzzer=1;
									user_status[t]=1;
								}
						else
							{
								wrt_command(0x01);
								wrt_command(0x81);
								str_data("SYSTEM BLOCK");
								Buzzer=0;
								while(1);
							}
						}
						}
		} 
}
void ext_int(void)interrupt 0
{
	u8 t,temp[4],i,cnt=0;
	wrt_command(0x01);
	str_data("PASSWORD CHANGE");
	delay_s(10);
	USER:t=user_check(user);
	if(user_sts_rd(t)==1)
	goto USER;
	psw_rd(t);
	IN:wrt_command(0x01);
  str_data("ENTER PRV PWD");
	if(password_check()==0)
		{
			wrt_command(0x01);
			str_data("WRONG PASSWORD");
			delay_s(5);
			cnt++;
			if(cnt<3)
				goto IN;
		}
		if(cnt==3)
			{
				flag=1;
				return;
			}
			wrt_command(0x01);
			str_data("ENTER NEW PWD");
			for(i=0;i<4;i++)
			{
				temp[i]=key_val();
				while(!col_stat());//waiting for switch release
				wrt_command(0xc6+i);
				wrt_data('*');
			}
			i2cPageWrite(I2C_EEPROM1_SA,t*5,temp,4);
			wrt_command(0x01); 
			wrt_command(0x81);
			str_data("SUCCESSFULLY");
			wrt_command(0xC0);
			str_data ("CHANGED");
			delay_s(15);
			flag=1;
			return;
}