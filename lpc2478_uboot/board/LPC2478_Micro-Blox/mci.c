/*****************************************************************************
 *   mci.c:  SD/MMC module file for NXP LPC23xx/24xx Family Microprocessors
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

/* these are filled out by a call to mmc_hw_get_parameters */
extern int hw_size;		/* in kbytes */
extern int hw_nr_sects;
extern int hw_sect_size;	/* in bytes */

#if defined(CONFIG_LPC2468)
#define FIX_P1_6_PROBLEM
#endif

#define MCIPWR_ACTIVE_LOW

#define HIGHEST_PRIORITY	0x01
#define MCI_INT			24

volatile DWORD MCI_DataErrorProcess_count = 0;
volatile DWORD MCI_DATA_END_InterruptService_count = 0;
volatile DWORD MCI_FIFOInterruptService_count = 0;
volatile DWORD MCI_CmdProcess_count = 0;

volatile DWORD CmdCRCErrCount = 0;
volatile DWORD CmdTimeoutErrCount = 0;
volatile DWORD CmdRespEndCount = 0;
volatile DWORD CmdSentCount = 0;
volatile DWORD CmdActiveCount = 0;

volatile DWORD DataCRCErrCount = 0;
volatile DWORD DataTimeoutErrCount = 0;
volatile DWORD DataTxUnderrunErrCount = 0;
volatile DWORD DataRxOverrunErrCount = 0;
volatile DWORD DataStartbitErrCount = 0;

volatile DWORD DataEndCount = 0;
volatile DWORD DataBlockEndCount = 0;
volatile DWORD DataTxActiveCount = 0;
volatile DWORD DataRxActiveCount = 0;

volatile DWORD MCI_Block_End_Flag = 0;

volatile DWORD DataFIFOCount = 0;
volatile DWORD DataRxFIFOCount = 0;
volatile DWORD DataTxFIFOCount = 0;

volatile DWORD CardRCA;
volatile DWORD MCI_CardType;


extern DWORD MCI_ReadFifo(DWORD *dest);
extern DWORD MCI_WriteFifo(DWORD *src);

#if MCI_DMA_ENABLED
volatile unsigned char *WriteBlock = (unsigned char *)(DMA_SRC);
volatile unsigned char *ReadBlock  = (unsigned char *)(DMA_DST);
volatile DWORD TXBlockCounter = 0, RXBlockCounter = 0;
#else
extern volatile tU8 WriteBlock[BLOCK_LENGTH], ReadBlock[BLOCK_LENGTH];
extern volatile DWORD TXBlockCounter, RXBlockCounter;
#endif

static unsigned char csddata[16];
static unsigned char ciddata[16];


/******************************************************************************
** Function name:		MCI_Interrupt related
**
** Descriptions:		MCI interrupt handler and related APIs
**
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void MCI_TXEnable( void )
{
	MCI_MASK0 |= ((DATA_END_INT_MASK) | (ERR_TX_INT_MASK));	/* Enable TX interrupts only */
	MCI_MASK1 |= ((DATA_END_INT_MASK) | (ERR_TX_INT_MASK));  	
}

void MCI_TXDisable( void )
{
	MCI_MASK0 &= ~((DATA_END_INT_MASK) | (ERR_TX_INT_MASK));	/* Disable TX interrupts only */
	MCI_MASK1 &= ~((DATA_END_INT_MASK) | (ERR_TX_INT_MASK));
}

/*****************************************************************/
void MCI_RXEnable( void )
{
	MCI_MASK0 |= ((DATA_END_INT_MASK) | (ERR_RX_INT_MASK));	/* Enable RX interrupts only */
	MCI_MASK1 |= ((DATA_END_INT_MASK) | (ERR_RX_INT_MASK));  
}

void MCI_RXDisable( void )
{
	MCI_MASK0 &= ~((DATA_END_INT_MASK) | (ERR_RX_INT_MASK));	/* Disable RX interrupts only */
	MCI_MASK1 &= ~((DATA_END_INT_MASK) | (ERR_RX_INT_MASK));  	
}

/******************************************************************************
** Function name:		MCI_CheckStatus
**
** Descriptions:		MCI Check status before and after the block read and 
**						write. Right after the block read and write, this routine 
**						is important that, even the FIFO is empty, complete
**						block has been sent, but, data is still being written
**						to the card, this routine is to ensure that the data
**						has been written based on the state of the card, not
**						by the length being set.  
**
** parameters:			None
** Returned value:		TRUE or FALSE
** 
******************************************************************************/
DWORD MCI_CheckStatus( void )
{
	DWORD respValue;
	while ( 1 )
	{
		if ( (respValue = MCI_Send_Status()) == INVALID_RESPONSE )
		{
			break;
		}
		else
		{
			/* The only valid state is TRANS per MMC and SD state diagram.
			RCV state may be seen, but, I have found that it happens
			only when TX_ACTIVE or RX_ACTIVE occurs before the WRITE_BLOCK and 
			READ_BLOCK cmds are being sent, which is not a valid sequence. */  
			if ( (respValue & (0x0F << 8)) == 0x0900 )
			{
				return ( TRUE );
			}
		}
	}
	return ( FALSE );
}

/******************************************************************************
** Function name:		MCI_CmdProcess
**
** Descriptions:		Called by MCI interrupt handler
**						To simplify the process, for card initialization, the 
**						CMD interrupts are disable.
**						
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void MCI_CmdProcess( void )
{
	DWORD MCIStatus;

	MCIStatus = MCI_STATUS;
	if ( MCIStatus &  MCI_CMD_CRC_FAIL )
	{
		CmdCRCErrCount++;
		MCI_CLEAR = MCI_CMD_CRC_FAIL;
	}
	if ( MCIStatus &  MCI_CMD_TIMEOUT )
	{
		CmdTimeoutErrCount++;
		MCI_CLEAR = MCI_CMD_TIMEOUT;
	}
	/* Cmd Resp End or Cmd Sent */
	if ( MCIStatus &  MCI_CMD_RESP_END )
	{
		CmdRespEndCount++;
		MCI_CLEAR = MCI_CMD_RESP_END;
	}
	if ( MCIStatus &  MCI_CMD_SENT )
	{
		CmdSentCount++;
		MCI_CLEAR = MCI_CMD_SENT;
	}
	if ( MCIStatus &  MCI_CMD_ACTIVE )
	{
		CmdActiveCount++;
		MCI_CLEAR = MCI_CMD_ACTIVE;
	}
	return;
}

/******************************************************************************
** Function name:		MCI_DataErrorProcess
**
** Descriptions:		Called by MCI interrupt handler
**						Process data error. 
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void MCI_DataErrorProcess( void )
{
	DWORD MCIStatus;

	MCIStatus = MCI_STATUS;
    if ( MCIStatus &  MCI_DATA_CRC_FAIL )
	{
		DataCRCErrCount++;
		MCI_CLEAR = MCI_DATA_CRC_FAIL;
	}
	if ( MCIStatus &  MCI_DATA_TIMEOUT )
	{
		DataTimeoutErrCount++;
		MCI_CLEAR = MCI_DATA_TIMEOUT;
	}

	/* Underrun or overrun */
	if ( MCIStatus &  MCI_TX_UNDERRUN )
	{
		DataTxUnderrunErrCount++;
		MCI_CLEAR = MCI_TX_UNDERRUN;
	}
	if ( MCIStatus &  MCI_RX_OVERRUN )
	{
		DataRxOverrunErrCount++;
		MCI_CLEAR = MCI_RX_OVERRUN;
	}
	/* Start bit error on data signal */
	if ( MCIStatus &  MCI_START_BIT_ERR )
	{
		DataStartbitErrCount++;
		MCI_CLEAR = MCI_START_BIT_ERR;
	}
	return;
}

