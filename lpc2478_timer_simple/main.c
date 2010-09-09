/**********************************
 * Small LED flash program
 * Author  : MOJ@hih.au.dk
 * Date    : July 27, 2010
 *********************************/
#include "lpc2468.h"
#include "framework.h"
#include "uart.h"
#include "timer_delay.h"

/* init GPIO for LED, set P2.10 as output */
static void ledInit()
{
	PINSEL4 = 0;
	FIO2DIR |= (1 << 10);
	FIO2CLR = (1 << 10);
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

/* main loop
 * basic CPU initialization is done here.
 */
int main(void)
{
	char c = 0;
	char str[40] =
	{ "Timer test, toggling led every 123 ms \n" };

	/* init low level stuff */
	lowLevelInit();

	/*initialize uart #1: 9600 bps, 8N1*/
	initUart0(B9600(Fpclk), UART_8N1);

	/* init led */
	ledInit();

	/* print msg */
	for (c = 0; c < 40; c++)
	{
		sendchar(str[c]);
	}

	/* now flash led */
	while (1)
	{
		ledOn(1 << 10);
		delayMs(0, 123);
		ledOff(1 << 10);
		delayMs(0, 123);

	}
	return 0;
}
