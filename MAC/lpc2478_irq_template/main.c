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
#include "extint.h"

/* global variables */


/* main loop
 * basic CPU initialization is done here.
 */
int main(void)
{
	int c;

	/* Flash memory setup */
	MEMMAP = 1;
	MAMTIM = 3;
	MAMCR = 2;

	/*setup VIC*/
	init_VIC();

	/* init ext int 0 */
	extint0Init();

	/* enable IRQ's globally */
	enableIRQ();

	/*waste time...*/
	while (1)
		c++;

	return 0;
}