/******************************************************************************
** Function name:		MCI_DATA_END_InterruptService
**
** Descriptions:		Called by MCI interrupt handler
**						This is the last interrupt module processing 
**                      the block write and	read to and from the MM card.
**					 
**                      FIFO interrupts are also used when DMA is disabled                                   
**						This routine simply clears the
**                      MCI_Block_End_Flag, and increments counters for debug
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void MCI_DATA_END_InterruptService( void )
{
  DWORD MCIStatus;

  MCIStatus = MCI_STATUS;
  if ( MCIStatus &  MCI_DATA_END )		/* Data end, and Data block end  */
  {
	DataEndCount++;
	MCI_CLEAR = MCI_DATA_END;
	return;
  }
  if ( MCIStatus &  MCI_DATA_BLK_END )
  {
	DataBlockEndCount++;
	MCI_CLEAR =  MCI_DATA_BLK_END;
	MCI_TXDisable();
	MCI_Block_End_Flag = 0;
	return;
  }
	
  /* Tx active  */
  if ( MCIStatus & MCI_TX_ACTIVE )
  {
	DataTxActiveCount++;
  }
  /* Rx active  */
  if ( MCIStatus & MCI_RX_ACTIVE )
  {
	DataRxActiveCount++;
  }
  return;
}

/******************************************************************************
** Function name:	MCI_FIFOInterruptService
**
** Descriptions:	Called by MCI interrupt handler when using FIFO 
**					interrupts and DMA is disabled
**						
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void MCI_FIFOInterruptService( void )
{
#if !MCI_DMA_ENABLED
  DWORD MCIStatus; 

  MCIStatus = MCI_STATUS;
  if ( MCIStatus & (FIFO_TX_INT_MASK ) )
  {
	DataTxFIFOCount++;			 /* if using TX_HALF_EMPTY remove one WriteFifo below */
	if ( MCIStatus & MCI_TX_HALF_EMPTY ) /* empty is multiple of 512 block size */
	{
	  MCI_WriteFifo((DWORD *)&WriteBlock[TXBlockCounter]); /* write 8 words to fifo */
	  TXBlockCounter += 32;
	}
	if (TXBlockCounter == BLOCK_LENGTH)	/* block complete */
	{
	  TXBlockCounter = 0;
	  MCI_MASK0 &= ~(FIFO_TX_INT_MASK);  /* disable FIFO int until next block write */
	  MCI_MASK1 &= ~(FIFO_TX_INT_MASK); 
	  /* wait for SD card to complete sending data i.e MCI_DATA_BLK_END interrupt */
	}
  }
  else if ( MCIStatus & (FIFO_RX_INT_MASK) )
  {
	DataRxFIFOCount++;
	if ( MCIStatus & MCI_RX_HALF_FULL )	/* if using RX_HALF_FULL remove one ReadFIFO below */
	{
	  MCI_ReadFifo((DWORD *)&ReadBlock[RXBlockCounter]); /* read 8 words from fifo */
	  RXBlockCounter += 32;
	}
	if (RXBlockCounter == BLOCK_LENGTH)	/* block complete */
	{
	  RXBlockCounter = 0;
	}
  }
#endif
  DataFIFOCount++;
  return;
}

/******************************************************************************
** Function name:		MCI_IRQHandler
**
** Descriptions:		MCI interrupt handler
**						The handler to handle the block data write and read
**						not for the commands.
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void MCI_IRQHandler(void)
{
  DWORD MCI_Status;

  MCI_Status = MCI_STATUS;	   

  /* handle MCI_STATUS interrupt */
  if ( MCI_Status & DATA_ERR_INT_MASK )
  {
	MCI_DataErrorProcess();
	MCI_DataErrorProcess_count++;
	VICVectAddr = 0;		/* Acknowledge Interrupt */
	return;
  }
  if ( MCI_Status & DATA_END_INT_MASK )
  {
	MCI_DATA_END_InterruptService();
	MCI_DATA_END_InterruptService_count++;
	VICVectAddr = 0;		/* Acknowledge Interrupt */
	return;
  }
  else if ( MCI_Status & FIFO_INT_MASK )
  {
	MCI_FIFOInterruptService();
	MCI_FIFOInterruptService_count++;
	VICVectAddr = 0;		/* Acknowledge Interrupt */
	return;
  }
  else if ( MCI_Status & CMD_INT_MASK )
  {
	MCI_CmdProcess();
	MCI_CmdProcess_count++;
	VICVectAddr = 0;		/* Acknowledge Interrupt */
	return;
  }
  VICVectAddr = 0;		/* Acknowledge Interrupt */
}


/******************************************************************************
** Function name:		MCI_Set_MCIClock
**
** Descriptions:		Set MCI clock rate, during initialization phase < 400K
**						during data phase < 20Mhz. 		
**
** parameters:			Clock rate to be set			
** Returned value:		None
** 
******************************************************************************/
void MCI_Set_MCIClock( DWORD ClockRate )
{
  DWORD i, ClkValue = 0;

  if ( ClockRate == SLOW_RATE ) 
	ClkValue |= MCLKDIV_SLOW;	/* slow clock */
  else if ( ClockRate == NORMAL_RATE ) 
	ClkValue |= MCLKDIV_NORMAL;/* normal clock */

  MCI_CLOCK &= ~(0xFF); /* clear clock divider */
  MCI_CLOCK |= (1 << 8)  |ClkValue;
  for ( i = 0; i < 0x10; i++ );	/* delay 3MCLK + 2PCLK before next write */
  return;
}

/******************************************************************************
** Function name:		SD_Set_BusWidth
**
** Descriptions:		1-bit bus or 4-bit bus.
**
** parameters:			bus width			
** Returned value:		TRUE or FALSE
** 
******************************************************************************/
DWORD SD_Set_BusWidth( DWORD width )
{
  DWORD i;

  for ( i = 0; i < 0x10; i++ );	/* delay 3MCLK + 2PCLK  */
  if ( width == SD_1_BIT )
  { 
	MCI_CLOCK &=  ~(1 << 11);	/* 1 bit bus */
  }
  else if ( width == SD_4_BIT )
  { 
	MCI_CLOCK |=  (1 << 11);/* 4 bit bus */
  }
	
  if ( MCI_Send_ACMD_Bus_Width( BUS_WIDTH_4BITS ) == FALSE ) 
  {
	return( FALSE );
  }
  return TRUE;
}

