/**********************************
 * Small rprintf test program
 * Author  : MOJ@hih.au.dk
 * Date    : July 27, 2010
 *********************************/
#include "lpc2468.h"
#include "framework.h"
#include "uart.h"
#include "spi.h"
#include "rprintf.h"

#define TRUE 1

/* main loop
 * basic CPU initialization is done here.
 */
int main(void)
{
	char rx, tx;

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

	/* init SPI0 @ 1Mhz, MSB fisrt */
	SPIInit(CLK1M, 0, 0, 0);

	while (1)
	{

		// print a little intro message so we know things are working
		rprintf("\r\nWelcome to SPI test program!\r\n");
		rprintf("\r\nCharachters recieved on UART0 is echoed on SPI 0\r\n");

		// print single characters
		//rprintfChar('H');
		while (1)
		{
			tx = getchar();
			rx = SpiXfer(tx);
			rprintf("\r TX:%x RX:%x   ",tx,rx);

		}
}

return 0;
}
