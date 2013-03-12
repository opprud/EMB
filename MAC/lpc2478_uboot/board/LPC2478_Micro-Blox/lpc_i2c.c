/*
 * (C) Copyright 2006 Embedded Artists AB <www.embeddedartists.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

//#ifdef CONFIG_HARD_I2C

/******************************************************************************
 * Includes
 *****************************************************************************/
#include <common.h>
#include <asm/arch/hardware.h>
#include <i2c.h>
#include "lpc_i2c.h"


/******************************************************************************
 * Defines and typedefs
 *****************************************************************************/
#define I2C_REG_CONSET      0x00000040 /* Control Set Register         */
#define I2C_REG_CONSET_MASK 0x0000007C /* Used bits                    */
#define I2C_REG_DATA        0x00000000 /* Data register                */
#define I2C_REG_DATA_MASK   0x000000FF /* Used bits                    */
#define I2C_REG_ADDR        0x00000000 /* Slave address register       */
#define I2C_REG_ADDR_MASK   0x000000FF /* Used bits                    */
//#define I2C_REG_SCLH        0x00000040 /* SCL Duty Cycle high register */
#define I2C_REG_SCLH        0x000000f0 /* SCL Duty Cycle high register */
#define I2C_REG_SCLH_MASK   0x0000FFFF /* Used bits                    */
//#define I2C_REG_SCLL        0x00000040 /* SCL Duty Cycle low register  */
#define I2C_REG_SCLL        0x00000160 /* SCL Duty Cycle low register  */
#define I2C_REG_SCLL_MASK   0x0000FFFF /* Used bits                    */


#define	DEBUG_I2C
#ifdef DEBUG_I2C
DECLARE_GLOBAL_DATA_PTR;

//#define PRINTD(fmt,args...) printf (fmt ,##args)
#define PRINTD(fmt,args...)	do {	\
	if (gd->have_console)		\
		printf (fmt ,##args);	\
	} while (0)
#else
#define PRINTD(fmt,args...)
#endif

/******************************************************************************
 * Local functions
 *****************************************************************************/

/******************************************************************************
 *
 * Description:
 *    Checks the I2C status.
 *
 *
 *  Returns:
 *      00h Bus error
 *      08h START condition transmitted
 *      10h Repeated START condition transmitted
 *      18h SLA + W transmitted, ACK received
 *      20h SLA + W transmitted, ACK not received
 *      28h Data byte transmitted, ACK received
 *      30h Data byte transmitted, ACK not received
 *      38h Arbitration lost
 *      40h SLA + R transmitted, ACK received
 *      48h SLA + R transmitted, ACK not received
 *      50h Data byte received in master mode, ACK transmitted
 *      58h Data byte received in master mode, ACK not transmitted
 *      60h SLA + W received, ACK transmitted
 *      68h Arbitration lost, SLA + W received, ACK transmitted
 *      70h General call address received, ACK transmitted
 *      78h Arbitration lost, general call addr received, ACK transmitted
 *      80h Data byte received with own SLA, ACK transmitted
 *      88h Data byte received with own SLA, ACK not transmitted
 *      90h Data byte received after general call, ACK transmitted
 *      98h Data byte received after general call, ACK not transmitted
 *      A0h STOP or repeated START condition received in slave mode
 *      A8h SLA + R received, ACK transmitted
 *      B0h Arbitration lost, SLA + R received, ACK transmitted
 *      B8h Data byte transmitted in slave mode, ACK received
 *      C0h Data byte transmitted in slave mode, ACK not received
 *      C8h Last byte transmitted in slave mode, ACK received
 *      F8h No relevant status information, SI=0
 *      FFh Channel error
 *
 *****************************************************************************/
static __u8
i2cCheckStatus(void)
{
  __u8 status = 0;

  /* wait for I2C Status changed */
  while( (I20CONSET & 0x08)  == 0)   /* while SI == 0 */
  {
    ;
  }

  /* Read I2C State */
  status = I20STAT;

  /* NOTE! SI flag is not cleared here */

  return status;
}

