/*
 * (C) Copyright 2006-2007 Embedded Artists AB <www.embeddedartists.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <config.h>
#include <version.h>
#include <exports.h>
#include <asm/arch/hardware.h>
#include <asm/hardware.h>


/******************************************************************************
 * Defines, macros, and typedefs
 *****************************************************************************/
#define	USE_USB					1

#define PLL_MValue		(CONFIG_PLL_MVALUE-1)
#define PLL_NValue		(CONFIG_PLL_NVALUE-1)
#define CCLKDivValue		(CONFIG_PLL_CLKDIV-1)
#define USBCLKDivValue		(CONFIG_PLL_USBCLKDIV-1)

#define Fcco	((2*CONFIG_PLL_MVALUE*CONFIG_FOSC) / CONFIG_PLL_NVALUE)
#define Fcclk	(Fcco / CONFIG_PLL_CLKDIV)
#define Fpclk	(Fcclk / CONFIG_FPCLK_DIV)

#define MAM_SETTING  1                /* 0=disabled,
                                         1=partly enabled (enabled for code prefetch, but not for data),
                                         2=fully enabled */

#define MEM_MAP 2                     /* When executing from RAM, MAM_MAP should always be 2 */

#define SDRAM_BASE_ADDR 0xA0000000


/*****************************************************************************
 *
 * Description:
 *    Delay execution by a specified number of milliseconds by using
 *    timer #1. A polled implementation.
 *
 * Params:
 *    [in] delayInMs - the number of milliseconds to delay.
 *
 ****************************************************************************/
void
delayMs(unsigned short delayInMs)
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

/******************************************************************************
** Function name:		GPIOinit
**
** Descriptions:		Sets all GPIO ports to a known state
** parameters:			None
** Returned value:	None
** 
******************************************************************************/
static void
GPIOinit(void)
{
  PINSEL0 = 0;
  PINSEL1 = 0;
  PINSEL2 = 0;
  PINSEL3 = 0;
  PINSEL4 = 0;
  PINSEL5 = 0;
  PINSEL6 = 0;
  PINSEL7 = 0;
  PINSEL8 = 0;
  PINSEL9 = 0;
  PINSEL10 = 0;

  IODIR0 = 0;
  IODIR1 = 0;
  IOSET0 = 0xffffffff;
  IOSET1 = 0xffffffff;

  FIO0DIR = 0;
  FIO1DIR = 0;
  FIO2DIR = 0;
  FIO3DIR = 0;
  FIO4DIR = 0;

  FIO0SET = 0xffffffff;
  FIO1SET = 0xffffffff;
  FIO2SET = 0xffffffff;
  FIO3SET = 0xffffffff;
  FIO4SET = 0xffffffff;
}

