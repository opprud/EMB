/*****************************************************************************
 *   lpc_params.h:  Header file for NXP LPC23xx/24xx Family Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2008.07.29  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/

#ifndef __LCD_PARAM_H
#define __LCD_PARAM_H

#include "type.h"

/* LCD display types */
typedef enum {
  TFT = 0,   /* standard TFT */
  ADTFT,     /* advanced TFT */
  HRTFT,     /* highly reflective TFT */
  MONO_4BIT, /* 4-bit mono */
  MONO_8BIT, /* 8-bit mono */
  CSTN       /* color STN */
} LCD_PANEL_T;

/* Structure containing the parameters for the LCD panel */
typedef struct {
  BYTE        h_back_porch;         /* Horizontal back porch in clocks */
  BYTE        h_front_porch;        /* Horizontal front porch in clocks */
  BYTE        h_sync_pulse_width;   /* HSYNC pulse width in clocks */
  WORD        pixels_per_line;      /* Pixels per line (horizontal resolution) */
  BYTE        v_back_porch;         /* Vertical back porch in clocks */
  BYTE        v_front_porch;        /* Vertical front porch in clocks */
  BYTE        v_sync_pulse_width;   /* VSYNC pulse width in clocks */
  WORD        lines_per_panel;      /* Lines per panel (vertical resolution) */
  BYTE        invert_output_enable; /* Invert output enable, 1 = invert*/
  BYTE        invert_panel_clock;   /* Invert panel clock, 1 = invert*/
  BYTE        invert_hsync;         /* Invert HSYNC, 1 = invert */
  BYTE        invert_vsync;         /* Invert VSYNC, 1 = invert */
  BYTE        ac_bias_frequency;    /* AC bias frequency in clocks */
  BYTE        bits_per_pixel;       /* Maximum bits per pixel the display supports */
  DWORD       optimal_clock;        /* Optimal clock rate (Hz) */
  LCD_PANEL_T lcd_panel_type;       /* LCD panel type */
  BYTE        dual_panel;           /* Dual panel, 1 = dual panel display */
} LCD_PARAM_T;

/* Truly TFT-G240320LTSW-118W-E portrait mode display */
extern const LCD_PARAM_T truly_tft_g240320ltsw_118w_e_portrait;

#endif /* __LCD_PARAM_H */
