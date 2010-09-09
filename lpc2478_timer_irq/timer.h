/*
 * timer.h
 *
 *  Created on: 09/09/2010
 *      Author: morten
 */

#ifndef TIMER_H_
#define TIMER_H_

/** typedefs */
typedef void (*tmrFunc);
typedef void (*cb)(void);

/** prototypes */
//signed int initTimer(int TimerNum, int IntervalMs, tmrFunc callback);
signed int initTimer(int TimerNum, int IntervalMs);
void enableTimer( int timerNum );
void disableTimer( int timerNum );

#endif /* TIMER_H_ */