/******************************************************************************
** Function name:		VICinit
**
** Descriptions:		Initialize the VIC to a known state
** parameters:			None
** Returned value:	None
** 
******************************************************************************/
static void
VICinit(void)
{
  //initialize VIC
  VICIntEnClr    = 0xFFFFFFFF;           /* Disable ALL interrupts                             */
  VICProtection  = 0;                    /* Setup interrupt controller                         */
  VICVectAddr    = 0;
  VICIntSelect   = 0;
  VICVectAddr0   = (unsigned int)0;      /* Set the vector address                             */
  VICVectAddr1   = (unsigned int)0;      /* Set the vector address                             */
  VICVectAddr2   = (unsigned int)0;      /* Set the vector address                             */
  VICVectAddr3   = (unsigned int)0;      /* Set the vector address                             */
  VICVectAddr4   = (unsigned int)0;      /* Set the vector address                             */
  VICVectAddr5   = (unsigned int)0;      /* Set the vector address                             */
  VICVectAddr6   = (unsigned int)0;      /* Set the vector address                             */
  VICVectAddr7   = (unsigned int)0;      /* Set the vector address                             */
  VICVectAddr8   = (unsigned int)0;      /* Set the vector address                             */
  VICVectAddr9   = (unsigned int)0;      /* Set the vector address                             */
  VICVectAddr10  = (unsigned int)0;      /* Set the vector address                             */
  VICVectAddr11  = (unsigned int)0;      /* Set the vector address                             */
  VICVectAddr12  = (unsigned int)0;      /* Set the vector address                             */
  VICVectAddr13  = (unsigned int)0;      /* Set the vector address                             */
  VICVectAddr14  = (unsigned int)0;      /* Set the vector address                             */
  VICVectAddr15  = (unsigned int)0;      /* Set the vector address                             */
  VICVectAddr16  = (unsigned int)0;      /* Set the vector address                             */
  VICVectAddr17  = (unsigned int)0;      /* Set the vector address                             */
  VICVectAddr18  = (unsigned int)0;      /* Set the vector address                             */
  VICVectAddr19  = (unsigned int)0;      /* Set the vector address                             */
  VICVectAddr20  = (unsigned int)0;      /* Set the vector address                             */
  VICVectAddr21  = (unsigned int)0;      /* Set the vector address                             */
  VICVectAddr22  = (unsigned int)0;      /* Set the vector address                             */
  VICVectAddr23  = (unsigned int)0;      /* Set the vector address                             */
  VICVectAddr24  = (unsigned int)0;      /* Set the vector address                             */
  VICVectAddr25  = (unsigned int)0;      /* Set the vector address                             */
  VICVectAddr26  = (unsigned int)0;      /* Set the vector address                             */
  VICVectAddr27  = (unsigned int)0;      /* Set the vector address                             */
  VICVectAddr28  = (unsigned int)0;      /* Set the vector address                             */
  VICVectAddr29  = (unsigned int)0;      /* Set the vector address                             */
  VICVectAddr30  = (unsigned int)0;      /* Set the vector address                             */
  VICVectAddr31  = (unsigned int)0;      /* Set the vector address                             */

  VICVectCntl0   = (unsigned int)0xf;
  VICVectCntl1   = (unsigned int)0xf;
  VICVectCntl2   = (unsigned int)0xf;
  VICVectCntl3   = (unsigned int)0xf;
  VICVectCntl4   = (unsigned int)0xf;
  VICVectCntl5   = (unsigned int)0xf;
  VICVectCntl6   = (unsigned int)0xf;
  VICVectCntl7   = (unsigned int)0xf;
  VICVectCntl8   = (unsigned int)0xf;
  VICVectCntl9   = (unsigned int)0xf;
  VICVectCntl10   = (unsigned int)0xf;
  VICVectCntl11   = (unsigned int)0xf;
  VICVectCntl12   = (unsigned int)0xf;
  VICVectCntl13   = (unsigned int)0xf;
  VICVectCntl14   = (unsigned int)0xf;
  VICVectCntl15   = (unsigned int)0xf;
  VICVectCntl16   = (unsigned int)0xf;
  VICVectCntl17   = (unsigned int)0xf;
  VICVectCntl18   = (unsigned int)0xf;
  VICVectCntl19   = (unsigned int)0xf;
  VICVectCntl20   = (unsigned int)0xf;
  VICVectCntl21   = (unsigned int)0xf;
  VICVectCntl22   = (unsigned int)0xf;
  VICVectCntl23   = (unsigned int)0xf;
  VICVectCntl24   = (unsigned int)0xf;
  VICVectCntl25   = (unsigned int)0xf;
  VICVectCntl26   = (unsigned int)0xf;
  VICVectCntl27   = (unsigned int)0xf;
  VICVectCntl28   = (unsigned int)0xf;
  VICVectCntl29   = (unsigned int)0xf;
  VICVectCntl30   = (unsigned int)0xf;
  VICVectCntl31   = (unsigned int)0xf;
}

/******************************************************************************
** Function name:		UART0init
**
** Description: 		Configure UART #0 to 38400 bps, 8N1
** Parameters:			None
** Returned value:	None
** 
******************************************************************************/
void
UART0init(void)
{
  PINSEL0 = 0x00000050;  
  PINSEL1 = 0x00000000;  

  U0LCR = 0;
  U0IER = 0;
  U0LCR = 0x80;	/* DLAB=1 */
  U0DLL = (Fpclk/16)/CONFIG_BAUDRATE;
  U0DLM = 0;
  U0LCR = 0x03;	/* 8N1, DLAB=0  */
  U0FCR = 0x07;		/* Enable RX and TX FIFOs */
}

