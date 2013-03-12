/**
 * @file
 * @ingroup drv
 * This driver shows how the CAN interface of the Philips LPC2000 can be used to receive ALL messages on the CANbus.
 * @remarks
 *
 */
#include "can.h"
#include "lpc2468.h"
#include "irq.h"
#include "config.h"

/*profiling*/
#include "../simple_profiler.h"

/***********************************************************************
 * CAN driver compiler options
 **********************************************************************/

/*Define CPU clock frequency TODO: place somewhere global */
#undef F_CPU_60MHZ

/* Default CAN ring buffer size, buffer size is in no. of CANMSG_T.
 * Each object is 16bytes. For each enabled CAN, a
 * transmit and receive ring buffer of this size will be defined */
#define CAN_RING_BUFSIZE 256

/* The CAN_1_ENABLE define enables support for CAN 1 when set to 1 */
#define CAN_1_ENABLE 1

/* The CAN_2_ENABLE define enables support for CAN 2 when set to 1 */
#define CAN_2_ENABLE 1

/* The CAN_3_ENABLE define enables support for CAN 3 when set to 1 */
//#define CAN_3_ENABLE 1

/* The CAN_4_ENABLE define enables support for CAN 1 when set to 1 */
//#define CAN_4_ENABLE 1

/***********************************************************************
 * IRQ handler prototype
 **********************************************************************/
void CAN_Handler(void) __attribute__ ((interrupt("IRQ")));

/***********************************************************************
 * CAN driver private data and types
 **********************************************************************/
/**
 *  CAN Ring buffer structure
 */
typedef struct
{
	CANMSG_T tx[CAN_RING_BUFSIZE]; ///< CAN Tx data ring buffer
	CANMSG_T rx[CAN_RING_BUFSIZE]; ///< CAN Rx data ring buffer
	int tx_head; ///< CAN Tx ring buffer head index
	int tx_tail; ///< CAN Tx ring buffer tail index
	int rx_head; ///< CAN Rx ring buffer head index
	int rx_tail; ///< CAN Rx ring buffer tail index
} CAN_RING_BUFFER_T; ///< CAN Ring buffer structure

/**
 *  CAN device configuration structure type
 */
typedef struct
{
	int init; ///< Device initialized flag
	CAN_RING_BUFFER_T rb; ///< Device ring buffer
	CAN_REGS_T *regptr; ///< Pointer to CAN registers
} CAN_CFG_T; ///< CAN device configuration structure type

/* CAN device configuration structure for CAN 1 */
#if CAN_1_ENABLE == 1
static CAN_CFG_T can1cfg;
#endif

/* CAN device configuration structure for CAN 2 */
#if CAN_2_ENABLE == 1
static CAN_CFG_T can2cfg;
#endif

/* CAN device configuration structure for CAN 3 */
#if CAN_3_ENABLE == 1
static CAN_CFG_T can3cfg;
#endif

/* CAN device configuration structure for CAN 3 */
#if CAN_4_ENABLE == 1
static CAN_CFG_T can4cfg;
#endif

//#ifdef Fpclk == 28800000
////TODO place new baudrates here
//#endif

#ifdef Fcclk == 57600000
/* The bitrate is dependent on the clock frequency, and for a
 *  setup with cclk = 57.7MHz (using PLL) and PCLKDIV = 1:
 *
 *  Bitrate = cclk / ( VPBDIV * (BRP+1) * ( TSEG1+1 + 1 + TSEG2+1))
 *
 *  CAN Bit Timings      SAM TSEG1 TSEG2 SJW SP%  BRP
 *  20kBaud              0+1  5+1   2+1  3+1 70  149+1
 *  50kBaud              0+1  5+1   2+1  3+1 70   59+1
 *  100kBaud             0+1  5+1   2+1  3+1 70   14+1
 *  125kBaud             0+1  5+1   2+1  3+1 70   11+1
 *  250kBaud             0+1  5+1   2+1  3+1 70    5+1
 *  500kBaud             0+1  5+1   2+1  3+1 70    2+1
 *  1000kBaud            0+1  8+1   4+1  3+1 67    0+1
 *
 * Use "CAN baudrate calculator.xls"
 * to caculate baudrates for other CPU frequencies
 */
