/******************************************************************************
 *   irq.h:  Interrupt related Header file for NXP LPC23xx/24xx Family 
 *   Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2006.09.01  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#ifndef __ADC_H
#define __ADC_H

void initAdc(int channel);
int readAdc( int channel);

#endif /* end __IRQ_H */

/******************************************************************************
**                            End Of File
******************************************************************************/
