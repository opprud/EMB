/******************************************************************************
 * RTC test program
 * Author  : morten@hih.au.dk
 * Date    : Sept. 30, 2010
 *
 * NOTE, remember to adjust
 * #define USE_32_BIT_DATABUS
 * in config.h, according to your platform
 ******************************************************************************/
#include "lpc2468.h"
#include "framework.h"
#include "uart.h"
#include "rprintf.h"
#include "irq.h"
#include "rtc.h"
#include "calib_app.h"
#include "calib_app.h"

/* LCD includes */
#include "drivers/lcd/ex_sdram.h"
#include "drivers/lcd/lpc_lcd_params.h"
#include "drivers/lcd/lcd_driver.h"

/*  lcd defs */
#define LCD_DISPLAY truly_tft_g240320ltsw_118w_e_portrait	// EA2478 built-in display
long dev_lcd;

/* DIRTY!! : external refs */
//extern void lcd_colorbars(void);
//extern void print_CAN_msg(CAN_MSG *msg);
//extern void print_time(RTCTime *tm);

/* external variables */
extern volatile int alarm_on;

/* global variables */
//RTCTime local_time, alarm_time, current_time;

void open_lcd()
{
	/** LCD stuff **/
	/* Power Control for CLCDC peripheral */
	PCONP |= 0x00100000;

	/* Open LCD */
	if ((dev_lcd = lcd_open(CLCDC, (long) &LCD_DISPLAY)) == 0x0)
	{
		/* Error opening the device */
		rprintf("Error opening LCD \n");
	}

	/* Set LCD display pointer */
	lcd_ioctl(dev_lcd, LCD_SET_BUFFER, (long) SDRAM_BASE_ADDR);

	/* Enable LCD */
	lcd_ioctl(dev_lcd, LCD_ENABLE, 1);

	/* Turn on LCD */
	lcd_ioctl(dev_lcd, LCD_PWR_ON, 1);

}


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

	/*init LCD*/
	open_lcd();

	/* Display 3 color bars on the lcd display */
	//lcd_colorbars();

	/* print msg */
	rprintf("LCD calibrate test program \n");

	/* do calibration */
	calibrateStart();

	/* enable IRQ's globally */
	enableIRQ();

	while (1)
	{ /* Loop forever */
		lcd_test();
	}
	return 0;
}

