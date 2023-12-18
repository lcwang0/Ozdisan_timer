#include "MS51_16K.h"

uint16_t tick16timermS;

void set_Timer_0(unsigned int);
uint16_t millis(void);
uint16_t temp = 0;
uint16_t ctr = 0;
	uint16_t databuff;
void set_Timer_0 (unsigned int value)
{
  TL0 = (value & 0x00FF); 
  TH0 = ((value & 0xFF00) >> 8);
}

uint16_t millis(void)
{
	return tick16timermS;
}

void test()
{

//	temp = millis();
	temp = tick16timermS;
	databuff = temp - ctr;
	if( databuff >= 1000 )
	{
		P12^=1;
		printf("tick16timermS : %u \t Temp : %u \t Ctr : %u \t Diff : %u \t ",tick16timermS,temp,ctr,(tick16timermS-ctr));
		ctr = temp;
		printf("New Ctr : %u\n",ctr);
		P12^=1;
	}
	if (temp > 64000)
	{
		tick16timermS = 0;
	}
}

void Timer0_ISR (void) interrupt 1	//1ms
{

	_push_(SFRS);
	TR0=0;
	set_Timer_0(63536);
	TF0 = 0 ;
	TR0= 1;
	
	tick16timermS++;

	_pop_(SFRS);
}

void main (void) 
{
	MODIFY_HIRC(HIRC_24);
	P12_PUSHPULL_MODE;
	
	P06_PUSHPULL_MODE;
	UART_Open(24000000,UART0_Timer3,115200);
	ENABLE_UART0_PRINTF;

	ENABLE_TIMER0_MODE1;
	TIMER0_FSYS_DIV12;
	set_Timer_0(63536);
	ENABLE_TIMER0_INTERRUPT;
	set_TCON_TR0;
	
	ENABLE_GLOBAL_INTERRUPT;
	
	printf("Project Started\r\n");
	
	while(1)
	{
		test();
	}
}