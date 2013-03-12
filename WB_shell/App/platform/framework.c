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

	/* init pins for External Memory Controller, CS3 */
	/**************************************************************************
	 * Initialize EMC for NOR FLASH
	 **************************************************************************/
	EMC_CTRL = 0x00000001;

	/* Turn On EMC PCLK */
	PCONP |= 0x00000800;
	/* enable CS2, P2.14 */
	PINSEL4 = 0x10000000;
	/* setup dynamic EMC pins  */
	//PINSEL5 = 0x05050555;
	/* enable D0-15 */
	PINSEL6 = 0x55555555;
	/* enable D16-31 */
	//PINSEL7 = 0x55555555;
	/* enable A0-15 */
	PINSEL8 = 0x55555555;
	/*enable A16-19, and OE,P4.24 + WE,P4.25 */
	PINSEL9 = 0x50555555;

	/* CS2 memory bank
	 * bit
	 * 0:1	MW = 16bit
	 * 3	PM = 1 ?
	 * 6	PC = 0
	 * 7	PB = 0
	 * 8	EW = 0
	 * 19	B  = 0
	 * 20	WP = 0
	 *  */
	EMC_STA_CFG2 = 0x00000081;

	//READ timing
	EMC_STA_WAITOEN2  = 0;//0x1;
	EMC_STA_WAITRD2   = 8;//0x03;

	// Write timing
	EMC_STA_WAITWEN2  = 0;//0x2;
	EMC_STA_WAITWR2   = 0x8;

	EMC_STA_WAITPAGE2 = 0x1f;
	EMC_STA_WAITTURN2 = 0xf;
}