/* Common CAN bit rates */
#define  CAN_BRP_25K          127
#define  CAN_BRP_50K          63
#define  CAN_BRP_100K         31
#define  CAN_BRP_125K         25
#define  CAN_BRP_250K         12
#define  CAN_BRP_500K         5
#define  CAN_BRP_1000K        2

#define CAN_BRP_DEFAULT			CAN_BRP_125K
/* Predefined timing parameters for the CAN bus  when running 10-500kbps
 * A CAN telegram
 *
 * */
#define	CAN_SJW					3
#define	CAN_TSEG1				10
#define	CAN_TSEG2				5
#define	CAN_SAM					0

/* Predefined timing parameters for the CAN bus  when running 1000kbps*/
#define	CAN_SJW_1000K			3
#define	CAN_TSEG1_1000K		    11
#define	CAN_TSEG2_1000K		    5
#define	CAN_SAM_1000K			0

#else
#error NO Valid Fcclk defined, currently only "57.6MHZ" supported
#endif

/* ---------------------- PRIVATE FUNCTIONS ------------------------ */

/**
 *
 * Function: can_standard_receive
 *
 * Purpose: Common CAN receive function
 *
 * Processing:
 *     If the CAN RX registers holds data, read a data
 *     block from it and place it into the receive ring buffer
 *     indexed by the head pointer. Increment the head pointer and reset
 *     it to the start of the ring buffer if it overflows.
 *
 * @param cancfg: Pointer to CAN config structure
 *
 * Outputs: None
 *
 * @return : Nothing
 *
 * @remarks :
 *     There is no protection against overflowing the ring buffer.
 *
 */
static void can_standard_receive(CAN_CFG_T *cancfg)
{
	CAN_REGS_T *canregs = cancfg->regptr;

	/* Copy CAN RX data to RX circular buffer */
	cancfg->rb.rx[cancfg->rb.rx_head].Frame.Bit.DLC = canregs->canrfs.DLC;
	cancfg->rb.rx[cancfg->rb.rx_head].Frame.Bit.RTR = canregs->canrfs.RTR;
	cancfg->rb.rx[cancfg->rb.rx_head].Frame.Bit.FF = canregs->canrfs.FF;
	cancfg->rb.rx[cancfg->rb.rx_head].MsgID = canregs->canrid.ID29_0;
	cancfg->rb.rx[cancfg->rb.rx_head].Data[0] = canregs->canrda.Data1;
	cancfg->rb.rx[cancfg->rb.rx_head].Data[1] = canregs->canrda.Data2;
	cancfg->rb.rx[cancfg->rb.rx_head].Data[2] = canregs->canrda.Data3;
	cancfg->rb.rx[cancfg->rb.rx_head].Data[3] = canregs->canrda.Data4;
	cancfg->rb.rx[cancfg->rb.rx_head].Data[4] = canregs->canrdb.Data5;
	cancfg->rb.rx[cancfg->rb.rx_head].Data[5] = canregs->canrdb.Data6;
	cancfg->rb.rx[cancfg->rb.rx_head].Data[6] = canregs->canrdb.Data7;
	cancfg->rb.rx[cancfg->rb.rx_head].Data[7] = canregs->canrdb.Data8;

	/* Increment receive ring buffer head pointer */
	cancfg->rb.rx_head++;
	if (cancfg->rb.rx_head >= CAN_RING_BUFSIZE)
	{
		cancfg->rb.rx_head = 0;
	}

	/* Release receive buffer */
	canregs->cancmr.RRB = 1;
}

/**
 *
 * Function: can_standard_transmit
 *
 * Purpose: Common CAN transmit function
 *
 * Processing:
 *     While there is data to transmit in the ring buffer, write a data value
 *     from the ring buffer into the next available three-buffer transmit
 *     queue of the selected CAN interface.
 *     Increment the transmit ring buffer tail pointer and reset
 *     it to the start of the ring buffer
 *     if it overflows. If all the data has been transmitted from
 *     the ring buffer, disable the transmit interrupt, otherwise
 *     (re)enable it.
 *
 * @param :
 *     cancfg: Pointer to CAN config structure
 *
 * Outputs: None
 *
 * @return : Nothing
 *
 * @remarks : None
 *
 */
