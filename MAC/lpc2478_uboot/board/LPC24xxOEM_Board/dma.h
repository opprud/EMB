/*****************************************************************************
 *   dma.h:  Header file for NXP LPC23xx/24xx Family Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2006.07.20  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#ifndef __DMA_H 
#define __DMA_H

#define DWORD unsigned long

#define DMA_SRC			0x7FD00000		/* This is the area original data is stored
										or data to be written to the SD/MMC card. */
#define DMA_DST			0x7FD01000		/* This is the area, after writing to the SD/MMC,
										data read from the SD/MMC card. */
#define DMA_MCIFIFO		0xE008C080
#define DMA_SIZE		BLOCK_LENGTH	/* DMA_SIZE is the same BLOCK_LENGTH defined in mci.h */

/* DMA mode */
#define M2M				0x00
#define M2P				0x01
#define P2M				0x02
#define P2P				0x03

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

extern void DMAHandler( void ) ; //__irq; 
extern DWORD DMA_Move( DWORD ChannelNum, DWORD DMAMode );
extern DWORD DMA_Init(void);

#endif /* end __DMA_H */
/****************************************************************************
**                            End Of File
****************************************************************************/
