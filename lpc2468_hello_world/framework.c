/******************************************************************************
 *
 * Copyright:
 *    (C) 2000 - 2005 Embedded Artists AB
 *
 * Description:
 *    Framework for ARM7 processor
 *
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "lpc2468.h"                            /* LPC2xxx definitions */
#include "config.h"
#include "framework.h"

/******************************************************************************
 * Defines, macros, and typedefs
 *****************************************************************************/

/******************************************************************************
 * External variables
 *****************************************************************************/
extern char end asm ("end"); //the symbol "end" is defined by the linker script


/******************************************************************************
 * Public variables
 *****************************************************************************/
unsigned char *pHeapStart;
unsigned char *pHeapEnd;

/******************************************************************************
 * External functions
 *****************************************************************************/

/******************************************************************************
 * Public functions
 *****************************************************************************/


/******************************************************************************
 * Implementation of public functions
 *****************************************************************************/


/******************************************************************************
 ** Function name:		ConfigurePLL
 **
 ** Descriptions:		Configure PLL switching to main OSC instead of IRC
 **						at power up and wake up from power down.
 **						This routine is used in TargetResetInit() and those
 **						examples using power down and wake up such as
 **						USB suspend to resume, ethernet WOL, and power management
 **						example
 ** parameters:			None
 ** Returned value:		None
 **
 ******************************************************************************/
void ConfigurePLL(void)
{
	unsigned int MValue, NValue;

	if (PLLSTAT & (1 << 25))
	{
		PLLCON = 1; /* Enable PLL, disconnected */
		PLLFEED = 0xaa;
		PLLFEED = 0x55;
	}

	PLLCON = 0; /* Disable PLL, disconnected */
	PLLFEED = 0xaa;
	PLLFEED = 0x55;

	SCS |= 0x20; /* Enable main OSC */
	while (!(SCS & 0x40))
		; /* Wait until main OSC is usable */

	CLKSRCSEL = 0x1; /* select main OSC, 12MHz, as the PLL clock source */

	PLLCFG = PLL_MValue | (PLL_NValue << 16);
	PLLFEED = 0xaa;
	PLLFEED = 0x55;

	PLLCON = 1; /* Enable PLL, disconnected */
	PLLFEED = 0xaa;
	PLLFEED = 0x55;

	CCLKCFG = CCLKDivValue; /* Set clock divider */
#if USE_USB
	USBCLKCFG = USBCLKDivValue; /* usbclk = 288 MHz/6 = 48 MHz */
#endif

	while (((PLLSTAT & (1 << 26)) == 0))
		; /* Check lock bit status */

	MValue = PLLSTAT & 0x00007FFF;
	NValue = (PLLSTAT & 0x00FF0000) >> 16;
	while ((MValue != PLL_MValue) && (NValue != PLL_NValue))
		;

	PLLCON = 3; /* enable and connect */
	PLLFEED = 0xaa;
	PLLFEED = 0x55;
	while (((PLLSTAT & (1 << 25)) == 0))
		; /* Check connect bit status */
}

/*****************************************************************************
 *
 * Description:
 *    Initialize system functions and GPIO
 *
 ****************************************************************************/
void lowLevelInit(void)
{
	PINSEL0 = 0x00000000;
	PINSEL1 = 0x00000000;

	//  IOSET0 = 0x00000000;       //Initialize pins to high level
	//  IOCLR0 = 0xffffffff;       //Initialize pins to low level
	//  IODIR0 = 0x00000000;       //Set pin direction

	//initialize the exception vector mapping
	MEMMAP = MAM_MAP;

#if USE_USB
	PCONP |= 0x80000000; /* Turn On USB PCLK */
#endif
	/* Configure PLL, switch from IRC to Main OSC */
	ConfigurePLL();

	/* Set system timers for each component */
#if (Fpclk / (Fcclk / 4)) == 1
	PCLKSEL0 = 0x00000000; /* PCLK is 1/4 CCLK */
	PCLKSEL1 = 0x00000000;
#endif
#if (Fpclk / (Fcclk / 4)) == 2
	PCLKSEL0 = 0xAAAAAAAA; /* PCLK is 1/2 CCLK */
	PCLKSEL1 = 0xAAAAAAAA;
#endif
#if (Fpclk / (Fcclk / 4)) == 4
	PCLKSEL0 = 0x55515555; /* PCLK is the same as CCLK */
	PCLKSEL1 = 0x55555555;
#endif

	/* Set memory accelerater module*/
	MAMCR = 0;
#if Fcclk < 20000000
	MAMTIM = 1;
#else
#if Fcclk < 40000000
	MAMTIM = 2;
#else
	MAMTIM = 3;
#endif
#endif
	MAMCR = MAM_SETTING; //0=disabled, 1=partly enabled (enabled for code prefetch, but not for data), 2=fully enabled
}