/******************************************************************************
 *
 * Description:
 *    Generates a start condition on I2C when bus is free.
 *    Master mode will also automatically be entered.
 *
 *    Note: After a stop condition, you may need a bus free time before you 
 *          can generate a new start condition.
 *
 * Returns:
 *    I2C_CODE_OK or I2C status code
 *
 *****************************************************************************/
static char
i2cStart(void)
{
  __u8   status  = 0;
  char   retCode = 0;

  /* issue a start condition */
  I20CONSET |= 0x20; /* STA = 1, set start flag */

  /* wait until START transmitted */
  while(1)
  {
    status = i2cCheckStatus();

    /* start transmitted */
    if((status == 0x08) || (status == 0x10))
    {
      retCode = I2C_CODE_OK;
      break;
    }

    /* error */
    else if(status != 0xf8)
    {
      retCode = (char) status;
      break;
    }

    else
    {
      /* clear SI flag */
      I20CONCLR = 0x08;
    }    
  }

  /* clear start flag */
  I20CONCLR = 0x20;

	PRINTD("i2cStart()\n");

  return retCode;
}

/******************************************************************************
 *
 * Description:
 *    Generates a start condition on I2C when bus is free.
 *    Master mode will also automatically be entered.
 *
 *    Note: After a stop condition, you may need a bus free time before you 
 *          can generate a new start condition.
 *
 * Returns:
 *    I2C_CODE_OK or I2C status code
 *
 *****************************************************************************/
static char
i2cRepeatStart(void)
{
  __u8   status  = 0;
  char   retCode = 0;

  /* issue a start condition */
  I20CONSET |= 0x20; /* STA = 1, set start flag */
  I20CONCLR = 0x08;  /* clear SI flag           */

  /* wait until START transmitted */
  while(1)
  {
    status = i2cCheckStatus();

    /* start transmitted */
    if((status == 0x08) || (status == 0x10))
    {
      retCode = I2C_CODE_OK;
      break;
    }

    /* error */
    else if(status != 0xf8)
    {
      retCode = (char) status;
      break;
    }

    else
    {
      /* clear SI flag */
      I20CONCLR = 0x08;
    }    
  }

  /* clear start flag */
  I20CONCLR = 0x20;

	PRINTD("i2cRepeatStart()\n");
  return retCode;
}

/******************************************************************************
 *
 * Description:
 *    Generates a stop condition in master mode or recovers from an error
 *    condition in slave mode.
 *
 *    Note: After this function is run, you may need a bus free time before
 *          you can generate a new start condition.
 *
 * Returns:
 *    I2C_CODE_OK
 *
 *****************************************************************************/
static char
i2cStop(void)
{
  I20CONSET |= 0x10; /* STO = 1, set stop flag */ 
  I20CONCLR = 0x08;  /* clear SI flag          */ 

  /* wait for STOP detected (while STO = 1) */
  while((I20CONSET & 0x10) == 0x10 )
  {
    /* do nothing */
    ;
  }

	PRINTD("i2cStop()\n");

  return I2C_CODE_OK;
}

/******************************************************************************
 *
 * Description:
 *    Sends a character on the I2C network
 *
 * Params:
 *    [in] data - the character to send
 *
 * Returns:
 *    I2C_CODE_OK   - successful
 *    I2C_CODE_BUSY - data register is not ready -> byte was not sent
 *
 *****************************************************************************/
static char
i2cPutChar(__u8 data)
{
  char retCode = 0;

	PRINTD("i2cPutChar: %02X\n", data);

  /* check if I2C Data register can be accessed */
  if((I20CONSET & 0x08) != 0)  /* if SI = 1 */
  {
    /* send data */
    I20DAT    = data;
    I20CONCLR = 0x08; /* clear SI flag */ 
    retCode    = I2C_CODE_OK;
  }
  else
  {
    /* data register not ready */
    retCode = I2C_CODE_BUSY;
  }

  return retCode;
}

