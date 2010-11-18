/**
 * @defgroup lcd
 * @file
 * @brief LCD TFT driver
 *
 *   lcd_driver.c:  LCD module file for NXP LPC24xx Family 
 *   Microprocessors
 *
 *   Copyright(C) 2008, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2008.07.29  ver 1.00    Prelimnary version, first Release
 *
 ******************************************************************************/

#include <lpc2468.h>
#include <config.h>
#include "type.h"
#include "lpc_lcd_params.h"
#include "lcd_driver.h"
#include "spi.h"

/***********************************************************************
 * LCD driver private data and types
 **********************************************************************/

/* LCD device configuration structure type */
typedef struct
{
	BOOL init; /* Device initialized flag */
	CLCDC_REGS_T *regptr; /* Pointer to LCD registers */
	LCD_PARAM_T *dptr; /* Pointer to LCD controller settings */
} CLCDC_CFG_T;

static unsigned short (*spifunc)(unsigned short c);
/***********************************************************************
 * Forward declarations
 **********************************************************************/
void lcd_display_init(void);
void lcd_configure_gpio(void);

/* LCD device configuration structure */
static CLCDC_CFG_T lcdcfg;

#define ACTIVATE_CS   IOCLR0 = 0x10000
#define DEACTIVATE_CS IOSET0 = 0x10000
#define SET_RS        IOSET0 = 0x80000
#define RESET_RS      IOCLR0 = 0x80000

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
void mdelay(unsigned short delayInMs)
{
	/*
	 * setup timer #1 for delay
	 */
	T1TCR = 0x02; //stop and reset timer
	T1PR = 0x00; //set prescaler to zero
	T1MR0 = delayInMs * (Fpclk / 1000);
	T1IR = 0xff; //reset all interrrupt flags
	T1MCR = 0x04; //stop timer on match
	T1TCR = 0x01; //start timer

	//wait until delay time has elapsed
	while (T1TCR & 0x01)
		;
}
/******************************************************************************
 *
 * Copyright:
 *    (C) 2000 - 2008 Embedded Artists AB
 *
 *****************************************************************************/
/*****************************************************************************
 *
 * Description:
 *    Send data on the SPI interface.
 *
 * Params:
 *    [in] data - data to send
 *
 ****************************************************************************/
void spiSend(BYTE data)
{
	DWORD failsafe;

	S0SPDR = data;
	failsafe = 0;
	while (((S0SPSR & 0x80) == 0) && (failsafe < 5000))
		failsafe++;

	if (failsafe >= 5000)
	{
		S0SPCCR = 8;
		S0SPCR = 0x38;
	}
}

/******************************************************************************
 *
 * Copyright:
 *    (C) 2000 - 2008 Embedded Artists AB
 *
 *****************************************************************************/
/*****************************************************************************
 *
 * Description:
 *    Write to display register
 *
 * Params:
 *    [in] addr - register address
 *    [in] data - data to write
 *
 ****************************************************************************/
void writeToReg(WORD addr, WORD data)
{
	RESET_RS;
	ACTIVATE_CS;
	spifunc(0);
	spifunc(addr & 0xff);
	DEACTIVATE_CS;

	SET_RS;
	ACTIVATE_CS;
	spifunc(data >> 8);
	spifunc(data & 0xff);
	DEACTIVATE_CS;

	RESET_RS;
	ACTIVATE_CS;
	spifunc(0);
	spifunc(0x22);
	DEACTIVATE_CS;
}

/******************************************************************************
 *
 * Copyright:
 *    (C) 2000 - 2008 Embedded Artists AB
 *
 *****************************************************************************/
/*****************************************************************************
 *
 * Description:
 *    Initialize the Truly LCD display module
 *
 ****************************************************************************/