/*****************************************************************************
** Function name:   UART1init
**
** Description:     Configure UART #1 to 9600 bps, 8N1
** Parameters:      None
** Returned value:  None
**
*****************************************************************************/
void
UART1init(void)
{
  PINSEL7 |= 0x30003fff;
  U1LCR = 0;
  U1IER = 0;
  U1LCR = 0x80;
  U1DLL = ((Fpclk/16)/9600) & 0xff;
  U1DLM = ((Fpclk/16)/9600) >> 8;
  U1LCR = 0x03;
  U1FCR = 0x07;

  U1THR = '\n';
}

/******************************************************************************
** Function name:		UART0init
**
** Description: 		Configure UART #0 to 38400 bps, 8N1
** Parameters:			None
** Returned value:	None
** 
******************************************************************************/
void
UART2init(void)
{

  PCONP |= (1<<24);
  
  PINSEL0 |= 0x00500000;
#if 0
  U2LCR = 0;
  U2IER = 0;
  U2LCR = 0x80;	/* DLAB=1 */
  U2DLL = (Fpclk/16)/38400;
  U2DLM = 0;
  U2LCR = 0x03;	/* 8N1, DLAB=0  */
  U2FCR = 0x07;		/* Enable RX and TX FIFOs */
  U2THR = '\n';
#endif

}


/******************************************************************************
** Function name:		printEndBanner
**
** Descriptions:		Prints a couple of ending chars
** parameters:			None
** Returned value:	None
** 
******************************************************************************/
void
printEndBanner(void)
{
  static const unsigned char message[] = "..\r\n";
  unsigned int i;
  unsigned char *pMsg = (unsigned char *)message - (unsigned char *)TEXT_BASE;

  for(i=0; i<(sizeof(message)-1); i++)
  {
    while((U0LSR & (1<<5)) == 0); /* Wait for empty U0THR */
    U0THR = *pMsg++;
  }
}

/******************************************************************************
** Function name:		printBanner
**
** Descriptions:		Prints a startup banner
** parameters:			None
** Returned value:	None
** 
******************************************************************************/
void
printBanner(void)
{
  static const unsigned char message[] =
    "\n\n\r**********************************************\n\r"
          "*        LPC2478 Micro-Blox Board from       *\n\r"
          "*             Future Electronics             *\n\r"
          "*                Version: 1.0                *\n\r"
          "**********************************************\n\r"
          "Booting (may take some time).";

  unsigned int i;
  unsigned char *pMsg = (unsigned char *)message - (unsigned char *)TEXT_BASE;

  for(i=0; i<(sizeof(message)-1); i++)
  {
    while((U0LSR & (1<<5)) == 0); /* Wait for empty U0THR */
    U0THR = *pMsg++;
  }
}

/******************************************************************************
** Function name:		ConfigurePLL
**
** Descriptions:		Configure the PLL
** parameters:			None
** Returned value:	None
** 
******************************************************************************/
void
ConfigurePLL(void)
{
	volatile unsigned long MValue;
	volatile unsigned long NValue;

	if ( PLLSTAT & (1 << 25) )
	{
		PLLCON = 1;			/* Enable PLL, disconnected */
		PLLFEED = 0xaa;
		PLLFEED = 0x55;
	}

    PLLCON = 0;				/* Disable PLL, disconnected */
    PLLFEED = 0xaa;
    PLLFEED = 0x55;
    
	SCS |= 0x20;			/* Enable main OSC */
    while( !(SCS & 0x40) );	/* Wait until main OSC is usable */

    CLKSRCSEL = 0x1;		/* select main OSC, 12MHz, as the PLL clock source */

    PLLCFG = PLL_MValue | (PLL_NValue << 16);
    PLLFEED = 0xaa;
    PLLFEED = 0x55;
      
    PLLCON = 1;				/* Enable PLL, disconnected */
    PLLFEED = 0xaa;
    PLLFEED = 0x55;

    CCLKCFG = CCLKDivValue;	/* Set clock divider */
#if USE_USB
    USBCLKCFG = USBCLKDivValue;		/* usbclk = 288 MHz/6 = 48 MHz */
#endif

    while ( ((PLLSTAT & (1 << 26)) == 0) );	/* Check lock bit status */
    
    MValue = PLLSTAT & 0x00007FFF;
    NValue = (PLLSTAT & 0x00FF0000) >> 16;
    while ((MValue != PLL_MValue) && ( NValue != PLL_NValue) );

    PLLCON = 3;				/* enable and connect */
    PLLFEED = 0xaa;
    PLLFEED = 0x55;
	while ( ((PLLSTAT & (1 << 25)) == 0) );	/* Check connect bit status */
}

