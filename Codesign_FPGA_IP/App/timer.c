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
static int tmrResolutionMs[2];
static int timers[10];
static callback callbacks[2];


/* declare IRQ prototypes */
void TMR0_Handler(void) __attribute__ ((interrupt("IRQ")));
void TMR1_Handler(void) __attribute__ ((interrupt("IRQ")));

/*
 * This is our TMR0_IRQ routine
 */
void TMR0_Handler(void)
{
	/* increment ticks */
	tmrTicks[0]++;

	/* any call backs to execute ? */
	if(callbacks[0])
		(*callbacks[0])();

	//(*tmr0Callback[MATCH0].tF)( &tmr0values[MATCH0] );

	/* reset all interrrupts */
	T0IR = 0xff;
	/* ack. interrupt */
	VICVectAddr = 0;
}

/*
 * This is our TMR0_IRQ routine
 */
void TMR1_Handler(void)
{

	/* increment ticks */
	tmrTicks[1]++;

	/* reset all interrrupts */
	T1IR = 0xff;
	/* ack. interrupt */
	VICVectAddr = 0;
}

/**
 * initTimer
 * This function  initializes a timer (0/1) to generate periodic interrupts,
 * after a specified number of clock ticks.
 * An interrupt must be attached to a callback funtion, to have any effect
 * The Peripheral clock, PCLK is set up to FCPU/2, in framework.c
 *
 * @param timer_num: 0, 1 for timers
 * @param IntervalMs: ms between interrupts
 *
 * @remarks som handling of invalid callbacks could be added
 */
signed int initTimerTicks(int TimerNum, int ticks, callback cb)
{
	if (TimerNum == 0)
	{
		/* setup timer #0 for generating periodic interrupts */
		T0TCR = 0x02; /* reset timer */
		T0PR = 0x00; /* set prescaler to zero */
//		T0MR0 = ticks; /*match register set to requested PCLK ticks - */
		T0MR1 = ticks; /*match register set to requested PCLK ticks - */
		T0IR = 0xff; /* reset all interrrupts */
//		T0MCR = 0x03; /* interrupt on match 0 */
		T0MCR = 0x18; /* reset & interrupt on match 1*/
		tmrResolutionMs[0] = ticks; /* save interval, for time calculation*/
		/* install callback*/
		callbacks[0] = cb;

		if (install_irq(TIMER0_INT, (void *) TMR0_Handler, HIGHEST_PRIORITY) == 1)
			return 0; /* success */
		else
			return -1; /* fail */
	}
	else if (TimerNum == 1)
	{
		/* setup timer #0 for generating periodic interrupts */
		T1TCR = 0x02; /* reset timer */
		T1PR = 0x00; /* set prescaler to zero */
		T1MR0 = ticks;
		T1IR = 0xff; /* reset all interrrupts */
		T1MCR = 0x03; /* interrupt on match */
		tmrResolutionMs[1] = ticks; /* save interval, for time calculation*/
		/* install callback*/
		//callbacks[0] = callback;

		if (install_irq(TIMER1_INT, (void *) TMR1_Handler, HIGHEST_PRIORITY) == 1)
			return 0; /* success */
		else
			return -1; /* fail */
	}
	return -1;
}

/**
 * initTimer
 * This function  initializes a timer (0/1) to generate periodic interrupts.
 * An interrupt must be attached to a callback funtion, to have any effect
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
		T0TCR = 0x02; /* reset timer */
		T0PR = 0x00; /* set prescaler to zero */
//		T0MR0 = (IntervalMs * (Fpclk / (1000 - 1)));
		T0MR1 = (IntervalMs * (Fpclk / (1000 - 1)));
		T0IR = 0xff; /* reset all interrrupts */
//		T0MCR = 0x03; /* interrupt on match 0 */
		T0MCR = 0x18; /* reset & interrupt on match 1 */
		tmrResolutionMs[0] = IntervalMs; /* save interval, for time calculation*/
		/* install callback*/
		//callbacks[0] = callback;

		if (install_irq(TIMER0_INT, (void *) TMR0_Handler, HIGHEST_PRIORITY) == 1)
			return 0; /* success */
		else
			return -1; /* fail */
	}
	else if (TimerNum == 1)
	{
		/* setup timer #0 for generating periodic interrupts */
		T1TCR = 0x02; /* reset timer */
		T1PR = 0x00; /* set prescaler to zero */
		T1MR0 = (IntervalMs * (Fpclk / (1000 - 1)));
		T1IR = 0xff; /* reset all interrrupts */
		T1MCR = 0x03; /* interrupt on match */
		tmrResolutionMs[1] = IntervalMs; /* save interval, for time calculation*/
		/* install callback*/
		//callbacks[0] = callback;

		if (install_irq(TIMER1_INT, (void *) TMR1_Handler, HIGHEST_PRIORITY) == 1)
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

/**
 * This function reads the number of elapsed ticks a timer
 * \n
 *
 * @param timeNum: tmr to invoke
 *
 * @return elapsed ticks
 */
int getTimerTicks(int timerNum)
{
	if (timerNum == 0)
		return tmrTicks[0];
	else if (timerNum == 1)
		return tmrTicks[1];
}

/**
 * This function returns the resolution (interval) on a timer
 * \n
 *
 * @param timeNum: tmr to invoke
 *
 * @return interval in [ms]
 */
int getTimerResolution(int timerNum)
{
	if (timerNum == 0)
		return tmrResolutionMs[0];
	else if (timerNum == 1)
		return tmrResolutionMs[1];
	return -1;
}

/**
 * soft timer, runs of ticks from tmr/cnt 0
 * \n
 *
 * @param timeNum: tmr to invoke
 * @param clear  : 1 resets timer
 *
 * @return elapsed milliseconds.
 *
 * !note the resolution in set when initializing the HW timer!
 */
int timer(int timerNum, int clear)
{

	if (timerNum < 11)
	{
		if (clear)
		{
			timers[timerNum] = (getTimerResolution(0) * getTimerTicks(0));
			return 0;
		}
		else
			return ((getTimerResolution(0) * getTimerTicks(0)) - timers[timerNum]);

	}
	return -1;
}
