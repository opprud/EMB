/*
 * pwm.c
 *
 *  Created on: 16/09/2010
 *      Author: morten
 */
#include "type.h"
#include "lpc2468.h"

/**
 * initPwm
 * This function  initializes a PWM generate periodic signal .
 *
 * @param pwm_num: 0, 1
 * @param period: ticks per period
 *
 * @remarks currently only pwm 1.1 supported
 */
int initPwm(int period, int pwm_num, int output)
{
	if (pwm_num == 0)
	{
		return FAIL;
	}
	else if (pwm_num == 1)
	{
		if (output == 1)
		{
			//set P1.18 as PWM output (PWM1.1, second alternative function)
			PINSEL3 &= 0xFFFFFFCF;
			PINSEL3 |= 0x00000020;
			PWM1LER = 0x03; //latch MR0 and MR1
			PWM1PCR = 0x0200; //enable PWMENA1
			PWM1MCR = 0x02; //reset counter if MR0 match
			PWM1MR0 = period; //period time
			PWM1MR1 = 0x0000;
		}
		else
			return FAIL;

		PWM1PR = 0x00; //no prescaling
		PWM1TCR = 0x09; //enable counter and PWM

		return OK;
	}
	return FAIL;

}

int setPwmDc(int dc, int pwm_num, int output)
{
	if (pwm_num == 1)
	{
		if (output == 1)
		{
			//PWM1MR1 = ((unsigned long) 0x3000 * (unsigned long) (100 - percent)) / (unsigned long) 100;
			PWM1MR1 = ((unsigned long) PWM1MR0 * (unsigned long) (100 - dc)) / (unsigned long) 100;
			PWM1LER = 0x02;
			return OK;
		}
	}
	return FAIL;
}