void lcd_display_init(void)
{
	writeToReg(0x00, 0x0001);
	//  delayMs(1, 20);
	mdelay(20);
	writeToReg(0x03, 0xA2A4);
	writeToReg(0x0C, 0x0004);
	writeToReg(0x0D, 0x0308);
	writeToReg(0x0E, 0x3000);
	//  delayMs(1, 50);
	mdelay(50);
	writeToReg(0x1E, 0x00AF);
	writeToReg(0x01, 0x2B3F);

	writeToReg(0x02, 0x0600);
	writeToReg(0x10, 0x0000);
	writeToReg(0x07, 0x0233);
	writeToReg(0x0B, 0x0039);
	writeToReg(0x0F, 0x0000);
	//  delayMs(1, 50);
	mdelay(50);
	writeToReg(0x30, 0x0707);
	writeToReg(0x31, 0x0204);
	writeToReg(0x32, 0x0204);
	writeToReg(0x33, 0x0502);
	writeToReg(0x34, 0x0507);
	writeToReg(0x35, 0x0204);
	writeToReg(0x36, 0x0204);
	writeToReg(0x37, 0x0502);
	writeToReg(0x3A, 0x0302);
	writeToReg(0x3B, 0x0302);

	writeToReg(0x23, 0x0000);
	writeToReg(0x24, 0x0000);

	writeToReg(0x48, 0x0000);
	writeToReg(0x49, 0x013F);
	writeToReg(0x4A, 0x0000);
	writeToReg(0x4B, 0x0000);

	writeToReg(0x41, 0x0000);
	writeToReg(0x42, 0x0000);

	writeToReg(0x44, 0xEF00);
	writeToReg(0x45, 0x0000);
	writeToReg(0x46, 0x013F);
	//  delayMs(1, 50);
	mdelay(50);
	writeToReg(0x44, 0xEF00);
	writeToReg(0x45, 0x0000);
	writeToReg(0x4E, 0x0000);
	writeToReg(0x4F, 0x0000);
	writeToReg(0x46, 0x013F);
}

/***********************************************************************
 *
 * Function: lcd_configure_gpio
 *
 * Purpose: Configure the GPIO muxing for LCD based on LCD type
 *
 * Processing:
 *
 * Parameters: 	None
 *
 * Outputs: None
 *
 * Returns: None
 *
 **********************************************************************/
void lcd_configure_gpio(void)
{
	FIO2DIR |= 0x00000001;
	FIO2SET = 0x00000001;

	/* PWM1.1 output is LCD backlight control for EA2478 board */
	PINSEL3 = (PINSEL3 & 0xFFFFFFCF) | 0x00000020;
	PWM1PR = 0x00; //no prescaling
	PWM1MCR = 0x02; //reset counter if MR0 match
	PWM1MR0 = 0x3000; //period time equal about 5 ms
	PWM1MR1 = 0x0000;
	PWM1LER = 0x03; //latch MR0 and MR1
	PWM1PCR = 0x0200; //enable PWMENA1
	PWM1TCR = 0x09; //enable counter and PWM

	PINSEL0 = (PINSEL0 & 0x3FFFFFFF) | 0xC0000000;
	PINSEL1 = (PINSEL1 & 0xFFFFFFC3) | 0x0000003C;

	IODIR0 |= 0x90000;
	IOSET0 = 0x90000;

	//  S0SPCCR = 8;
	//  S0SPCR  = 0x38;

	PINSEL10 = 0x0; /* disable ETM interface */

	PINSEL11 = 0x0000000D; /* TFT 16bpp (1:5:5:5 mode) */

	PINSEL0 = (PINSEL0 & 0xFFF000FF) | 0x00055500;

	PINSEL3 = (PINSEL3 & 0xF00000FF) | 0x05555500; /* enable LCD6_LCD10, LCD7_LCD11, LCD8_LCD12, LCD9_LCD13
	 LCD10_LCD14, LCD11_LCD15, LCD12_LCD20, LCD13_LCD21
	 LCD14_LCD22, LCD15_LCD23 */

	PINSEL4 = (PINSEL4 & 0xF0300000) | 0x054FFFFF; /* enable LCDPWR, LCDCP, LCDFP, LCDAC, LCDLP
	 LCD0_LCD4, LCD1_LCD5, LCD2_LCD6, LCD3_LCD7
	 LCD4_LCD3_LCD8_LCD18, LCD5_LCD9_LCD19 */

	PINSEL9 = (PINSEL9 & 0xF0FFFFFF) | 0x0A000000; /* enable LCD6_LCD10_LCD2, LCD7_LCD11_LCD3 */
}

