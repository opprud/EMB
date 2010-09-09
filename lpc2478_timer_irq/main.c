/**********************************
 * Small IRQ demonstration program
 * Sets up EINT0 to generate an IRQ
 * through the ARM VIC
 *
 * Author  : morten@hih.au.dk
 * Date    : Aug. 27, 2010
 *********************************/
#include "lpc2468.h"
#include "VIClowlevel.h"
#include "irq.h"
#include "timer.h"
#include "extint.h"
#include "type.h"

/* global variables */
static int ledOn =0;

/* defines */
#define LED_MASK (1<<10);

//void callback10Ms(void) __attribute__ ((interrupt("IRQ")));

/* timer callback function, toggles LED */
// wont work for now....
void callback10Ms(void)
{
	if(ledOn)
	{
		FIO2CLR = LED_MASK;
		ledOn=0;
	}
	else
	{
		FIO2SET = LED_MASK;
		ledOn=1;
	}
}


/* init GPIO for LED, set P2.10 as output */
static void ledInit()
{
	PINSEL4 = 0;
	FIO2DIR |= LED_MASK;
	FIO2CLR =  LED_MASK;
}

/* main loop
 * basic CPU initialization is done here.
 */
int main(void)
{
	int c;


	/* init low level stuff */
	lowLevelInit();

	/*setup VIC*/
	init_VIC();

	/* init ext int 0 */
	extint0Init();

	/* init LED */
	//ledInit();
	FIO2DIR |= (1<<21);

	/* init timer 0, to generate IRQ's every 10mS */
	initTimer(0, 10 );

	/* init timer 1, to generate IRQ's every 227mS */
	//initTimer(0, 300 , & callback10Ms );

	/* enable IRQ's globally */
	enableIRQ();

	/* start timer */
	enableTimer(0);

	/*waste time...*/
	while (1)
		c++;

	return 0;
}
