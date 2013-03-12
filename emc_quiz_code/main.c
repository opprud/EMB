/**********************************
 * Small rprintf test program
 * Author  : MOJ@hih.au.dk
 * Date    : July 27, 2010
 *********************************/
#include "lpc2468.h"
#include "framework.h"
#include "uart.h"
#include "keys.h"
#include "rprintf.h"

#define TRUE 1

#define EXT_HW_BASE_ADDR	0x82000000
#define HW_REG1       (*(volatile unsigned short *)(EXT_HW_BASE_ADDR + 0x000))
#define HW_REG2       (*(volatile unsigned long *)(EXT_HW_BASE_ADDR + 0x004))
#define HW_REG3       (*(volatile unsigned short *)(EXT_HW_BASE_ADDR + 0x008))

void quiz(void);

/* main loop
 * basic CPU initialization is done here.
 */
int main(void)
{
	short Sval;
	long  Lval;
	/* init low level stuff */
	lowLevelInit();

	/*initialize uart #1: 9600 bps, 8N1*/
	initUart0(B115200(Fpclk), UART_8N1);

	// initialize rprintf system
	// - use uartSendByte as the output for all rprintf statements
	//   this will cause all rprintf library functions to direct their
	//   output to the uart
	// - rprintf can be made to output to any device which takes characters.
	//   You must write a function which takes an unsigned char as an argument
	//   and then pass this to rprintfInit like this: rprintfInit(YOUR_FUNCTION);
	rprintfInit(putchar);


	while (1)
	{

		// print a little intro message so we know things are working
		rprintf("\r\nWelcome to EMC test program!\r\n");
		rprintf("\r\nPush joystick \r\n UP/DOWN/RIGHT/LEFT \r\n to get activity on the Address Data bus");

		// print single characters
		//rprintfChar('H');
		while (1)
		{

			/* Read joystick and read/write 16/32 bit accordingly */
			switch (readKeys())
			{
			case UP:
				HW_REG1 = Sval++;
				break;

			case DOWN:
				HW_REG2 = Lval++;
				break;
			case LEFT:
				Sval = HW_REG3;
				break;

			case RIGHT:
				Lval = HW_REG2;
				break;

			}
		}
	}

	return 0;
}

