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

#define FPGA_BASE_ADDR	0x82000000
#define WBO_REG1       (*(volatile unsigned short *)(FPGA_BASE_ADDR + 0x000))
#define WBO_REG2       (*(volatile unsigned short *)(FPGA_BASE_ADDR + 0x002))
#define WBO_REG3       (*(volatile unsigned short *)(FPGA_BASE_ADDR + 0x004))

#define FPGA_RESET		(1<<19)

/* main loop
 * basic CPU initialization is done here.
 */
int main(void)
{
	short Sval;
	long  Lval;
	short count = 0;
	int pressed, released;

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

	/* set reset FPGA pin to out */
	FIO2DIR |= FPGA_RESET;

	while (1)
	{
		FIO2SET = FPGA_RESET;

		// print a little intro message so we know things are working
		rprintf("\r\nWelcome to FPGA + ARM7 test program!\r\n");
		rprintf("\r\nPush joystick \r\n UP/DOWN/RIGHT/LEFT \r\n to light an LED on the Spartan 3A board");

		FIO2CLR = FPGA_RESET;

		// print single characters
		//rprintfChar('H');
		while (1)
		{

			/* count button press/releases */
			if (pressed & released)
			{
				count++;
				pressed=0;
				released=0;
			}

			switch (readKeys())
			{
				case UP:
					WBO_REG1 = count;
					pressed = 1;
					break;
				case DOWN:
					WBO_REG2 = count;
					pressed = 1;
					break;
				case LEFT:
					WBO_REG1 = 0x0002;
					pressed = 1;
					break;
				case RIGHT:
					WBO_REG1 = 0x0004;
					pressed = 1;
					break;
				case CENTER:
					WBO_REG1 = 0x0000;
					WBO_REG2 = 0x0000;
					break;
				default :
					released = 1;
					break;
			}
		}
	}

	return 0;
}