static void can_standard_transmit(CAN_CFG_T *cancfg)
{
	CAN_REGS_T *canregs = cancfg->regptr;

	/* If TX queue is empty, return */
	if (cancfg->rb.tx_head == cancfg->rb.tx_tail)
	{
		/* Disable CAN TX interrupt */
		canregs->canier.TIE1 = 0;
		return;
	}

	/* Add data to TX buffer*/
	if (canregs->cansr.TBS1)
	{
		/* Now that we have a TX buffer, start filling it up */
		/* Write DLC, RTR and FF */
		canregs->cantfi1.DLC = (cancfg->rb.tx[cancfg->rb.tx_tail].Frame.Bit.DLC);
		canregs->cantfi1.RTR = (cancfg->rb.tx[cancfg->rb.tx_tail].Frame.Bit.RTR);
		canregs->cantfi1.FF = (cancfg->rb.tx[cancfg->rb.tx_tail].Frame.Bit.FF);

		/* Write CAN ID */
		canregs->cantid1.ID29_0 = (cancfg->rb.tx[cancfg->rb.tx_tail].MsgID);

		/* Write first 4 data bytes */
		canregs->cantda1.Data1 = (cancfg->rb.tx[cancfg->rb.tx_tail].Data[0]);
		canregs->cantda1.Data2 = (cancfg->rb.tx[cancfg->rb.tx_tail].Data[1]);
		canregs->cantda1.Data3 = (cancfg->rb.tx[cancfg->rb.tx_tail].Data[2]);
		canregs->cantda1.Data4 = (cancfg->rb.tx[cancfg->rb.tx_tail].Data[3]);

		/* Write next 4 data bytes */
		canregs->cantdb1.Data5 = (cancfg->rb.tx[cancfg->rb.tx_tail].Data[4]);
		canregs->cantdb1.Data6 = (cancfg->rb.tx[cancfg->rb.tx_tail].Data[5]);
		canregs->cantdb1.Data7 = (cancfg->rb.tx[cancfg->rb.tx_tail].Data[6]);
		canregs->cantdb1.Data8 = (cancfg->rb.tx[cancfg->rb.tx_tail].Data[7]);

		/* Set TX flag on the buffer used */
		canregs->cancmr.STB1 = 1;

		/* Update transmit ring FIFO tail pointer */
		cancfg->rb.tx_tail++;

		/* If we have reached the end, roll back to start */
		if (cancfg->rb.tx_tail >= CAN_RING_BUFSIZE)
			cancfg->rb.tx_tail = 0;

		/* (Re)Enable CAN TX interrupt */
		canregs->canier.TIE1 = 1;

		/* Write transmission request */
		canregs->cancmr.TR = 1;
	}
}

/**
 *
 * Function: can_get_free_tx_count
 *
 * Purpose: Return the amount of free space in the transmit ring buffer
 *
 * Processing:
 *     Compute a count value by subtracting the transmit ring buffer
 *     tail index from the head index. If the value is less than 0,
 *     then recompute the count value adding the head index to the
 *     total ring buffer size minus the tail index. Return the number
 *     of free entries left in the ring buffer by subtracting the
 *     (count + 1) from the total ring buffer size.
 *
 * @param :
 *     cancfg: Pointer to CAN config structure
 *
 * Outputs: None
 *
 * @return : The number of free bytes in the transmit ring buffer
 *
 * @remarks : None
 *
 */
static int can_get_free_tx_count(CAN_CFG_T *cancfg)
{
	int count = 0;

	count = cancfg->rb.tx_head - cancfg->rb.tx_tail;
	if (count < 0)
	{
		/* head pointer has flipped to start of ringbuffer */
		count = (CAN_RING_BUFSIZE - cancfg->rb.tx_tail) + cancfg->rb.tx_head;
	}

	return (CAN_RING_BUFSIZE - count - 1);
}

