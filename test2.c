#include "stc8.h"
#include "intrins.h"

#define FOSC	11059200UL
#define BRT		(256 - FOSC / 115200 / 32)

bit busy;
char wptr;
char rptr;
char buffer[16];
void UartIsr() interrupt 4 using 1
{
 if(TI)
 {
  TI = 0;
  busy = 0;
 }
 if(RI)
 {
  RI = 0;
  buffer[wptr++] = SBUF;
  wptr &= 0x0f;
 }
}

void UartInit()
{
 SCON = 0x50;
 TMOD = 0x20;
 TL1 = BRT;
 TH1 = BRT;
 TR1 = 1;
 AUXR = 0x40;
 wptr = 0x00;
 rptr = 0x00;
 busy = 0;
}

void UartSend(char dat)
{
 while(busy);
 busy = 1;
 SBUF = dat;
}

void UartSendStr(char *p)
{
 while(*p)
 {
  UartSend(*p++);
 }
}

void main()
{
 UartInit();
 ES = 1;
 EA = 1;
	while(1)
 UartSendStr("Uart Test !\r\n");
}