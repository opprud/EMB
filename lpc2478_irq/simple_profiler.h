/**
 * @file
 * @author  Morten Opprud <morten@hih.au.dk>
 * @version 1.0
 * @date    06/01/2011
 *
 * @section DESCRIPTION
 * GPIO defines for simple GPIO based profiling
 */

#ifndef SIMPLE_PROFILER_H_
#define SIMPLE_PROFILER_H_

#include "lpc2468.h"

/* ADJUST CONSTANTS BELOW TO FIT WHATEVER AVAILIBLE GPIO PINS YOU HAVE ! */
/* Constants for GPIO Profiling */
#define PROF_PIN_1	(1<<26)	//P2.26 J3-57	//P3.17 - J 1.59
#define PROF_PIN_2	(1<<16)	//P2.21 J3-55	//P3.16 - J 1.60
#define PROF_PIN_3	(1<<19)	//P2.27 J3-49	//P3.19 - J 1.57
#define PROF_PIN_4	(1<<18)	//P2.22 J3-47	//P3.18 - J 1.58
#define PROF_PIN_5	(1<<21)	//P2.14 J3-45	//P3.21 - J 1.55
#define PROF_PIN_6	(1<<20)	//P2.19 J3-46	//P3.20 - J 1.56
#define PROF_PIN_MASK (PROF_PIN_1|PROF_PIN_2|PROF_PIN_3|PROF_PIN_4|PROF_PIN_5|PROF_PIN_6)

/* FIOBANK used */
#define PROF_DIR	FIO2DIR
#define PROF_SET	FIO2SET
#define PROF_CLR	FIO2CLR

#endif /* SIMPLE_PROFILER_H_ */
