/*
 * spi.h
 *
 *  Created on: 12/10/2010
 *      Author: morten
 */

#ifndef SPI_H_
#define SPI_H_

extern short wSPIErrors[4];

/* SUPORTED SPI PORTS */
#define SPIPORT0        0
#define SPIPORT1        1

/* CS pins */
#define CS0IODIR IODIR0
#define CS_PIN   (1 << 20)

/* SETUP DEFINES */
#define MSB             0
#define LSB             1
#define EDGE1ST         0
#define EDGE2ND         1
#define AHIGH           0
#define ALOW            1

/* ERROR DEFINES  */
#define SPISLAVEABORT   wSPIErrors[0]
#define SPIMODEFAULT    wSPIErrors[1]
#define SPIREADOVERRUN  wSPIErrors[2]
#define SPIWRITECOL     wSPIErrors[3]

// SUPPORTED CLOCK
#define CLK10M                10000                           // 10000 kHz
#define CLK5M                 5000                            // 5000 kHz
#define CLK2M5                2500                            // 2500 kHz
#define CLK2M                 2000                            // 2000 kHz
#define CLK1M                 1000                            // 1000 kHz
#define CLK500K               500                             // 500 kHz
#define CLK400K               400                             // 400 kHz
#define CLK300K               300                             // 300 kHz
#define CLK200K               200                             // 200 kHz
#define CLK100K               100                             // 100 kHz
#define CLK50K                50                              // 50 kHz
#define CLK25K                25                              // 25 kHz

void SPIInit (short wClock,                             // Desired clock speed
			  char nFramesize,                          // no of bits pr frame
		      char bCPHA,                               // CPHA - clock phase alignment (EDGE1ST or EDGE2ND)
              char bCPOL,                               // CPOL - Clock polarity (ALOW or AHIGH)
              char bLSBF);                              // LSBF - LSB first (LSB or MSB)


unsigned short SpiXfer(unsigned short nData);


#endif /* SPI_H_ */