/***********************************************************************
 *
 * Function: lcd_initialize
 *
 * Purpose: Initialize the LCD controller
 *
 * Processing:
 *     Prior to resetting LCD values, disable the LCD controller.
 *     Configurate the LCD registers with the values in the passed 
 *     LCD_PARAM_T structure.
 *
 * Parameters:
 *     lcdcfgptr : A pointer to an LCD configuration data structure
 *
 * Outputs: None
 *
 * Returns: _NO_ERROR if the LCD was initialized, otherwise _ERROR
 *
 **********************************************************************/
BOOL lcd_initialize(CLCDC_CFG_T *lcdcfgptr)
{
	DWORD tmp, i;
	long status = _NO_ERROR;
	CLCDC_REGS_T *lcdptr = lcdcfgptr->regptr;
	LCD_PARAM_T *cgdatptr = lcdcfgptr->dptr;

	/* Power Control for CLCDC peripheral */
	PCONP |= 0x00100000;

	/* Disable the display in case it is on */
	lcdptr->lcdctrl &= ~CLCDC_LCDCTRL_ENABLE;

	/* Generate the horizontal axis plane control word */
	tmp = (CLCDC_LCDTIMING0_PPL(cgdatptr->pixels_per_line) | CLCDC_LCDTIMING0_HSW(cgdatptr->h_sync_pulse_width)
			| CLCDC_LCDTIMING0_HFP(cgdatptr->h_front_porch) | CLCDC_LCDTIMING0_HBP(cgdatptr->h_back_porch));
	lcdptr->lcdtiming0 = tmp;

	/* Generate the vertical axis plane control word */
	tmp = (CLCDC_LCDTIMING1_LPP(cgdatptr->lines_per_panel) | CLCDC_LCDTIMING1_VSW(cgdatptr->v_sync_pulse_width)
			| CLCDC_LCDTIMING1_VFP(cgdatptr->v_front_porch) | CLCDC_LCDTIMING1_VBP(cgdatptr->v_back_porch));
	lcdptr->lcdtiming1 = tmp;

	/* Generate the clock and signal polarity control word */
	if (cgdatptr->ac_bias_frequency != 0)
	{
		/* STN panel has AC bias value */
		tmp = CLCDC_LCDTIMING2_ACB(cgdatptr->ac_bias_frequency);
	}
	else
	{
		tmp = 0;
	}

	if (cgdatptr->invert_output_enable != 0)
	{
		tmp |= CLCDC_LCDTIMING2_IOE;
	}

	if (cgdatptr->invert_panel_clock != 0)
	{
		tmp |= CLCDC_LCDTIMING2_IPC;
	}

	if (cgdatptr->invert_hsync != 0)
	{
		tmp |= CLCDC_LCDTIMING2_IHS;
	}

	if (cgdatptr->invert_vsync != 0)
	{
		tmp |= CLCDC_LCDTIMING2_IVS;
	}

	/* Compute clocks per line based on panel type */
	switch (cgdatptr->lcd_panel_type)
	{
	case MONO_4BIT:
		/* Clocks per line is a quarter of pixels per line */
		tmp = tmp | CLCDC_LCDTIMING2_CPL((cgdatptr->pixels_per_line / 4) - 1);
		break;

	case MONO_8BIT:
		/* Clocks per line is an eighth of pixels per line */
		tmp = tmp | CLCDC_LCDTIMING2_CPL((cgdatptr->pixels_per_line / 8) - 1);
		break;

	case CSTN:
		/* CSTN Clocks per line (* 3 / 8) */
		tmp = tmp | CLCDC_LCDTIMING2_CPL(((cgdatptr->pixels_per_line * 3) / 8) - 1);
		break;

	case TFT:
	case ADTFT:
	case HRTFT:
	default:
		/* Clocks per line and pixels per line are the same */
		tmp = tmp | CLCDC_LCDTIMING2_CPL(cgdatptr->pixels_per_line - 1);
		break;
	}

	/* Bypass pixel clock divider */
	tmp = tmp | CLCDC_LCDTIMING2_PCD_LO(8);

	lcdptr->lcdtiming2 = tmp;

	/* Skip line end control word - just set to 0x0 */
	lcdptr->lcdtiming3 = 0x00000000;

	/* Default with all interrupts of */
	lcdptr->lcdimsc = 0x00000000;

	/* Default configuration is 16 bits per pixel with blue and blue not swapped */
	tmp = CLCDC_LCDCTRL_BPP16;

	/* blue and blue swapped */
	tmp |= CLCDC_LCDCTRL_BGR;

	switch (cgdatptr->lcd_panel_type)
	{
	case ADTFT:
	case HRTFT:
	case TFT:
		tmp |= CLCDC_LCDCTRL_TFT;
		break;

	case MONO_4BIT:
		tmp |= CLCDC_LCDCTRL_BW_MONO;
		break;

	case MONO_8BIT:
		tmp |= (CLCDC_LCDCTRL_MON8 | CLCDC_LCDCTRL_BW_MONO);
		break;

	case CSTN:
		;
		break;

	default:
		/* Unsupported panel type */
		status = _ERROR;
		break;
	}

	/* Dual panel operation */
	if (cgdatptr->dual_panel == 1)
	{
		tmp |= CLCDC_LCDCTRL_DUAL;
	}

	lcdptr->lcdctrl = tmp;

	/* clear the palette (color is black )*/
	for (i = 0; i < sizeof(lcdptr->lcdpalette) / sizeof(lcdptr->lcdpalette[0]); i++)
	{
		lcdptr->lcdpalette[i] = 0;
	}

	LCD_CFG = 0x0;

	return status;
}