/******************************************************************************
** Function name:		MCI_Init
**
** Descriptions:		Set MCI clock and power registers, setup VIC for
**						data interrupt.		
**
** parameters:			None
** Returned value:		true or fase, if VIC table is full, return false
** 
******************************************************************************/
DWORD MCI_Init( void )
{
  DWORD i;

	PCONP |= ( 1 << 28 );			/* Enable clock to the MCI block */


	if ( MCI_CLOCK & (1 << 8) )
	{
		MCI_CLOCK &= ~(1 << 8);
	}
	if ( MCI_POWER & 0x02 )
	{
		MCI_POWER = 0x00;
	}
	//for ( i = 0; i < 0x10000; i++ ) asm volatile("nop");
	delayMs(1000);

	/* Disable all interrupts for now */
	MCI_MASK0 = 0;
	MCI_MASK1 = MCI_MASK0;    

#ifndef FIX_P1_6_PROBLEM

  // 15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0
  // xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx
  //          10 10          10 10 10    10 10
  //      |     |     |     |     |     |     |
  //  F      C     3     F     0     3     0     F  (AND mask)
  //  0      2     8     0     A     8     A     0  (OR value)
	//PINSEL2 &= 0xFC3F030F;
	//PINSEL2 |= 0x0280A8A0;

	// p0.20 must not be set
	//PINSEL1 &= 0xFFFFC03F;

	PINSEL1 &= ~0x00003FC0;
	PINSEL1 |=  0x00002A80;
	PINSEL2 &= ~0x03C0C000;
	PINSEL2 |=  0x02808000;

#if defined(MCIPWR_ACTIVE_LOW)
	SCS &= ~0x08; 
#else
	SCS |= 0x08; 
#endif


#else

  // 31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16
  // xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx
  //                            10
  //      |     |     |     |     |     |     |
  //   F     F     F     F     C     F     F     F  (AND mask)
  //   0     0     0     0     2     0     0     0  (OR value)

	PINSEL1 &= 0xFFFFCFFF;
	PINSEL1 |= 0x00002000;

  // 15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0
  // xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx
  //          10 10          10    10    10 10
  //      |     |     |     |     |     |     |
  //  F      C     3     F     3     3     0     F  (AND mask)
  //  0      2     8     0     8     8     A     0  (OR value)
	PINSEL2 &= 0xFC3F330F;

#if defined(MCIPWR_ACTIVE_LOW)
  PINSEL2 |= 0x028080A0;
  SCS     &= ~0x08;
  IODIR1  |= 0x20;
  IOCLR1   = 0x20; 
#else  //MCIPWR active high
  PINSEL2 |= 0x028088A0;
  SCS     |= 0x08;  
#endif
#endif

//	MCI_POWER = 0x00;		/* power off for 100 mS */

	/*set up clocking default mode, clear any registers as needed */
	MCI_COMMAND = 0;
	MCI_DATA_CTRL = 0;			
	MCI_CLEAR = 0x7FF;		/* clear all pending interrupts */

	MCI_POWER = 0x02;		/* power up */
	while ( !(MCI_POWER & 0x02) );
	for ( i = 0; i < 0x100; i++ ) asm volatile("nop");

	/* During identification phase, the clock should be less than
	400Khz. Once we pass this phase, the normal clock can be set up
	to 25Mhz on SD card and 20Mhz on MMC card. */	
	MCI_Set_MCIClock( SLOW_RATE );
	MCI_POWER |= 0x01;		/* bit 1 is set already, from power up to power on */
	
	for ( i = 0; i < 0x2000; i++ ) asm volatile("nop");

/*
    if ( install_irq( MCI_INT, (void *)MCI_IRQHandler, HIGHEST_PRIORITY ) == FALSE )
    {
      printf("\nerror installing MCI interrupt handler\n");
    	return ( FALSE );
    }
*/
	/* During the initialization phase, to simplify the process, the CMD related 
	interrupts are disabled. The DATA related interrupts are enabled when
	the FIFOs are used and just before WRITE_BLOCK READ_BLOCK cmds are issues, and 
	disabled after the data block has been written and read. Please also note,
	before WRITE_BLOCK only TX related data interrupts are enabled, and before
	READ_BLOCK only RX related data interrupts are enabled. */   
    return (TRUE);
}

/******************************************************************************
** Function name:		MCI_SendCmd
**
** Descriptions:		The routine is used to send a CMD to the card		
**
** parameters:			CmdIndex, Argument, ExpectResp Flag, AllowTimeout flag
** Returned value:		None
** 
******************************************************************************/
void MCI_SendCmd( DWORD CmdIndex, DWORD Argument, DWORD ExpectResp, DWORD AllowTimeout )
{
    DWORD i, CmdData = 0;
	DWORD CmdStatus;
	
	/* the command engine must be disabled when we modify the argument
     or the peripheral resends */
	while ( (CmdStatus = MCI_STATUS) & MCI_CMD_ACTIVE )	/* Command in progress. */
	{
		MCI_COMMAND = 0;
		MCI_CLEAR = CmdStatus | MCI_CMD_ACTIVE;
	}
	for ( i = 0; i < 0x100; i++ ) asm volatile(" nop");	

    /*set the command details, the CmdIndex should 0 through 0x3F only */
	CmdData |= (CmdIndex & 0x3F);	/* bit 0 through 5 only */
	if ( ExpectResp == EXPECT_NO_RESP )			/* no response */
	{
		CmdData &= ~((1 << 6) | (1 << 7));		/* Clear long response bit as well */
	}
	else if ( ExpectResp == EXPECT_SHORT_RESP )	/* expect short response */
	{
		CmdData |= (1 << 6);
	}
	else if ( ExpectResp == EXPECT_LONG_RESP )	/* expect long response */
	{
		CmdData |= (1 << 6) | (1 << 7);
	}

	if ( AllowTimeout )			/* allow timeout or not */
	{
		CmdData |= (1 << 8);
	}
	else
	{
		CmdData &= ~(1 << 8);
	}

	/*send the command*/
	CmdData |= (1 << 10);		/* This bit needs to be set last. */
	MCI_ARGUMENT = Argument;	/* Set the argument first, finally command */
	MCI_COMMAND = CmdData;
	return;

}

