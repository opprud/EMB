#include <config.h>
#include <common.h>
#include <net.h>
#include <asm/arch/hardware.h>
#include "spi.h"

/*
 * Control Registers in Bank 0
 */

#define CTL_REG_ERDPTL   0x00
#define CTL_REG_ERDPTH   0x01
#define CTL_REG_EWRPTL   0x02
#define CTL_REG_EWRPTH   0x03
#define CTL_REG_ETXSTL   0x04
#define CTL_REG_ETXSTH   0x05
#define CTL_REG_ETXNDL   0x06
#define CTL_REG_ETXNDH   0x07
#define CTL_REG_ERXSTL   0x08
#define CTL_REG_ERXSTH   0x09
#define CTL_REG_ERXNDL   0x0A
#define CTL_REG_ERXNDH   0x0B
#define CTL_REG_ERXRDPTL 0x0C
#define CTL_REG_ERXRDPTH 0x0D
#define CTL_REG_ERXWRPTL 0x0E
#define CTL_REG_ERXWRPTH 0x0F
#define CTL_REG_EDMASTL  0x10
#define CTL_REG_EDMASTH  0x11
#define CTL_REG_EDMANDL  0x12
#define CTL_REG_EDMANDH  0x13
#define CTL_REG_EDMADSTL 0x14
#define CTL_REG_EDMADSTH 0x15
#define CTL_REG_EDMACSL  0x16
#define CTL_REG_EDMACSH  0x17
/* these are common in all banks */
#define CTL_REG_EIE      0x1B
#define CTL_REG_EIR      0x1C
#define CTL_REG_ESTAT    0x1D
#define CTL_REG_ECON2    0x1E
#define CTL_REG_ECON1    0x1F

/*
 * Control Registers in Bank 1
 */

#define CTL_REG_EHT0    0x00
#define CTL_REG_EHT1    0x01
#define CTL_REG_EHT2    0x02
#define CTL_REG_EHT3    0x03
#define CTL_REG_EHT4    0x04
#define CTL_REG_EHT5    0x05
#define CTL_REG_EHT6    0x06
#define CTL_REG_EHT7    0x07
#define CTL_REG_EPMM0   0x08
#define CTL_REG_EPMM1   0x09
#define CTL_REG_EPMM2   0x0A
#define CTL_REG_EPMM3   0x0B
#define CTL_REG_EPMM4   0x0C
#define CTL_REG_EPMM5   0x0D
#define CTL_REG_EPMM6   0x0E
#define CTL_REG_EPMM7   0x0F
#define CTL_REG_EPMCSL  0x10
#define CTL_REG_EPMCSH  0x11
#define CTL_REG_EPMOL   0x14
#define CTL_REG_EPMOH   0x15
#define CTL_REG_ERXFCON 0x18
#define CTL_REG_EPKTCNT 0x19

/*
 * Control Registers in Bank 2
 */

#define CTL_REG_MACON1   0x00
#define CTL_REG_MACON3   0x02
#define CTL_REG_MACON4   0x03
#define CTL_REG_MABBIPG  0x04
#define CTL_REG_MAIPGL   0x06
#define CTL_REG_MAIPGH   0x07
#define CTL_REG_MACLCON1 0x08
#define CTL_REG_MACLCON2 0x09
#define CTL_REG_MAMXFLL  0x0A
#define CTL_REG_MAMXFLH  0x0B
//#define CTL_REG_MAPHSUP  0x0D
//#define CTL_REG_MICON    0x11
#define CTL_REG_MICMD    0x12
#define CTL_REG_MIREGADR 0x14
#define CTL_REG_MIWRL    0x16
#define CTL_REG_MIWRH    0x17
#define CTL_REG_MIRDL    0x18
#define CTL_REG_MIRDH    0x19

/*
 * Control Registers in Bank 3
 */

#define CTL_REG_MAADR1  0x00
#define CTL_REG_MAADR0  0x01
#define CTL_REG_MAADR3  0x02
#define CTL_REG_MAADR2  0x03
#define CTL_REG_MAADR5  0x04
#define CTL_REG_MAADR4  0x05
#define CTL_REG_EBSTSD  0x06
#define CTL_REG_EBSTCON 0x07
#define CTL_REG_EBSTCSL 0x08
#define CTL_REG_EBSTCSH 0x09
#define CTL_REG_MISTAT  0x0A
#define CTL_REG_EREVID  0x12
#define CTL_REG_ECOCON  0x15
#define CTL_REG_EFLOCON 0x17
#define CTL_REG_EPAUSL  0x18
#define CTL_REG_EPAUSH  0x19


/*
 * PHY Register
 */

#define PHY_REG_PHCON1 0x00
#define PHY_REG_PHID1  0x02
#define PHY_REG_PHID2  0x03
#define PHY_REG_PHCON2 0x10
#define PHY_REG_PHLCON 0x14


/*
 * Receive Filter Register (ERXFCON) bits
 */

