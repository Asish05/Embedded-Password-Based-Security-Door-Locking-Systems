#include<reg51.h>
sbit r0=P1^0;
sbit r1=P1^1;
sbit r2=P1^2;
sbit r3=P1^3;
sbit c0=P1^4;
sbit c1=P1^5;
sbit c2=P1^6;
extern bit flag;
char Row_Val,Col_Val;


char code a[4][3]={{'1','2','3'},
									{'4','5','6'},
									{'7','8','9'},
									{'*','0','#'}};
void Row_Init(void)
{
	r0=r1=r2=r3=0;
}
bit col_stat(void)
{
   return(c0&c1&c2);
}
char key_val()
{		 
	Row_Init();
	while(col_stat())//waiting for coloum status
	{
	if(flag==1)
		return '@';
	}
	r0=0;
	r1=r2=r3=1;
	if(!col_stat())
	{
		Row_Val=0;
		goto col_Check;
	}
	r1=0;
	r0=r2=r3=1;
	if(!col_stat())
	{
		Row_Val=1;
		goto col_Check;
	}
	r2=0;
	r0=r1=r3=1;
	if(!col_stat())
	{
		Row_Val=2;
		goto col_Check;
	}
	r3=0;
	r0=r1=r2=1;
	if(!col_stat())
		Row_Val=3;
	
	
col_Check:
	if(c0==0)
		Col_Val=0;
	else if(c1==0)
		Col_Val=1;
	else 
		Col_Val=2;
		return(a[Row_Val][Col_Val]);
}



