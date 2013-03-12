#ifndef  _CAN_H
#define  _CAN_H
/**
 * @file
 * @ingroup drv
 *         This driver shows how the CAN interface of the Philips LPC2000
 *         can be used to receive ALL messages on the CANbus.
 *         All incoming messages are copied into a receive queue.
 *         NOTE: This example does not implement any queue overrun handling
 *
 * 		  The code is based on a license free CAN driver examples
 * 		  "LPC2_CANAll_V110:" downloaded from www.keil.com
 * @remarks
 *
 * Usage example:
 *
 *  // CAN device handle
 * 	static int candev1;
 *
 * 	candev1 = can_open(CAN1, 0 );
 *
 *
 *
 */
#include "lpc2468.h"
#include "can_bitdefs.h"
#include "type.h"

/***********************************************************************
 * CAN driver compiler options
 **********************************************************************/


#define F_CPU_60MHZ ///<Define CPU clock frequency TODO: place somewhere global

/* Default CAN ring buffer size, buffersize is in no. of CANMSG_T.
 * Each object is 16bytes. For each enabled CAN, a
 * transmit and receive ring buffer of this size will be defined */
#define CAN_RING_BUFSIZE 256 ///< Buffer size

#define CAN_1_ENABLE 1	///< The CAN_1_ENABLE define enables support for CAN 1 when set to 1

#define CAN_2_ENABLE 1	///< The CAN_2_ENABLE define enables support for CAN 2 when set to 1

//#define CAN_3_ENABLE 1	///< The CAN_3_ENABLE define enables support for CAN 3 when set to 1

//#define CAN_4_ENABLE 1	///< The CAN_4_ENABLE define enables support for CAN 1 when set to 1


/**
 * CAN module register structure (when using UNS_32 types and accessing
 * through this structure HW registers are mapped in accordance with
 * chip layout)
 */
typedef struct
{
   volatile __canmod_bits canmod; /* Controls the operating mode of the CAN Controller. */
   volatile __cancmr_bits cancmr; /* Command bits that affect the state of the CAN Controller */
   volatile __cangsr_bits cangsr; /* Global Controller Status and Error Counters */
   volatile __canicr_bits canicr; /* Interrupt status, Arbitration Lost Capture, Error Code Capture */
   volatile __canier_bits canier; /* Interrupt Enable */
   volatile __canbtr_bits canbtr; /* Bus Timing */
   volatile __canewl_bits canewl; /* Error Warning Limit */
   volatile __cansr_bits  cansr;  /* Status Register */
   volatile __canrfs_bits canrfs; /* Receive frame status */
   volatile __canrid_bits canrid; /* Received Identifier */
   volatile __canrda_bits canrda; /* Received data bytes 1-4 */
   volatile __canrdb_bits canrdb; /* Received data bytes 5-8 */
   volatile __cantfi_bits cantfi1; /* Transmit frame info (1) */
   volatile __cantid_bits cantid1; /* Transmit Identifier (1) */
   volatile __cantda_bits cantda1; /* Transmit data bytes 1-4 (1) */
   volatile __cantdb_bits cantdb1; /* Transmit data bytes 5-8 (1) */
   volatile __cantfi_bits cantfi2; /* Transmit frame info (2) */
   volatile __cantid_bits cantid2; /* Transmit Identifier (2) */
   volatile __cantda_bits cantda2; /* Transmit data bytes 1-4 (2) */
   volatile __cantdb_bits cantdb2; /* Transmit data bytes 5-8 (2) */
   volatile __cantfi_bits cantfi3; /* Transmit frame info (3) */
   volatile __cantid_bits cantid3; /* Transmit Identifier (3) */
   volatile __cantda_bits cantda3; /* Transmit data bytes 1-4 (3) */
   volatile __cantdb_bits cantdb3; /* Transmit data bytes 5-8 (3) */
} CAN_REGS_T;                      ///< CAN register types to be able to access bitfields

/**
 *  Type definition to hold a CAN message
 */
typedef struct
{
   union
   {
      unsigned int Raw;
      struct
      {
         unsigned int Dummy :16;
         unsigned int DLC :4;    /* Bits 16..19: DLC - Data Length Counter */
         unsigned int Dummy2 :10;
         unsigned int RTR :1;    /* Set if this is a RTR message */
         unsigned int FF :1;     /* Set if this is a 29-bit ID message */
      } Bit;
   } Frame;
   unsigned int MsgID;           /* CAN Message ID (11-bit or 29-bit) */
   unsigned char Data[8];
} CANMSG_T;                      ///< Type definition to hold a CAN message

