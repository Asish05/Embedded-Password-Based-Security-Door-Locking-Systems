#include "vtype.h"
void delay_s(u16 t)
{
  u16 i;
  for(t;t>0;t--)
	for(i=0;i<12000;i++);
}