/******************************************************************************
 *
 * Description:
 *    Read a character. I2C master mode is used.
 *    This function is also used to prepare if the master shall generate
 *    acknowledge or not acknowledge.
 *
 * Params:
 *    [in]  mode  - I2C_MODE_ACK0 Set ACK=0. Slave sends next byte
 *                  I2C_MODE_ACK1 Set ACK=1. Slave sends last byte
 *                  I2C_MODE_READ Read data from data register
 *    [out] pData - a pointer to where the data shall be saved.
 *
 * Returns:
 *    I2C_CODE_OK    - successful
 *    I2C_CODE_EMPTY - no data is available
 *
 *****************************************************************************/
static char
i2cGetChar(__u8  mode,
           __u8* pData)
{
  char retCode = I2C_CODE_OK;

  if(mode == I2C_MODE_ACK0)
  {
    /* the operation mode is changed from master transmit to master receive */

    /* set ACK=0 (informs slave to send next byte) */

    I20CONSET |= 0x04; /* AA=1          */
    I20CONCLR = 0x08;  /* clear SI flag */   
PRINTD("ACK=0\n");
  }
  else if(mode == I2C_MODE_ACK1)
  {
    /* set ACK=1 (informs slave to send last byte) */
    I20CONCLR = 0x04;     
    I20CONCLR = 0x08; /* clear SI flag */ 
PRINTD("ACK=1\n");
  }
  else if(mode == I2C_MODE_READ)
  {
    /* check if I2C Data register can be accessed */
    if((I20CONSET & 0x08) != 0)  /* SI = 1 */
    {
      /* read data  */
      *pData = (__u8) I20DAT;
PRINTD("i2cGetChar: %02X\n", *pData);
    }
    else
    {
      /* No data available */
      retCode = I2C_CODE_EMPTY;
    }
  }

  return retCode;
}


/******************************************************************************
 * Public functions
 *****************************************************************************/

/******************************************************************************
 *
 * Description:
 *    Reset and initialize the I2C module.
 *
 *****************************************************************************/
void
i2c_init (int speed, int slaveaddr)
{
	/*
	 * WARNING: Do NOT save speed in a static variable: if the
	 * I2C routines are called before RAM is initialized (to read
	 * the DIMM SPD, for instance), RAM won't be usable and your
	 * system will crash.
	 */

  PINSEL1 &= 0xfc3fffff;
  PINSEL1 |= 0x01400000;
  
  /* clear flags */
  I20CONCLR = 0x6c;    

  /* reset registers */
  I20SCLL   = ( I20SCLL   & ~I2C_REG_SCLL_MASK )   | I2C_REG_SCLL;
  I20SCLH   = ( I20SCLH   & ~I2C_REG_SCLH_MASK )   | I2C_REG_SCLH;
  I20ADR    = ( I20ADR    & ~I2C_REG_ADDR_MASK )   | I2C_REG_ADDR;
  I20CONSET = ( I20CONSET & ~I2C_REG_CONSET_MASK ) | I2C_REG_CONSET;

  //set reset?
}

/*-----------------------------------------------------------------------
 * Probe to see if a chip is present.  Also good for checking for the
 * completion of EEPROM writes since the chip stops responding until
 * the write completes (typically 10mSec).
 */
int i2c_probe(uchar addr)
{
  signed char retCode = 0;

  /* Generate Start condition */
  retCode = i2cStart();

  /* Transmit address */
  if(retCode == I2C_CODE_OK )
  {
    /* write SLA+R */
    retCode = i2cPutChar((addr << 1) | 0);
    while(retCode == I2C_CODE_BUSY)
    {
      retCode = i2cPutChar(addr << 1);
    }
  }

  /*--- Generate Stop condition ---*/
  i2cStop();

  if(retCode == I2C_CODE_OK )
    return 0;
  else
    return 1;
}

