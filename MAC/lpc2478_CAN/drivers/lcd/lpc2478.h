/*****************************************************************************
 *   lpc2478.h:  Header file for NXP LPC23xx/24xx Family Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2008.07.29  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/

#ifndef __LPC2478_H
#define __LPC2478_H

#include "..\swim\lpc_types.h"

/* LCD controller base address */
#define LCD_BASE (0xFFE10000)

/* LCD Configuration and clocking control */
#define LCD_CFG (*(volatile unsigned long *)(0xE01FC1B8))

/* Pin Connect Block */
#define PINSEL11 (*(volatile unsigned long *)(0xE002C02C))

#endif /* __LPC2478_H */ 
