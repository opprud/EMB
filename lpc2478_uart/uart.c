/******************************************************************************
 * Simple non-interrupt driven UART driver
 *
 * File:
 *    uart.c
 *
 * Author  : MOJ@hih.au.dk
 * Date    : Sep. 3 2010
 *
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/
#include <lpc2468.h>
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
 *    [in] uart       - uart base address
 *
 ****************************************************************************/
void initUart(unsigned long uart, unsigned short div_factor, unsigned char mode)
{
	uart_t * dev;

	switch (uart)
	{
		case UART0 :
		dev = UART0_BASE_ADDR;
		/* enable uart #0 pins in GPIO (P0.2 = TxD0, P0.3 = RxD0) */
		//PINSEL0 |= ((1 << 4) | (1 << 6));
		PINSEL0 = ((1 << 4) | (1 << 6));
		break;
		case UART1 :
		dev = (unsigned long *) UART1_BASE_ADDR;
		/* enable uart #1 pins in GPIO (P3.16 = TxD1, P3.17 = RxD1) */
		PINSEL7 |= ((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3));
		break;
		case UART2 :
		dev = (unsigned long *) UART2_BASE_ADDR;
		/* enable uart #2 pins in GPIO (P0.10 = TxD2, P0.11 = RxD2) */
		PINSEL0 = PINSEL0 | ((1 << 20) | (1 << 22));
		break;
		case UART3 :
		dev = (unsigned long *) UART3_BASE_ADDR;
		/* enable uart #0 pins in GPIO (P0.0 = TxD3, P0.1 = RxD3) */
		PINSEL0 = PINSEL0 | ((1 << 1) | (1 << 3));
		break;
	}
	/* allow acess to prescalers */
	dev->LCR = 0x80;

	/*set the bit rate = set uart clock (pclk) divisionfactor*/
	/* LSB */
	dev->DLL = (unsigned char) div_factor;

	/* MSB */
	dev->DLM = (unsigned char) (div_factor >> 8);

	/* set transmissiion and fifo mode */
	/* DLAB bit (bit 7) must be reset */
	dev->LCR = (mode & ~0x80);

	/* ENABLE fifo */
	dev->FCR = 0x01;

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
int sendchar(unsigned long uart, unsigned char ch)
{
	/* we're using the FIFO, assume room */
	/* THR has offsett 0 */
	//while(!(U0LSR & 0x20));
	//U0THR = ch;
	/*cast UART nbase addr to 32bit register type */
	*(unsigned long *) uart = ch;
	//TODO add index counter to keep track of tx FIFO
}

/*****************************************************************************
 *
 * Description:
 *    Non Blocking function that returns received characters. 
 *
 * Return:
 *    The received character. 
 *
 ****************************************************************************/
char getkey(unsigned long uart)
{
	unsigned long *lsr;

	/* assign lsr to selected uarts lsr*/
	lsr = (uart + 0x14);

	/* data ?, return RBR (offset 0) */
	if (*lsr & 0x01)
		return (char)uart;

	/* else return 0 */
	return 0;
}