/******************************************************************************
** Function name:		MCI_GetCmdResp
**
** Descriptions:		Get response from the card. This module is always used
**						in pair with MCI_SendCmd()		
**
** parameters:			Expected cmd data, expect response flag, pointer to the 
**						response
**						Expected cmd data should be the same as that in SendCmd()
**						expect response flag could be	EXPECT_NO_RESP
**														EXPECT_SHORT_RESP
**														EXPECT_LONG_RESP
**						if GetCmdResp() is 0, check the pointer to the response
**						field to get the response value, if GetCmdResp() returns 
**						non-zero, no need to check the response field, just resend
**						command or bailout. 
** Returned value:		Response status, 0 is valid response. 
** 
******************************************************************************/
DWORD MCI_GetCmdResp( DWORD ExpectCmdData, DWORD ExpectResp, DWORD *CmdResp )
{
	DWORD CmdRespStatus = 0;
	DWORD LastCmdIndex;

    if ( ExpectResp == EXPECT_NO_RESP )
	{
		return ( 0 );
	}

	while ( 1 )
	{
		CmdRespStatus = MCI_STATUS;	
		if ( CmdRespStatus & (MCI_CMD_TIMEOUT) )
		{
			MCI_CLEAR = CmdRespStatus | MCI_CMD_TIMEOUT;
			MCI_COMMAND = 0;
			MCI_ARGUMENT = 0xFFFFFFFF;
			return ( CmdRespStatus );
		}
		if (  CmdRespStatus & MCI_CMD_CRC_FAIL )
		{
			MCI_CLEAR = CmdRespStatus | MCI_CMD_CRC_FAIL;
			LastCmdIndex = MCI_COMMAND & 0x003F;
			if ( (LastCmdIndex == SEND_OP_COND) || (LastCmdIndex == SEND_APP_OP_COND) 
					|| (LastCmdIndex == STOP_TRANSMISSION) )
			{
				MCI_COMMAND = 0;
				MCI_ARGUMENT = 0xFFFFFFFF;	 
				break;			/* ignore CRC error if it's a resp for SEND_OP_COND 
								or STOP_TRANSMISSION. */
			}
			else
			{ 
				return ( CmdRespStatus );
			}
		}
		else if ( CmdRespStatus & MCI_CMD_RESP_END )
		{
			MCI_CLEAR = CmdRespStatus | MCI_CMD_RESP_END;
			break;	/* cmd response is received, expecting response */
		}
	}
	
	if ( (MCI_RESP_CMD & 0x3F) != ExpectCmdData )
	{
		/* If the response is not R1, in the response field, the Expected Cmd data
		won't be the same as the CMD data in SendCmd(). Below four cmds have
		R2 or R3 response. We don't need to check if MCI_RESP_CMD is the same
		as the Expected or not. */
		if ( (ExpectCmdData != SEND_OP_COND) && (ExpectCmdData != SEND_APP_OP_COND) 
				&& (ExpectCmdData != ALL_SEND_CID) && (ExpectCmdData != SEND_CSD) )
		{
			CmdRespStatus = INVALID_RESPONSE;	/* Reuse error status */
			return ( INVALID_RESPONSE );
		}
	}

	if ( ExpectResp == EXPECT_SHORT_RESP )
	{
		*CmdResp = MCI_RESP0;
	}
	else if ( ExpectResp == EXPECT_LONG_RESP )
	{
		*CmdResp = MCI_RESP0;
		*(CmdResp+1) = MCI_RESP1;
		*(CmdResp+2) = MCI_RESP2;
		*(CmdResp+3) = MCI_RESP3;
	}
	return ( 0 );	/* Read MCI_RESP0 register assuming it's not long response. */
}

/******************************************************************************
** Function name:		MCI_Go_Idle_State
**
** Descriptions:		CMD0, the very first command to be sent to initialize
**						either MMC or SD card.		
**
** parameters:			None
** Returned value:		true or false, true if card has been initialized.
** 
******************************************************************************/
DWORD MCI_Go_Idle_State( void )
{
	DWORD retryCount;
	DWORD respStatus;
	DWORD respValue[4];

	retryCount = 0x20;
	while ( retryCount > 0 )
	{
		/* Send CMD0 command repeatedly until the response is back correctly */
    	MCI_SendCmd( GO_IDLE_STATE, 0x00000000, EXPECT_NO_RESP, 0 );
		respStatus = MCI_GetCmdResp( GO_IDLE_STATE, EXPECT_NO_RESP, (DWORD *)respValue );
		if ( respStatus == 0 )
		{
			break;
		}
		retryCount--;
	}
	
	if ( respStatus != 0 )		/* timeout, give up */
	{
		return ( FALSE );
	}
	return( TRUE );
}

/******************************************************************************
** Function name:		MCI_Send_OP_Cond
**
** Descriptions:		CMD1 for MMC		
**
** parameters:			None
** Returned value:		true or false, true if card has response back before
**						timeout, false is timeout on the command.
** 
******************************************************************************/
DWORD MCI_Send_OP_Cond( void )
{
	volatile DWORD i, retryCount;
	DWORD respStatus;
	DWORD respValue[4];

	retryCount = 0x200;			/* reset retry counter */
	while ( retryCount > 0 )
	{
		/* Send CMD1 command repeatedly until the response is back correctly */
		MCI_SendCmd( SEND_OP_COND, OCR_INDEX, EXPECT_SHORT_RESP, 0 );
		respStatus = MCI_GetCmdResp( SEND_OP_COND, EXPECT_SHORT_RESP, (DWORD *)&respValue[0] );
		/* bit 0 and bit 2 must be zero, or it's timeout or CRC error */
		if ( !(respStatus & MCI_CMD_TIMEOUT) && (respValue[0] & 0x80000000) )
		{
			return ( TRUE );	/* response is back and correct. */
		}
		for ( i = 0; i < 0x20; i++ ) asm volatile(" nop");
		retryCount--;
	}
	return( FALSE );
}

/******************************************************************************
** Function name:		MCI_Send_ACMD
**
** Descriptions:		CMD55, before sending an ACMD, call this routine first  		
**
** parameters:			None
** Returned value:		true or false, true if card has responded before timeout.
**						false is timeout.
** 
******************************************************************************/
DWORD MCI_Send_ACMD( void )
{
	volatile DWORD i, retryCount;
	DWORD CmdArgument;
	DWORD respStatus;
	DWORD respValue[4];

	if ( MCI_CardType == SD_CARD )
	{
		CmdArgument = CardRCA;	/* Use the address from SET_RELATIVE_ADDR cmd */
	}
	else			/* if MMC or unknown card type, use 0x0. */
	{
		CmdArgument = 0x00000000;
	}

	retryCount = 20;
	while ( retryCount > 0 )
	{
		/* Send CMD55 command followed by an ACMD */
		MCI_SendCmd( APP_CMD, CmdArgument, EXPECT_SHORT_RESP, 0 );
		respStatus = MCI_GetCmdResp( APP_CMD, EXPECT_SHORT_RESP, (DWORD *)&respValue[0] );
		if ( !respStatus && (respValue[0] & CARD_STATUS_ACMD_ENABLE) )	/* Check if APP_CMD enabled */
		{
			return( TRUE );
		}
		for ( i = 0; i < 0x20; i++ ) asm volatile (" nop");
		retryCount--;
	}
	return( FALSE );
}

/******************************************************************************
** Function name:		MCI_Send_ACMD_OP_Cond
**
** Descriptions:		If Send_OP_Cond is timeout, it's not a MMC card, try 
**						this combination to see if we can communicate with 
**						a SD card. 		
**
** parameters:			None
** Returned value:		true or false, true if card has been initialized.
** 
******************************************************************************/
DWORD MCI_Send_ACMD_OP_Cond( void )
{
	volatile DWORD i, retryCount;
	DWORD respStatus;
	DWORD respValue[4];

	/* timeout on SEND_OP_COND command on MMC, now, try SEND_APP_OP_COND 
	command to SD */
	retryCount = 0x200;			/* reset retry counter */
	while ( retryCount > 0 )
	{
		MCI_POWER &= ~(1 << 6 );	/* Clear Open Drain output control for SD */
		for ( i = 0; i < 0x3000; i++ ) asm volatile (" nop");

		if ( MCI_Send_ACMD() == FALSE )
		{
      retryCount--;  //Added EA
			continue;
		}
			
		/* Send ACMD41 command repeatedly until the response is back correctly */
		MCI_SendCmd( SEND_APP_OP_COND, OCR_INDEX, EXPECT_SHORT_RESP, 0 );
		respStatus = MCI_GetCmdResp( SEND_APP_OP_COND, EXPECT_SHORT_RESP, (DWORD *)&respValue[0] );
		if ( !(respStatus & MCI_CMD_TIMEOUT) && (respValue[0] & 0x80000000) )
		{
			return ( TRUE );	/* response is back and correct. */
		}
		for ( i = 0; i < 0x20; i++ ) asm volatile (" nop");
		retryCount--;
	}
	return( FALSE );
}