/***********************************************************************
 *
 * Function: lcd_open
 *
 * Purpose: Open the LCD controller
 *
 * Processing:
 *     If init is not FALSE, return 0x00000000.
 *     Otherwise, return a pointer to the LCD configuration data structure.
 *
 * Parameters:
 *     ipbase: LCD descriptor device address
 *     arg   : LCD type - LCD_PARAM_T
 *
 * Outputs: None
 *
 * Returns: The pointer to a LCD config structure or 0
 *
 **********************************************************************/
long lcd_open(void *lcdbase, long arg)
{
	long status = 0;

	if ((lcdcfg.init == FALSE) && ((CLCDC_REGS_T *) lcdbase == CLCDC))
	{
		/* Device is valid and not previously initialized */
		lcdcfg.init = TRUE;

		/* Save and return address of peripheral block */
		lcdcfg.regptr = (CLCDC_REGS_T *) lcdbase;
		lcdcfg.dptr = (LCD_PARAM_T *) arg;

		/* Disable LCD */
		lcdcfg.regptr->lcdctrl &= ~CLCDC_LCDCTRL_ENABLE;

		/* If the passed argument is not NULL, then it is a pointer
		 to a LCD panel parameters structure that needs to be configured */
		if (arg != 0)
		{
			lcd_initialize(&lcdcfg);
			lcd_configure_gpio();

		}

		/* Return pointer to LCD configuration data structure */
		status = (long) &lcdcfg;
	}
	return status;
}

/***********************************************************************
 *
 * Function: lcd_close
 *
 * Purpose: Close the LCD controller
 *
 * Processing:
 *     If init is not TRUE, then return _ERROR to the caller as the
 *     device was not previously opened. Otherwise, disable the LCD,
 *     set init to FALSE, and return _NO_ERROR to the caller.
 *
 * Parameters:
 *     devid: Pointer to LCD config structure
 *
 * Outputs: None
 *
 * Returns: The status of the close operation
 *
 **********************************************************************/
long lcd_close(long devid)
{
	CLCDC_CFG_T *lcdcfgptr = (CLCDC_CFG_T *) devid;
	long status = _ERROR;

	if (lcdcfgptr->init == TRUE)
	{
		/* 'Uninitialize' device */
		lcdcfgptr->init = FALSE;
		status = _NO_ERROR;
	}
	return status;
}

/***********************************************************************
 *
 * Function: lcd_ioctl
 *
 * Purpose: LCD configuration block
 *
 * Processing:
 *     This function is a large case block. Based on the passed function
 *     and option values, set or get the appropriate LCD
 *     parameter.
 *
 * Parameters:
 *     devid: Pointer to LCD config structure
 *     cmd:   ioctl command
 *     arg:   ioctl argument
 *
 * Outputs: None
 *
 * Returns: The status of the ioctl operation
 *
 **********************************************************************/