/* ---------------------- PUBLIC FUNCTIONS ------------------------ */

/**
 *     Determine the CAN configuration structure to use based on the
 *     passed arg value. If the arg value doesn't match an available
 *     CAN block, return NULL to the caller. Otherwise, check the status of
 *     the init flag. If it is TRUE, return NULL to the caller.
 *     Otherwise, set init to TRUE and save the pointer to the CAN
 *     registers.
 *     Set the receive and transmit ring buffers to empty by setting
 *     the head and tail pointers to 0. Default the CAN is set to
 *     125K baudrate.
 *
 *
 *     @param *ipbase: CAN descriptor device address
 *     @param arg - Not used
 */
int can_open(void *ipbase, signed int arg)
{
	CAN_CFG_T *cancfg;
	CAN_REGS_T *canregs;

	/* Invalid or disabled CAN */
	cancfg = (CAN_CFG_T *) NULL;

#if CAN_1_ENABLE == 1
	if ((CAN_REGS_T *) ipbase == CAN1)
	{
		/* CAN 1 selected */
		cancfg = &can1cfg;
		/* Enable CAN 1 */
		PCONP |= (1 << 13);
		/* Set bit P0.0:RD1  P0.1:TD1 */
		PINSEL0 &= ~0x0000000F;
		PINSEL0 |= 0x0000005;
	}
#endif

#if CAN_2_ENABLE == 1
	if ((CAN_REGS_T *) ipbase == CAN2)
	{
		/* CAN 2 selected */
		cancfg = &can2cfg;
		/* Enable CAN 2 */
		PCONP |= (1 << 14);
		/* Set bit P0.4:RD2  P0.5:TD2 */
		PINSEL0 &= ~0x00000F00;
		PINSEL0 |= 0x0000A05;
	}
#endif

#if CAN_3_ENABLE == 1
	if ((CAN_REGS_T *) ipbase == CAN3)
	{
		/* CAN 3 selected */
		cancfg = &can3cfg;
		/* Set bits 11 and 12 */
		PINSEL1 |= 0x00001800L;
	}
#endif

#if CAN_4_ENABLE == 1
	if ((CAN_REGS_T *) ipbase == CAN4)
	{
		/* CAN 4 selected */
		cancfg = &can3cfg;
		/* Set bits 24 to 27 */
		PINSEL0 |= 0x0F000000L;
	}
#endif

	if (cancfg != (CAN_CFG_T *) NULL)
	{
		/* Valid CAN selected, has it been previously initialized? */
		if (cancfg->init == FALSE)
		{
			/* Device not initialized and it is usable, so set it to used */
			cancfg->init = TRUE;

			/* Save address of register block */
			cancfg->regptr = (CAN_REGS_T *) ipbase;
			canregs = (CAN_REGS_T *) ipbase;//MOJ

			/* Acceptance Filter Mode Register = filter off, receive all */
			CAN_AFMR = 0x00000002L;

			/* Go into Reset mode */
			canregs->canmod.RM = 1;

			/* Clear Status register */
			canregs->cangsr.RBS = 0;
			canregs->cangsr.DOS = 0;
			canregs->cangsr.ES = 0;
			canregs->cangsr.RBS = 0;
			canregs->cangsr.RS = 0;
			canregs->cangsr.RXERR = 0;
			canregs->cangsr.TBS = 1;
			canregs->cangsr.TCS = 1;
			canregs->cangsr.TS = 0;
			canregs->cangsr.TXERR = 0;

			/* Set bit timing */
			canregs->canbtr.BRP = CAN_BRP_DEFAULT;
			canregs->canbtr.SJW = CAN_SJW;
			canregs->canbtr.TSEG1 = CAN_TSEG1;
			canregs->canbtr.TSEG2 = CAN_TSEG2;
			canregs->canbtr.SAM = CAN_SAM;

			/* Enable Receive and Transmit Interrupt */
			canregs->canier.RIE = 1;
			canregs->canier.TIE1 = 1;

			/* Initialize the ring buffers */
			cancfg->rb.tx_head = cancfg->rb.tx_tail = 0;
			cancfg->rb.rx_head = cancfg->rb.rx_tail = 0;

			/* Set Operating Mode */
			canregs->canmod.RM = 0;

			/* Install CAN interrupt handler note! this can be called multiple times */
			install_irq(CAN_INT, (void *) CAN_Handler, HIGHEST_PRIORITY);
		}
	}
	return (int) cancfg;
}

