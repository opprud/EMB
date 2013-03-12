/*
 * pwh.h
 *
 *  Created on: 16/09/2010
 *      Author: morten
 */

#ifndef PWH_H_
#define PWH_H_

int initPwm(int period, int pwm_num, int output);
int setPwmDc(int dc, int pwm_num, int output);

#endif /* PWH_H_ */