/******************************************************************************
** Function name:		MCI_CardInit
**
** Descriptions:		Try CMD1 first for MMC, if it's timeout, try CMD55 
**						and CMD41 for SD, if both failed, initialization faliure, 
**						bailout with unknown card type. Otherwise, return the
**						card type, either MMC or SD.		
**
** parameters:			None
** Returned value:		Card type.
** 
******************************************************************************/
DWORD MCI_CardInit( void )
{
	volatile DWORD i, CardType;

	if ( MCI_Go_Idle_State() == FALSE )
	{
		return( CARD_UNKNOWN );
	}

	MCI_POWER |= (1 << 6 );		/* Set Open Drain output control for MMC */
	for ( i = 0; i < 0x3000; i++ ) asm volatile("nop");

	/* Try CMD1 first for MMC, if it's timeout, try CMD55 and CMD41 for SD,
	if both failed, initialization faliure, bailout. */
	if ( MCI_Send_OP_Cond() == TRUE )	
	{
		CardType = MMC_CARD;
		return ( CardType );	/* Found the card, it's a MMC */
	}
	else if ( MCI_Send_ACMD_OP_Cond() == TRUE )
	{
		CardType = SD_CARD;
		return ( CardType );	/* Found the card, it's a SD */
	}	

	/* tried both MMC and SD card, give up */
	return ( CARD_UNKNOWN );
}

/******************************************************************************
** Function name:		MCI_Check_CID
**
** Descriptions:		Send CMD2, ALL_SEND_CID		
**
** parameters:			None
** Returned value:		If not timeout, return true.
** 
******************************************************************************/
DWORD MCI_Check_CID( void )
{
	DWORD i, retryCount;
	DWORD respStatus;
	DWORD respValue[4];

	/* This command is normally after CMD1(MMC) or ACMD41(SD). */
	retryCount = 0x20;			/* reset retry counter */
	while ( retryCount > 0 )
	{
		/* Send CMD2 command repeatedly until the response is back correctly */
		MCI_SendCmd( ALL_SEND_CID, 0, EXPECT_LONG_RESP, 0 );
		respStatus = MCI_GetCmdResp( ALL_SEND_CID, EXPECT_LONG_RESP, (DWORD *)&respValue[0] );
		/* bit 0 and bit 2 must be zero, or it's timeout or CRC error */
		if ( !(respStatus & MCI_CMD_TIMEOUT) )
		{
      ciddata[3] = (respValue[0] >> 0) & 0xff;
      ciddata[2] = (respValue[0] >> 8) & 0xff;
      ciddata[1] = (respValue[0] >> 16) & 0xff;
      ciddata[0] = (respValue[0] >> 24) & 0xff;
      ciddata[7] = (respValue[1] >> 0) & 0xff;
      ciddata[6] = (respValue[1] >> 8) & 0xff;
      ciddata[5] = (respValue[1] >> 16) & 0xff;
      ciddata[4] = (respValue[1] >> 24) & 0xff;
      ciddata[11] = (respValue[2] >> 0) & 0xff;
      ciddata[10] = (respValue[2] >> 8) & 0xff;
      ciddata[9] = (respValue[2] >> 16) & 0xff;
      ciddata[8] = (respValue[2] >> 24) & 0xff;
      ciddata[15] = (respValue[3] >> 0) & 0xff;
      ciddata[14] = (respValue[3] >> 8) & 0xff;
      ciddata[13] = (respValue[3] >> 16) & 0xff;
      ciddata[12] = (respValue[3] >> 24) & 0xff;

			return ( TRUE );	/* response is back and correct. */
		}
		for ( i = 0; i < 0x20; i++ ) asm volatile("nop");
		retryCount--;
	}
	return ( FALSE );
}

/******************************************************************************
** Function name:		MCI_Set_Address
**
** Descriptions:		Send CMD3, STE_RELATIVE_ADDR, should after CMD2		
**
** parameters:			None
** Returned value:		TRUE if response is back before timeout.
** 
******************************************************************************/
DWORD MCI_Set_Address( void )
{
	volatile DWORD i, retryCount;
	DWORD respStatus;
	DWORD respValue[4];
	DWORD CmdArgument;

	/* If it's a SD card, SET_RELATIVE_ADDR is to get the address
	from the card and use this value in RCA, if it's a MMC, set default
	RCA addr. 0x00010000. */ 
	if ( MCI_CardType == SD_CARD )
	{
		CmdArgument = 0;
	}
	else			/* If it's unknown or MMC_CARD, fix the RCA address */
	{
		CmdArgument = 0x00010000;
	}

	retryCount = 0x20;			/* reset retry counter */
	while ( retryCount > 0 )
	{
		/* Send CMD3 command repeatedly until the response is back correctly */
		MCI_SendCmd( SET_RELATIVE_ADDR, CmdArgument, EXPECT_SHORT_RESP, 0 );
		respStatus = MCI_GetCmdResp( SET_RELATIVE_ADDR, EXPECT_SHORT_RESP, (DWORD *)&respValue[0] );
		/* bit 0 and bit 2 must be zero, or it's timeout or CRC error */
		/* It should go to IDEN state and bit 8 should be 1 */
		if ( !(respStatus & MCI_CMD_TIMEOUT) && ((respValue[0] & (0x0F << 8)) == 0x0500) )
		{
			CardRCA = respValue[0] & 0xFFFF0000;	/* Save the RCA value from SD card */
			return ( TRUE );	/* response is back and correct. */
		}
		for ( i = 0; i < 0x20; i++ ) asm volatile (" nop");
		retryCount--;
	}
	return ( FALSE );
}