#define ENC_RFR_UCEN  0x80
#define ENC_RFR_ANDOR 0x40
#define ENC_RFR_CRCEN 0x20
#define ENC_RFR_PMEN  0x10
#define ENC_RFR_MPEN  0x08
#define ENC_RFR_HTEN  0x04
#define ENC_RFR_MCEN  0x02
#define ENC_RFR_BCEN  0x01

/*
 * ECON1 Register Bits
 */

#define ENC_ECON1_TXRST  0x80
#define ENC_ECON1_RXRST  0x40
#define ENC_ECON1_DMAST  0x20
#define ENC_ECON1_CSUMEN 0x10
#define ENC_ECON1_TXRTS  0x08
#define ENC_ECON1_RXEN   0x04
#define ENC_ECON1_BSEL1  0x02
#define ENC_ECON1_BSEL0  0x01

/*
 * ECON2 Register Bits
 */
#define ENC_ECON2_AUTOINC 0x80
#define ENC_ECON2_PKTDEC  0x40
#define ENC_ECON2_PWRSV   0x20
#define ENC_ECON2_VRPS    0x08

/*
 * EIR Register Bits
 */
#define ENC_EIR_PKTIF  0x40
#define ENC_EIR_DMAIF  0x20
#define ENC_EIR_LINKIF 0x10
#define ENC_EIR_TXIF   0x08
#define ENC_EIR_WOLIF  0x04
#define ENC_EIR_TXERIF 0x02
#define ENC_EIR_RXERIF 0x01

/*
 * ESTAT Register Bits
 */

#define ENC_ESTAT_INT     0x80
#define ENC_ESTAT_LATECOL 0x10
#define ENC_ESTAT_RXBUSY  0x04
#define ENC_ESTAT_TXABRT  0x02
#define ENC_ESTAT_CLKRDY  0x01

/*
 * EIE Register Bits
 */

#define ENC_EIE_INTIE  0x80
#define ENC_EIE_PKTIE  0x40
#define ENC_EIE_DMAIE  0x20
#define ENC_EIE_LINKIE 0x10
#define ENC_EIE_TXIE   0x08
#define ENC_EIE_WOLIE  0x04
#define ENC_EIE_TXERIE 0x02
#define ENC_EIE_RXERIE 0x01

/*
 * MACON1 Register Bits
 */
#define ENC_MACON1_LOOPBK  0x10
#define ENC_MACON1_TXPAUS  0x08
#define ENC_MACON1_RXPAUS  0x04
#define ENC_MACON1_PASSALL 0x02
#define ENC_MACON1_MARXEN  0x01


/*
 * MACON3 Register Bits
 */
#define ENC_MACON3_PADCFG2 0x80
#define ENC_MACON3_PADCFG1 0x40
#define ENC_MACON3_PADCFG0 0x20
#define ENC_MACON3_TXCRCEN 0x10
#define ENC_MACON3_PHDRLEN 0x08
#define ENC_MACON3_HFRMEN  0x04
#define ENC_MACON3_FRMLNEN 0x02
#define ENC_MACON3_FULDPX  0x01

/*
 * MICMD Register Bits
 */
#define ENC_MICMD_MIISCAN 0x02
#define ENC_MICMD_MIIRD   0x01

/*
 * MISTAT Register Bits
 */
#define ENC_MISTAT_NVALID 0x04
#define ENC_MISTAT_SCAN   0x02
#define ENC_MISTAT_BUSY   0x01

/*
 * PHID1 and PHID2 values
 */
#define ENC_PHID1_VALUE 0x0083
#define ENC_PHID2_VALUE 0x1400
#define ENC_PHID2_MASK  0xFC00


#define ENC_SPI_SLAVE_CS 0x00010000  //pin P1.16
#define ENC_RESET        0x00020000  //pin P1.17

#define FAILSAFE_VALUE 5000

/*
 * Controller memory layout:
 *
 * 0x0000 - 0x17ff  6k bytes receive buffer
 * 0x1800 - 0x1fff  2k bytes transmit buffer
 */
/* Use the lower memory for receiver buffer. See errata pt. 5 */
#define ENC_RX_BUF_START 0x0000
#define ENC_TX_BUF_START 0x1800

/* maximum frame length */
#define ENC_MAX_FRM_LEN 1518

#define enc_enable() PUT32(IO1CLR, ENC_SPI_SLAVE_CS)
#define enc_disable() PUT32(IO1SET, ENC_SPI_SLAVE_CS)
#define enc_cfg_spi() spi_set_cfg(0, 0, 0); spi_set_clock(8); 


static unsigned char encReadReg(unsigned char regNo);
static void encWriteReg(unsigned char regNo, unsigned char data);
static void encWriteRegRetry(unsigned char regNo, unsigned char data, int c);
static void encReadBuff(unsigned short length, unsigned char *pBuff);
static void encWriteBuff(unsigned short length, unsigned char *pBuff);
static void encBitSet(unsigned char regNo, unsigned char data);
static void encBitClr(unsigned char regNo, unsigned char data);
static void encReset(void);
static void encInit(unsigned char *pEthAddr);
static unsigned short phyRead(unsigned char addr);
static void phyWrite(unsigned char addr, unsigned short data);
static void encPoll(void);
static void encRx(void);