/******************************************************************************
** Function name:		ConfigureEMC
**
** Descriptions:		Configure EMC for external SDRAM, NAND and NOR FLASH
** parameters:			None
** Returned value:	None
** 
******************************************************************************/
void
ConfigureEMC(void)
{
	volatile unsigned int i, dummy = dummy;

//  SCS     &= ~0x00000002;
//  SCS     |= 0x00000002;
  EMC_CTRL = 0x00000001;
	PCONP   |= 0x00000800;		/* Turn on EMC PCLK */
  
	// CS2 & CS3 not used PINSEL4  = 0x50000000;
#ifdef CONFIG_LPC_USE_32_BIT_DATABUS
  PINSEL5  = 0x55010115;
  PINSEL7  = 0x55555555;
#else
  PINSEL5  = 0x05050515;
#endif
  PINSEL6  = 0x55555555;
  PINSEL8  = 0x55555555;
  PINSEL9  = 0x50555555;

  //all registers...
#ifdef CONFIG_LPC_USE_32_BIT_DATABUS
  EMC_DYN_RP     = 1;   //>20ns = 2 clk
  EMC_DYN_RAS    = 3;   //>45ns = 3 clk
  EMC_DYN_SREX   = 5;   //>80-100ns = 6 clk
  EMC_DYN_APR    = 1;   //
  EMC_DYN_DAL    = 5;   //2 clk
  EMC_DYN_WR     = 1;   //2 clk
  EMC_DYN_RC     = 5;   //>65ns = 4 clk
  EMC_DYN_RFC    = 5;   //>80-100ns = 6 clk
  EMC_DYN_XSR    = 5;   //>80-100ns = 6 clk
  EMC_DYN_RRD    = 1;   //>15ns = 1-2 clk
  EMC_DYN_MRD    = 1;   //2 clk
  EMC_DYN_RD_CFG = 1;   //or 1,2,3
    
  //
  EMC_DYN_RASCAS0 = 0x00000202;

  //
  EMC_DYN_CFG0 = 0x00005480;
#else
  EMC_DYN_RP     = 2;   //>20ns = 2 clk
  EMC_DYN_RAS    = 3;   //>45ns = 3 clk
  EMC_DYN_SREX   = 7;   //>80-100ns = 6 clk
  EMC_DYN_APR    = 2;   //
  EMC_DYN_DAL    = 5;   //2 clk
  EMC_DYN_WR     = 1;   //2 clk
  EMC_DYN_RC     = 5;   //>65ns = 4 clk
  EMC_DYN_RFC    = 5;   //>80-100ns = 6 clk
  EMC_DYN_XSR    = 7;   //>80-100ns = 6 clk
  EMC_DYN_RRD    = 1;   //>15ns = 1-2 clk
  EMC_DYN_MRD    = 2;   //2 clk
  EMC_DYN_RD_CFG = 1;   //or 1,2,3
    
  //
  EMC_DYN_RASCAS0 = 0x00000303;
    
  //
  EMC_DYN_CFG0 = 0x00000680;
#endif
    
  //wait 100mS
  delayMs(100);

  //Send command: NOP
  EMC_DYN_CTRL = 0x00000183;
    
  //wait 200mS
  delayMs(200);
    
  //Send command: PRECHARGE-ALL, shortest possible refresh period
  EMC_DYN_CTRL = 0x00000103;
  EMC_DYN_RFSH = 0x00000002;
    
  //wait 128 ABH clock cycles
  for(i=0; i<0x40; i++)
    asm volatile(" nop");
    
  //Set correct refresh period
  EMC_DYN_RFSH = 28;
    
  //Send command: MODE
  EMC_DYN_CTRL = 0x00000083;
    
  //Set mode register in SDRAM
#ifdef CONFIG_LPC_USE_32_BIT_DATABUS
  dummy = *((volatile unsigned int*)(SDRAM_BASE_ADDR | (0x22 << 11)));
#else
  dummy = *((volatile unsigned int*)(SDRAM_BASE_ADDR | (0x33 << 12)));
#endif

  //Send command: NORMAL
  EMC_DYN_CTRL = 0x00000000;

  //Enable buffer
  EMC_DYN_CFG0 |= 0x00080000;

  //initial system delay
  delayMs(1);

  EMC_STA_WAITWEN0  = 0x2;
  EMC_STA_WAITOEN0  = 0x2;
  EMC_STA_WAITRD0   = 0x1f;
  EMC_STA_WAITPAGE0 = 0x1f;
  EMC_STA_WAITWR0   = 0x1f;
  EMC_STA_WAITTURN0 = 0xf;
  EMC_STA_CFG0      = 0x00000081;

  EMC_STA_WAITWEN1  = 0x2;
  EMC_STA_WAITOEN1  = 0x2;
  EMC_STA_WAITRD1   = 0x8;
  EMC_STA_WAITPAGE1 = 0x1f;
  EMC_STA_WAITWR1   = 0x8;
  EMC_STA_WAITTURN1 = 0xf;
  EMC_STA_CFG1      = 0x00000080;

}



