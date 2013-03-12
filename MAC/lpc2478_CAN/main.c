/**
 * @mainpage LPC2478CAN - CAN bus demo project
 *
 * @section CAN Demo program showing how to initialize the LPC2478's two CAN controllers, and sending from one controller to the other.
 *
 *
 * @image html ea.png "EA LPC2478 OEM"
 * @file
 * @author  Morten Opprud <morten@hih.au.dk>
 * @version 1.0
 * @date    28/10/2010
 *
 * @section DESCRIPTION
 * This project build on the previous project for setting up the LCD
 * and touchscreen controller.
 * The touch screen can be used to select a baudrate to initiate the controllers
 * The joystick will be used for sending mesages.
 * Incoming messages will be displayed on the LCD
 *
 * NOTE, remember to adjust
 * #define USE_32_BIT_DATABUS
 * in config.h, according to your platform
 *
 */
#include "lpc2468.h"
#include "framework.h"
#include "uart.h"
#include "rprintf.h"
#include "irq.h"
#include "rtc.h"
#include "calib_app.h"
#include "type.h"
#include "can.h"
/* LCD includes */
#include "drivers/lcd/ex_sdram.h"

/*profiling*/
#include "simple_profiler.h"

//#define USE_PRINTF
#undef USE_PRINTF

/**
 *  CAN device handles
 */
unsigned int candev1;
unsigned int candev2;

CANMSG_T TxMsg, RxMsg;

/* main loop
 * basic CPU initialization is done here.
 */
int main(void)
{
	int seconds_last;
	int c;

	/* init low level stuff */
	lowLevelInit();

	/*setup VIC*/
	init_VIC();

	/*initialize uart #1: 115200 bps, 8N1*/
	initUart0(B115200(Fpclk), UART_8N1);

	/* initialize rprintf system */
	rprintfInit(putchar);

	/* Initialize RTC module */
	//	/* initialize external SDRAM interface */
	SDRAMInit();

	/* print msg */
	rprintf("LCD calibrate test program \n");

	/* do LCD setup, and calibration */
	//	calibrateStart();

	/* Open CAN interface */
	candev1 = can_open(CAN1, 0);

	/* Set CAN baudrate to 125 kbps */
	can_ioctl(candev1, CAN_SET_BAUD_RATE, BPS_125K);

	/* Open CAN interface */
	candev2 = can_open(CAN2, 0);

	/* Set CAN baudrate to 125 kbps */
	can_ioctl(candev2, CAN_SET_BAUD_RATE, BPS_125K);

	/* enable IRQ's globally */
	enableIRQ();

	/* Setup profiling pins to outputs */
	PROF_DIR = PROF_PIN_MASK;

	while (1)
	{
		if(1)
		{
			//SET
			PROF_SET = PROF_PIN_5;
			/* compose TX msg, and send */
			TxMsg.MsgID++;
			TxMsg.Frame.Bit.DLC = 7;

			if(can_write(candev1, &TxMsg, 1)== 0)
			{
				#ifdef USE_PRINTF
				rprintf("Failed TX msg on CAN1 \n");
				#endif
			}
			//CLR
			PROF_CLR = PROF_PIN_5;

		}
		//CLR
		PROF_CLR = PROF_PIN_1;

		//SET
		PROF_SET = PROF_PIN_6;
        /*wait for tx to complete*/
		for	(c=0;c<500000;c++);
		//CLR
		PROF_CLR = PROF_PIN_6;

		//SET
		PROF_SET = PROF_PIN_1;

		// SET
		PROF_SET = PROF_PIN_2;
		/* read all can messages*/
		while(can_read(candev2, &RxMsg, 1))
		{
			//CLR
			PROF_CLR = PROF_PIN_2;
			// SET
			PROF_SET = PROF_PIN_3;
			#ifdef USE_PRINTF
			rprintf("RX id: %d Data [0]:%d \n",RxMsg.MsgID ,RxMsg.Data[0]);
			#endif
		}
		#ifdef USE_PRINTF
		rprintf("No more RX msg on CAN2 \n");
		#endif
		//CLR
		PROF_CLR = PROF_PIN_3;

	}
	return 0;
}

