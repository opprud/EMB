/**********************************
 * Small rprintf test program
 * Author  : MOJ@hih.au.dk
 * Date    : July 27, 2010
 *********************************/
#include "lpc2468.h"
#include "framework.h"
#include "uart.h"
#include "rprintf.h"

#define TRUE 1

/* main loop
 * basic CPU initialization is done here.
 */
int main(void)
{
	short val;
	char mydata;
	char mystring[10];
	float b;
	char small;
	short medium;
	int big;

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
		rprintf("\r\nWelcome to rprintf Test!\r\n");
		rprintf("\r\nThis is my cool program!\r\n");

		// print single characters
		rprintfChar('H');
		rprintfChar('e');
		rprintfChar('l');
		rprintfChar('l');
		rprintfChar('o');
		// print a string from ROM or constants section
		rprintfStr(" World!");
		// print a carriage return, line feed combination
		rprintfCRLF();
		// note that using rprintfCRLF() is more memory-efficient than
		// using rprintf("\r\n"), especially if you do it repeatedly

		mystring[0] = 'A';
		mystring[1] = ' ';
		mystring[2] = 'S';
		mystring[3] = 't';
		mystring[4] = 'r';
		mystring[5] = 'i';
		mystring[6] = 'n';
		mystring[7] = 'g';
		mystring[8] = '!';
		mystring[9] = 0; // null termination

		// print a null-terminated string from RAM
		rprintfStr(mystring);
		rprintfCRLF();

		// print a section of a string from RAM
		// - start at index 2
		// - print 6 characters
		rprintfStrLen(mystring, 2, 6);
		rprintfCRLF();

		val = 24060;
		mydata = 'L';

		// print a decimal number
		rprintf("This is a decimal number: %d\r\n", val);

		// print a hex number
		rprintf("This is a hex number: %x\r\n", mydata);

		// print a character
		rprintf("This is a character: %c\r\n", mydata);

		// print hex numbers
		small = 0x12; // a char
		medium = 0x1234; // a short
		big = 0x12345678; // a long

		rprintf("This is a 2-digit hex number (char) : ");
		rprintfu08(small);
		rprintfCRLF();

		rprintf("This is a 4-digit hex number (short): ");
		rprintfu16(medium);
		rprintfCRLF();

		rprintf("This is a 8-digit hex number (long) : ");
		rprintfu32(big);
		rprintfCRLF();

		// print a formatted decimal number
		// - use base 10
		// - use 8 characters
		// - the number is signed [TRUE]
		// - pad with '.' periods
		rprintf("This is a formatted decimal number: ");
		rprintfNum(10, 8, TRUE, '.', val);
		rprintfCRLF();

		b = 1.23456;

		// print a floating point number
		// use 10-digit precision

		// NOTE: TO USE rprintfFloat() YOU MUST ENABLE SUPPORT IN rprintfconf.h

		//rprintf("This is a floating point number: ");
		//rprintfFloat(8, b);
		//rprintfCRLF();

		while(1);
	}

	return 0;
}
