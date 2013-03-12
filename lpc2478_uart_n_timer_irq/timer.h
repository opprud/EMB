/*
 * timer.h
 *
 *  Created on: 09/09/2010
 *      Author: morten
 */

#ifndef TIMER_H_
#define TIMER_H_

/** defines for soft-timers */
#define RUN 	0
#define RESET 	1

/** prototypes */
//signed int initTimer(int TimerNum, int IntervalMs, tmrFunc callback);
signed int initTimer(int TimerNum, int IntervalMs);
void enableTimer( int timerNum );
void disableTimer( int timerNum );
int getTimerTicks(int timerNum);
int getTimerResolution(int timerNum);
int timer(int timerNum, int clear);

#endif /* TIMER_H_ */
