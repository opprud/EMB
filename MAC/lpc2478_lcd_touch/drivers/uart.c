/******************************************************************************
 * Simple non-interrupt driven UART driver
 *
 * File:
 *    uart.c
 *
 * Author  : MOJ@hih.au.dk
 * Date    : AUG 3 2010
 *
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "lpc2468.h"
#include "uart.h"


/*****************************************************************************
 *
 * Description:
 *    Initialize UART #0 in polled mode, i.e., interrupts are not used.
 *
 * Parameters:
 *    [in] div_factor - UART clock division factor to get desired bit rate.
 *                      Use definitions in uart.h to calculate correct value.
 *    [in] mode       - transmission format settings. Use constants in uart.h
 *
 ****************************************************************************/
void initUart0(unsigned short div_factor, unsigned char mode)
{
	//enable uart #0 pins in GPIO (P0.2 = TxD0, P0.3 = RxD0)
	PINSEL0 = PINSEL0 | ((1 << 4) | (1 << 6));

	//set the bit rate = set uart clock (pclk) divisionfactor
	U0LCR = 0x80; //enable divisor latches (DLAB bit set, bit 7)
	U0DLL = (unsigned char) div_factor; //write division factor LSB
	U0DLM = (unsigned char) (div_factor >> 8); //write division factor MSB

	//set transmissiion and fifo mode
	U0LCR = (mode & ~0x80); //DLAB bit (bit 7) must be reset
}

/*****************************************************************************
 *
 * Description:
 *    Blocking output routine, i.e., the routine waits until the
 *    uart tx register is free and then sends the character.
 *
 * Params:
 *    [in] charToSend - The character to print
 *
 ****************************************************************************/
//int sendchar(int ch)
void putchar (unsigned char character)
{
	if(character == '\n')
	{
		while(!(U0LSR & 0x20));
		U0THR = '\r';
	}
	while(!(U0LSR & 0x20));
	return (U0THR = character);
}

/*****************************************************************************
 *
 * Description:
 *    Blocking function that waits for a received character.
 *
 * Return:
 *    The received character.
 *
 ****************************************************************************/
//int getkey(void)
int getchar(void)
{
	while(!(U0LSR & 0x01));
	return (U0RBR);
}
