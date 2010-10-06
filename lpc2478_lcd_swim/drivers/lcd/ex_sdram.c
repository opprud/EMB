/*****************************************************************************
 *   ex_sdram.c:  External SDRAM memory module file for NXP LPC24xx Family 
 *   Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2007.01.13  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
//#include <general.h>
#include "lpc2468.h"
//#include "lcd_hw.h"
#include <config.h>

//#include "LPC23xx.h"				/* LPC23xx/24xx definitions */
#include "type.h"
//#include "irq.h"
//#include "target.h"
//#include "timer.h"
#include "ex_sdram.h"

void delay(short delayInMs)
{
  /*
   * setup timer #1 for delay
   */
  T1TCR = 0x02;          //stop and reset timer
  T1PR  = 0x00;          //set prescaler to zero
  T1MR0 = delayInMs * (Fpclk / 1000);
  T1IR  = 0xff;          //reset all interrrupt flags
  T1MCR = 0x04;          //stop timer on match
  T1TCR = 0x01;          //start timer

  //wait until delay time has elapsed
  while (T1TCR & 0x01)
    ;
}

/*****************************************************************************
** Function name:		SDRAMInit
**
** Descriptions:		Initialize external SDRAM memory Samsung 
**						K4S561632H, 256Mbit(4M x 16 bit). The same
**						code can be used for the Micron's MT48LC16M 
** 						For more info. regarding the details of the 
**						SDRAMs, go to their website for data sheet.  
**
** parameters:			None			 
** 						
** Returned value:		None
** 
*****************************************************************************/
void SDRAMInit( void )
{
  DWORD i, dummy = dummy;

  /*************************************************************************
  * Initialize EMC and SDRAM
  *************************************************************************/
  EMC_CTRL = 0x00000001;		/*Disable Address mirror*/
  PCONP   |= 0x00000800;		/* Turn On EMC PCLK */
  PINSEL5  = 0x55010115;
  PINSEL6  = 0x55555555;
  PINSEL7  = 0x55555555;
  PINSEL8  = 0x55555555;
  PINSEL9  = 0x50555555;  
  
  EMC_DYN_RP     = 2;		/* command period: 3(n+1) clock cycles */
  EMC_DYN_RAS    = 3;		/* RAS command period: 4(n+1) clock cycles */
  EMC_DYN_SREX   = 7;		/* Self-refresh period: 8(n+1) clock cycles */
  EMC_DYN_APR    = 2;		/* Data out to active: 3(n+1) clock cycles */
  EMC_DYN_DAL    = 5;		/* Data in to active: 6(n+1) clock cycles */
  EMC_DYN_WR     = 1;		/* Write recovery: 2(n+1) clock cycles */
  EMC_DYN_RC     = 5;		/* Active to Active cmd: 6(n+1) clock cycles */
  EMC_DYN_RFC    = 5;		/* Auto-refresh: 6(n+1) clock cycles */
  EMC_DYN_XSR    = 7;		/* Exit self-refresh: 8(n+1) clock cycles */
  EMC_DYN_RRD    = 1;		/* Active bank A->B: 2(n+1) clock cycles */
  EMC_DYN_MRD    = 2;		/* Load Mode to Active cmd: 3(n+1) clock cycles */

  EMC_DYN_RD_CFG = 1;		/* Command delayed strategy */

  /* Default setting, RAS latency 2 CCLKs, CAS latenty 2 CCLKs. */
//  EMC_DYN_RASCAS0 = 0x00000202;

  //
  EMC_DYN_RASCAS0 = 0x00000303;

#if ENG_BOARD_LPC24XX		/* NXP engineering board */
  /* 256MB, 16Mx16, 4 banks, row=12, column=9 */
  EMC_DYN_CFG0 = 0x00000480;
#else						/* Embedded Artists board */
  /* 256Mb, 8Mx32, 4 banks, row=12, column=9 */
//  EMC_DYN_CFG0 = 0x00005488;
  /* 256MB, 16Mx16, 4 banks, row=12, column=9 */
//  EMC_DYN_CFG0 = 0x00000480;
  EMC_DYN_CFG0 = 0x00000680;

#endif
  delay(100);			/* use timer 1 */

  /* Mem clock enable, CLKOUT runs, send command: NOP */
  EMC_DYN_CTRL = 0x00000183;
  delay(200);			/* use timer 1 */
    
  /* Send command: PRECHARGE-ALL, shortest possible refresh period */
  EMC_DYN_CTRL = 0x00000103;

  /* set 32 CCLKs between SDRAM refresh cycles */
  EMC_DYN_RFSH = 0x00000002;
  for(i = 0; i < 0x40; i++);	/* wait 128 AHB clock cycles */
    
  /* set 70 x 16CCLK = 1120CCLK = 15.556uS(@72MHz) between SDRAM refresh cycles */
  //EMC_DYN_RFSH = 70;
  //Set correct refresh period
  EMC_DYN_RFSH = 28;
    
  /* To set mode register in SDRAM, enter mode by issue
  MODE command, after finishing, bailout and back to NORMAL mode. */    
  /* Mem clock enable, CLKOUT runs, send command: MODE */
  EMC_DYN_CTRL = 0x00000083;
  
  /* Set mode register in SDRAM */
  /* Mode regitster table for Micron's MT48LCxx */
  /* bit 9:   Programmed burst length(0)
     bit 8~7: Normal mode(0)
	 bit 6~4: CAS latency 3
	 bit 3:   Sequential(0)
	 bit 2~0: Burst length is 8
	 row position is 12 */
  //dummy = *((volatile DWORD *)(SDRAM_BASE_ADDR | (0x22 << 11)));
  dummy = *((volatile unsigned int*)(SDRAM_BASE_ADDR | (0x33 << 12)));
  
  EMC_DYN_CTRL = 0x00000000;	  /* Send command: NORMAL */

  EMC_DYN_CFG0 |= 0x00080000;	  /* Enable buffer */
  delay(1);				  /* Use timer 1 */
  return;

}

/*********************************************************************************
**                            End Of File
*********************************************************************************/