/******************************************************************************
** Function name:		MCI_Send_CSD
**
** Descriptions:		CMD9, SEND_CSD cmd, it should be sent only at
**						STBY state and after CMD3. See MMC and SD spec. state 
**						diagram.		
**
** parameters:			None
** Returned value:		Response value
** 
******************************************************************************/
DWORD MCI_Send_CSD( void )
{
	volatile DWORD i, retryCount;
	DWORD respStatus;
	DWORD respValue[4];
	DWORD CmdArgument;
  unsigned int csd_ver = 0;

	if ( MCI_CardType == SD_CARD )
	{
		CmdArgument = CardRCA;
	}
	else			/* if MMC or unknown card type, use default RCA addr. */
	{
		CmdArgument = 0x00010000;
	}

	retryCount = 0x20;
	while ( retryCount > 0 )
	{
		/* Send SET_BLOCK_LEN command before read and write */
		MCI_CLEAR |= (MCI_CMD_TIMEOUT | MCI_CMD_CRC_FAIL | MCI_CMD_RESP_END);
		MCI_SendCmd( SEND_CSD, CmdArgument, EXPECT_LONG_RESP, 0 );
		respStatus = MCI_GetCmdResp( SEND_CSD, EXPECT_LONG_RESP, (DWORD *)&respValue[0] );
		if ( !respStatus )
		{
  unsigned int sizemult;
  unsigned int size;

//printf("\nCSD=%x, %x, %x, %x\n", respValue[0],respValue[1],respValue[2],respValue[3]);

  csddata[3] = (respValue[0] >> 0) & 0xff;
  csddata[2] = (respValue[0] >> 8) & 0xff;
  csddata[1] = (respValue[0] >> 16) & 0xff;
  csddata[0] = (respValue[0] >> 24) & 0xff;
  csddata[7] = (respValue[1] >> 0) & 0xff;
  csddata[6] = (respValue[1] >> 8) & 0xff;
  csddata[5] = (respValue[1] >> 16) & 0xff;
  csddata[4] = (respValue[1] >> 24) & 0xff;
  csddata[11] = (respValue[2] >> 0) & 0xff;
  csddata[10] = (respValue[2] >> 8) & 0xff;
  csddata[9] = (respValue[2] >> 16) & 0xff;
  csddata[8] = (respValue[2] >> 24) & 0xff;
  csddata[15] = (respValue[3] >> 0) & 0xff;
  csddata[14] = (respValue[3] >> 8) & 0xff;
  csddata[13] = (respValue[3] >> 16) & 0xff;
  csddata[12] = (respValue[3] >> 24) & 0xff;

  hw_sect_size = 1<<(csddata[5] & 0x0f);
  size = ((csddata[6]&0x03)<<10)+(csddata[7]<<2)+(csddata[8]&0xc0);
  sizemult = ((csddata[10] & 0x80)>>7)+((csddata[9] & 0x03)<<1);
  hw_nr_sects = (size+1)*(1<<(sizemult+2));
  hw_size = hw_nr_sects*hw_sect_size/1024;

      if ( MCI_CardType == SD_CARD )
      {
        printf("\nmci-cid (SD memory card):");
        csd_ver = (csddata[0] >> 6) & 0x03;
        if (csd_ver != 0)
          printf("\n  Unrecognized CSD structure version (%d)", csd_ver);
        else
        {
          printf("\n  Manufacturer ID: 0x%02x",   ciddata[0]);
          if (ciddata[0] == 0x03)
            printf(" = SanDisk");
          printf("\n  OEM/Application ID: %c%c",  ciddata[1], ciddata[2]);
          printf("\n  Product Name: %c%c%c%c%c",  ciddata[3], ciddata[4], ciddata[5], ciddata[6], ciddata[7]);
          printf("\n  Product Revision: %u.%u",   ciddata[8] >> 4, ciddata[8] & 0x0f);
          printf("\n  Serial Number: 0x%04x%04x", (ciddata[9] << 8)+ciddata[10], (ciddata[11] <<8) + ciddata[12]);
          printf("\n  Date Code: %d.%d",          (ciddata[13] << 4) + (ciddata[14] >> 4) + 2000, (ciddata[14] & 0x0f));
        }
      }
      else
      {
        csd_ver = (csddata[0] >> 6) & 0x03;
        if ((csd_ver != 1) && (csd_ver != 2))
          printf("\n  Unrecognized CSD structure version (%d)", csd_ver);

        switch((csddata[0] >> 2) & 0x0f)  //get mmca_vsn field
        {
          case 0:
            printf("\nmci-cid (MMC memory card, v1.0-v1.2):");
            printf("\n  Manufacturer ID: 0x%06x",   ( (ciddata[0] << 16) + (ciddata[1] << 8) + ciddata[2]));
            printf("\n  Product Name: %c%c%c%c%c%c%c",  ciddata[3], ciddata[4], ciddata[5], ciddata[6], ciddata[7], ciddata[8], ciddata[9]);
            printf("\n  Product Revision: %u.%u",   (ciddata[10] >> 4), ciddata[10] & 0x0f);
            printf("\n  Serial Number: 0x%02x%04x", ciddata[11], (ciddata[12] <<8) + ciddata[13]);
            printf("\n  Date Code: %d.%d",          (ciddata[14] & 0x0f) + 1997, (ciddata[14] >> 4));
            break;
          case 1:
            printf("\nmci-cid (MMC memory card, v1.4):");
            printf("\n  Manufacturer ID: 0x%06x",   (ciddata[0] << 16) + (ciddata[1] << 8) + ciddata[2]);
            printf("\n  Product Name: %c%c%c%c%c%c%c",  ciddata[3], ciddata[4], ciddata[5], ciddata[6], ciddata[7], ciddata[8], ciddata[9]);
            printf("\n  Product Revision: %u.%u",   (ciddata[10] >> 4), (ciddata[10] & 0x0f));
            printf("\n  Serial Number: 0x%02x%04x", ciddata[11], (ciddata[12] <<8) + ciddata[13]);
            printf("\n  Date Code: %d.%d",          (ciddata[14] & 0x0f) + 1997, (ciddata[14] >> 4));
            break;
          case 2:
            printf("\nmci-cid (MMC memory card, v2.0-v2.2):");
            printf("\n  Manufacturer ID: 0x%02x",   ciddata[0]);
            if (ciddata[0] == 0x03)
              printf(" = SanDisk");
            printf("\n  OEM/Application ID: %c%c",  ciddata[1], ciddata[2]);
            printf("\n  Product Name: %c%c%c%c%c%c",  ciddata[3], ciddata[4], ciddata[5], ciddata[6], ciddata[7], ciddata[8]);
            printf("\n  Serial Number: 0x%04x%04x", (ciddata[10] << 8)+ciddata[11], (ciddata[12] <<8) + ciddata[13]);
            printf("\n  Date Code: %d.%d",          (ciddata[14] & 0x0f) + 1997, (ciddata[14] >> 4));
            break;
          case 3:
            printf("\nmci-cid (MMC memory card, v3.1-v3.3):");
            printf("\n  Manufacturer ID: 0x%02x",   ciddata[0]);
            if (ciddata[0] == 0x03)
              printf(" = SanDisk");
            printf("\n  OEM/Application ID: %c%c",  ciddata[1], ciddata[2]);
            printf("\n  Product Name: %c%c%c%c%c%c",  ciddata[3], ciddata[4], ciddata[5], ciddata[6], ciddata[7], ciddata[8]);
            printf("\n  Serial Number: 0x%04x%04x", (ciddata[10] << 8)+ciddata[11], (ciddata[12] <<8) + ciddata[13]);
            printf("\n  Date Code: %d.%d",          (ciddata[14] & 0x0f) + 1997, (ciddata[14] >> 4));
            break;
          case 4:
            printf("\nmci-cid (MMC memory card, v4):");
            printf("\n  Manufacturer ID: 0x%02x",   ciddata[0]);
            if (ciddata[0] == 0x03)
              printf(" = SanDisk");
            printf("\n  OEM/Application ID: %c%c",  ciddata[1], ciddata[2]);
            printf("\n  Product Name: %c%c%c%c%c%c",  ciddata[3], ciddata[4], ciddata[5], ciddata[6], ciddata[7], ciddata[8]);
            printf("\n  Serial Number: 0x%04x%04x", (ciddata[10] << 8)+ciddata[11], (ciddata[12] <<8) + ciddata[13]);
            printf("\n  Date Code: %d.%d",          (ciddata[14] & 0x0f) + 1997, (ciddata[14] >> 4));
            break;
          default:
            printf("\nmci-cid (unknown version of MMC memory card (%d))", (csddata[0] >> 2) & 0x0f);
            break;
        }
      }

      printf("\n  sector size = %d (Bytes), card size = %d (MBytes)", hw_sect_size, hw_size/1024);
      
      printf("\n  dump csd data: %02x%02x%02x%02x%02x%02x%02x%02x",
        csddata[0],csddata[1],csddata[2],csddata[3],csddata[4],csddata[5],csddata[6],csddata[7]);
      printf("\n                 %02x%02x%02x%02x%02x%02x%02x%02x\n",
        csddata[8],csddata[9],csddata[10],csddata[11],csddata[12],csddata[13],csddata[14],csddata[15]);


			return ( TRUE );
		}
		for ( i = 0; i < 0x20; i++ );
		retryCount--;
	}
	return ( FALSE );
}

