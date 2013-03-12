/******************************************************************************
 * Touch screen test program
 * Author  : morten@hih.au.dk
 * Date    : Oct. 14, 2010
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
#include "type.h"

/* LCD includes */
#include "drivers/lcd/ex_sdram.h"

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

	/*initialize uart #1: 115200 bps, 8N1*/
	initUart0(B115200(Fpclk), UART_8N1);

	/* initialize rprintf system */
	rprintfInit(putchar);

	/* Initialize RTC module */
	RTCInit();

	/* initialize external SDRAM interface */
	SDRAMInit();

	/* print msg */
	rprintf("LCD calibrate test program \n");

	/* do LCD setup, and calibration */
	calibrateStart();

	/* enable IRQ's globally */
	enableIRQ();

	while (1)
	{ /* Loop forever */
		lcd_test();
	}
	return 0;
}