/*****************************************************************************
 *
 * Description:
 *    Initialize system functions and GPIO
 *
 ****************************************************************************/
void
lowlevel_init(void)
{
  /********************************************************************************************
   * Remap vectors for RAM execution
   ********************************************************************************************/
  MEMMAP = 1;

#ifndef CONFIG_SKIP_LOWLEVEL_INIT
	/* move vectors to beginning of SRAM */
	asm volatile("  mov	r2, #0x40000000		\n"
	             "  mov	r0, #0x00000000   \n"
	             "  ldmneia r0!, {r3-r10}	\n"
	             "  stmneia r2!, {r3-r10} \n"
	             "  ldmneia r0, {r3-r9}   \n"
	             "  stmneia r2, {r3-r9}   \n" : : : 
		     "r0","r2","r3","r4","r5","r6","r7","r8","r9","r10");
#else
  /* move vectors to beginning of SRAM */
  asm volatile("  mov r2, #0x40000000   \n"
               "  mov r0, #0xa1000000   \n"
               "  ldmneia r0!, {r3-r10} \n"
               "  stmneia r2!, {r3-r10} \n"
               "  ldmneia r0, {r3-r9}   \n"
               "  stmneia r2, {r3-r9}   \n" : : :
         "r0", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10");
#endif

  //initialize the exception vector mapping
  MEMMAP = MEM_MAP;

	
#if USE_USB
	PCONP |= 0x80000000;		/* Turn On USB PCLK */
#endif

  ConfigurePLL();

  /* Set system timers for each component */
#if (Fpclk / (Fcclk / 4)) == 1
    PCLKSEL0 = 0x00000000;	/* PCLK is 1/4 CCLK */
    PCLKSEL1 = 0x00000000;
#endif
#if (Fpclk / (Fcclk / 4)) == 2
    PCLKSEL0 = 0xAAAAAAAA;	/* PCLK is 1/2 CCLK */
    PCLKSEL1 = 0xAAAAAAAA;	 
#endif
#if (Fpclk / (Fcclk / 4)) == 4
    PCLKSEL0 = 0x55555555;	/* PCLK is the same as CCLK */
    PCLKSEL1 = 0x55555555;	
#endif

  /* Set memory accelerater module*/
  MAMCR  = 0;
  MAMTIM = 4;
//  MAMCR  = 2;

  //Init GPIO
  GPIOinit();

  //initialize VIC
  VICinit();

  //initialize UART #0 to 38400, 8N1
  UART0init();

#ifndef CONFIG_LPC_USE_32_BIT_DATABUS
  //initialize UART #1
  UART1init();
#endif

  UART2init();

  //short delay
  delayMs(10);

  //print startup banner
  printBanner();



  /********************************************************************************************
   * Initialize external memory interface (EMC)
   ********************************************************************************************/
#ifndef CONFIG_SKIP_LOWLEVEL_INIT
  ConfigureEMC();
#endif

  //print final chars
  printEndBanner();

}


