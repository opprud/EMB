/******************************************************************************
 *
 * Copyright:
 *    (C) 2000 - 2008 Embedded Artists AB
 *
 * Description:
 *    Touch sensor.
 *
 *****************************************************************************/

#ifndef _TOUCH__h
#define _TOUCH__h

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "type.h"

typedef struct
{
  tU16 x;
  tU16 y;
} tTouchPoint;


/******************************************************************************
 * Public variables
 *****************************************************************************/


/******************************************************************************
 * Public functions
 *****************************************************************************/

void touch_xyz(tS32 *x, tS32* y, tS32* z);
tU32 touch_temp(void);
void touch_init(void);
void touch_calibrate(tTouchPoint ref1, tTouchPoint ref2, tTouchPoint ref3, 
  tTouchPoint scr1, tTouchPoint scr2, tTouchPoint scr3);


#endif
