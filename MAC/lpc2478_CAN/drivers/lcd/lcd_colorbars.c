/**
 * @ingroup lcd
 * @file
 * @brief LCD colorbars
 *
 *  $Id:: lcd_colorbars.c 1260 2008-10-28 23:20:43Z wellsk             $
 *
 * Project: NXP EA2478 LCD example
 *
 * Description:
 *     Draws color bars on the LCD. Requires RGB565 mode.
 *
 **********************************************************************
 * Software that is described herein is for illustrative purposes only  
 * which provides customers with programming information regarding the  
 * products. This software is supplied "AS IS" without any warranties.  
 * NXP Semiconductors assumes no responsibility or liability for the 
 * use of the software, conveys no license or title under any patent, 
 * copyright, or mask work right to the product. NXP Semiconductors 
 * reserves the right to make changes in the software without 
 * notification. NXP Semiconductors also make no representation or 
 * warranty that such application will be suitable for the specified 
 * use without further testing or modification. 
 *********************************************************************/
#include <stdio.h>
/* LCD includes */
#include "..\swim\lpc_types.h"
#include "..\swim\lpc_swim.h"
#include "..\swim\lpc_swim_font.h"
#include "..\swim\lpc_swim.h"
#include "..\swim\lpc_rom8x16.h"
#include "..\swim\lpc_winfreesystem14x16.h"
#include "..\swim\lpc_x6x13.h"
#include "lpc_lcd_params.h"      /* LCD panel parameters */

/* CAN includes */
//#include "..\can.h"
#include "..\rtc.h"

/* other includes */
#include "lpc2478.h"
#include "type.h"

/* external references */
extern DWORD timer0_counter;
extern long dev_lcd;


/* DEFINES */
#define EA2478_LCD_FRAME_BUF 0xA0000000
#define LCD_DISPLAY truly_tft_g240320ltsw_118w_e_portrait	// EA2478 built-in display
#define FONT  font_winfreesys14x16  //font_x6x13 // font_rom8x16 font_rom8x16 font_winfreesys14x16
#define LINE_GAP 20
#define SCREN_END_Y 280



/* module global variables */
int count;

SWIM_WINDOW_T win1;
UNS_16 xgs, ygs, curx, cury, curym, xidx;
UNS_16 ypos = 0;

/***********************************************************************
 *
 * Function: make_number_str
 *
 * Purpose: Convert a number to a positive decimal string
 *
 * Processing:
 *     Using successive division, compute the base10 value of a number
 *     into a string and return the string value to the caller.
 *
 * Parameters:
 *     str      : Where to place the generated strinjg
 *     iteration: Number to generate a string from
 *
 * Outputs: None
 *
 * Returns: Nothing
 *
 * Notes: None
 *
 **********************************************************************/
void make_number_str(CHAR *str, INT_32 iteration)
{
	UNS_8 tmp[32];
	INT_32 dvv1, dvv2;
	INT_32 cnt = 0;

	/* Number 0 in string */
	str[cnt] = '0';

	/* Main loop */
	while (iteration > 0)
	{
		dvv1 = iteration / 10;
		dvv2 = dvv1 * 10;
		dvv1 = iteration - dvv2;

		tmp[cnt] = dvv1;
		iteration = iteration / 10;
		cnt++;
	}

	if (cnt > 0)
	{
		for (dvv1 = 0; dvv1 < cnt; dvv1++)
		{
			str[dvv1] = (CHAR) ('0' + tmp[cnt - (dvv1 + 1)]);
		}
	}

	str[cnt + 1] = '\0';
}

/**********************************************************************
 *
 * Function: lcd_colorbars
 *
 * Purpose: Draw colorbars on LCD panel
 *
 * Processing:
 *     See function.
 *
 * Parameters: None
 *
 * Outputs: None
 *
 * Returns: Nothing
 *
 * Notes: None
 *
 *********************************************************************/
