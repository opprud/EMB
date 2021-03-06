/**
 * @file
 * @ingroup drv
 * @brief Module to handle LPC2xxx SPIport
 *
 *  Created on: 12/10/2010
 *      Author: morten
 */
#include "lpc2468.h"
#include "spi.h"
#include "config.h"
#include "type.h"

short wSPIErrors[4] =
{ 0, 0, 0, 0 };

/**
 * Initialization of SPI port.\n
 *
 * @param nFramesize  	Bits per frame 8...16
 * @param wClock 		Desired clock speed in KHz
 * @param bCPHA   		CPHA - clock phase alignment (EDGE1ST or EDGE2ND)
 * @param bCPOL   		CPOL - Clock polarity (ALOW or AHIGH)
 * @param bLSBF  		LSBF - LSB first (LSB or MSB)
 *
 */
void SPIInit(short wClock, char nFramesize, char bCPHA, char bCPOL, char bLSBF)
{
	short wTempClock = 0;

	// Calculate SPI clock value
	wTempClock = (Fpclk / 1000) / wClock;

	// According to doc, this value must be at least 8
	if (wTempClock < 8)
		wTempClock = 8;

	// The SPI clock value must be an even number
	// If odd add 1 to get an even number
	if ((wTempClock % 2) != 0)
		wTempClock++; //

	// Set clock speed
	S0SPCCR = wTempClock;

	// Clear register
	S0SPCR = 0x00;

	// P0.15-SCK, P0.17-MISO, P0.18-MOSI
	PINSEL0 |= 0xc0000000;
	PINSEL1 |= 0x0000003c;

	// enusure framesize is between 8..16
	if((nFramesize < 8) || (nFramesize > 16))
		nFramesize = 8;

	S0SPCR =(nFramesize << 8) |	// setup bits pr. frame length
			(bCPHA << 3) | 		// Setup of clock phase alignment
			(bCPOL << 4) | 		// Setup of clock polarity
			(BIT5) | 			// Setup of master mode
			(bLSBF << 6); 		// Setup of data alignment
}

/**
 * Error handler for SPI port. If an error can be read from the \n
 * status register, the errors are counted up.\n
 *
 *
 * @return
 * TRUE  : No errors\n
 * FALSE : Error present\n
 *
 */
char SPIError(void)
{
	char bAnyError = FALSE;

	if ((S0SPSR & BIT3) == 1)
	{
		SPISLAVEABORT++;
		bAnyError = TRUE;
	} // Slave abort error
	if ((S0SPSR & BIT4) == 1)
	{
		SPIMODEFAULT++;
		bAnyError = TRUE;
	} // Mode fault
	if ((S0SPSR & BIT5) == 1)
	{
		SPIREADOVERRUN++;
		bAnyError = TRUE;
	} // Read buffer overrun
	if ((S0SPSR & BIT6) == 1)
	{
		SPIWRITECOL++;
		bAnyError = TRUE;
	} // Write collision

	return bAnyError;
}

/**
 * Read and write data from SPI port 0.
 *
 * The data read from the slave device is returned.
 *
 * @param  nData Data TX on SPI0
 * @return RX Data from SPI0
 *
 */
unsigned short SpiXfer(unsigned short nData)
{
	short i = 0;

	S0SPDR = nData; // Put one byte in the data register (bidirectional)

	while ((S0SPSR & BIT7) == 0) // Wait until data is send
	{
		if (i++ > 500) // Ensure software does not hang
			break;
	}

	SPIError(); // Error handling

	return S0SPDR; // Read SPI port data
}
