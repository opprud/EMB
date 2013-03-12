/*
 * extint.c
 *
 *  Created on: 27/08/2010
 *  Author: morten
 */

#ifndef EXTINT_C_
#define EXTINT_C_

#include "type.h"
#include "lpc2468.h"
#include "irq.h"

/* declare an IRQ prototype */
void EINT0_Handler(void) __attribute__ ((interrupt("IRQ")));

/*
 * This is our EXTINT0_IRQ routine
 */
void EINT0_Handler(void)
{
	static int i;

	/*clear pending INT0 interrupts*/
	EXTINT = 1;

	/*do some usefull stuff */
	i++;
	i++;

	/* ack. interrupt */
	VICVectAddr = 0;
}

/**********************************************************
 * Initializes EINT0 to generate an IRQ when triggered
 * EINT0 is falling edge sensitive
 *
 * returns : OK or FAIL
 **********************************************************/
int extint0Init(void)
{
	/* Pin 2.10 as EINT0, all GPIO are inputs as default */
	PINSEL4 |= (1 << 20);
	/* INT0 Edge sensitive */
	EXTMODE |= 1;
	/* falling edge */
	EXTPOLAR &= (~1);
	/*clear pending INT0 interrupts*/
	EXTINT = 1;

	/* try t install IRQ */
	if (install_irq(EINT0_INT, (void *) EINT0_Handler, HIGHEST_PRIORITY) == FALSE)
	{
		return (FAIL);
	}
	return (OK);
}

#endif /* EXTINT_C_ */