/*-----------------------------------------------------------------------
 * Read bytes
 */
int  i2c_read(uchar chip, uint addr, int alen, uchar *buffer, int len)
{
  signed char retCode = 0;
  __u8 status  = 0;
  int  i       = 0;
	int  shift;

	PRINTD("i2c_read: chip %02X addr %02X alen %d buffer %p len %d\n",
		chip, addr, alen, buffer, len);

#ifdef CFG_I2C_EEPROM_ADDR_OVERFLOW
	/*
	 * EEPROM chips that implement "address overflow" are ones
	 * like Catalyst 24WC04/08/16 which has 9/10/11 bits of
	 * address and the extra bits end up in the "chip address"
	 * bit slots. This makes a 24WC08 (1Kbyte) chip look like
	 * four 256 byte chips.
	 *
	 * Note that we consider the length of the address field to
	 * still be one byte because the extra address bits are
	 * hidden in the chip address.
	 */
	chip |= ((addr >> (alen * 8)) & CFG_I2C_EEPROM_ADDR_OVERFLOW);

	PRINTD("i2c_read: fix addr_overflow: chip %02X addr %02X\n",
		chip, addr);
#endif

	/*
	 * Do the addressing portion of a write cycle to set the
	 * chip's address pointer.  If the address length is zero,
	 * don't do the normal write cycle to set the address pointer,
	 * there is no address pointer in this chip.
	 */

  /* Generate Start condition */
  retCode = i2cStart();

  /* Transmit address */
  if((retCode == I2C_CODE_OK ) && (alen > 0))
  {
    /* write SLA+R */
    retCode = i2cPutChar(chip << 1);
    while(retCode == I2C_CODE_BUSY)
    {
      retCode = i2cPutChar(chip << 1);
    }

    if(retCode != I2C_CODE_OK )
    {
      /*--- Generate Stop condition ---*/
      i2cStop();
			PRINTD("i2c_read, no chip responded %02X\n", chip);
			return(1);
    }

		shift = (alen-1) * 8;
		while(alen-- > 0)
    {
      retCode = i2cPutChar(addr >> shift);
      while(retCode == I2C_CODE_BUSY)
      {
        retCode = i2cPutChar(addr >> shift);
      }

      if(retCode != I2C_CODE_OK )
      {
        /*--- Generate Stop condition ---*/
        i2cStop();
				PRINTD("i2c_read, address not <ACK>ed\n");
				return(1);
			}
			shift -= 8;
		}

    /*--- Generate Stop condition ---*/
    i2cStop();                              /* reportedly some chips need a full stop */

    /* Generate Start condition */
    retCode = i2cStart();
//    retCode = i2cRepeatStart();
  }

	/*
	 * Send the chip address again, this time for a read cycle.
	 * Then read the data.  On the last byte, we do a NACK instead
	 * of an ACK(len == 0) to terminate the read.
	 */
  retCode = i2cPutChar((chip << 1) | 1);
  while(retCode == I2C_CODE_BUSY)
  {
    retCode = i2cPutChar((chip << 1) | 1);
  }

  if(retCode != I2C_CODE_OK )
  {
    /*--- Generate Stop condition ---*/
    i2cStop();
    PRINTD("i2c_read, repeat start and read access failed\n");
    return(1);
  }

	while(len-- > 0)
  {
    /* wait until data transmitted */
    while(1)
    {
      /* get new status */
      status = i2cCheckStatus();

      /*
       * SLA+R transmitted, ACK received or
       * SLA+R transmitted, ACK not received
       * data byte received in master mode, ACK transmitted
       */
      if((status == 0x40 ) || (status == 0x48 ) || (status == 0x50 ))
      {
        /* data received */

        /* Read data */
        retCode = i2cGetChar(I2C_MODE_READ, buffer);
        while(retCode == I2C_CODE_EMPTY)
        {
          retCode = i2cGetChar(I2C_MODE_READ, buffer);
        }
        buffer++;

        if(0 == len)
        {
          /* Set generate NACK */
          retCode = i2cGetChar(I2C_MODE_ACK1, NULL);
        }
        else
        {
          retCode = i2cGetChar(I2C_MODE_ACK0, NULL);
        }

        break;
      }

      /* no relevant status information */
      else if(status != 0xf8 )
      {
        /* error */
        len = 0;
        retCode = I2C_CODE_ERROR;
        break;
      }
    }
	}

  /*--- Generate Stop condition ---*/
  i2cStop();
  if(retCode == I2C_CODE_OK)
    return(0);
  else
    return(1);
}

