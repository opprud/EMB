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
#include "keys.h"
#include "pwm.h"


/* global variables */

/* defines */
/* task timers, in [ms] */
#define BUTTON_TASK_INTERVAL	200


/* main loop
 * basic CPU initialization is done here.
 */
int main(void)
{
	int c;
	char dc=0;
	/* init low level stuff */
	lowLevelInit();

	/*setup VIC*/
	init_VIC();

	/* init pwm, tp 430uS = 2.3KHz */
	initPwm(0x3000,1,1);

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
		if(timer(0, RUN) > BUTTON_TASK_INTERVAL)
		{
			/*reset soft timer */
			timer(0, RESET);

			switch (readKeys()) {
				case UP:
					if(dc<100)
						dc +=5;
					setPwmDc(dc,1,1);
					break;
				case DOWN:
					if(dc>0)
						dc -=5;
					setPwmDc(dc,1,1);
					break;
			}
		}
	}
	return 0;
}
