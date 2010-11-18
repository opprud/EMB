/**
 * @ingroup lcd
 * @file
 * @brief LCD parameters, for various LCD panels
 *   lcd_params.c:  LCD module file for NXP LPC24xx Family 
 *   Microprocessors
 *
 *   Copyright(C) 2008, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2008.07.29  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/

#include "lpc_lcd_params.h"

/* Truly TFT-G240320LTSW-118W-E portrait mode display EA-2478 Board*/
const LCD_PARAM_T truly_tft_g240320ltsw_118w_e_portrait =
{
  30,      /* Horizontal back porch */
  10,      /* Horizontal front porch */
  2,       /* HSYNC pulse width */
  240,     /* Pixels per line */
  4,       /* Vertical back porch */
  2,       /* Vertical front porch */
  2,       /* VSYNC pulse width */
  320,     /* Lines per panel */
  0,       /* Invert output enable */
  1,       /* Do not invert panel clock */
  1,       /* Invert HSYNC */
  1,       /* Invert VSYNC */
  1,       /* AC bias frequency (not used) */
  16,      /* Bits per pixel */
  4800000, /* Optimal clock rate (Hz) */
  TFT,     /* LCD panel type */
  0,       /* Single panel display */
};

/* Toshiba LTA057A347F display IRD 2.0 2478 Board*/
const LCD_PARAM_T LCD_LTA057A347F_params16bit =
{
  30,      /*11 * Horizontal back porch */
  10,      /*0 * Horizontal front porch */
  96,       /* HSYNC pulse width */
  320,     /* Pixels per line */
  8,       /* Vertical back porch */
  2,       /*0 * Vertical front porch */
  15,       /* VSYNC pulse width */
  240,     /* Lines per panel */
  0,       /* Invert output enable */
  0,       /* 0,Do not invert panel clock */
  0,       /* 0,Invert HSYNC */
  0,       /* 0,Invert VSYNC */
  0,       /* 0,AC bias frequency (not used) */
  16,      /* Bits per pixel */
  4800000, /* Optimal clock rate (Hz) */
  TFT,     /* LCD panel type */
  0,       /* Single panel display */
};