/**
 *
 * Function: can_close
 *
 * Purpose: Close a CAN
 *
 * Processing:
 *     If init is not TRUE, then return _ERROR to the caller as the
 *     device was not previously opened. Otherwise, disable the CAN,
 *     set init to FALSE, and return _NO_ERROR to the caller.
 *
 * @param :
 *     devid: Pointer to CAN config structure
 *
 * Outputs: None
 *
 * @return : The status of the close operation
 *
 * @remarks : None
 *
 */
int can_close(int devid)
{
	CAN_CFG_T *cancfgptr = (CAN_CFG_T *) devid;
	int status = _ERROR;

	/* First check that device is open*/
	if (cancfgptr->init == TRUE)
	{
		/* CAN1 Selected */
		if (cancfgptr->regptr == CAN1)
			/* Disable CAN 1 */
			PCONP &= ~(1 << 13);

		/* CAN2 Selected */
		else if (cancfgptr->regptr == CAN2)
			/* Disable CAN 2 */
			PCONP &= ~(1 << 14);

		/* Error return */
		else
			return status;

		/* close succeeded */
		status = _NO_ERROR;
		cancfgptr->init = FALSE;
	}
	return status;
}

/**
 *
 * Function: can_ioctl
 *
 * Purpose: CAN configuration block
 *
 * Processing:
 *     This function is a large case block. Based on the passed function
 *     and option values, set or get the appropriate CAN parameter.
 *
 * @param :
 *     devid: Pointer to CAN config structure
 *     cmd:   ioctl command
 *     arg:   ioctl argument
 *
 * Outputs: None
 *
 * @return : The status of the ioctl operation
 *
 * @remarks : None
 *
 */
