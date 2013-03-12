/**********************************
 * RTC test program
 * Author  : morten@hih.au.dk
 * Date    : Sept. 30, 2010
 *********************************/
#include "lpc2468.h"
#include "framework.h"
#include "uart.h"
#include "rprintf.h"
#include "irq.h"
#include "rtc.h"

/* LCD includes */
#include "drivers/lcd/ex_sdram.h"
#include "drivers/lcd/lpc_lcd_params.h"
#include "drivers/lcd/lcd_driver.h"

/*  lcd defs */
#define LCD_DISPLAY truly_tft_g240320ltsw_118w_e_portrait	// EA2478 built-in display
long dev_lcd;

/* DIRTY!! : external refs */
extern void lcd_colorbars(void);
//extern void print_CAN_msg(CAN_MSG *msg);
extern void print_time(RTCTime *tm);


/* external variables */
extern volatile int alarm_on;

/* global variables */
RTCTime local_time, alarm_time, current_time;

/* main loop
 * basic CPU initialization is done here.
 */
int main(void)
{
	int seconds_last;

	/* init low level stuff */
	lowLevelInit();

	/*setup VIC*/
	init_VIC();

	/*initialize uart #1: 9600 bps, 8N1*/
	initUart0(B115200(Fpclk), UART_8N1);

	/* initialize rprintf system */
	rprintfInit(putchar);

	/* Initialize RTC module */
	RTCInit();

	/* initialize external SDRAM interface */
	SDRAMInit();

	/** LCD stuff **/
	/* Power Control for CLCDC peripheral */
	PCONP |= 0x00100000;

	/* Open LCD */
	if ((dev_lcd = lcd_open(CLCDC, (long) &LCD_DISPLAY)) == 0x0)
	{
		/* Error opening the device */
		while (1)
			;
	}

	/* Set LCD display pointer */
	lcd_ioctl(dev_lcd, LCD_SET_BUFFER, (long) SDRAM_BASE_ADDR);

	/* Enable LCD */
	lcd_ioctl(dev_lcd, LCD_ENABLE, 1);

	/* Turn on LCD */
	lcd_ioctl(dev_lcd, LCD_PWR_ON, 1);

	/* initialize timer 0 */
	//init_timer(0, 1000);

	/* enable timer 0 */
	//enable_timer(0);

	/* Display 3 color bars on the lcd display */
	lcd_colorbars();

	/* print msg */
	rprintf("RTC Alarm test program \n");

	/* set time */
	local_time.RTC_Sec = 23;
	local_time.RTC_Min = 44;
	local_time.RTC_Hour = 10;
	local_time.RTC_Mday = 30;
	local_time.RTC_Wday = 3;
	local_time.RTC_Yday = 201; /* current date 07/12/2006 */
	local_time.RTC_Mon = 9;
	local_time.RTC_Year = 2010;

	alarm_time.RTC_Sec = 35;
	alarm_time.RTC_Min = 44;
	alarm_time.RTC_Hour = 10;
	alarm_time.RTC_Mday = 30;
	alarm_time.RTC_Wday = 3;
	alarm_time.RTC_Yday = 201; /* alarm date 01/01/2007 */
	alarm_time.RTC_Mon = 0;
	alarm_time.RTC_Year = 2010;

	rprintf(" Alarm set today at %d:%d:%d \n",alarm_time.RTC_Hour,alarm_time.RTC_Min,alarm_time.RTC_Sec);

	/* only hour, min and sec har to macth to get IRQ */
	RTCSetAlarmMask(AMRDOM | AMRDOW | AMRDOY | AMRMON | AMRYEAR);

	//RTC_CIIR = IMMIN | IMYEAR;
	/* 2007/01/01/00:00:00 is the alarm on */

	RTCStart();

	/* enable IRQ's globally */
	enableIRQ();

	while (1)
	{ /* Loop forever */

		/*if keypress*/
		if (!(FIO2PIN & (1 << 10)))
		{
			/* Set local time */
			RTCSetTime(local_time);

			/* set alarm time */
			RTCSetAlarm(alarm_time);
		}

		current_time = RTCGetTime();
		if (alarm_on != 0)
		{
			alarm_on = 0;

			rprintf("\n !!RTC ALARM !!!! \n");
		}
		if (seconds_last != current_time.RTC_Sec)
		{
			//allign seconds
			seconds_last = current_time.RTC_Sec;

			//print current time
			rprintf("\r Time %d:%d:%d",current_time.RTC_Hour,current_time.RTC_Min,current_time.RTC_Sec);
			print_time(&current_time);
		}
	}
	return 0;
}