#define m_nic_read(reg) encReadReg(reg)
#define m_nic_write(reg, data) encWriteReg(reg, data)
#define m_nic_write_retry(reg, data, count) encWriteRegRetry(reg, data, count)
#define m_nic_read_data(len, buf) encReadBuff((len), (buf))
#define m_nic_write_data(len, buf) encWriteBuff((len), (buf))

// bit field set
#define m_nic_bfs(reg, data) encBitSet(reg, data)

// bit field clear
#define m_nic_bfc(reg, data) encBitClr(reg, data)

static unsigned char bank = 0;  /* current bank in enc28j60 */
static unsigned char next_pointer_lsb;
static unsigned char next_pointer_msb;
static unsigned int  chipRev;

static unsigned char buffer[ENC_MAX_FRM_LEN];
static int rxResetCounter = 0;
#define RX_RESET_COUNTER 1000

static bd_t *local_bis;
/*-----------------------------------------------------------------------------
 * Returns 0 when failes otherwize 1
 */
int eth_init(bd_t *bis)
{
  unsigned char estatVal;

  local_bis = bis;

  /* configure GPIO */
  (*((volatile unsigned long *) IO1DIR)) |= ENC_SPI_SLAVE_CS;
  (*((volatile unsigned long *) IO1DIR)) |= ENC_RESET;

  /* CS and RESET active low */
  PUT32(IO1SET, ENC_SPI_SLAVE_CS);
  PUT32(IO1SET, ENC_RESET);

  PUT32(IO1CLR, ENC_RESET);
  udelay(5000);
  PUT32(IO1SET, ENC_RESET);
  udelay(10000);

  //(*((volatile unsigned long *) PINSEL1)) &= ~0x30000000;
  //(*((volatile unsigned long *) PINSEL1)) |=  0x20000000;
  //PUT32(PINSEL1, 0x20000000);

  spi_init();

  /* Wait for CLKRDY to become set */
  do
  {
    estatVal = m_nic_read(CTL_REG_ESTAT);
  } while ((estatVal & 0x08) || (~estatVal & ENC_ESTAT_CLKRDY));

  /* initialize controller */
  encReset();
  encInit(bis->bi_enetaddr);

  /* Read chip revision */
  m_nic_bfs(CTL_REG_ECON1, (ENC_ECON1_BSEL1 | ENC_ECON1_BSEL0)); //switch to bank #3
  chipRev = m_nic_read(CTL_REG_EREVID);
  printf("\neth_init: detected ENC28J60 chip rev: %d\n", chipRev);
  m_nic_bfc(CTL_REG_ECON1, (ENC_ECON1_BSEL1 | ENC_ECON1_BSEL0)); //switch to bank #0

  m_nic_bfs(CTL_REG_ECON1, ENC_ECON1_RXEN); /* enable receive */  

  encPoll();

  return 0;
}

