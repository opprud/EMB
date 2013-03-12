/**********************************
 * Small Timer demonstration program
 * Sets up TMR0 to generate an IRQ
 * through the ARM VIC every 10 ms
 * Also demonstrates a sof timer implementation
 *
 * Author  : morten@hih.au.dk
 * Date    : Sep. 17, 2010
 *********************************/
#include "lpc2468.h"
#include "VIClowlevel.h"
#include "irq.h"
#include "timer.h"
#include "framework.h"
#include "type.h"
#include "uart.h"

/* global variables */

/* defines */
/* task timers, in [ms] */
#define TASK0_INTERVAL	230
#define TASK1_INTERVAL	100

/* init GPIO for LED, set P2.10 as output */
static void ledInit(){
	PINSEL4  = 0;
	FIO2DIR |= (1<<10);
	FIO2SET  = (1<<10);//led off
}


/* main loop
 * basic CPU initialization is done here.
 */
int main(void)
{
	char str[50] = {"Hello EMB world, from EA2478, MOJ Sept. 3 2012.\n"};
	char c;

	/* init low level stuff */
	lowLevelInit();

	/*setup VIC*/
	init_VIC();

	/* init 2 GPIOs for debug */
	FIO2DIR |= (1<<21);
	FIO2DIR |= (1<<22);

	/*initialize uart #1: 9600 bps, 8N1*/
	initUart0(B9600(Fpclk), UART_8N1);

	/* init led */
	ledInit();

	/* init timer 0, to generate IRQ's every 10mS */
	initTimer(0, 10 );

	/* enable IRQ's globally */
	enableIRQ();

	/* start timer */
	enableTimer(0);

	/* reset soft timers */
	timer(0, RESET);
	timer(1, RESET);

	c=0;

	/*loop....*/
	while (1)
	{

		if(c<40)
		{
			sendchar(str[c]);
			c++;
		}

		/* task 0, needs service ? */
		if(timer(0, RUN) > TASK0_INTERVAL)
		{
			/*reset soft timer */
			timer(0, RESET);
			/* toggle GPIO */
			//FIO2PIN = ((~FIO2PIN) & (1 << 22));
		}
		/* task 1, needs service ? */
		if(timer(1, RUN) > TASK1_INTERVAL)
		{
			/*reset soft timer */
			timer(1, RESET);
			/* toggle GPIO */
			//FIO2PIN = ((~FIO2PIN) & (1 << 21));
		}
	}
	return 0;
}
