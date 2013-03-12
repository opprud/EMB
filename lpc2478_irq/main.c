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

/*profiling*/
#include "simple_profiler.h"

/* global variables */
int i;

/* declare an IRQ prototype */
void IRQ_Routine(void) __attribute__ ((interrupt("IRQ")));

/*
 * This is our IRQ routine
 */
void IRQ_Routine(void) {
	//SET
	PROF_SET = PROF_PIN_1;

	/*clear pending INT0 interrupts*/
	EXTINT = 1;

	/*do some usefull stuff */
	i++;
	i++;

	/* ack. interrupt */
	VICVectAddr = 0;

	//CLR
	PROF_CLR = PROF_PIN_1;

}

/* main loop
 * basic CPU initialization is done here.
 */
int main(void) {
	int c;
	/* Flash memory setup */
	MEMMAP = 1;
	MAMTIM = 3;
	MAMCR = 2;

	/* Pin 2.10 as EINT0, all GPIO are inputs as default */
	PINSEL4 |= (1 << 20);
	/* INT0 Edge sensitive */
	EXTMODE |= 1;
	/* falling edge */
	EXTPOLAR &= (~1);
	/*clear pending INT0 interrupts*/
	EXTINT = 1;

	/* init the VIC to EINT0, at vector 14 */
	VICVectAddr14 =	(unsigned) IRQ_Routine;
	/*priority 2, rather high*/
	VICVectCntl14 = 0x00000002;
	/*enable EINT0 interrupt*/
	VICIntEnable = (1 << 14);

	/* Setup profiling pins to outputs */
	PROF_DIR = PROF_PIN_MASK;

	enableIRQ();

	/*waste time...*/
	while (1)
	{
		c++;
	}
	return 0;
}