int eth_send(volatile void *packet, int length)
{
//  printf("eth_send\n");

  /* check frame length, etc. */

  /* switch to bank 0 */
  m_nic_bfc(CTL_REG_ECON1, (ENC_ECON1_BSEL1 | ENC_ECON1_BSEL0));  

  /* set EWRPT */
  m_nic_write(CTL_REG_EWRPTL, ((ENC_TX_BUF_START) & 0xff));
  m_nic_write(CTL_REG_EWRPTH, ((ENC_TX_BUF_START) >> 8));

  /* set ETXND */
  m_nic_write(CTL_REG_ETXNDL, (length + ENC_TX_BUF_START) & 0xff);
  m_nic_write(CTL_REG_ETXNDH, (length + ENC_TX_BUF_START) >> 8);
  
  /* set ETXST */
  m_nic_write(CTL_REG_ETXSTL, ENC_TX_BUF_START & 0xFF);
  m_nic_write(CTL_REG_ETXSTH, ENC_TX_BUF_START >> 8);

  /* write packet */
  m_nic_write_data(length, (unsigned char*)packet);
  
  /* Reset transmit logic (silicon errata workaround) */
//  if (m_nic_read(CTL_REG_EIR) & ENC_EIR_TXERIF)
if (1)
  {
    m_nic_bfs(CTL_REG_ECON1, ENC_ECON1_TXRST);
    m_nic_bfc(CTL_REG_ECON1, ENC_ECON1_TXRST);
  }
  m_nic_bfc(CTL_REG_EIR, (ENC_EIR_TXERIF | ENC_EIR_TXIF));

  /* Set ECON1.TXRTS = Start transmission */
  m_nic_bfs(CTL_REG_ECON1, ENC_ECON1_TXRTS);

  /* check if solicon errata workaround for rev 5 */
  if ((chipRev == 0x05) || (chipRev == 0x06))
  {
    unsigned int attemptCounter = 0;
    while(!(m_nic_read(CTL_REG_EIR) & (ENC_EIR_TXERIF | ENC_EIR_TXIF)) &&
      (++attemptCounter < 1000))
      ;
    if((m_nic_read(CTL_REG_EIR) & ENC_EIR_TXERIF) || (attemptCounter >= 1000))
    {
      unsigned char  readPtrSaveLsb;
      unsigned char  readPtrSaveMsb;
      unsigned short txEnd;
      unsigned char  txStatus[7];
      unsigned int   i;

      //cancel the previous transmission if it has become stuck set
      m_nic_bfc(CTL_REG_ECON1, ENC_ECON1_TXRTS);

      /* switch to bank 0 */
      m_nic_bfc(CTL_REG_ECON1, (ENC_ECON1_BSEL1 | ENC_ECON1_BSEL0));  

      //save the current read pointer
      readPtrSaveLsb = m_nic_read(CTL_REG_ERDPTL);
      readPtrSaveMsb = m_nic_read(CTL_REG_ERDPTH);

      //get location of transmit status vector
      txEnd  = m_nic_read(CTL_REG_ETXNDL);
      txEnd |= ((unsigned short)m_nic_read(CTL_REG_ETXNDH)) << 8;
      txEnd++;

      //read the transmit status vector
      m_nic_write(CTL_REG_ERDPTL, txEnd & 0xff);
      m_nic_write(CTL_REG_ERDPTH, txEnd >> 8);
      m_nic_read_data(7, txStatus);

      //Implement retransmission if a late collision occured
      for(i=0; i<16; i++)
      {
        if((m_nic_read(CTL_REG_EIR) & ENC_EIR_TXERIF) &&
           (txStatus[2] & 0x04)) //bit#2 = Late collision indication bit
        {
          //reset transmit logic
          m_nic_bfs(CTL_REG_ECON1, ENC_ECON1_TXRST);
          m_nic_bfc(CTL_REG_ECON1, ENC_ECON1_TXRST);
          m_nic_bfc(CTL_REG_EIR, ENC_EIR_TXERIF | ENC_EIR_TXIF);

          //transmit the packet again
          m_nic_bfs(CTL_REG_ECON1, ENC_ECON1_TXRTS);
          while(!(m_nic_read(CTL_REG_EIR) & (ENC_EIR_TXERIF | ENC_EIR_TXIF)))
            ;

          //cancel the previous transmission of it becom stuck set
          m_nic_bfc(CTL_REG_ECON1, ENC_ECON1_TXRTS);

          //read the transmit status vector
          m_nic_write(CTL_REG_ERDPTL, txEnd & 0xff);
          m_nic_write(CTL_REG_ERDPTH, txEnd >> 8);
          m_nic_read_data(7, txStatus);
        }
        else
        {
          break;
        }
      }

      //restore the current read pointer
      m_nic_write(CTL_REG_ERDPTL, readPtrSaveLsb);
      m_nic_write(CTL_REG_ERDPTH, readPtrSaveMsb);
    }    
  }

  return 0;
}


/*****************************************************************************
 * This function resets the receiver only. This function may be called from
 * interrupt-context.
 */
static void encReceiverReset(void)
{
  unsigned char econ1;
#if 1
  eth_init(local_bis);
  econ1 = m_nic_read(CTL_REG_ECON1);
  if((econ1 & ENC_ECON1_RXRST) == 0) {
    m_nic_bfs(CTL_REG_ECON1, ENC_ECON1_RXRST);
    rxResetCounter = RX_RESET_COUNTER;
  }
#else
eth_init(local_bis);
#endif
}

/*****************************************************************************
 * receiver reset timer
 */
static void encReceiverResetCallback(void)
{
  m_nic_bfc(CTL_REG_ECON1, ENC_ECON1_RXRST);
  m_nic_bfs(CTL_REG_ECON1, ENC_ECON1_RXEN); /* enable receive */
}

/*-----------------------------------------------------------------------------
 * Check for received packets. Call NetReceive for each packet. The return
 * value is ignored by the caller.
 */
int eth_rx(void)
{
  if(rxResetCounter > 0 && --rxResetCounter == 0)
  {
    encReceiverResetCallback();
  }

  encPoll();

  return 0;
}

void eth_halt(void)
{
//  printf("eth_halt\n");
  m_nic_bfc(CTL_REG_ECON1, ENC_ECON1_RXEN); /* disable receive */
}

/*****************************************************************************/

