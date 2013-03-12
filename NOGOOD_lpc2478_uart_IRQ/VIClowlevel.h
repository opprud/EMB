/*
 * VIClowlevel.h
 *
 *  Created on: 26/08/2010
 *      Author: Administrator
 */

#ifndef VICLOWLEVEL_H_
#define VICLOWLEVEL_H_

unsigned enableIRQ(void);
unsigned disableIRQ(void);
unsigned restoreIRQ(unsigned oldCPSR);
#define IRQ_MASK 0x00000080

#endif /* VICLOWLEVEL_H_ */
