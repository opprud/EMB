/*****************************************************************************
 *   irq.c: Interrupt handler C file for NXP LPC23xx/24xx Family Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2006.07.13  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#include "LPC2468.h"			/* LPC23XX/24xx Peripheral Registers */
#include "type.h"

#define ADC_CHANNEL_2		0x00000004
#define ADC_PRESCALE_TICKS	0x00000600
#define ADC_ENABLE			(1<<21)

#define ADC_START			(1<<24)
#define ADC_START_ON_MAT0_1	(1<<26)
#define ADC_FALLING_EDGE	(1<<27)
#define ADC_TIMED_START		(ADC_CHANNEL_2 | ADC_PRESCALE_TICKS | ADC_ENABLE | ADC_START_ON_MAT0_1 )
#define ADC_INIT_MASK		(ADC_CHANNEL_2 | ADC_PRESCALE_TICKS | ADC_ENABLE | ADC_START )



/**
 * Initialization of ADC port.\n
 *
 * @param channel  	Which ADC port to setup (0-7)
 */
int initAdc(int channel)
{
	/* currently only channel 2 P0.25 is supported */
	if(channel != 2)
		return -1;

	/*  1. Power: In the PCONP register (Table 4Ð63), set bits PCADC.
		Remark: On reset, the ADC is disabled. To enable the ADC, first set the PCADC bit,
		and then enable the ADC in the AD0CR register (bit PDN) Table 28Ð593. To disable
		the ADC, first clear the PDN bit, and then clear the PCADC bit.*/

	/* #foot note in PCONP register description */
	/* first disable ADC */
	AD0CR = 0;
	/* then enable power */
	PCONP |= (1<<12);

	/*
	2. Clock: In the PCLK_SEL0 register (Table 4Ð56), select PCLK_ADC.
	To scale the clock for the ADC, see Table 28Ð593 bits CLKDIV.
	 */
	/* ADC clk = PCLK 0 28,8M , clr bit 24 and 25, set24*/
	PCLKSEL0 &=  ~((1<<25)|(1<<24)) ;
	PCLKSEL0 |=    (1<<24);

	/*3. Pins: Select ADC pins and pin modes in registers PINSELn and PINMODEn */
	PINSEL1 &=  ~((1<<19)|(1<<18)) ;
	PINSEL1 |=    (1<<18);
	/* disable pull up/down's */
	PINMODE1 &=  ~((1<<19)|(1<<18)) ;
	PINMODE1 |=    (1<<19);

	/* if timer match is active*/
	/* channel 2, SAR speed 4,5MHz, start conversion when TMR0 MATCH1 occurs */
	if(T0MCR & 0x18)
		AD0CR = ADC_TIMED_START;
	else
		AD0CR = ADC_INIT_MASK;

	/* use IRQ bits to signal done*/
	AD0INTEN = 0x1FF;

	return 0;
}

/**
 * Perform 10 bit ADC conversion on the selected channel
 *
 * @return 10 bit ADC value
 *
 */
int readAdc( int channel)
{
	int stat;

	stat = AD0GDR;

	/* bit 16 ADINT set when any conversion finishes */
	while( !(stat & 0x80000000))
		stat = AD0GDR;

	AD0CR =   ADC_TIMED_START;

	/* shift and mask to a readable 10 bit number */
	return ((stat >> 6) & 0x3FF);
	//return ((AD0GDR >> 6) & 0x3FF);
}
