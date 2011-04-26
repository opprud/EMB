/******************************************************************************
 *
 * Copyright:
 *    (C) 2000 - 2005 Embedded Artists AB
 *
 * Description:
 *    Framework for ARM7 processor
 *
 *****************************************************************************/
#ifndef _config_h_
#define _config_h_


/******************************************************************************
 * Defines, macros, and typedefs
 *****************************************************************************/
#define	USE_USB					1

#if USE_USB		/* 1 is USB, 0 is non-USB related */  
/* Fcck = 57.6Mhz, Fosc = 288Mhz, and USB 48Mhz */
#define PLL_MValue			11
#define PLL_NValue			0
#define CCLKDivValue		4
#define USBCLKDivValue		5

/* System configuration: Fosc, Fcclk, Fcco, Fpclk must be defined */
/* PLL input Crystal frequence range 4KHz~20MHz. */
#define Fosc	12000000
/* System frequence,should be less than 80MHz. */
#define Fcclk	57600000
#define Fcco	288000000
#else

/* Fcck = 50Mhz, Fosc = 300Mhz, and USB 48Mhz */
#define PLL_MValue			24
#define PLL_NValue			1
#define CCLKDivValue		5
#define USBCLKDivValue		6

/* System configuration: Fosc, Fcclk, Fcco, Fpclk must be defined */
/* PLL input Crystal frequence range 4KHz~20MHz. */
#define Fosc	12000000
/* System frequence,should be less than 80MHz. */
#define Fcclk	50000000
#define Fcco	300000000

#endif

/* APB clock frequence , must be 1/2/4 multiples of ( Fcclk/4 ). */
/* If USB is enabled, the minimum APB must be greater than 16Mhz */ 
#if USE_USB
#define Fpclk	(Fcclk / 2)
#else
#define Fpclk	(Fcclk / 4)
#endif

#define MAM_SETTING  2                /* 0=disabled,
                                         1=partly enabled (enabled for code prefetch, but not for data),
                                         2=fully enabled */

#define IRQ_HANDLER  1                /* 0 = Jump to common IRQ handler
                                         1 = Load vector directly from VIC, i.e., LDR PC,[PC,#-0xFF0] */

/* initialize the exception vector mapping */
#ifndef RAM_EXEC
#define MAM_MAP      1                  /* 1 = exception vectors are in FLASH at 0x0000 0000,
                                           2 = exception vectors are in SRAM at 0x4000 0000   */
#else
#define MAM_MAP 2                       /* When executing from RAM, MAM_MAP should always be 2 */
#endif


#define SRAM_SIZE  (64 * 1024)   /* LPC2468 */

#define SRAM_SADDR   0x40000000               /* SRAM starting address */
#define SRAM_TOP     (SRAM_SADDR+SRAM_SIZE)   /* SRAM end address + 1 */
#define SRAM_EADDR   (SRAM_SADDR+SRAM_SIZE-1) /* SRAM end address */

#define STK_SIZE     (stackSize_SYS+stackSize_SVC+stackSize_UND+stackSize_ABT+stackSize_IRQ+stackSize_FIQ)
#define STK_SADDR    (SRAM_EADDR+1-STK_SIZE)  /* Stack start address */

#endif  /* _config_h_ */