static void encPoll(void) 
{
  unsigned char eir_reg;
  volatile unsigned char estat_reg;
  unsigned char pkt_cnt;
  unsigned char estat;

  /* clear global interrupt enable bit in enc28j60 */
  m_nic_bfc(CTL_REG_EIE, ENC_EIE_INTIE);
  estat_reg = m_nic_read(CTL_REG_ESTAT);

  eir_reg = m_nic_read(CTL_REG_EIR);

  if (eir_reg & ENC_EIR_TXIF){

    /* check stats */
    estat = m_nic_read(CTL_REG_ESTAT);
    if (estat & ENC_ESTAT_TXABRT)
    {
      printf("\neth: tx error");;
    }
    else
    {
      volatile unsigned short addr;
      addr = m_nic_read(CTL_REG_ETXNDL);
      addr |= (m_nic_read(CTL_REG_ETXNDH) << 8);
      m_nic_write(CTL_REG_ERDPTL, addr & 0xff);
      m_nic_write(CTL_REG_ERDPTH, addr >> 8);    
      m_nic_read_data(6, buffer);
    }



    /* clear TXIF bit in EIR */
    m_nic_bfc(CTL_REG_EIR, ENC_EIR_TXIF);
  }

  /* We have to use pktcnt and not pktif bit, see errata pt. 6 */

  /* move to bank 1 */
  m_nic_bfc(CTL_REG_ECON1, ENC_ECON1_BSEL1);
  m_nic_bfs(CTL_REG_ECON1, ENC_ECON1_BSEL0);
  
  /* read pktcnt */
  pkt_cnt = m_nic_read(CTL_REG_EPKTCNT);
  
  if (pkt_cnt > 0) {
    if ((eir_reg & ENC_EIR_PKTIF) == 0) {
      //printf("encPoll: pkt cnt > 0, but pktif not set\n");
    }
    encRx();
    /* clear PKTIF bit in EIR, this should not need to be done but it
       seems like we get problems if we do not */
    m_nic_bfc(CTL_REG_EIR, ENC_EIR_PKTIF);
  }

  if (eir_reg & ENC_EIR_RXERIF) {
    printf("encPoll: rx error\n");
    m_nic_bfc(CTL_REG_EIR, ENC_EIR_RXERIF);
  }
  if (eir_reg & ENC_EIR_TXERIF) {
    printf("encPoll: tx error\n");
    m_nic_bfc(CTL_REG_EIR, ENC_EIR_TXERIF);
  }

  /* set global interrupt enable bit in enc28j60 */
  m_nic_bfs(CTL_REG_EIE, ENC_EIE_INTIE);
}

static void encRx(void)
{
  unsigned short pkt_len;
  unsigned short copy_len;
  unsigned short status;
  unsigned char  eir_reg;
  unsigned char  pkt_cnt = 0;
  unsigned int   next_pointer;

  /* switch to bank 0 */
  m_nic_bfc(CTL_REG_ECON1, (ENC_ECON1_BSEL1 | ENC_ECON1_BSEL0));

  m_nic_write(CTL_REG_ERDPTL, next_pointer_lsb);
  m_nic_write(CTL_REG_ERDPTH, next_pointer_msb);

  do {
    m_nic_read_data(6, buffer);
    next_pointer_lsb = buffer[0];
    next_pointer_msb = buffer[1];
    pkt_len = buffer[2];
    pkt_len |= (unsigned short)buffer[3] << 8;
    status = buffer[4];
    status |= (unsigned short)buffer[5] << 8;

    if (pkt_len <= ENC_MAX_FRM_LEN){
      copy_len = pkt_len;
    } else {
      copy_len = 0;
//      printf("eth_rx: copy_len=0, pkt_len = %d\n", pkt_len);
//      p_priv->stats.rx_dropped++;
      /* we will drop this packet */
    }

    if ((status & (1L << 7)) == 0) { /* check Received Ok bit */
      copy_len = 0;
//      printf("eth_rx: copy_len=0, rx OK bit not set\n");
//      p_priv->stats.rx_errors++;
    }

    /* check if "next pointer" is within valid bounds */
    if ((((unsigned int)next_pointer_msb << 8) | (unsigned int) next_pointer_lsb) >= ENC_TX_BUF_START)
    {
      copy_len = 0;
//      printf("eth_rx: copy_len=0, nextptr not within bound msb = 0x%x, lsb = 0x%x\n", (unsigned int)next_pointer_msb, (unsigned int)next_pointer_lsb);
//      p_priv->stat.rx_errors++;
    }
    
    if (copy_len > 0) { 
      m_nic_read_data(copy_len, buffer);
    }    

    /* switch to bank 0 */
    m_nic_bfc(CTL_REG_ECON1, (ENC_ECON1_BSEL1 | ENC_ECON1_BSEL0));    
    
    if (copy_len == 0) {
      eir_reg = m_nic_read(CTL_REG_EIR);
//      printf("eth_rx: copy_len=0\n");
      encReceiverReset();
      break;  //continue;
    }

//printf("eth_rx(%d)\n", pkt_len);
    NetReceive( (char*)(buffer), pkt_len);

    eir_reg = m_nic_read(CTL_REG_EIR);

    /* switch to bank # 0 */
    m_nic_bfc(CTL_REG_ECON1, (ENC_ECON1_BSEL1 | ENC_ECON1_BSEL0));

    /* decrease packet counter */
    m_nic_bfs(CTL_REG_ECON2, ENC_ECON2_PKTDEC);

    next_pointer = (((unsigned int)next_pointer_msb) << 8) | (unsigned int)next_pointer_lsb;
    next_pointer -= 1;
    if (next_pointer >= ENC_TX_BUF_START)
      next_pointer = ENC_TX_BUF_START - 1;
    m_nic_write(CTL_REG_ERXRDPTL, next_pointer & 0xff);
    m_nic_write(CTL_REG_ERXRDPTH, next_pointer >> 8);

    /* switch to bank #1 */
    m_nic_bfc(CTL_REG_ECON1, ENC_ECON1_BSEL1);
    m_nic_bfs(CTL_REG_ECON1, ENC_ECON1_BSEL0);

    /* read pktcnt */
    pkt_cnt = m_nic_read(CTL_REG_EPKTCNT);

  }while (pkt_cnt);     /* Use EPKTCNT not EIR.PKTIF flag, see errata pt.. 6 */

}

