/*****************************************************************************
 *   dma.c:  DMA module file for NXP LPC23xx/24xx Family Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2006.07.20  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#include <config.h>
#include <common.h>
#include <asm/arch/hardware.h>
#include "mci.h"
#include "dma.h"
#include "irqVec.h"

#if MCI_DMA_ENABLED
/******************************************************************************
** Function name:		DMAHandler
**
** Descriptions:		DMA interrupt handler for MCI
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void DMAHandler (void)  //__irq
{
  DWORD regVal;
  static DWORD DMATCCount, DMAErrCount;

  regVal = GPDMA_INT_TCSTAT;
  if ( regVal )
  {
	DMATCCount++;
	GPDMA_INT_TCCLR |= regVal;
  } 

  regVal = GPDMA_INT_ERR_STAT;
  if ( regVal )
  {
	DMAErrCount++;
	GPDMA_INT_ERR_CLR |= regVal;
  } 
  VICVectAddr = 0;		/* Acknowledge Interrupt */
}

/******************************************************************************
** Function name:		DMA_Init
**
** Descriptions:		Setup GPDMA clock and install interrupt handler		
**
** parameters:			None			
** Returned value:		TRUE or FALSE, FALSE if ISR can't be installed.		
** 
******************************************************************************/
DWORD DMA_Init(void)
{
  PCONP |= (1 << 29);	/* Enable GPDMA clock */

  /* clear all interrupts on channel 0 and 1 */
  GPDMA_INT_TCCLR = 0x03;
  GPDMA_INT_ERR_CLR = 0x03;

  GPDMA_CONFIG = 0x01;	/* Enable DMA channels, little endian */
  while ( !(GPDMA_CONFIG & 0x01) );

  if ( install_irq( GPDMA_INT, (void *)DMAHandler, HIGHEST_PRIORITY ) == FALSE )
  {
	printf("\nerror installing DMA interrupt handler\n");
	return( FALSE );
  }

  return( TRUE );
}  

/******************************************************************************
** Function name:		DMA_Move
**
** Descriptions:		Setup GPDMA for MCI DMA transfer
**						including mode, M2P or M2M, or P2M,
**						src and dest. address, control reg. etc.		
**
** parameters:			Channel number, DMA mode			
** Returned value:		TRUE or FALSE		
** 
******************************************************************************/
DWORD DMA_Move( DWORD ChannelNum, DWORD DMAMode )
{
  /* USB RAM is used for test.
  Please note, Ethernet has its own SRAM, but GPDMA can't access
  that. GPDMA can access USB SRAM and IRAM. Ethernet DMA controller can 
  access both IRAM and Ethernet SRAM. */
  if ( ChannelNum == 0 )
  {
	GPDMA_INT_TCCLR = 0x01;   
	GPDMA_INT_ERR_CLR = 0x01;
	if ( DMAMode == M2M )
	{
	  /* Ch0 is set for M2M tranfer from AHB1 to AHB2 SRAM */
	  GPDMA_CH0_SRC = DMA_SRC;
	  GPDMA_CH0_DEST = DMA_DST;
	  /* Terminal Count Int enable */
	  GPDMA_CH0_CTRL = (DMA_SIZE & 0x0FFF) | (0x04 << 12) | (0x04 << 15) 
			| (0x02 << 18) | (0x02 << 21) | (1 << 26) | (1 << 27) | 0x80000000;	
	}
	else if ( DMAMode == M2P )
	{
	  /* Ch0 set for M2P transfer from mempry to MCI FIFO. */
	  GPDMA_CH0_SRC = DMA_SRC;
	  GPDMA_CH0_DEST = DMA_MCIFIFO;
	  /* The burst size is set to 8, the size is 8 bit too. */
	  /* Terminal Count Int enable */
	  GPDMA_CH0_CTRL = (DMA_SIZE & 0x0FFF) | (0x04 << 12) | (0x02 << 15)
			| (0x02 << 18) | (0x02 << 21) | (1 << 26) | 0x80000000;
	}
	else if ( DMAMode == P2M )
	{
	  /* Ch0 set for P2M transfer from MCI FIFO to memory. */
	  GPDMA_CH0_SRC = DMA_MCIFIFO;
	  GPDMA_CH0_DEST = DMA_DST;
	  /* The burst size is set to 8, the size is 8 bit too. */
	  /* Terminal Count Int enable */
	  GPDMA_CH0_CTRL = (DMA_SIZE & 0x0FFF) | (0x02 << 12) | (0x04 << 15)
			| (0x02 << 18) | (0x02 << 21) | (1 << 27) | 0x80000000;
	}
	else
	{
	  return ( FALSE );
	}
  }
  else if ( ChannelNum == 1 )
  {   
	GPDMA_INT_TCCLR = 0x02;   
	GPDMA_INT_ERR_CLR = 0x02;
	if ( DMAMode == M2M )
	{
	  /* Ch1 is set for M2M tranfer */
	  GPDMA_CH1_SRC = DMA_SRC;
	  GPDMA_CH1_DEST = DMA_DST;
	  /* Terminal Count Int enable */
	  GPDMA_CH1_CTRL = (DMA_SIZE & 0x0FFF) | (0x04 << 12) | (0x04 << 15) 
			| (0x02 << 18) | (0x02 << 21) | (1 << 26) | (1 << 27) | 0x80000000;	
	}
	else if ( DMAMode == M2P )
	{
	  /* Ch1 set for M2P transfer from memory to MCI FIFO. */
	  GPDMA_CH1_SRC = DMA_SRC;
	  GPDMA_CH1_DEST = DMA_MCIFIFO;
	  /* The burst size is set to 8, the size is 8 bit too. */
	  /* Terminal Count Int enable */
	  GPDMA_CH1_CTRL = (DMA_SIZE & 0x0FFF) | (0x04 << 12) | (0x02 << 15)
			| (0x02 << 18) | (0x02 << 21) | (1 << 26) | 0x80000000;
	}
	else if ( DMAMode == P2M )
	{
	  /* Ch1 set for P2M transfer from MCI_FIFO to memory. */
	  GPDMA_CH1_SRC = DMA_MCIFIFO;
	  GPDMA_CH1_DEST = DMA_DST;
	  /* The burst size is set to 8, the size is 8 bit too. */
	  /* Terminal Count Int enable */
	  GPDMA_CH1_CTRL = (DMA_SIZE & 0x0FFF) | (0x02 << 12) | (0x04 << 15)
			| (0x02 << 18) | (0x02 << 21) | (1 << 27) | 0x80000000;
	}
	else
	{
	  return ( FALSE );
	}
  }
  else
  {
	return ( FALSE );
  }
	
  GPDMA_CONFIG = 0x01;	/* Enable DMA channels, little endian */
  while ( !(GPDMA_CONFIG & 0x01) );    
  return (TRUE);
}

#endif

/******************************************************************************
**                            End Of File
******************************************************************************/