long lcd_ioctl(long devid, long cmd, long arg)
{
	CLCDC_REGS_T *lcd;
	DWORD i;
	CLCDC_CFG_T *lcdcfgptr = (CLCDC_CFG_T *) devid;
	long status = _ERROR;

	PALETTE_ENTRY_T pal_entry, *ptr_pal_entry;
	BYTE *pal_ptr;
	DWORD *crsr_ptr;

	if (lcdcfgptr->init == TRUE)
	{
		status = _NO_ERROR;
		lcd = lcdcfgptr->regptr;

		switch (cmd)
		{
		case LCD_SET_BUFFER:
			/* Set LCD display frame buffer, arg = memory pointer for
			 image to be displayed */
			lcd->lcdupbase = (DWORD) arg;
			break;

		case LCD_SET_SPIFUNC:
			/* assign spi function */
			spifunc = arg;
			break;

		case LCD_CRSR_SET_BUFFER:
			/* Set LCD cursor image, arg = memory pointer for
			 image to be displayed */
			crsr_ptr = (DWORD *) arg;
			for (i = 0; i < 256; i++)
			{
				lcd->cursorimage[i] = *(DWORD *) crsr_ptr;
				crsr_ptr++;
			}
			break;

		case LCD_PWR_ON:
			/* Turn on the LCD controller power,
			 arg = 1, turn on, arg = 0, turn off */
			if (arg == 1)
			{
				lcd->lcdctrl |= CLCDC_LCDCTRL_ENABLE;
				lcd->lcdctrl |= CLCDC_LCDCTRL_PWR;
				lcd_display_init();
			}
			else if (arg == 0)
			{
				lcd->lcdctrl &= ~CLCDC_LCDCTRL_PWR;
				lcd->lcdctrl &= ~CLCDC_LCDCTRL_ENABLE;
			}
			else
			{
				status = _ERROR;
			}
			break;

		case LCD_ENABLE:
			/* Enable the LCD controller,
			 arg = 1, enable, arg = 0, disable */
			if (arg == 1)
			{
				lcd->lcdctrl |= CLCDC_LCDCTRL_ENABLE;
			}
			else if (arg == 0)
			{
				lcd->lcdctrl &= ~CLCDC_LCDCTRL_ENABLE;
			}
			else
			{
				status = _ERROR;
			}
			break;

		case LCD_CRSR_ENABLE:
			/* Enable the LCD cursor,
			 arg = 1, enable, arg = 0, disable */
			if (arg == 1)
			{
				lcd->clcdcrsrctrl |= 1;
			}
			else if (arg == 0)
			{
				lcd->clcdcrsrctrl &= ~1;
			}
			else
			{
				status = _ERROR;
			}
			break;

		case LCD_CRSR_NUM:
			/* Select the LCD cursor number,
			 arg = 0, 64x64, arg = 0, 1, 2, 3, 32x32 */
			lcd->clcdcrsrctrl &= ~30;
			lcd->clcdcrsrctrl |= (DWORD) arg << 3;
			break;

		case LCD_CRSR_SIZE:
			/* Set the LCD cursor size,
			 arg = 1, 64x64, arg = 0, 32x32 */
			if (arg == 1)
			{
				lcd->clcdcrsrconfig |= 1;
			}
			else if (arg == 0)
			{
				lcd->clcdcrsrconfig &= ~1;
			}
			else
			{
				status = _ERROR;
			}
			break;

		case LCD_CRSR_SYNC:
			/* Set the LCD cursor frame sync,
			 arg = 1, sync, arg = 0, async */
			if (arg == 1)
			{
				lcd->clcdcrsrconfig |= 2;
			}
			else if (arg == 0)
			{
				lcd->clcdcrsrconfig &= ~2;
			}
			else
			{
				status = _ERROR;
			}
			break;

		case LCD_LOAD_PALETTE:
			/* This function supports loading of the color palette from
			 the C file generated by the bmp2c utility. It expects the
			 palette to be passed as an array of 32-bit BGR entries having
			 the following format:
			 7:3 - Blue
			 2:0 - Not used
			 15:11 - Green
			 10:8 - Not used
			 23:19 - Red
			 18:16 - Not used
			 31:24 - Not used
			 arg = pointer to input palette table address */
			ptr_pal_entry = &pal_entry;
			pal_ptr = (BYTE *) arg;

			/* 256 entry in the palette table */
			for (i = 0; i < 256 / 2; i++)
			{
				pal_entry.Bl = (*pal_ptr++) >> 3; /* blue first */
				pal_entry.Gl = (*pal_ptr++) >> 3; /* get green */
				pal_entry.Rl = (*pal_ptr++) >> 3; /* get red */
				pal_ptr++; /* skip over the unused byte */
				/* do the most significant halfword of the palette */
				pal_entry.Bu = (*pal_ptr++) >> 3; /* blue first */
				pal_entry.Gu = (*pal_ptr++) >> 3; /* get green */
				pal_entry.Ru = (*pal_ptr++) >> 3; /* get red */
				pal_ptr++; /* skip over the unused byte */

				lcd->lcdpalette[i] = *(DWORD *) ptr_pal_entry;
			}
			break;

		case LCD_CRSR_LOAD_PALETTE0:
			/* 7:0 - Red
			 15:8 - Green
			 23:16 - Blue
			 31:24 - Not used*/
			lcd->clcdcrsrpalette0 = (DWORD) arg;
			break;

		case LCD_CRSR_LOAD_PALETTE1:
			/* 7:0 - Red
			 15:8 - Green
			 23:16 - Blue
			 31:24 - Not used*/
			lcd->clcdcrsrpalette1 = (DWORD) arg;
			break;

		case LCD_SET_BPP:
			/* Set current display bits per pixel, arg = bits per pixel */
			switch (arg)
			{
			case BPP_IS_1:
				lcd->lcdctrl &= ~_SBF(1, _BITMASK(3));
				lcd->lcdctrl |= CLCDC_LCDCTRL_BPP1;
				break;

			case BPP_IS_2:
				lcd->lcdctrl &= ~_SBF(1, _BITMASK(3));
				lcd->lcdctrl |= CLCDC_LCDCTRL_BPP2;
				break;

			case BPP_IS_4:
				lcd->lcdctrl &= ~_SBF(1, _BITMASK(3));
				lcd->lcdctrl |= CLCDC_LCDCTRL_BPP4;
				break;

			case BPP_IS_8:
				lcd->lcdctrl &= ~_SBF(1, _BITMASK(3));
				lcd->lcdctrl |= CLCDC_LCDCTRL_BPP8;
				break;

			case BPP_IS_16:
				lcd->lcdctrl &= ~_SBF(1, _BITMASK(3));
				lcd->lcdctrl |= CLCDC_LCDCTRL_BPP16;
				break;

			case BPP_IS_24:
				lcd->lcdctrl &= ~_SBF(1, _BITMASK(3));
				lcd->lcdctrl |= CLCDC_LCDCTRL_BPP24;
				break;

			case BPP_IS_16_565_MODE:
				lcd->lcdctrl &= ~_SBF(1, _BITMASK(3));
				lcd->lcdctrl |= CLCDC_LCDCTRL_BPP16_565_MODE;
				break;

			case BPP_IS_12_444_MODE:
				lcd->lcdctrl &= ~_SBF(1, _BITMASK(3));
				lcd->lcdctrl |= CLCDC_LCDCTRL_BPP12_444_MODE;
				break;

			default:
				status = _ERROR;
				break;
			}
			break;

		case LCD_SET_BGR:
			/* LCD invert the bgr bit in the LCD controller. arg = 1, invert
			 BGR for reverse color, arg = 0, BGR for normal color */
			if (arg == 1)
			{
				/*
				 *   Palette and direct color storage is
				 *   blue in bits 4-0,
				 *   green in bits 9-5,
				 *   red in bits 14-10,
				 *   intensity in bit 15.
				 */
				lcd->lcdctrl |= CLCDC_LCDCTRL_BGR;
			}
			else if (arg == 0)
			{
				/*
				 *   Palette and direct color storage is
				 *   red in bits 4-0,
				 *   green in bits 9-5,
				 *   blue in bits 14-10,
				 *   intensity in bit 15.
				 */
				lcd->lcdctrl &= ~CLCDC_LCDCTRL_BGR;
			}
			else
			{
				status = _ERROR;
			}
			break;

		case LCD_SET_INTERRUPT:
			/* Set interrupt mask set/clear register */
			if (arg == CLCDC_LCD_INTERRUPT_FUF)
			{
				lcd->lcdimsc |= CLCDC_LCD_INTERRUPT_FUF;
			}
			else if (arg == CLCDC_LCD_INTERRUPT_LNBU)
			{
				lcd->lcdimsc |= CLCDC_LCD_INTERRUPT_LNBU;
			}
			else if (arg == CLCDC_LCD_INTERRUPT_VCOMP)
			{
				lcd->lcdimsc |= CLCDC_LCD_INTERRUPT_VCOMP;
			}
			else if (arg == CLCDC_LCD_INTERRUPT_MBERROR)
			{
				lcd->lcdimsc |= CLCDC_LCD_INTERRUPT_MBERROR;
			}
			else
			{
				status = _ERROR;
			}
			break;

		case LCD_CLR_INTERRUPT:
			/* Set interrupt clear register*/
			if (arg == CLCDC_LCD_INTERRUPT_FUF)
			{
				lcd->lcdicr |= CLCDC_LCD_INTERRUPT_FUF;
			}
			else if (arg == CLCDC_LCD_INTERRUPT_LNBU)
			{
				lcd->lcdicr |= CLCDC_LCD_INTERRUPT_LNBU;
			}
			else if (arg == CLCDC_LCD_INTERRUPT_VCOMP)
			{
				lcd->lcdicr |= CLCDC_LCD_INTERRUPT_VCOMP;
			}
			else if (arg == CLCDC_LCD_INTERRUPT_MBERROR)
			{
				lcd->lcdicr |= CLCDC_LCD_INTERRUPT_MBERROR;
			}
			else
			{
				status = _ERROR;
			}
			break;

		case LCD_CRSR_SET_INTERRUPT:
			/* Set cursor interrupt mask set/clear register */
			lcd->lcdcrsrimsc |= 1;
			break;

		case LCD_CRSR_CLR_INTERRUPT:
			/* Set cursor interrupt clear register*/
			lcd->lcdcrsricr |= 1;
			break;

		case LCD_CRSR_XY:
			/* Set lcd cursor xy position register*/
			lcd->clcdcrsrxy = (DWORD) arg;
			break;

		case LCD_CRSR_CLIP:
			/* Set lcd cursor clip position register*/
			lcd->clcdcrsrclip = (DWORD) arg;
			break;

		case LCD_GET_STATUS:
			switch (arg)
			{
			case LCD_GET_BUFFER:
				/* Returns LCD fram buffer address */
				status = (long) lcd->lcdupbase;
				break;

			case LCD_GET_BPP:
				/* Returns current LCD panel bit per pixel */
				i = (lcd->lcdctrl) & _SBF(1,_BITMASK(3) );
				switch (i)
				{
				case CLCDC_LCDCTRL_BPP1:
					status = BPP_IS_1;
					break;

				case CLCDC_LCDCTRL_BPP2:
					status = BPP_IS_2;
					break;

				case CLCDC_LCDCTRL_BPP4:
					status = BPP_IS_4;
					break;

				case CLCDC_LCDCTRL_BPP8:
					status = BPP_IS_8;
					break;

				case CLCDC_LCDCTRL_BPP16:
					status = BPP_IS_16;
					break;

				case CLCDC_LCDCTRL_BPP24:
					status = BPP_IS_24;
					break;

				case CLCDC_LCDCTRL_BPP16_565_MODE:
					status = BPP_IS_16_565_MODE;
					break;

				case CLCDC_LCDCTRL_BPP12_444_MODE:
					status = BPP_IS_12_444_MODE;
					break;

				default:
					status = _ERROR;
					break;
				}
				break;

			case LCD_GET_PALETTE:
				/* Returns a pointer to palette table */
				status = (long) lcd->lcdpalette;
				break;

			case LCD_CRSR_GET_PALETTE0:
				/* Returns a pointer to cursor palette table */
				status = (long) lcd->clcdcrsrpalette0;
				break;

			case LCD_CRSR_GET_PALETTE1:
				/* Returns a pointer to cursor palette table */
				status = (long) lcd->clcdcrsrpalette1;
				break;

			case LCD_GET_INTERRUPT:
				/* Get interrupt mask sstatus register */
				status = (long) lcd->lcdmis;
				break;

			case LCD_CRSR_GET_INTERRUPT:
				/* Get cursor interrupt mask sstatus register */
				status = (long) lcd->lcdcrsrmis;
				break;

			default:
				/* Unsupported parameter */
				status = _ERROR;
				break;
			}
			break;

		default:
			/* Unsupported parameter */
			status = _ERROR;
			break;
		}
	}
	return status;
}
