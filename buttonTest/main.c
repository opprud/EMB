/**********************************
 * Small LED flash program
 * Author  : MOJ@hih.au.dk
 * Date    : July 27, 2010
 *********************************/
#include <stdio.h>
#include "lpc2468.h"
#include "framework.h"
#include "uart.h"
#include "keys.h"

/* Constant that gives app. 500msec delay, when using IRC as clock source */
#define DELAY_SEC	160000

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

/* very dumb delay */
void delay(int d)
{
	for (; d; --d)
		;
}

/* main loop
 * basic CPU initialization is done here.
 */
int main(void)
{
	char c = 0;
	char str[40] = { "Hello EMB world, from EA2478 \n" };
	int keypress;

	/* init low level stuff */
	lowLevelInit();

	/*initialize uart #1: 9600 bps, 8N1*/
	initUart0(B9600(Fpclk), UART_8N1);

	/* init led */
	ledInit();

	/* write string on UART */
	while (1)
	{
		/*assume pressed*/
		keypress = 1;

		switch (readKeys())
		{
		case UP:
			sprintf(str, "UP   \n");
			break;
		case DOWN:
			sprintf(str, "DOWN \n");
			break;
		case LEFT:
			sprintf(str, "LEFT \n");
			break;
		case RIGHT:
			sprintf(str, "RIGHT \n");
			break;

		case CENTER:
			sprintf(str, "CENTER\n");
			break;

		default:
			keypress = 0;
			break;
		}

		if (keypress)
		{
			for (c = 0; c < 10; c++)
			{
				sendchar(str[c]);
			}
		}
	}
	return 0;
}
