/*
 * timer.c
 *
 *  Created on: 09/09/2010
 *      Author: morten
 */

#include <lpc2468.h>
#include "timer.h"
#include "irq.h"
#include "config.h"


/* global variables */
static int tmrTicks[2];

/* callback array */
cb callbacks[2];

/* declare an IRQ prototype */
void TMR0_Handler(void) __attribute__ ((interrupt("IRQ")));

/*
 * This is our TMR0_IRQ routine
 */
void TMR0_Handler(void)
{
	/* toggle LED */
	//FIO2PIN = ((~FIO2PIN ) & (1<<10));
	FIO2PIN = ((~FIO2PIN ) & (1<<21));


	/* increment ticks */
	tmrTicks[0]++;

	/* fire callbacks, if any */
//	if(callbacks[0])
//		(*callbacks[0])();

	/* reset all interrrupts */
	T0IR = 0xff;
	/* ack. interrupt */
	VICVectAddr = 0;
}

/**
 * initTimer
 * This function  initializes a timer (0/1) to generate periodic interrupts.
 * An interrupt must be attache to a callback funtion, to have any effect
 * Still rather dumb, just burning MIPS...\n
 * The Peripheral clock, PCLK is set up to FCPU/2, in framework.c
 *
 * @param timer_num: 0, 1 for timers
 * @param IntervalMs: ms between interrupts
 *
 * @remarks som handling of invalid callbacks could be added
 */
//signed int initTimer(int TimerNum, int IntervalMs, tmrFunc callback)
signed int initTimer(int TimerNum, int IntervalMs)
{

	if (TimerNum == 0)
	{
		/* setup timer #0 for generating periodic interrupts */
		T0TCR = 0x02; 	/* reset timer */
		T0PR = 0x00; 	/* set prescaler to zero */
		T0MR0 = (IntervalMs * (Fpclk / (1000 - 1)));
		T0IR = 0xff; 	/* reset all interrrupts */
		T0MCR = 0x03; 	/* interrupt on match */

		/* install callback*/
		//callbacks[0] = callback;

		if (install_irq(TIMER0_INT, (void *) TMR0_Handler, HIGHEST_PRIORITY) == 1)
			return 0; /* success */
		else
			return -1; /* fail */
	}

	return -1;
}

/**
 * This function Enables a timer
 * \n
 *
 * @param timeNum: tmr to invoke
 */
void enableTimer(int timerNum)
{
	if (timerNum == 0)
	{
		T0TCR = 1;
	}
	else if (timerNum == 1)
	{
		T1TCR = 1;
	}
	return;
}

/**
 * This function disables a timer
 * \n
 *
 * @param timeNum: tmr to invoke
 */
void disableTimer(int timerNum)
{
	if (timerNum == 0)
	{
		T0TCR = 0;
	}
	else if (timerNum == 1)
	{
		T1TCR = 0;
	}
	return;
}

