/*
 * (C) Copyright 2008
 * Embedded Artists AB, Sweden <www.EmbeddedArtists.com>
 *
 * Configuation settings for the LPC2468 OEM Board, 32 bit databus.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
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

#ifndef _LPC2468OEM_BOARD_32BIT_H
#define _LPC2468OEM_BOARD_32BIT_H

#include "LPC24xxOEM_Board_common.h"

#define CONFIG_LPC2468
#define CONFIG_LPC_USE_32_BIT_DATABUS

/* Clock settings */

/* Crystal frequency */
#define CONFIG_FOSC 12000000

/*
 * Fcco = 2*M*Fosc / N
 *
 * Fcco = 288000000 -> M = 12, N = 1
 *
 * PLLCFG (MSEL) = (M-1)
 * PLLCFG (NSEL) = (N-1)
 */
#define CONFIG_PLL_MVALUE 12
#define CONFIG_PLL_NVALUE  1

/*
 * Fcclk = Fcco / CLKDIV
 * CLKDIV must be an even number
 *
 * CCLKCFG = CLKDIV-1 (odd number must be written to register)
 * CLKDIV = 4 -> Fcclk = 72 MHz (if Fcco = 288 MHz)
 * CLKDIV = 6 -> Fcclk = 48 MHz (if Fcco = 288 MHz)
 */
#define CONFIG_PLL_CLKDIV 6

/*
 * The USB clock must be 48 MHz
 * Fusb = Fcco / USBCLKDIV
 * USBCLKCFG = (USBCLKDIV-1)
 */
#define CONFIG_PLL_USBCLKDIV 6 

/*
 * Periperhal clock divider, i.e. Fpclk = Fcclk / divider
 * Valid values are 1, 2, or 4
 */
#define CONFIG_FPCLK_DIV 1

/* Monitor Command Prompt */
#define	CFG_PROMPT		"LPC2468_OEM_Board # " 

#endif	/* _LPC2468OEM_BOARD_32BIT_H */