/******************************************************************************
** Function name:		MCI_Select_Card
**
** Descriptions:		CMD7, SELECT_CARD, should be after CMD9, the state
**						will be inter-changed between STBY and TRANS after 
**						this cmd.		
**
** parameters:			None
** Returned value:		return false if response times out.
** 
******************************************************************************/
DWORD MCI_Select_Card( void )
{
	volatile DWORD i, retryCount;
	DWORD respStatus;
	DWORD respValue[4];
	DWORD CmdArgument;

	if ( MCI_CardType == SD_CARD )
	{
		CmdArgument = CardRCA;
	}
	else			/* if MMC or unknown card type, use default RCA addr. */
	{
		CmdArgument = 0x00010000;
	}

	retryCount = 0x20;
	while ( retryCount > 0 )
	{
		/* Send SELECT_CARD command before read and write */
		MCI_CLEAR |= (MCI_CMD_TIMEOUT | MCI_CMD_CRC_FAIL | MCI_CMD_RESP_END);
		MCI_SendCmd( SELECT_CARD, CmdArgument, EXPECT_SHORT_RESP, 0 );
		respStatus = MCI_GetCmdResp( SELECT_CARD, EXPECT_SHORT_RESP, (DWORD *)&respValue[0] );
		if ( !respStatus && ((respValue[0] & (0x0F << 8)) == 0x0700 ))
		{						/* Should be in STANDBY state now and ready */
			return ( TRUE );
		}
		for ( i = 0; i < 0x20; i++ ) asm volatile (" nop");
		retryCount--;
	}
	return ( FALSE );
}

/******************************************************************************
** Function name:		MCI_Send_Status
**
** Descriptions:		CMD13, SEND_STATUS, the most important cmd to
**						debug the state machine of the card.		
**
** parameters:			None
** Returned value:		Response value(card status), true if the ready bit 
**						is set in the card status register, if timeout, return 
**						INVALID_RESPONSE 0xFFFFFFFF.
** 
******************************************************************************/
DWORD MCI_Send_Status( void )
{
	volatile DWORD i, retryCount;
	DWORD respStatus;
	DWORD respValue[4];
	DWORD CmdArgument;

	if ( MCI_CardType == SD_CARD )
	{
		CmdArgument = CardRCA;
	}
	else			/* if MMC or unknown card type, use default RCA addr. */
	{
		CmdArgument = 0x00010000;
	}

	/* Note that, since it's called after the block write and read, this timeout 
	is important based on the clock you set for the data communication. */
	retryCount = 0x2000;
	while ( retryCount > 0 )
	{
		/* Send SELECT_CARD command before read and write */
		MCI_CLEAR |= (MCI_CMD_TIMEOUT | MCI_CMD_CRC_FAIL | MCI_CMD_RESP_END);
		MCI_SendCmd( SEND_STATUS, CmdArgument, EXPECT_SHORT_RESP, 0 );
		respStatus = MCI_GetCmdResp( SEND_STATUS, EXPECT_SHORT_RESP, (DWORD *)&respValue[0] );
		if ( !respStatus && (respValue[0] & (1 << 8)) )
		{	/* The ready bit should be set, it should be in either TRAN or RCV 
			state now */
			return ( respValue[0] );
		}
//		for ( i = 0; i < 0x20; i++ ) asm volatile (" nop");
		retryCount--;
	}
	return ( INVALID_RESPONSE );
}
		
/******************************************************************************
** Function name:		MCI_Set_BlockLen
**
** Descriptions:		CMD16, SET_BLOCKLEN, called after CMD7(SELECT_CARD)
**						called in the TRANS state.		
**
** parameters:			The length of the data block to be written or read.
** Returned value:		true or false, return TRUE if ready bit is set, and it's
**						in TRANS state.
** 
******************************************************************************/
DWORD MCI_Set_BlockLen( DWORD blockLength )
{
	volatile DWORD i, retryCount;
	DWORD respStatus;
	DWORD respValue[4];

	retryCount = 0x20;
	while ( retryCount > 0 )
	{
		/* Send SET_BLOCK_LEN command before read and write */
		MCI_CLEAR |= (MCI_CMD_TIMEOUT | MCI_CMD_CRC_FAIL | MCI_CMD_RESP_END);
		MCI_SendCmd( SET_BLOCK_LEN, blockLength, EXPECT_SHORT_RESP, 0 );
		respStatus = MCI_GetCmdResp( SET_BLOCK_LEN, EXPECT_SHORT_RESP, (DWORD *)&respValue[0] );
		/* bit 9 through 12 should be in transfer state now. bit 8 is ready. */
		if ( !respStatus && ((respValue[0] & (0x0F << 8)) == 0x0900))			  		
		{		
			return ( TRUE );
		}
		for ( i = 0; i < 0x20; i++ ) asm volatile (" nop");
		retryCount--;
	}
	return ( FALSE );
}

/******************************************************************************
** Function name:		MCI_Send_ACMD_Bus_Width
**
** Descriptions:		ACMD6, SET_BUS_WIDTH, if it's SD card, we can
**						use the 4-bit bus instead of 1-bit. This cmd
**						can only be called during TRANS state.
**						Since it's a ACMD, CMD55 APP_CMD needs to be
**						sent out first. 		
**
** parameters:			Bus width value, 1-bit is 0, 4-bit is 10
** Returned value:		true or false, true if the card is still in the 
**						TRANS state after the cmd.
** 
******************************************************************************/
DWORD MCI_Send_ACMD_Bus_Width( DWORD buswidth )
{
	volatile DWORD i, retryCount;
	DWORD respStatus;
	DWORD respValue[4];

	retryCount = 0x20;			/* reset retry counter */
	while ( retryCount > 0 )
	{
		if ( MCI_Send_ACMD() == FALSE )
		{
			continue;
		}
			
		/* Send ACMD6 command to set the bus width */
		MCI_SendCmd( SET_ACMD_BUS_WIDTH, buswidth, EXPECT_SHORT_RESP, 0 );
		respStatus = MCI_GetCmdResp( SET_ACMD_BUS_WIDTH, EXPECT_SHORT_RESP, (DWORD *)&respValue[0] );
		if ( !respStatus && ((respValue[0] & (0x0F << 8)) == 0x0900) )
		{
			return ( TRUE );	/* response is back and correct. */
		}
		for ( i = 0; i < 0x20; i++ ) asm volatile (" nop");
		retryCount--;
	}
	return( FALSE );
}

/******************************************************************************
** Function name:		MCI_Send_Stop
**
** Descriptions:		CMD12, STOP_TRANSMISSION. if that happens, the card is 
**						maybe in a unknown state that need a warm reset.		
**
** parameters:			None
** Returned value:		true or false, true if, at least, the card status
**						shows ready bit is set.
** 
******************************************************************************/
DWORD MCI_Send_Stop( void )
{
	volatile DWORD i, retryCount;
	DWORD respStatus;
	DWORD respValue[4];

	retryCount = 0x20;
	while ( retryCount > 0 )
	{
		MCI_CLEAR = 0x7FF;
		MCI_SendCmd( STOP_TRANSMISSION, 0x00000000, EXPECT_SHORT_RESP, 0 );
		respStatus = MCI_GetCmdResp( STOP_TRANSMISSION, EXPECT_SHORT_RESP, (DWORD *)respValue );
		/* ready bit, bit 8, should be set in the card status register */
		if ( !respStatus && (respValue[0] & (1 << 8)) )
		{
			return( TRUE );
		}
		for ( i = 0; i < 0x20; i++ ) asm volatile (" nop");
		retryCount--;
	}
	return ( FALSE );
}