//extern
int can_ioctl(int devid, int cmd, int arg)
{
	CAN_REGS_T *canregs;
	CAN_CFG_T *cancfgptr = (CAN_CFG_T *) devid;
	int status = _ERROR;
	int carry_on = FALSE;

	if (cancfgptr->init == TRUE)
	{
		status = _NO_ERROR;
		canregs = cancfgptr->regptr;

		switch (cmd)
		{
		case CAN_CLEAR_ERROR_CONDITION:
			canregs->canmod.RM = 1;
			switch (arg)
			{
			case RX_ERROR_CONDITION:
				canregs->cangsr.RXERR = 0;
				canregs->cangsr.RBS = 0;
				canregs->cangsr.ES = 0;
				canregs->canier.RIE = 1;
				break;

			case TX_ERROR_CONDITION:
				canregs->cangsr.TXERR = 0;
				canregs->cangsr.TBS = 1;
				canregs->cangsr.ES = 0;
				canregs->canier.TIE1 = 1;
				break;
			}
			canregs->canmod.RM = 0;
			break;

		case CAN_SET_MODE:
			switch (arg)
			{
			case MODE_SELF_TEST:
				/* Reset mode, clears other bits in register to default */
				canregs->canmod.RM = 1;
				/* Enable CAN */
				canregs->canmod.RM = 0;
				/* Set test mode */
				canregs->canmod.TM = 1;
				break;

			case MODE_LISTEN_ONLY:
				/* Reset mode, clears other bits in register to default */
				canregs->canmod.RM = 1;
				/* Set listen only mode */
				canregs->canmod.LOM = 1;
				/* Enable CAN */
				canregs->canmod.RM = 0;
				break;

			case MODE_OPERATING:
				/* Enable CAN */
				canregs->canmod.RM = 0;
				break;

			case MODE_RESET:
				/* Disable CAN */
				canregs->canmod.RM = 1;
				break;

			default:
				/* Bad BPS rate */
				status = SMA_BAD_PARAMS;
			}
			break;

		case CAN_SET_BAUD_RATE:
			/* Go into Reset mode */
			canregs->canmod.RM = 1;
			/* Disable Receive and Transmit Interrupt */
			canregs->canier.RIE = 0;
			canregs->canier.TIE1 = 0;
			/* Set default sample parameters */
			canregs->canbtr.SJW = CAN_SJW;
			canregs->canbtr.TSEG1 = CAN_TSEG1;
			canregs->canbtr.TSEG2 = CAN_TSEG2;
			canregs->canbtr.SAM = CAN_SAM;

			/* Set the bps rate */
			switch (arg)
			{
			case BPS_25K:
				canregs->canbtr.BRP = CAN_BRP_25K;
				break;

			case BPS_50K:
				canregs->canbtr.BRP = CAN_BRP_50K;
				break;

			case BPS_125K:
				canregs->canbtr.BRP = CAN_BRP_125K;
				break;

			case BPS_250K:
				canregs->canbtr.BRP = CAN_BRP_250K;
				break;

			case BPS_500K:
				canregs->canbtr.BRP = CAN_BRP_500K;
				break;

			case BPS_1000K:
				canregs->canbtr.BRP = CAN_BRP_1000K;
				canregs->canbtr.SJW = CAN_SJW_1000K;
				canregs->canbtr.TSEG1 = CAN_TSEG1_1000K;
				canregs->canbtr.TSEG2 = CAN_TSEG2_1000K;
				canregs->canbtr.SAM = CAN_SAM_1000K;
				break;

			default:
				/* Bad BPS rate */
				status = SMA_BAD_PARAMS;
			}

			/* Clear Status register */
			canregs->cangsr.RBS = 0;
			canregs->cangsr.DOS = 0;
			canregs->cangsr.ES = 0;
			canregs->cangsr.RBS = 0;
			canregs->cangsr.RS = 0;
			canregs->cangsr.RXERR = 0;
			canregs->cangsr.TBS = 1;
			canregs->cangsr.TCS = 1;
			canregs->cangsr.TS = 0;
			canregs->cangsr.TXERR = 0;

			/* Initialize the ring buffers */
			cancfgptr->rb.tx_head = cancfgptr->rb.tx_tail = 0;
			cancfgptr->rb.rx_head = cancfgptr->rb.rx_tail = 0;

			/* Enable Receive and Transmit Interrupt */
			canregs->canier.RIE = 1;
			canregs->canier.TIE1 = 1;

			/* Go into operational mode */
			canregs->canmod.RM = 0;

			break;

		case CAN_AUTO_SET_BAUD_RATE:
			/* TODO find out how ?? */

			break;

		case CAN_SET_FILTER:
			/* TODO find out how ?? */
			break;

		case CAN_FORCE_BUS_OFF:
			canregs->cangsr.TXERR = 255; // Force CAN Controller into BUS OFF
			carry_on = FALSE;
			while (!carry_on)
			{
				/*wait for buss-off*/
				if (canregs->cangsr.BS == 1)
				{
					carry_on = TRUE;
				}
			}
			canregs->cangsr.TXERR = 0; // Clear BUS OFF in CAN Controller
			break;

		case CAN_GET_STATUS:
			switch (arg)
			{
			case CAN_MODE:
				/* if listen only mode set */
				if (canregs->canmod.LOM)
					status = MODE_LISTEN_ONLY;

				/* if reset mode is set */
				else if (canregs->canmod.RM)
					status = MODE_RESET;

				/* if test mode is set */
				else if (canregs->canmod.TM)
					status = MODE_SELF_TEST;

				/* if normal mode is set */
				else if (!(canregs->canmod.RM) && !(canregs->canmod.SM))
					status = MODE_OPERATING;
				break;

			case CAN_BAUD_RATE:
				/* decide which baudrate is used */
				switch (canregs->canbtr.BRP)
				{
				case CAN_BRP_25K:
					status = BPS_25K;
					break;

				case CAN_BRP_50K:
					status = BPS_50K;
					break;

				case CAN_BRP_125K:
					status = BPS_125K;
					break;

				case CAN_BRP_250K:
					status = BPS_250K;
					break;

				case CAN_BRP_500K:
					status = BPS_500K;
					break;

				case CAN_BRP_1000K:
					status = BPS_1000K;
					break;

				default:
					/* Unsupported parameter */
					status = SMA_BAD_PARAMS;
				}
				break;

			case CAN_FILTER_BITS:

				break;

			case CAN_MASK_BITS:

				break;

			case CAN_DATA_OVERRUN:
				status = canregs->cangsr.DOS;
				break;

			case CAN_ERROR_STATUS:
				status = canregs->cangsr.ES;
				break;

			case CAN_BUS_STATUS:
				status = canregs->cangsr.BS;
				break;

			case CAN_RX_ERRORS:
				status = canregs->cangsr.RXERR;
				break;

			case CAN_TX_ERRORS:
				status = canregs->cangsr.TXERR;
				break;

			default:
				/* Unsupported parameter */
				status = SMA_BAD_PARAMS;
				break;
			}
			break;

		default:
			/* Unsupported parameter */
			status = SMA_BAD_PARAMS;
		}
	}

	return status;
}

