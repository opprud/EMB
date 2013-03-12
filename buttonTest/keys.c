/*
 * keys.c
 * the joystick keys are mapped on the following gpios
 * DOWN   P.2.27
 * UP     P.2.25
 * RIGHT  P.2.26
 * LEFT   P.2.23
 * CENTER P.2.22
 *
 *  Created on: 16/09/2010
 *      Author: morten
 */
#include "lpc2468.h"
#include "keys.h"

#define KEY_MASK (CENTER | LEFT | RIGHT | UP | DOWN)

/*
 * return the value read on the FIO port
 * */
int readKeys(void)
{
	int val;

	// buttons are active low, so invert
	val = ~FIO2PIN;

	// mask all button pins
	val &= KEY_MASK;

	// return keypress
	return val;
}
