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
	if(iir & UIIR_RDA)
	{
		/* read incoming char */
		rx = U0RBR;
		/*debug*/
		FIO2CLR = (1<<10);
		if(rx == '1')
			FIO2SET = (1<<10);
	}
	/* Transmit Holding register  */
	if(iir & UIIR_THRE)
	{


	}
	/* Recieve data availible */
	if(iir & UIIR_RLS)
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
 *    [in] uart       - uart base address
 *
 ****************************************************************************/
int initUart(unsigned long uart, unsigned short div_factor, unsigned char mode)
{
	uart_t * dev;

	switch (uart)
	{
		case UART0 :
		dev = (unsigned long *) UART0_BASE_ADDR;
		/* enable uart #0 pins in GPIO (P0.2 = TxD0, P0.3 = RxD0) */
		PINSEL0 |= ((1 << 4) | (1 << 6));
#if 1		//set the bit rate = set uart clock (pclk) divisionfactor
		U0LCR = 0x80; //enable divisor latches (DLAB bit set, bit 7)
		U0DLL = (unsigned char) div_factor; //write division factor LSB
		U0DLM = (unsigned char) (div_factor >> 8); //write division factor MSB
		U0IER = ((1<<0)|(1<<1)|(1<<2));
		//set transmissiion and fifo mode
		U0LCR = (mode & ~0x80); //DLAB bit (bit 7) must be reset
		return;
#endif

		/* install IRQ */
		if (install_irq(UART0_INT, (void *) Uart0_Handler, HIGHEST_PRIORITY) == 1)
			return 0; /* success */
		else
			return -1; /* fail */
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

	/* enable RDA, THRE, RL line IRQ's*/
	dev->IER = ((1<<0)|(1<<1)|(1<<2));

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
int sendchar(unsigned long uart, int ch)
{
	/* we're using the FIFO, assume room */
	/* THR has offsett 0 */
	//*(unsigned int *) uart = ch;
	U0THR=ch;
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
	lsr = *(unsigned long *)(uart + 0x14);

	/* data ?, return RBR (offset 0) */
	if (*lsr & 0x01)
		return (char)(*(unsigned long *)uart);

	/* else return 0 */
	return 0;
}
