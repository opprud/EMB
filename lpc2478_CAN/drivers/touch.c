/**
 * @ingroup drv
 * @file
 * @brief Touch controller driver
 * Copyright:
 *    (C) 2000 - 2009 Embedded Artists AB
 *
 * Description:
 *    Touch screen.
 *
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/

#include <stdlib.h> 
#include <string.h>

#include "touch.h"
#include "calibrate.h"
#include <lpc2468.h>
#include "config.h"

/******************************************************************************
 * Defines, macros, and typedefs
 *****************************************************************************/

typedef struct
{
	tU32 validatePattern1;
	MATRIX storedMatrix;
	tU32 validatePattern2;
} tStoredCalData;

#define CS_PIN            0x00100000        //P0.20
#define SPI_CLOCK_DIVIDER ((Fpclk / 2500000)+1)

/******************************************************************************
 * Public variables
 *****************************************************************************/

/******************************************************************************
 * Local variables
 *****************************************************************************/
static tStoredCalData storedCalData;
static tBool calibrated;

static unsigned short (*spifunc)(unsigned short c);

/******************************************************************************
 * Local functions
 *****************************************************************************/


/***********************************************************************
 *
 * Function: touch_cs_low
 *
 * Purpose: Chip select Low for touch panel
 *
 * Parameters: None
 *
 * Outputs: None
 *
 * Returns: None
 *
 **********************************************************************/
static void touch_cs_low(void)
{
	IOCLR0 = CS_PIN;
}

/***********************************************************************
 *
 * Function: touch_cs_high
 *
 * Purpose: Chip select High for touch panel
 *
 * Parameters: None
 *
 * Outputs: None
 *
 * Returns: None
 *
 **********************************************************************/
static void touch_cs_high(void)
{
	IOSET0 = CS_PIN;
}

/***********************************************************************
 *
 * Function: tp_xfer
 *
 * Purpose: Transfer data to/from touch panel
 *
 * Processing:
 *     See function.
 *
 * Parameters:
 *     out   : Output data
 *     in    : Input data
 *     bytes : Number of bytes to send/receive
 *
 * Outputs: None
 *
 * Returns: TRUE if the byte was transferred
 *
 * Notes: Do not use this function to transfer more than 8 bytes!
 *
 **********************************************************************/
static void tp_xfer(tU8 *out, tU8 *in, int bytes)
{
	int rbytes = 0;

	while (rbytes < bytes)
	{
		// transfer, and cast to chars since we only use bytes
		in[rbytes] = (char)spifunc((char)out[rbytes]);
		rbytes++;
	}
}

/***********************************************************************
 *
 * Function: spi_read_tp
 *
 * Purpose: Read data from touch panel
 *
 * Parameters:
 *     cmd   : Touch controller command
 *
 * Outputs: None
 *
 * Returns: Read data
 *
 **********************************************************************/
static tU16 spi_read_tp(tU8 command)
{
	tU8 datao[6];
	tU8 result[6];

	datao[0] = command;
	datao[1] = 0;
	datao[2] = 0;

	touch_cs_low();
	tp_xfer(datao, &result[0], 3);
	touch_cs_high();

	return (((tU16) result[1]) << 8) | (tU16) result[2];
}

/******************************************************************************
 * Implementation of public functions
 *****************************************************************************/

/***********************************************************************
 *
 * Function: touch_calibrate
 *
 * Purpose: Store calibration data for the touch panel. 
 *
 * Parameters:
 *
 * Outputs: None
 *
 * Returns: None
 *
 **********************************************************************/
void touch_calibrate(tTouchPoint ref1, tTouchPoint ref2, tTouchPoint ref3, tTouchPoint scr1, tTouchPoint scr2,
		tTouchPoint scr3)
{
	POINT disp[3];
	POINT scr[3];

	disp[0].x = ref1.x;
	disp[0].y = ref1.y;
	disp[1].x = ref2.x;
	disp[1].y = ref2.y;
	disp[2].x = ref3.x;
	disp[2].y = ref3.y;

	scr[0].x = scr1.x;
	scr[0].y = scr1.y;
	scr[1].x = scr2.x;
	scr[1].y = scr2.y;
	scr[2].x = scr3.x;
	scr[2].y = scr3.y;

	setCalibrationMatrix(disp, scr, &storedCalData.storedMatrix);

	calibrated = TRUE;

}

/***********************************************************************
 *
 * Function: touch_xyz
 *
 * Purpose: Get x, y and z (pressure) value from touch panel 
 *
 * Parameters: None
 *
 * Outputs:
 *    x : x value
 *    y : y value
 *    z: pressure value
 *
 * Returns: None
 *
 **********************************************************************/
void touch_xyz(tS32 *x, tS32* y, tS32* z)
{
	tS32 ix, iy, iz1, iz2 = 0;
	POINT displayPoint, screenSample;

	ix = spi_read_tp(0x83 | 0x50 | 0x00);
	ix >>= 3;

	//Read Y value
	iy = spi_read_tp(0x83 | 0x10 | 0x00);
	iy >>= 3;

	//Read Z1 value
	iz1 = spi_read_tp(0x83 | 0x30 | 0x00);
	iz1 >>= 3;

	//Read Z2 value
	iz2 = spi_read_tp(0x83 | 0x40 | 0x00);
	iz2 >>= 3;

	*z = (ix * iz2 - iz1) / iz1;
	if (*z > 40000)
		*z = 0;

	if (calibrated)
	{
		screenSample.x = ix;
		screenSample.y = iy;
		getDisplayPoint(&displayPoint, &screenSample, &(storedCalData.storedMatrix));
		*x = displayPoint.x;
		*y = displayPoint.y;
	}
	else
	{
		*x = ix;
		*y = iy;
	}
}

/***********************************************************************
 *
 * Function: touch_temp
 *
 * Purpose: Get tempature value from touch panel 
 *
 * Parameters: None
 *
 * Outputs: None
 *
 * Returns: Tempature value
 *
 **********************************************************************/
tU32 touch_temp(void)
{

	tU32 t0 = 0;
	tU32 t1 = 0;

	t0 = spi_read_tp(0x83 | 0x00 | 0x04);
	t0 >>= 3;

	t1 = spi_read_tp(0x83 | 0x70 | 0x04);
	t1 >>= 3;
	t0 = (2573 * (t1 - t0) - 273000) / 1000;

	return t0;
}

/***********************************************************************
 *
 * Function: touch_init
 *
 * Purpose: Initialize touch
 *
 * Parameters: None
 *
 * Outputs: None
 *
 * Returns: None
 *
 **********************************************************************/
//void touch_init(void)
void touch_init(unsigned short (*pSpifunc)(unsigned short c))
{
	/* assign spi function */
	spifunc = pSpifunc;
	/* init SPI CS pin */
	IODIR0 |= CS_PIN;
	IOSET0 = CS_PIN;
}

