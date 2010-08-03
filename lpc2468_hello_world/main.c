/**********************************
 * Small LED flash program
 * Author  : MOJ@hih.au.dk
 * Date    : July 27, 2010
 *********************************/
#include "lpc2468.h"
#include "framework.h"
#include "uart.h"

/* Constant that gives app. 500msec delay, when using IRC as clock source */
#define DELAY_SEC	160000

/* init GPIO for LED, set P2.10 as output */
static void ledInit(){
	PINSEL4  = 0;
	FIO2DIR |= (1<<10);
	FIO2CLR  = (1<<10);
}

/* Turn on LED, writing to FIO2CLR register, LED is active LOW*/
static void ledOn(int led)
{
	FIO2CLR = led;
}

/* Turn on LED, writing to FIO2SET register*/
static void ledOff(int led)
{
	FIO2SET = led;
}

/* very dumb delay */
void delay(int d){     
  for(; d; --d);
}

/* main loop
 * basic CPU initialization is done here.
 */
int main(void)
{
	char c = 'a';

	/* init low level stuff */
	eaInit();

	/*initialize uart #1: 9600 bps, 8N1*/
	initUart1(B9600(Fpclk), UART_8N1);

	/* init led */
	ledInit();

	/* now flash led */
	while (1)
	{
		ledOn(1<<10);
		delay(DELAY_SEC);
		ledOff(1<<10);
		delay(DELAY_SEC);
		sendchar(c++);
	}
	return 0;
}
