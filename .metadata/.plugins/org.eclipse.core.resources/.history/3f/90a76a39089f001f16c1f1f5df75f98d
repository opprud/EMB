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

/* setup stack sizes */
#define stackSize_SYS    600
#define stackSize_SVC     64
#define stackSize_UND     64
#define stackSize_ABT     64
#define stackSize_IRQ   1024
#define stackSize_FIQ     64

/* define consol settings */
#define CONSOL_UART              0
#define CONSOL_BITRATE       38400
/*#define USE_UART_FIFO             FALSE  */      /* Will be added in a future release */
/*#define UART_API_NONBLOCKING      FALSE  */      /* Will be added in a future release */
/*#define UART_API_NONBLOCKING_SIZE   512  */      /* Will be added in a future release */
#define CONSOL_STARTUP_DELAY                       /* Short startup delay in order to remove
                                                      risk for false startbit detection,
                                                      timer #1 will be used in polled mode */
#define CONSOL_STARTUP_DELAY_LENGTH 1000           /* 100 us is slightly more than one character at 115200 bps */


#define USE_NEWLIB           0   /* 0 = do not use newlib (= save about 22k FLASH),
                                    1 = use newlib = full implementation of printf(), scanf(), and malloc() */
#define CONSOLE_API_PRINTF   1   /* 0 = printf() = sendString,
                                    1 = simple, own implementation of printf() */
#define CONSOLE_API_SCANF    1   /* 0 = none,
                                    1 = simple, own implementation of scanf() */

/* define SRAM size */
#ifdef LPC2101
#define SRAM_SIZE  (2 * 1024)   /* LPC2101 */
#elif defined (LPC2102)
#define SRAM_SIZE  (4 * 1024)   /* LPC2102 */
#elif defined (LPC2103)
#define SRAM_SIZE  (8 * 1024)   /* LPC2103 */
#elif defined (LPC2104)
#define SRAM_SIZE  (16 * 1024)   /* LPC2104 */
#elif defined (LPC2105)
#define SRAM_SIZE  (32 * 1024)   /* LPC2105 */
#elif defined (LPC2106)
#define SRAM_SIZE  (64 * 1024)   /* LPC2106 */

#elif defined (LPC2114)
#define SRAM_SIZE  (16 * 1024)   /* LPC2114 */
#elif defined (LPC2124)
#define SRAM_SIZE  (16 * 1024)   /* LPC2124 */
#elif defined (LPC2119)
#define SRAM_SIZE  (16 * 1024)   /* LPC2119 */
#elif defined (LPC2129)
#define SRAM_SIZE  (16 * 1024)   /* LPC2129 */

#elif defined (LPC2131)
#define SRAM_SIZE  (8 * 1024)   /* LPC2131 */
#elif defined (LPC2132)
#define SRAM_SIZE  (16 * 1024)   /* LPC2132 */
#elif defined (LPC2134)
#define SRAM_SIZE  (16 * 1024)   /* LPC2134 */
#elif defined (LPC2136)
#define SRAM_SIZE  (32 * 1024)   /* LPC2136 */
#elif defined (LPC2138)
#define SRAM_SIZE  (32 * 1024)   /* LPC2138 */

#elif defined (LPC2141)
#define SRAM_SIZE  (8 * 1024)   /* LPC2141 */
#elif defined (LPC2142)
#define SRAM_SIZE  (16 * 1024)   /* LPC2142 */
#elif defined (LPC2144)
#define SRAM_SIZE  (16 * 1024)   /* LPC2144 */
#elif defined (LPC2146)
#define SRAM_SIZE  (32 * 1024)   /* LPC2146 */
#elif defined (LPC2148)
#define SRAM_SIZE  (32 * 1024)   /* LPC2148 */

#elif defined (LPC2194)
#define SRAM_SIZE  (16 * 1024)   /* LPC2194 */

#elif defined (LPC2210)
#define SRAM_SIZE  (16 * 1024)   /* LPC2210 */
#elif defined (LPC2220)
#define SRAM_SIZE  (64 * 1024)   /* LPC2220 */
#elif defined (LPC2212)
#define SRAM_SIZE  (16 * 1024)   /* LPC2212 */
#elif defined (LPC2214)
#define SRAM_SIZE  (16 * 1024)   /* LPC2214 */
#elif defined (LPC2290)
#define SRAM_SIZE  (16 * 1024)   /* LPC2290 */
#elif defined (LPC2292)
#define SRAM_SIZE  (16 * 1024)   /* LPC2292 */
#elif defined (LPC2294)
#define SRAM_SIZE  (16 * 1024)   /* LPC2294 */

#elif defined (LPC2468)
#define SRAM_SIZE  (64 * 1024)   /* LPC2468 */

#else
#error CPU_VARIANT not defined in the makefile, or illegal value 
#endif

#define SRAM_SADDR   0x40000000               /* SRAM starting address */
#define SRAM_TOP     (SRAM_SADDR+SRAM_SIZE)   /* SRAM end address + 1 */
#define SRAM_EADDR   (SRAM_SADDR+SRAM_SIZE-1) /* SRAM end address */

#define STK_SIZE     (stackSize_SYS+stackSize_SVC+stackSize_UND+stackSize_ABT+stackSize_IRQ+stackSize_FIQ)
#define STK_SADDR    (SRAM_EADDR+1-STK_SIZE)  /* Stack start address */

#endif  /* _config_h_ */
