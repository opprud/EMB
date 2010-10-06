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

	/* print msg */
	rprintf("RTC test program \n");

	/* set time */
	local_time.RTC_Sec = 23;
	local_time.RTC_Min = 44;
	local_time.RTC_Hour = 9;
	local_time.RTC_Mday = 30;
	local_time.RTC_Wday = 3;
	local_time.RTC_Yday = 201; /* current date 07/12/2006 */
	local_time.RTC_Mon = 9;
	local_time.RTC_Year = 2010;
	RTCSetTime(local_time); /* Set local time */

	alarm_time.RTC_Sec = 0;
	alarm_time.RTC_Min = 0;
	alarm_time.RTC_Hour = 0;
	alarm_time.RTC_Mday = 1;
	alarm_time.RTC_Wday = 0;
	alarm_time.RTC_Yday = 1; /* alarm date 01/01/2007 */
	alarm_time.RTC_Mon = 1;
	alarm_time.RTC_Year = 2007;
	RTCSetAlarm(alarm_time); /* set alarm time */

	/* install RTC timer handler mainly for alarm control */
	if (install_irq(RTC_INT, (void *) RTCHandler, HIGHEST_PRIORITY) == 0)
	{
		rprintf("IRQ install of RTC failed");
		while (1)
			;
	}

	/* mask off alarm mask, turn on IMYEAR in the counter increment interrupt
	 register */
	RTCSetAlarmMask(AMRSEC | AMRMIN | AMRHOUR | AMRDOM | AMRDOW | AMRDOY | AMRMON | AMRYEAR);

	RTC_CIIR = IMMIN | IMYEAR;
	/* 2007/01/01/00:00:00 is the alarm on */

	RTCStart();

	while (1)
	{ /* Loop forever */
		current_time = RTCGetTime();
		if (alarm_on != 0)
		{
			alarm_on = 0;
			/* Get current time when alarm is on */
			current_time = RTCGetTime();
		}
		if (seconds_last != current_time.RTC_Sec)
		{
			//allign seconds
			seconds_last = current_time.RTC_Sec;

			//print current time
			rprintf("\r Time %d:%d:%d",current_time.RTC_Hour,current_time.RTC_Min,current_time.RTC_Sec);
		}
	}
	return 0;
}

