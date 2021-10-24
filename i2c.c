#include "vtype.h"
#include "i2c.h"
#include "delay.h"

#ifndef __I2C_DEFINES_H__
#define __I2C_DEFINES_H__

#include <reg51.h>

sbit scl=P3^6;
sbit sda=P3^7;



void i2cStart(void)
{
   //sda=1; default level is high
   //scl=1; & default level is high,i.e bus is in idle state
   sda=0;//high to low
}

void i2cReStart(void)
{
   scl=0; //to change the status of sda during communication
   sda=1; //default level is high
   scl=1; // & thelevel on sda is now validated/confirmed
   sda=0;//high to low
}

void i2cStop(void)
{
   scl=0;//to change sda to low
   sda=0;//master makes sda low
   scl=1;//master release scl
   sda=1;//low to high on sda while scl is high
}

void i2cWrite(u8 dat)
{
  u8 i;
  for(i=0;i<8;i++)
  {
    scl=0;//to serially place data bit across sda line
    sda=(dat>>(7-i))&1; //from  msb to lsb order
    scl=1;//to confirm/validate bit status for slave to read
  }
}

u8 i2cRead(void)
{
  u8 i,buff=0;
  for(i=0;i<8;i++)
  {
    scl=0;//to serially let slave place data bit across sda line
    scl=1;//to confirm/validate bit status by master
    if(sda) //check/read from of sda from  msb to lsb order 
      buff|=0x80>>i; //update '1' only because already all bits in buff are '0s'
  }
  return buff;
}

void i2cAck(void)
{
  u8 i;
   scl=0; //master make scl low to allow change across sda
   sda=1; //master is release sda from its point of view/slave should pull down sda
   scl=1; //master releases scl to read the status of acknowledge across sda
   for(i=0;i<100;i++)
   {
     if(!sda)
       break;
   }
}

void i2cNAck(void)
{
   scl=0; //master make scl low to allow change across sda
   sda=1; //master is release sda from its point of view to indicate to slave no further reads
   scl=1; //master releases scl to for slave to validate
}

void i2cMasterAck(void)
{
   scl=0;//master make scl low so that slave can release sda high
   sda=0;//master make sda low to ack to slave
   scl=1;//master releases scl so that slave can validate/read sda status for ack
   scl=0;//master make scl low to release sda
   sda=1;//master now release sda
}
void i2cPageWrite(u8 slaveAddr8bit,
                  u8 wBuffStartAddr,
                  u8 *ptr8BytesMax,
                  u8 nBytes)
{
  u8 i;
  i2cStart();
  i2cWrite(slaveAddr8bit&0xfe);
  i2cAck();
  i2cWrite(wBuffStartAddr);
  i2cAck();
  for(i=0;i<nBytes;i++)
  {
    i2cWrite(ptr8BytesMax[i]);
    i2cAck();
  }
  i2cStop();
  delay_ms(10);
}

void i2cSeqRead(u8 slaveAddr8bit,
                u8 rBuffStartAddr,
                u8 *ptr8BytesMax,
                u8 nBytes)

{

  u8 i;

  i2cStart();
  i2cWrite(slaveAddr8bit&0xfe);
  i2cAck();
  i2cWrite(rBuffStartAddr);
  i2cAck();
  i2cReStart();
  i2cWrite(slaveAddr8bit|0x01);
  i2cAck();

  for(i=0;i<nBytes-1;i++)
  {
    ptr8BytesMax[i]=i2cRead();
    i2cMasterAck();
  }

  ptr8BytesMax[i++]=i2cRead();
  i2cNAck();
  ptr8BytesMax[i]='\0';
  i2cStop();
}