void lcd_colorbars(void)
{
	//SWIM_WINDOW_T win1;
	COLOR_T clr, *fblog;
	CHAR str[32];
	int idx;
	//    UNS_16 xgs, ygs, curx, cury, curym, xidx;
	//    UNS_16 ypos =0;


	/* Set LCD frame buffer address */
	fblog = (COLOR_T *) EA2478_LCD_FRAME_BUF;

	/* Create a SWIM window */
	swim_window_open(&win1, LCD_DISPLAY.pixels_per_line,
			LCD_DISPLAY.lines_per_panel, fblog, 0, 0,
			(LCD_DISPLAY.pixels_per_line - 1),
			(LCD_DISPLAY.lines_per_panel - 1), 1, LIGHTGRAY, WHITE, LIGHTGRAY);

	/* select the font to use */
	swim_set_font(&win1, (FONT_T *) &FONT);

	/* set the pen color to use */
	swim_set_pen_color(&win1, BLACK);

	/* Add a title bar */
	swim_set_title(&win1, "    -- 249 Euro watch --", LIGHTGRAY);

	/* set the location to write text */
	swim_set_xy(&win1, 60, 160);

	/* set the pen color to use */
	swim_set_pen_color(&win1, DARKGRAY);

	/* select the font to use */
	swim_set_font(&win1, (FONT_T *) &font_rom8x16);

	//   count = 20;
	//   /* put the timer tick on the panel */
	//   while(count--)
	//   {
	//	   if(count == 11)
	//		   swim_set_pen_color(&win1, RED);
	//	   else
	//		   swim_set_pen_color(&win1, DARKGRAY);
	//
	//	    sprintf(str,"x%04x-%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x", count, 10+count%255, count/85, 10+count/6255, 14+count%255, 5+count/225, 1+count%45, 16+count%5, 10+count%25);
	//        swim_set_xy(&win1, 0, ypos);
	//        swim_put_text(&win1, str );
	//        ypos += LINE_GAP;
	//  }

}

void print_time(RTCTime *tm)
{
	char str[32];
	static int screen_no;

	if (ypos > SCREN_END_Y)
	{
		ypos = 0;
		//swim_set_xy(&win1, 60, 160);

		if (screen_no == 0)
		{
			screen_no = 1;
			swim_set_pen_color(&win1, DARKGRAY);
		}
		else
		{
			screen_no = 0;
			swim_set_pen_color(&win1, BLUE);
		}
	}

	//sprintf(str,"#%04X-%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X", msg->MsgID, (unsigned char)(msg->DataA & 0xFF), (unsigned char)(msg->DataA >>8)& (0xFF),(unsigned char)(msg->DataA >>16)& (0xFF),(unsigned char)(msg->DataA >>24)& (0xFF),(unsigned char)(msg->DataB )& (0xFF),(unsigned char)(msg->DataB >>8)& (0xFF),(unsigned char)(msg->DataB >>16)& (0xFF),(unsigned char)(msg->DataB >>24)& (0xFF) );
	//sprintf(str, "ID:%04X-%08X:%08X ", msg->MsgID, msg->DataA, msg->DataB);
	sprintf(str,"Time:%02d:%02d:%02d", tm->RTC_Hour, tm->RTC_Min, tm->RTC_Sec  );
	swim_set_xy(&win1, 0, ypos);
	swim_put_text(&win1, str);
	ypos += LINE_GAP;

}


//void print_CAN_msg(CAN_MSG *msg)
//{
//	char str[32];
//	static int screen_no;
//
//	if (ypos > SCREN_END_Y)
//	{
//		ypos = 0;
//		//swim_set_xy(&win1, 60, 160);
//
//		if (screen_no == 0)
//		{
//			screen_no = 1;
//			swim_set_pen_color(&win1, DARKGRAY);
//		}
//		else
//		{
//			screen_no = 0;
//			swim_set_pen_color(&win1, BLUE);
//		}
//	}
//
//	sprintf(str,"#%04X-%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X", msg->MsgID, (unsigned char)(msg->DataA & 0xFF), (unsigned char)(msg->DataA >>8)& (0xFF),(unsigned char)(msg->DataA >>16)& (0xFF),(unsigned char)(msg->DataA >>24)& (0xFF),(unsigned char)(msg->DataB )& (0xFF),(unsigned char)(msg->DataB >>8)& (0xFF),(unsigned char)(msg->DataB >>16)& (0xFF),(unsigned char)(msg->DataB >>24)& (0xFF) );
//	//sprintf(str, "ID:%04X-%08X:%08X ", msg->MsgID, msg->DataA, msg->DataB);
//	swim_set_xy(&win1, 0, ypos);
//	swim_put_text(&win1, str);
//	ypos += LINE_GAP;
//
//}