/***********************************************************************
 * CAN device configuration commands (IOCTL commands and arguments)
 **********************************************************************/

/* CAN device arguments for CAN_CLEAR_ERROR_CONDITION (IOCTL arguments) */
typedef enum
{
     RX_ERROR_CONDITION,
     TX_ERROR_CONDITION
} ERROR_CONDITION_TYPE_T;

/* CAN device commands (IOCTL commands) */
typedef enum
{
   CAN_SET_BAUD_RATE, /* Set CAN baud rate, use an argument type of CAN_BAUD_RATE_T to set the rate */
   CAN_AUTO_SET_BAUD_RATE, /* Detect the bus baudrate and set the CAN baud rate accordingly */
   CAN_SET_FILTER, /* Set CAN Filter bits... TODO */
   CAN_SET_MODE, /**/
   CAN_GET_STATUS, /* Returns CAN status for the selected arg type of CAN_IOCTL_STS_T */
   CAN_FORCE_BUS_OFF,/*Force BUS OFF*/
   CAN_CLEAR_ERROR_CONDITION
} CAN_IOCTL_CMD_T;

/* CAN device arguments for CAN_SET_BAUD_RATE command (IOCTL arguments) */
typedef enum
{
   BPS_25K, /* 25 Kbps */
   BPS_50K, /* 50 Kbps */
   BPS_125K, /* 125 Kbps */
   BPS_250K, /* 250 Kbps */
   BPS_500K, /* 500 Kbps */
   BPS_1000K /* 1000 Kbps */
} CAN_BAUD_RATE_T;

/* CAN device arguments for CAN_SET_MODE (IOCTL arguments) */
typedef enum
{
   MODE_SELF_TEST, /* Set TestMode RX is looped onto TX pin  */
   MODE_LISTEN_ONLY, /* Set LOM, no acknowledge on CAN bus */
   MODE_OPERATING, /* Normal operating mode - Default after using CAN_open  */
   MODE_RESET /* Reset mode */
} CAN_MODE_T;

/* CAN device arguments for CAN_GET_STATUS command (IOCTL arguments) */
typedef enum
{
   CAN_MODE, /* Returns CAN status (1 = CAN
    enabled, 0 = CAN disabled */
   CAN_BAUD_RATE, /* Returns bps rate, an enumeration of type CAN_BAUD_RATE_T */
   CAN_FILTER_BITS, /* Returns the number of data bits (5 to 8) */
   CAN_MASK_BITS, /* Returns parity, an enumeration of type CAN_PARITY_T */
   CAN_DATA_OVERRUN, /* Returns DOS, Data Overrun Status */
   CAN_ERROR_STATUS, /* Returns ES, Error Status, TRUE if RX or TX errors has reached EWL (EWL is 96 as default) */
   CAN_BUS_STATUS, /* Returns BS, Bus Status, TRUE if TX errors has reached 255 */
   CAN_RX_ERRORS, /* Returns number of RX errors, if any */
   CAN_TX_ERRORS /* Returns number of TX errors, if any */
} CAN_IOCTL_STS_T;

/* Macro pointing to CAN registers at CAN 1 base address */
#define CAN1  ((CAN_REGS_T *)(0xE0044000))
/* Macro pointing to CAN registers at CAN 2 base address */
#define CAN2  ((CAN_REGS_T *)(0xE0048000))
/* Macro pointing to CAN registers at CAN 3 base address */
#define CAN3  ((CAN_REGS_T *)(0xE004C000))
/* Macro pointing to CAN registers at CAN 4 base address */
#define CAN4  ((CAN_REGS_T *)(0xE0050000))

/***********************************************************************
 * CAN driver API functions
 **********************************************************************/

/* Open a CAN block */
int can_open(void *ipbase, int arg);

/* Close a CAN block */
int can_close(int devid);

/* Configure a CAN block */
int can_ioctl(int devid, int cmd, int arg);

/* Read a number of can packets from can inbuffer */
int can_read(int devid, void *buffer, int max_bytes);

/* Write a number of can packets to can outbuffer */
int can_write(int devid, void *buffer, int n_telegrams);


#endif