static void encWriteReg(unsigned char regNo, unsigned char data)
{
  spi_lock();
  enc_cfg_spi();
  enc_enable();
  
  spi_write(0x40 | regNo);  /* write in regNo */
  spi_write(data);

  enc_disable();
  enc_enable();
  
  spi_write(0x1f);      /* write reg 0x1f */

  enc_disable();
  spi_unlock();
}

static void encWriteRegRetry(unsigned char regNo, unsigned char data, int c)
{
  unsigned char readback;
  int i;
  
  spi_lock();
  
  for (i=0; i<c; i++) {
    enc_cfg_spi();
    enc_enable();
    
    spi_write(0x40 | regNo);    /* write in regNo */
    spi_write(data);
    
    enc_disable();
    enc_enable();
    
    spi_write(0x1f);        /* write reg 0x1f */
    
    enc_disable();

    spi_unlock();       /* we must unlock spi first */

    readback = encReadReg(regNo);

    spi_lock();

    if (readback == data)
      break;
  }
  spi_unlock();

  if (i == c) {
    printf("enc28j60: write reg %d failed\n", regNo);
  }
}

static unsigned char encReadReg(unsigned char regNo)
{
  unsigned char rxByte;

  spi_lock();
  enc_cfg_spi();
  enc_enable();

  spi_write(0x1f);      /* read reg 0x1f */
  
  bank = spi_read() & 0x3;

  enc_disable();
  enc_enable();

  spi_write(regNo);
  rxByte = spi_read();
  
  /* check if MAC or MII register */
  if (((bank == 2) && (regNo <= 0x1a)) ||
      ((bank == 3) && (regNo <= 0x05 || regNo == 0x0a))) {
    /* ignore first byte and read another byte */    
    rxByte = spi_read();
  }  

  enc_disable();
  spi_unlock();

  return rxByte;
}

static void encReadBuff(unsigned short length, unsigned char *pBuff)
{
  spi_lock();
  enc_cfg_spi();
  enc_enable();

  spi_write(0x20 | 0x1a);         /* read buffer memory */
  
  while(length--){
    if(pBuff != NULL)
      *pBuff++ = spi_read();
    else
      spi_write(0);
  }

  enc_disable();
  spi_unlock();
}

static void encWriteBuff(unsigned short length, 
             unsigned char *pBuff)
{
  spi_lock();
  enc_cfg_spi();
  enc_enable();

  spi_write(0x60 | 0x1a);         /* write buffer memory */
  
  spi_write(0x00);      /* control byte */

  while(length--)
    spi_write(*pBuff++);
  
  enc_disable();
  spi_unlock();
}

static void encBitSet(unsigned char regNo, unsigned char data)
{
  spi_lock();
  enc_cfg_spi();
  enc_enable();

  spi_write(0x80 | regNo);  /* bit field set */
  spi_write(data);

  enc_disable();
  spi_unlock();
}

static void encBitClr(unsigned char regNo, unsigned char data)
{
  spi_lock();
  enc_cfg_spi();
  enc_enable();

  spi_write(0xA0 | regNo);  /* bit field clear */
  spi_write(data);

  enc_disable();
  spi_unlock();
}

static void encReset(void)
{
#if 0
  (*((volatile unsigned long*)IO1CLR)) &= ENC_RESET;
  mdelay(5);
  (*((volatile unsigned long*)IO1SET)) &= ENC_RESET;
  mdelay(10);
#endif

  spi_lock();
  enc_cfg_spi();
  enc_enable();

  spi_write(0xff);      /* soft reset */

  enc_disable();
  spi_unlock();

  /* sleep 1 ms. See errata pt. 2 */
  udelay(1000);

}

