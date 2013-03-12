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

/* global variables */

/* defines */
/* task timers, in [ms] */
#define TASK0_INTERVAL	230
#define TASK1_INTERVAL	100


/* main loop
 * basic CPU initialization is done here.
 */
int main(void)
{
	/* init low level stuff */
	lowLevelInit();

	/*setup VIC*/
	init_VIC();

	/* init 2 GPIOs for debug */
	FIO2DIR |= (1<<21);
	FIO2DIR |= (1<<22);

	/* init timer 0, to generate IRQ's every 10mS */
	initTimer(0, 10 );

	/* enable IRQ's globally */
	enableIRQ();

	/* start timer */
	enableTimer(0);

	/* reset soft timers */
	timer(0, RESET);
	timer(1, RESET);

	/*loop....*/
	while (1)
	{
		/* task 0, needs service ? */
		if(timer(0, RUN) > TASK0_INTERVAL)
		{
			/*reset soft timer */
			timer(0, RESET);
			/* toggle GPIO */
			FIO2PIN = ((~FIO2PIN) & (1 << 22));
		}
		/* task 1, needs service ? */
		if(timer(1, RUN) > TASK1_INTERVAL)
		{
			/*reset soft timer */
			timer(1, RESET);
			/* toggle GPIO */
			FIO2PIN = ((~FIO2PIN) & (1 << 21));
		}
	}
	return 0;
}
