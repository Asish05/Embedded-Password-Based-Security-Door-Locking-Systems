#include "vtype.h"
#include "delays.h"
#include "lcd.h"
#include "keypad.h"
#include "i2c.h"
#define I2C_EEPROM1_SA 0xA0
extern u8 password[5],p[5],user[][4];
extern u16 user_status[3];
extern bit flag;
u8 user_check(u8 (*p)[4])
{
	u8 t[4],i,j,f=0;
	while(1)
		{
			Row_Init();
			wrt_command(0x01);
			wrt_command(0x81);
	    str_data("ENTER USER ID");
			for(i=0;i<4;i++)
			{
				wrt_command(0xc6+i);
				wrt_data(t[i]=key_val());
					if(t[i]=='@')
					{
					return '@';
					}
				while(!col_stat());
				}
				delay_s(2);
	      for(i=0;i<3;i++)
				{
					for(j=0;j<4;j++)
					{
					if(p[i][j]!=t[j])
						 break;
				  }
					if(j==4)
					{
					f=1;
					break;
					}
				}
				if(f==0)
				{
				wrt_command(0x01);
     //   wrt_command(0x85);
	      str_data("INVALID ID");
				delay_s(10);
				}
				else
					break;
		  }
			return i;
}
bit password_check()
{
	u8 i;
  for(i=0;i<4;i++)
	 {
		password[i]=key_val();
		while(!col_stat());//waiting for switch release
		wrt_command(0xc6+i);
		wrt_data('*');
		}
		password[i]='\0';
	  for(i=0;i<4;i++)
	   {
	    if(p[i]!=password[i])
		  return 0;
	   }
		  return 1;
}
void psw_rd(u8 t)
{
		 //if(t==0)
	i2cSeqRead(I2C_EEPROM1_SA,t*5,p,4); 
		
}
bit user_sts_rd(u8 t)
{
	u8 i;
	for(i=0;i<3;i++)
	{
		if(user_status[i]!=0)
			{
				if(t==i)
				{
				wrt_command(0x01);
	//		wrt_command(0x85);
				str_data("ID BLOCK");
				delay_s(10);
				return 1;
				}
			}
		}
	return 0;
}