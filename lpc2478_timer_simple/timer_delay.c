/*
 * timer.c
 *
 *  Created on: 09/09/2010
 *      Author: morten
 */

#include <lpc2468.h>
#include "config.h"


/**
 * delayMs
 * This function Starts the timer, delays in milliseconds and waits until elapsed .
 * Still rather dumb, just burning MIPS...\n
 * The Peripheral clock, PCLK is set up to FCPU/2, in framework.c
 *
 * @param timer_num: 0, 1 for timers
 * @param delayInMs: ms to wait
 *
 * @remarks
 */

void delayMs(int timer_num, int delayInMs)
{
  if ( timer_num == 0 )
  {
	/*
	* setup timer #0 for delay
	*/
	T0TCR = 0x02;		/* reset timer */
	T0PR  = 0x00;		/* set prescaler to zero */
	T0MR0 = delayInMs * (Fpclk / 1000-1);
	T0IR  = 0xff;		/* reset all interrrupts */
	T0MCR = 0x04;		/* stop timer on match */
	T0TCR = 0x01;		/* start timer */

	/* wait until delay time has elapsed */
	while (T0TCR & 0x01);
  }
  else if ( timer_num == 1 )
  {
	/*
	* setup timer #1 for delay
	*/
	T1TCR = 0x02;		/* reset timer */
	T1PR  = 0x00;		/* set prescaler to zero */
	T1MR0 = delayInMs * (Fpclk / 1000-1);
	T1IR  = 0xff;		/* reset all interrrupts */
	T1MCR = 0x04;		/* stop timer on match */
	T1TCR = 0x01;		/* start timer */

	/* wait until delay time has elapsed */
	while (T1TCR & 0x01);
  }
  return;
}