/*-----------------------------------------------------------------------
 * Write bytes
 */
int  i2c_write(uchar chip, uint addr, int alen, uchar *buffer, int len)
{
	int shift, failures = 0;
  signed char retCode = 0;
  __u8 status  = 0;
  int  i       = 0;

	PRINTD("i2c_write: chip %02X addr %02X alen %d buffer %p len %d\n",
		chip, addr, alen, buffer, len);

  /* Generate Start condition */
  retCode = i2cStart();

  /* Transmit address */
  if(retCode == I2C_CODE_OK )
  {
    /* write SLA+R */
    retCode = i2cPutChar(chip << 1);
    while(retCode == I2C_CODE_BUSY)
    {
      retCode = i2cPutChar(chip << 1);
    }

    if(retCode != I2C_CODE_OK )
    {
      /*--- Generate Stop condition ---*/
      i2cStop();
			PRINTD("i2c_write, no chip responded %02X\n", chip);
			return(1);
    }

		shift = (alen-1) * 8;
		while(alen-- > 0)
    {
      retCode = i2cPutChar(addr >> shift);
      while(retCode == I2C_CODE_BUSY)
      {
        retCode = i2cPutChar(addr >> shift);
      }

      if(retCode != I2C_CODE_OK )
      {
        /*--- Generate Stop condition ---*/
        i2cStop();
				PRINTD("i2c_write, address not <ACK>ed\n");
				return(1);
			}
			shift -= 8;
		}
  }

	while(len-- > 0)
  {
    /* wait until data transmitted */
    while(1)
    {
      /* get new status */
      status = i2cCheckStatus();

      /* 
       * SLA+W transmitted, ACK received or
       * data byte transmitted, ACK received
       */
      if( (status == 0x18) || (status == 0x28) )
      {
        /* Data transmitted and ACK received */
        /* write data */
        retCode = i2cPutChar(*buffer);
        while(retCode == I2C_CODE_BUSY)
        {
          retCode = i2cPutChar(*buffer);
        }
        buffer++;

        break;
      }

      /* no relevant status information */
      else if( status != 0xf8 )
      {
        /* error */
        failures++;
        break;
      }
    }
	}

  /* wait until data transmitted */
  while(1)
  {
    /* get new status */
    status = i2cCheckStatus();

    /* 
     * SLA+W transmitted, ACK received or
     * data byte transmitted, ACK received
     */
    if( (status == 0x18) || (status == 0x28) )
    {
      /* data transmitted and ACK received */
      break;
    }

    /* no relevant status information */
    else if(status != 0xf8 )
    {
      retCode = I2C_CODE_ERROR;
      break;
    }
  }

  /*--- Generate Stop condition ---*/
  i2cStop();

  if(retCode == I2C_CODE_OK )
    return(failures);
  else
    return(failures+1);
}

/*-----------------------------------------------------------------------
 * Read a register
 */
uchar i2c_reg_read(uchar i2c_addr, uchar reg)
{
	uchar buf;

	i2c_read(i2c_addr, reg, 1, &buf, 1);

	return(buf);
}

/*-----------------------------------------------------------------------
 * Write a register
 */
void i2c_reg_write(uchar i2c_addr, uchar reg, uchar val)
{
	i2c_write(i2c_addr, reg, 1, &val, 1);
}

//#endif	/* CONFIG_HARD_I2C */
