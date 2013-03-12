#ifndef _I2C_H
#define _I2C_H

/* modes */
#define I2C_MODE_ACK0 0
#define I2C_MODE_ACK1 1
#define I2C_MODE_READ 2

/* return codes */
#define I2C_CODE_OK   1
#define I2C_CODE_DATA 2
#define I2C_CODE_RTR  3

#define I2C_CODE_ERROR -1
#define I2C_CODE_FULL  -2
#define I2C_CODE_EMPTY -3
#define I2C_CODE_BUSY  -4



#define I2C_SLAVEADR_RCV  0xA1
#define I2C_SLAVEADR_SEND 0xA0

/* I2C Control set register */

#define I2C_CONSET_I2EN              0x40   /* Bit 6: I2EN (I2C interface enable) */
#define I2C_CONSET_I2EN_DISABLED     0x00   /* Disabled */
#define I2C_CONSET_I2EN_ENABLED      0x01   /* Enabled */

#define I2C_CONSET_STA               0x20   /* Bit 5: STA (Start flag) */
#define I2C_CONSET_STA_NO_START      0x00   /* No start */
#define I2C_CONSET_STA_START         0x01   /* Start */

#define I2C_CONSET_STO               0x10   /* Bit 4: STO (Stop flag) */
#define I2C_CONSET_STO_NO_STOP       0x00   /* No stop */
#define I2C_CONSET_STO_STOP          0x01   /* Stop */

#define I2C_CONSET_SI                0x08   /* Bit 3: SI (I2C interrupt flag) */
#define I2C_CONSET_SI_NO_INTERRUPT   0x00   /* No interrupt */
#define I2C_CONSET_SI_INTERRUPT      0x01   /* Interrupt */

#define I2C_CONSET_AA                0x04   /* Bit 2: AA (Assert acknowledge flag) */
#define I2C_CONSET_AA_NO_ACK         0x00   /* Not Acknowledge (Not Acknowledge will be sent when data byte is received) */
#define I2C_CONSET_AA_ACK            0x01   /* Acknowledge (Acknowledge will be sent if certain conditions occur) */

/* I2C status register */
#define I2C_STAT_STATUS              0xff   /* Bit 7-0: STATUS (Status) */

/* I2C data register */

#define I2C_DATA_DATA                0xff   /* Bit 7-0: DATA (Data) */


/* I2C Slave Address Register */

#define I2C_ADDR_ADDR                0xfe   /* Bit 7-1: ADDR (Slave mode address) */

#define I2C_ADDR_GC                  0x01   /* Bit 0: GC (General call bit) */
#define I2C_ADDR_GC_NOT_GENERAL      0x00   /* Not general call */
#define I2C_ADDR_GC_GENERAL          0x01   /* General call received */

/* I2C SCL Duty Cycle high register */
#define I2C_SCLH_COUNT               0xffff /* Bit 15-0: COUNT (Count for SCL high time period) */

/*  I2C SCL Duty Cycle Low Register */

#define I2SCLL_COUNT               0xffff /* Bit 15-0: COUNT (Count for SCL low time period) */


/*  I2C Control Clear Register */

#define I2C_CONCLR_I2ENC             0x40   /* Bit 6: I2ENC (I2C interface disable) */
#define I2C_CONCLR_I2ENC_NO_EFFECT   0x00   /* No effect */
#define I2C_CONCLR_I2ENC_CLEAR       0x01   /* Clear  (I2C Disabled) */

#define I2C_CONCLR_STAC              0x20   /* Bit 5: STAC (Start flag clear) */
#define I2C_CONCLR_STAC_NO_EFFECT    0x00   /* No effect */
#define I2C_CONCLR_STAC_CLEAR        0x01   /* Clear start */

#define I2C_CONCLR_SIC               0x08   /* Bit 3: SIC (I2C interrupt clear) */
#define I2C_CONCLR_SIC_NO_EFFECT     0x00   /* No effect */
#define I2C_CONCLR_SIC_CLEAR         0x01   /* Clear interrupt */

#define I2C_CONCLR_AAC               0x04   /* Bit 2: AAC (Assert acknowledge clear) */
#define I2C_CONCLR_AAC_NO_EFFECT     0x00   /* No effect */
#define I2C_CONCLR_AAC_CLEAR         0x01   /* Clear acknowledge */



static __u8 i2cCheckStatus(void);
       void i2cInit(void);
static char i2cStart(void);
static char i2cRepeatStart(void);
static char i2cStop(void);
static char i2cPutChar(__u8 data);
static char i2cGetChar(__u8 mode, __u8* pData);

int i2c_probe(uchar chip);
int i2c_read(uchar chip, uint addr, int alen, uchar *buffer, int len);
int i2c_write(uchar chip, uint addr, int alen, uchar *buffer, int len);
uchar i2c_reg_read (uchar chip, uchar reg);
void  i2c_reg_write(uchar chip, uchar reg, uchar val);




#endif