/******************************************************************************
** Function name:		MCI_Send_Write_Block
**
** Descriptions:		CMD24, WRITE_BLOCK, send this cmd in the TRANS state
**						to write a block of data to the card.
**
** parameters:			block number
** Returned value:		Response value
** 
******************************************************************************/
DWORD MCI_Send_Write_Block( DWORD blockNum )
{
	volatile DWORD i, retryCount;
	DWORD respStatus;
	DWORD respValue[4];

	retryCount = 0x20;
	while ( retryCount > 0 )
	{
		MCI_CLEAR = 0x7FF;
		MCI_SendCmd( WRITE_BLOCK, blockNum * BLOCK_LENGTH, EXPECT_SHORT_RESP, 0 );
		respStatus = MCI_GetCmdResp( WRITE_BLOCK, EXPECT_SHORT_RESP, (DWORD *)&respValue[0] );
		/* it should be in the transfer state, bit 9~12 is 0x0100 and bit 8 is 1 */
		if ( !respStatus && ((respValue[0] & (0x0F << 8)) == 0x0900) )
		{
			return( TRUE );			/* ready and in TRAN state */
		}

		for ( i = 0; i < 0x20; i++ ) asm volatile (" nop");
		retryCount--;
	}
	return ( FALSE );				/* Fatal error */
}

/******************************************************************************
** Function name:		MCI_Send_Read_Block
**
** Descriptions:		CMD17, READ_SINGLE_BLOCK, send this cmd in the TRANS 
**						state to read a block of data from the card.
**
** parameters:			block number
** Returned value:		Response value
** 
******************************************************************************/
DWORD MCI_Send_Read_Block( DWORD blockNum )
{
	volatile DWORD i, retryCount;
	DWORD respStatus;
	DWORD respValue[4];

	retryCount = 0x20;
	while ( retryCount > 0 )
	{
		MCI_CLEAR = 0x7FF;
		MCI_SendCmd( READ_SINGLE_BLOCK, blockNum * BLOCK_LENGTH, EXPECT_SHORT_RESP, 0 );
		respStatus = MCI_GetCmdResp( READ_SINGLE_BLOCK, EXPECT_SHORT_RESP, (DWORD *)&respValue[0] );
		/* it should be in the transfer state, bit 9~12 is 0x0100 and bit 8 is 1 */
		if ( !respStatus && ((respValue[0] & (0x0F << 8)) == 0x0900) )
		{
			return( TRUE );			/* ready and in TRAN state */
		}

		for ( i = 0; i < 0x20; i++ ) asm volatile (" nop");
		retryCount--;
	}
	return ( FALSE );					/* Fatal error */
}
	
/******************************************************************************
** Function name:		MCI_Write_Block
**
** Descriptions:		Set MCI data control register, data length and data
**						timeout, send WRITE_BLOCK cmd, finally, enable
**						interrupt. On completion of WRITE_BLOCK cmd, TX_ACTIVE
**						interrupt will occurs, data can be written continuously
**						into the FIFO until the block data length is reached.		
**
** parameters:			block number
** Returned value:		true or false, if cmd times out, return false and no 
**						need to continue.
** 
******************************************************************************/
DWORD MCI_Write_Block( DWORD blockNum )
{
	volatile DWORD i;
	DWORD DataCtrl = 0;

	MCI_CLEAR = 0x7FF;
	MCI_DATA_CTRL = 0;
	for ( i = 0; i < 0x10; i++ ) asm volatile (" nop");

	/* Below status check is redundant, but ensure card is in TRANS state
	before writing and reading to from the card. */
	if ( MCI_CheckStatus() != TRUE )
	{
		MCI_Send_Stop();
		return( FALSE );
	}

	MCI_DATA_TMR = DATA_TIMER_VALUE;
	MCI_DATA_LEN = BLOCK_LENGTH;
	MCI_Block_End_Flag = 1;
  MCI_TXEnable();
  if ( MCI_Send_Write_Block( blockNum ) == FALSE )
	{
		return ( FALSE );
	}

#if MCI_DMA_ENABLED
  DMA_Move( 0, M2P );
  GPDMA_CH0_CFG |= 0x10001 | (0x00 << 1) | (0x04 << 6) | (0x05 << 11);
  /* Write, block transfer, DMA, and data length */
  DataCtrl |= ((1 << 0) | (1 << 3) | (DATA_BLOCK_LEN << 4));  
#else

  /* Write, block transfer, and data length */
  DataCtrl |= ((1 << 0) | (DATA_BLOCK_LEN << 4));
#endif  
  MCI_DATA_CTRL = DataCtrl;
  for ( i = 0; i < 0x10; i++ ) asm volatile(" nop");  

  return( TRUE );
}

/******************************************************************************
** Function name:		MCI_Read_Block
**
** Descriptions:		Set MCI data control register, data length and data
**						timeout, send READ_SINGLE_BLOCK cmd, finally, enable
**						interrupt. On completion of READ_SINGLE_BLOCK cmd, 
**						RX_ACTIVE interrupt will occurs, data can be read 
**						continuously into the FIFO until the block data 
**						length is reached.		
**
** parameters:			block number
** Returned value:		true or false, if cmd times out, return false and no 
**						need to continue.
**
** 
******************************************************************************/
DWORD MCI_Read_Block( DWORD blockNum )
{
	volatile DWORD i;
	DWORD DataCtrl = 0;
	MCI_CLEAR = 0x7FF;
	MCI_DATA_CTRL = 0;
	for ( i = 0; i < 0x10; i++ ) asm volatile (" nop");

	/* Below status check is redundant, but ensure card is in TRANS state
	before writing and reading to from the card. */
	if ( MCI_CheckStatus() != TRUE )
	{
		MCI_Send_Stop();
		return( FALSE );
	}

	MCI_RXEnable();

	MCI_DATA_TMR = DATA_TIMER_VALUE;
	MCI_DATA_LEN = BLOCK_LENGTH;
  MCI_Block_End_Flag = 1;

	if ( MCI_Send_Read_Block( blockNum ) == FALSE )
	{
		return ( FALSE );
	}

#if MCI_DMA_ENABLED
  DMA_Move( 1, P2M );
  GPDMA_CH1_CFG |= 0x10001 | (0x04 << 1) | (0x00 << 6) | (0x06 << 11);
  /* Write, block transfer, DMA, and data length */
  DataCtrl |= ((1 << 0) | (1 << 1) | (1 << 3) | (DATA_BLOCK_LEN << 4));   
#else
  /* Read, enable, block transfer, and data length */
  DataCtrl |= ((1 << 0) | (1 << 1) | (DATA_BLOCK_LEN << 4));  
#endif
  MCI_DATA_CTRL = DataCtrl;
  for ( i = 0; i < 0x10; i++ ) asm volatile(" nop");  
  return( TRUE );
}

/*****************************************************************************
**                            End Of File
******************************************************************************/