/**
 *
 * Function: can_read_ring
 *
 * Purpose: CAN read function for interrupt mode (using ring buffers)
 *
 * Processing:
 *     If the init flag for the CAN structure is FALSE, return 0 to
 *     the caller. Otherwise, save the state of the receive interrupts
 *     and disable the receive interrupts. Loop until max_bytes equals
 *     0 or until the receive ring buffer is empty, whichever comes
 *     first. Read the data from the ring buffer  indexed by the tail
 *     pointer and place it into the user buffer. Increment the tail
 *     pointer and user buffer pointer. If the tail pointer exceeds the
 *     buffer size, set the tail pointer to 0. Increment bytes, and
 *     decrement max_bytes. Exit the loop based on the loop conditions,
 *     re-enable the receive interrupts, and return the number of bytes
 *     read to the caller.
 *
 * @param :
 *     devid:     Pointer to CAN config structure
 *     buffer:    Pointer to data buffer to copy to
 *     max_bytes: Number of bytes to read
 *
 * Outputs: None
 *
 * @return : Number of bytes actually read from the ring buffer
 *
 * @remarks : None
 *
 */
int can_read(int devid, void *buffer, int max_bytes)
{
	CAN_REGS_T *canregs;
	unsigned int savedint;
	CAN_CFG_T *cancfgptr = (CAN_CFG_T *) devid;
	CANMSG_T *data = (CANMSG_T *) buffer;
	int bytes = 0;

	if (cancfgptr->init == TRUE)
	{
		canregs = cancfgptr->regptr;

		/* Temporarily lock out CAN receive interrupts during this
		 read so the CAN receive interrupt won't cause problems
		 with the index values */
		savedint = canregs->canier.RIE;
		canregs->canier.RIE = 0;

		/* Loop until receive FIFO is empty or until max_bytes expires */
		while ((max_bytes > 0) && (cancfgptr->rb.rx_tail != cancfgptr->rb.rx_head))
		{
			/* Read data from ring buffer into user buffer */
			*data = cancfgptr->rb.rx[cancfgptr->rb.rx_tail];
			data++;

			/* Update tail pointer */
			cancfgptr->rb.rx_tail++;

			/* Make sure tail didn't overflow */
			if (cancfgptr->rb.rx_tail >= CAN_RING_BUFSIZE)
			{
				cancfgptr->rb.rx_tail = 0;
			}

			/* Increment data count and decrement buffer size count */
			bytes++;
			max_bytes--;
		}

		/* Restore CAN interrupts */
		canregs->canier.RIE = savedint;
	}

	return bytes;
}

