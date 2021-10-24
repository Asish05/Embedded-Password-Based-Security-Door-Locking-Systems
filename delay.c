#include"vtype.h"
void delay_ms(u8 t)
{
  u8 i;
  for(t;t>0;t--)
	for(i=0;i<120;i++);
}