static void encInit(unsigned char *pEthAddr)
{
  unsigned short phid1 = 0;
  unsigned short phid2 = 0;
  static unsigned char dummyByte = 0;

  /* switch to bank 0 */
  m_nic_bfc(CTL_REG_ECON1, (ENC_ECON1_BSEL1 | ENC_ECON1_BSEL0));

  /* 
   * Setup the buffer space. The reset values are valid for the
   * other pointers. 
   */  
  m_nic_write(CTL_REG_ERXSTL,    ENC_RX_BUF_START      &  0xff);
  m_nic_write(CTL_REG_ERXSTH,    ENC_RX_BUF_START      >> 8);
  m_nic_write(CTL_REG_ERXRDPTL, (ENC_TX_BUF_START - 1) &  0xff);
  m_nic_write(CTL_REG_ERXRDPTH, (ENC_TX_BUF_START - 1) >> 8);
  m_nic_write(CTL_REG_ERXNDL,   (ENC_TX_BUF_START - 1) & 0xff);
  m_nic_write(CTL_REG_ERXNDH,   (ENC_TX_BUF_START - 1) >> 8);
  m_nic_write(CTL_REG_ETXSTL,    ENC_TX_BUF_START      & 0xff);
  m_nic_write(CTL_REG_ETXSTH,    ENC_TX_BUF_START      >> 8);
  m_nic_write(CTL_REG_ERDPTL,    ENC_RX_BUF_START      & 0xff);
  m_nic_write(CTL_REG_ERDPTH,    ENC_RX_BUF_START      >> 8);

  // Write a permanant per packet control byte of 0x00
  m_nic_write(CTL_REG_EWRPTL,    ENC_TX_BUF_START      & 0xff);
  m_nic_write(CTL_REG_EWRPTH,    ENC_TX_BUF_START      >> 8);
  encWriteBuff(1, &dummyByte);

  next_pointer_lsb = (ENC_RX_BUF_START & 0xFF);
  next_pointer_msb = (ENC_RX_BUF_START >> 8);

#if 0
  /* Setup receive filters. */

  // move to bank 1
  m_nic_bfc(CTL_REG_ECON1, ENC_ECON1_BSEL1);
  m_nic_bfs(CTL_REG_ECON1, ENC_ECON1_BSEL0);

  // OR-filtering, Unicast, CRC-check and broadcast
  m_nic_write_retry(CTL_REG_ERXFCON, 
            (ENC_RFR_UCEN|ENC_RFR_CRCEN|ENC_RFR_BCEN), 1);

  /* Wait for Oscillator Start-up Timer (OST). */

  while((m_nic_read(CTL_REG_ESTAT) & ENC_ESTAT_CLKRDY) == 0) {
    static int cnt = 0;    
    if(cnt++ >= 1000){
      cnt = 0;
    }
  }
#endif
  
  /* verify identification */
  phid1 = phyRead(PHY_REG_PHID1);
  phid2 = phyRead(PHY_REG_PHID2);

  if(phid1 != ENC_PHID1_VALUE 
     || (phid2 & ENC_PHID2_MASK) != ENC_PHID2_VALUE){
    printf("ERROR: failed to identify controller\n");
    printf("phid1 = %x, phid2 = %x\n", 
       phid1, (phid2&ENC_PHID2_MASK));
    printf("should be phid1 = %x, phid2 = %x\n", 
       ENC_PHID1_VALUE, ENC_PHID2_VALUE);
  }
  
  /*
   * --- MAC Initialization ---
   */

  /* Pull MAC out of Reset */

  /* switch to bank 2 */
  m_nic_bfc(CTL_REG_ECON1, ENC_ECON1_BSEL0);
  m_nic_bfs(CTL_REG_ECON1, ENC_ECON1_BSEL1);

  /* clear MAC reset bits */
//  m_nic_write_retry(CTL_REG_MACON2, 0, 1);

  /* enable MAC to receive frames */
  m_nic_write_retry(CTL_REG_MACON1, ENC_MACON1_MARXEN | ENC_MACON1_TXPAUS | ENC_MACON1_RXPAUS, 10);

  /* configure pad, tx-crc and duplex */ 
  m_nic_write_retry(CTL_REG_MACON3, (ENC_MACON3_PADCFG0 | ENC_MACON3_TXCRCEN | ENC_MACON3_FRMLNEN), 10);

  /* 
   * Set MAC back-to-back inter-packet gap. Recommended 0x12 for half duplex
   * and 0x15 for full duplex.
   */
  m_nic_write_retry(CTL_REG_MABBIPG, 0x12, 10);

  /* allow infinite deferals if the medium is continously busy */
  m_nic_write_retry(CTL_REG_MACON4, (1<<6), 10);

  /* late collisions occur beyond 63 bytes */
  m_nic_write_retry(CTL_REG_MACLCON2, 63, 10);

  /* Set (low byte) Non-Back-to_Back Inter-Packet Gap. Recommended 0x12 */
  m_nic_write_retry(CTL_REG_MAIPGL, 0x12, 10);

  /* 
   * Set (high byte) Non-Back-to_Back Inter-Packet Gap. Recommended 
   * 0x0c for half-duplex. Nothing for full-duplex
   */
  m_nic_write_retry(CTL_REG_MAIPGH, 0x0C, 10);

  /* set maximum frame length */
  m_nic_write_retry(CTL_REG_MAMXFLL, (ENC_MAX_FRM_LEN & 0xff), 10);
  m_nic_write_retry(CTL_REG_MAMXFLH, (ENC_MAX_FRM_LEN >> 8), 10);

  /* set MAC address */

  /* switch to bank 3 */
  m_nic_bfs(CTL_REG_ECON1, (ENC_ECON1_BSEL0|ENC_ECON1_BSEL1));

  m_nic_write_retry(CTL_REG_MAADR0, pEthAddr[5], 10);
  m_nic_write_retry(CTL_REG_MAADR1, pEthAddr[4], 10);
  m_nic_write_retry(CTL_REG_MAADR2, pEthAddr[3], 10);
  m_nic_write_retry(CTL_REG_MAADR3, pEthAddr[2], 10);
  m_nic_write_retry(CTL_REG_MAADR4, pEthAddr[1], 10);
  m_nic_write_retry(CTL_REG_MAADR5, pEthAddr[0], 10);

  /* */
  phyWrite(PHY_REG_PHCON2, (1<<8));

  /* configure LEDA to display LINK status and LEDB to display TX/RX activity */
  phyWrite(PHY_REG_PHLCON, 0x0472);

  /* reset PDPXMD-bit = half duplex */
  phyWrite(PHY_REG_PHCON1, 0);

  /* switch to bank 0 */
  m_nic_bfc(CTL_REG_ECON1, (ENC_ECON1_BSEL0|ENC_ECON1_BSEL1));

  /*
   * Receive settings
   */

  /* auto-increment RX-pointer when reading a received packet */
//  m_nic_bfs(CTL_REG_ECON2, ENC_ECON2_AUTOINC);

  /* enable interrupts */
  m_nic_bfs(CTL_REG_EIE, ENC_EIE_PKTIE);
  m_nic_bfs(CTL_REG_EIE, ENC_EIE_TXIE);
  m_nic_bfs(CTL_REG_EIE, ENC_EIE_RXERIE);
  m_nic_bfs(CTL_REG_EIE, ENC_EIE_TXERIE);
  m_nic_bfs(CTL_REG_EIE, ENC_EIE_INTIE);
}

