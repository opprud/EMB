/*
 *  VIClowlevel.c
 *  IRQ household functions
 *
 *  Created on: 26/08/2010
 *  Author: morten@hih.au.dk
 */
#include "VIClowlevel.h"
static inline unsigned asm_get_cpsr(void)
{
	unsigned long retval;
	asm volatile (" mrs  %0, cpsr" : "=r" (retval) : /* no inputs */);
	return retval;
}

static inline void asm_set_cpsr(unsigned val)
{
	asm volatile (" msr  cpsr, %0" : /* no outputs */: "r" (val) );
}

unsigned enableIRQ(void)
{
	unsigned _cpsr;
	_cpsr = asm_get_cpsr();
	asm_set_cpsr(_cpsr & ~IRQ_MASK);
	return _cpsr;
}

unsigned disableIRQ(void)
{
	unsigned _cpsr;
	_cpsr = asm_get_cpsr();
	asm_set_cpsr(_cpsr | IRQ_MASK);
	return _cpsr;
}

unsigned restoreIRQ(unsigned oldCPSR)
{
	unsigned _cpsr;
	_cpsr = asm_get_cpsr();
	asm_set_cpsr((_cpsr & ~IRQ_MASK) | (oldCPSR & IRQ_MASK));
	return _cpsr;
}