/**
 *
 * Function: can_write
 *
 * Purpose: CAN write function for interrupt mode (using ring buffers)
 *
 * Processing:
 *     If the init flag for the CAN structure is FALSE, return 0 to
 *     the caller. Otherwise, disable the receive interrupts. Loop until
 *     n_bytes equals 0 or until the transmit ring buffer is full,
 *     whichever comes first. Write the data from the user buffer to
 *     the transmit ring buffer indexed by the head pointer. Increment
 *     the user buffer pointer and head pointer. If the head pointer
 *     exceeds the buffer size, set the head pointer to 0. Increment
 *     bytes, and decrement n_bytes. Exit the loop based on the loop
 *     conditions. If the number bytes written to the ring buffer was
 *     greater then 0, call the can_standard_transmit() fucntion.
 *     Return the number of bytes read to the caller.
 *
 * @param :
 *     devid:   Pointer to CAN config structure
 *     buffer:  Pointer to data buffer to copy from
 *     n_bytes: Number of telegrams to write
 *
 * Outputs: None
 *
 * @return : Number of bytes actually written to the ring buffer
 *
 * @remarks :
 *     You cannot overflow the transmit ring buffer.
 *
 */
int can_write(int devid, void *buffer, int n_telegrams)
{
	CAN_REGS_T *canregs;
	__canier_bits savedint;
	CAN_CFG_T *cancfgptr = (CAN_CFG_T *) devid;
	CANMSG_T *data = (CANMSG_T *) buffer;
	int bytes = 0;

	if (cancfgptr->init == TRUE)
	{
		canregs = cancfgptr->regptr;

		/* Temporarily lock out CAN transmit interrupts during this
		 write so the CAN transmit interrupt won't cause problems
		 with the index values */
		savedint.TIE1 = canregs->canier.TIE1;
		savedint.TIE2 = canregs->canier.TIE2;
		savedint.TIE3 = canregs->canier.TIE3;

		/*Disable TX interupts*/
		canregs->canier.TIE1 = canregs->canier.TIE2 = canregs->canier.TIE3 = 0;

		/* Loop until transmit run buffer is full or until n_bytes expires */
		while ((n_telegrams > 0) && (can_get_free_tx_count(cancfgptr) > 0))
		{
			/* Write data from buffer into ring buffer */
			cancfgptr->rb.tx[cancfgptr->rb.tx_head] = *data;
			data++;

			/* Increment head pointer */
			cancfgptr->rb.tx_head++;
			if (cancfgptr->rb.tx_head >= CAN_RING_BUFSIZE)
			{
				cancfgptr->rb.tx_head = 0;
			}

			/* Increment data count and decrement buffer size count */
			bytes++;
			n_telegrams--;
		}

		/* Restore original interrupt state */
		canregs->canier.TIE1 = savedint.TIE1;
		canregs->canier.TIE2 = savedint.TIE2;
		canregs->canier.TIE3 = savedint.TIE3;

		/* Start transmitting */
		can_standard_transmit(cancfgptr);
	}

	return bytes;
}

/** ---------------------- INTERRUPT FUNCTIONS ------------------------ */

/* Dummy variables used for acknowleding interrupts */
unsigned int CanInt;
CAN_REGS_T *tmpcanregs;

/**
 *
 * Function: can_handler
 *
 * Purpose: CAN interrupt handler
 *
 * Processing:
 *		Handles all incoming interrupts, currently only RX and TX
 *		interrupts are supported
 **
 */
void CAN_Handler(void)
{
	//SET
	PROF_SET = PROF_PIN_4;

	/* read CAN Interrupt capture regs */

	/*read from ctrl 1, NOTE! IRQ bits are cleared on read*/
	/*that is interupts are ack'ed*/
	tmpcanregs = can1cfg.regptr;

	/*if this was no TX event*/
	if (tmpcanregs->canicr.RI)
		can_standard_receive(&can1cfg);

	/*if this was a TX event*/
	if ((tmpcanregs->canicr.TI1))
		can_standard_transmit(&can1cfg);

	/*read from ctrl 1, NOTE! IRQ bits are cleared on read*/
	/*that is interupts are ack'ed*/
	tmpcanregs = can2cfg.regptr;

	/*if this was no TX event*/
	if (tmpcanregs->canicr.RI)
		can_standard_receive(&can2cfg);

	/*if this was a TX event*/
	if (tmpcanregs->canicr.TI1)
		can_standard_transmit(&can2cfg);

	VICVectAddr = 0; /* Acknowledge Interrupt */

	//CLR
	PROF_CLR = PROF_PIN_4;

}

/*----------------------- END OF FILE ----------------------------------*/