/*****************************************************************************
 *
 * Description:
 *    Read PHY registers.
 *
 *    NOTE! This function will change to Bank 2.
 *
 * Params:
 *    [in] addr address of the register to read
 *
 * Returns:
 *    The value in the register
 */
static unsigned short phyRead(unsigned char addr)
{
  unsigned short ret = 0;

  /* move to bank 2 */
  m_nic_bfc(CTL_REG_ECON1, ENC_ECON1_BSEL0);
  m_nic_bfs(CTL_REG_ECON1, ENC_ECON1_BSEL1);

  /* write address to MIREGADR */
  m_nic_write(CTL_REG_MIREGADR, addr);

  /* set MICMD.MIIRD */
  m_nic_write(CTL_REG_MICMD, ENC_MICMD_MIIRD);

  /* switch to bank #3 */
  m_nic_bfs(CTL_REG_ECON1, (ENC_ECON1_BSEL0 | ENC_ECON1_BSEL1));

  /* poll MISTAT.BUSY bit until operation is complete */
  while((m_nic_read(CTL_REG_MISTAT) & ENC_MISTAT_BUSY) != 0){
    static int cnt = 0;
    
    if(cnt++ >= 1000){
      /* printf("#"); */
      cnt = 0;
    }
  }

  /* switch to bank #2 */
  m_nic_bfc(CTL_REG_ECON1, ENC_ECON1_BSEL0);
  m_nic_bfs(CTL_REG_ECON1, ENC_ECON1_BSEL1);

  /* clear MICMD.MIIRD */
  m_nic_write(CTL_REG_MICMD, 0);

  ret  = (m_nic_read(CTL_REG_MIRDH) << 8);
  ret |= (m_nic_read(CTL_REG_MIRDL) & 0xFF);

  return ret;
}

/*****************************************************************************
 *
 * Description:
 *    Write PHY registers.
 *
 *    NOTE! This function will change to Bank 3.
 *
 * Params:
 *    [in] addr address of the register to write to
 *    [in] data to be written
 *
 * Returns:
 *    None
 */
static void phyWrite(unsigned char addr, unsigned short data)
{
  /* move to bank 2 */
  m_nic_bfc(CTL_REG_ECON1, ENC_ECON1_BSEL0);
  m_nic_bfs(CTL_REG_ECON1, ENC_ECON1_BSEL1);

  /* write address to MIREGADR */
  m_nic_write(CTL_REG_MIREGADR, addr);

  m_nic_write(CTL_REG_MIWRL, data & 0xff);
  m_nic_write(CTL_REG_MIWRH, data >> 8);

  /* move to bank 3 */
  m_nic_bfs(CTL_REG_ECON1, ENC_ECON1_BSEL0);
  m_nic_bfs(CTL_REG_ECON1, ENC_ECON1_BSEL1);

  /* poll MISTAT.BUSY bit until operation is complete */
  while((m_nic_read(CTL_REG_MISTAT) & ENC_MISTAT_BUSY) != 0){
    static int cnt = 0;
    
    if(cnt++ >= 1000){
      /* printf("#"); */
      cnt = 0;
    }
  }
}

