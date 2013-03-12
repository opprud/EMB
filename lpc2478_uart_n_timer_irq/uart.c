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
#include <lpc2468.h>
#include "uart.h"
#include "irq.h"

/* declare IRQ prototypes */
void Uart0_Handler(void) __attribute__ ((interrupt("IRQ")));


void Uart0_Handler(void)
{

	char rx, tx, lsr;
	int iir;

	/*The UnIIR must be read in order to clear the interrupt prior to exiting the
	Interrupt Service Routine.See table 384*/
	iir = U0IIR;

	/* Recieve data availible */
	if(iir == UIIR_RDA)
	{
		/* read incoming char */
		rx = U0RBR;
		/*debug*/
		FIO2CLR = (1<<10);
		if(rx == '1')
			FIO2SET = (1<<10);
	}
	/* Transmit Holding register  */
	if(iir == UIIR_THRE)
	{


	}
	/* Recieve data availible */
	if(iir == UIIR_RLS)
	{
		/* irq cleared upon LSR read*/
		lsr = U0LSR;
	}

	/* ack IRQ in VIC */
	VICVectAddr = 0;
}

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
int initUart0(unsigned short div_factor, unsigned char mode)
{
	//enable uart #0 pins in GPIO (P0.2 = TxD0, P0.3 = RxD0)
	PINSEL0 = PINSEL0 | ((1 << 4) | (1 << 6));

	//set the bit rate = set uart clock (pclk) divisionfactor
	U0LCR = 0x80; //enable divisor latches (DLAB bit set, bit 7)
	U0DLL = (unsigned char) div_factor; //write division factor LSB
	U0DLM = (unsigned char) (div_factor >> 8); //write division factor MSB
	//set transmissiion and fifo mode
	U0LCR = (mode & ~0x80); //DLAB bit (bit 7) must be reset
	//enable THR, RBR and LSR int.
//	U0IER = ((1<<0)|(1<<1)|(1<<2));
	U0IER = 0x01;

	/* install IRQ */
	if (install_irq(UART0_INT, (void *) Uart0_Handler, HIGHEST_PRIORITY) == 1)
		return 0; /* success */
	else
		return -1; /* fail */

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
int sendchar(int ch)
{
	if(ch == '\n')
	{
		while(!(U0LSR & 0x20));
		U0THR = '\r';
	}
	while(!(U0LSR & 0x20));
	return (U0THR = ch);
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
int getkey(void)
{
	while(!(U0LSR & 0x01));
	return (U0RBR);
}
