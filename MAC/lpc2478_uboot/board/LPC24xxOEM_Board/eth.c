#include <config.h>
#include <common.h>
#include <net.h>
#include <asm/arch/hardware.h>



/******************************************************************************
 * Typedefs and defines
 *****************************************************************************/
#define MAX_PHY_INIT_RETRY 10

/* EMAC MODULE ID   */
#define OLD_EMAC_MODULE_ID  ((0x3902 << 16) | 0x2000)

/* MAC registers and parameters */
#define PCONP_EMAC_CLOCK    0x40000000

#define SPEED_100           1
#define SPEED_10            0
#define FULL_DUPLEX         1
#define HALF_DUPLEX         0

#define EMAC_RAM_ADDR       0x7FE00000
#define EMAC_RAM_SIZE       0x00004000

//#define EMAC_TX_DESCRIPTOR_COUNT    0x0010
//#define EMAC_RX_DESCRIPTOR_COUNT    0x0010

#define EMAC_TX_DESCRIPTOR_COUNT    5
#define EMAC_RX_DESCRIPTOR_COUNT    4


/*
 * (Rx|Tx)Descriptor ::
 *   [4] packet  - base address of the buffer containing the data
 *   [4] control - control information
 */
#define TX_DESCRIPTOR_SIZE  (EMAC_TX_DESCRIPTOR_COUNT * 8)
#define RX_DESCRIPTOR_SIZE  (EMAC_RX_DESCRIPTOR_COUNT * 8)

/* 
 * TxStatus ::
 *   [4] status
 */
#define TX_STATUS_SIZE      (EMAC_TX_DESCRIPTOR_COUNT * 4)

/* 
 * RxStatus ::
 *   [4] status        - receive status flags
 *   [4] statusHashCRC - concat of dest addr hash CRC and src addr CRC
 */
#define RX_STATUS_SIZE      (EMAC_RX_DESCRIPTOR_COUNT * 8)


#define TOTAL_DESCRIPTOR_SIZE   (TX_DESCRIPTOR_SIZE + RX_DESCRIPTOR_SIZE + TX_STATUS_SIZE + RX_STATUS_SIZE)

/* descriptors are placed at the end of the emac address space */
#define EMAC_DESCRIPTOR_ADDR    (EMAC_RAM_ADDR + EMAC_RAM_SIZE - TOTAL_DESCRIPTOR_SIZE) 

#define TX_DESCRIPTOR_ADDR  EMAC_DESCRIPTOR_ADDR
#define TX_STATUS_ADDR      (EMAC_DESCRIPTOR_ADDR + TX_DESCRIPTOR_SIZE)
#define RX_DESCRIPTOR_ADDR  (TX_STATUS_ADDR + TX_STATUS_SIZE)
#define RX_STATUS_ADDR      (RX_DESCRIPTOR_ADDR + RX_DESCRIPTOR_SIZE)

#define EMAC_DMA_ADDR       EMAC_RAM_ADDR
#define EMAC_DMA_SIZE       EMAC_RAM_ADDR + EMAC_RAM_END - TOTAL_DESCRIPTOR_SIZE


#define EMAC_BLOCK_SIZE         0x600
#define EMAC_TX_BLOCK_NUM       5   
#define EMAC_RX_BLOCK_NUM       5
#define TOTAL_EMAC_BLOCK_NUM    10

#define EMAC_BUFFER_SIZE    (EMAC_BLOCK_SIZE * (EMAC_TX_BLOCK_NUM + EMAC_RX_BLOCK_NUM ))
#define EMAC_TX_BUFFER_ADDR EMAC_RAM_ADDR
#define EMAC_RX_BUFFER_ADDR (EMAC_RAM_ADDR + EMAC_BLOCK_SIZE * EMAC_TX_BLOCK_NUM) 


/* EMAC Descriptor TX and RX Control fields */
#define EMAC_TX_DESC_INT        0x80000000
#define EMAC_TX_DESC_LAST       0x40000000
#define EMAC_TX_DESC_CRC        0x20000000
#define EMAC_TX_DESC_PAD        0x10000000
#define EMAC_TX_DESC_HUGE       0x08000000
#define EMAC_TX_DESC_OVERRIDE   0x04000000

#define EMAC_RX_DESC_INT        0x80000000

/* EMAC Descriptor status related definition */
#define TX_DESC_STATUS_ERR      0x80000000
#define TX_DESC_STATUS_NODESC   0x40000000
#define TX_DESC_STATUS_UNDERRUN 0x20000000
#define TX_DESC_STATUS_LCOL     0x10000000
#define TX_DESC_STATUS_ECOL     0x08000000
#define TX_DESC_STATUS_EDEFER   0x04000000
#define TX_DESC_STATUS_DEFER    0x02000000
#define TX_DESC_STATUS_COLCNT   0x01E00000  /* four bits, it's a mask, not exact count */

#define RX_DESC_STATUS_ERR      0x80000000
#define RX_DESC_STATUS_LAST     0x40000000
#define RX_DESC_STATUS_NODESC   0x20000000
#define RX_DESC_STATUS_OVERRUN  0x10000000
#define RX_DESC_STATUS_ALGNERR  0x08000000
#define RX_DESC_STATUS_RNGERR   0x04000000
#define RX_DESC_STATUS_LENERR   0x02000000
#define RX_DESC_STATUS_SYMERR   0x01000000
#define RX_DESC_STATUS_CRCERR   0x00800000
#define RX_DESC_STATUS_BCAST    0x00400000
#define RX_DESC_STATUS_MCAST    0x00200000
#define RX_DESC_STATUS_FAILFLT  0x00100000
#define RX_DESC_STATUS_VLAN     0x00080000
#define RX_DESC_STATUS_CTLFRAM  0x00040000

#define DESC_SIZE_MASK          0x000007FF  /* 11 bits for both TX and RX */

/* EMAC interrupt controller related definition */
#define EMAC_INT_RXOVERRUN  0x01 << 0
#define EMAC_INT_RXERROR    0x01 << 1 
#define EMAC_INT_RXFINISHED 0x01 << 2
#define EMAC_INT_RXDONE     0x01 << 3 
#define EMAC_INT_TXUNDERRUN 0x01 << 4
#define EMAC_INT_TXERROR    0x01 << 5 
#define EMAC_INT_TXFINISHED 0x01 << 6
#define EMAC_INT_TXDONE     0x01 << 7 
#define EMAC_INT_SOFTINT    0x01 << 12
#define EMAC_INT_WOL        0x01 << 13 


/* Micrel KSZ8001 PHY related registers */

/* PHY_ADDR, by default, AD0 has pull-up, AD1~4 have pull-downs, 
so, the default address is 0x0001 */
//#define PHY_ADDR        (0x0000 << 8)   /* in MAC_MADR, bit 8~12 */

static volatile int phyAddr = 0;
#define NATIONAL_PHY    0
#define MICREL_PHY      1
static volatile int phyType = MICREL_PHY;

#define PHY_BMCR        0x0000
#define PHY_BMSR        0x0001
#define PHY_PHYIDR1     0x0002
#define PHY_PHYIDR2     0x0003
#define PHY_ANAR        0x0004
#define PHY_ANLPAR      0x0005
#define PHY_ANLPARNP    0x0005
#define PHY_ANER        0x0006
#define PHY_ANNPTR      0x0007

/* Extended PHY registers */
/* Below is the National PHY definition */
#define NSM_PHY_PHYSTS		0x0010
#define NSM_PHY_MICR		0x0011
#define NSM_PHY_MISR		0x0012
#define NSM_PHY_RESERVED1	0x0013
#define NSM_PHY_FCSCR		0x0014
#define NSM_PHY_RECR		0x0015
#define NSM_PHY_PCSR		0x0016
#define NSM_PHY_RBR			0x0017
#define NSM_PHY_LEDCR		0x0018
#define NSM_PHY_PHYCR		0x0019
#define NSM_PHY_10BTSCR		0x001A
#define NSM_PHY_CDCTRL1		0x001B
#define NSM_PHY_RESERVED2	0x001C
#define NSM_PHY_EDCR		0x001D

/* Below is the Micrel PHY definition */
#define MIC_PHY_RXER_CNT			0x0015
#define MIC_PHY_INT_CTRL			0x001B
#define MIC_PHY_LINKMD_CTRL			0x001D
#define MIC_PHY_PHY_CTRL			0x001E
#define MIC_PHY_100BASE_PHY_CTRL	0x001F

/* BMCR setting */
#define BMCR_RESET          0x8000
#define BMCR_LOOPBACK       0x4000
#define BMCR_SPEED_100      0x2000
#define BMCR_AN             0x1000
#define BMCR_POWERDOWN      0x0800
#define BMCR_ISOLATE        0x0400
#define BMCR_RE_AN          0x0200
#define BMCR_DUPLEX         0x0100

/* BMSR setting */
#define BMSR_100BE_T4       0x8000
#define BMSR_100TX_FULL     0x4000
#define BMSR_100TX_HALF     0x2000
#define BMSR_10BE_FULL      0x1000
#define BMSR_10BE_HALF      0x0800
#define BMSR_AUTO_DONE      0x0020
#define BMSR_REMOTE_FAULT   0x0010
#define BMSR_NO_AUTO        0x0008
#define BMSR_LINK_ESTABLISHED   0x0004

#define MII_BMSR_TIMEOUT    0x1000000



/* Ethernet MAC (32 bit data bus) -- all registers are RW unless indicated in parentheses */
#define MAC_BASE_ADDR       0xFFE00000 /* AHB Peripheral # 0 */
#define EMAC_MAC1            (MAC_BASE_ADDR + 0x000) /* MAC config reg 1 */
#define EMAC_MAC2            (MAC_BASE_ADDR + 0x004) /* MAC config reg 2 */
#define EMAC_IPGT            (MAC_BASE_ADDR + 0x008) /* b2b InterPacketGap reg */
#define EMAC_IPGR            (MAC_BASE_ADDR + 0x00C) /* non b2b InterPacketGap reg */
#define EMAC_CLRT            (MAC_BASE_ADDR + 0x010) /* CoLlision window/ReTry reg */
#define EMAC_MAXF            (MAC_BASE_ADDR + 0x014) /* MAXimum Frame reg */
#define EMAC_SUPP            (MAC_BASE_ADDR + 0x018) /* PHY SUPPort reg */
#define EMAC_TEST            (MAC_BASE_ADDR + 0x01C) /* TEST reg */
#define EMAC_MCFG            (MAC_BASE_ADDR + 0x020) /* MII Mgmt ConFiG reg */
#define EMAC_MCMD            (MAC_BASE_ADDR + 0x024) /* MII Mgmt CoMmanD reg */
#define EMAC_MADR            (MAC_BASE_ADDR + 0x028) /* MII Mgmt ADdRess reg */
#define EMAC_MWTD            (MAC_BASE_ADDR + 0x02C) /* MII Mgmt WriTe Data reg (WO) */
#define EMAC_MRDD            (MAC_BASE_ADDR + 0x030) /* MII Mgmt ReaD Data reg (RO) */
#define EMAC_MIND            (MAC_BASE_ADDR + 0x034) /* MII Mgmt INDicators reg (RO) */

#define EMAC_SA0             (MAC_BASE_ADDR + 0x040) /* Station Address 0 reg */
#define EMAC_SA1             (MAC_BASE_ADDR + 0x044) /* Station Address 1 reg */
#define EMAC_SA2             (MAC_BASE_ADDR + 0x048) /* Station Address 2 reg */

#define EMAC_COMMAND         (MAC_BASE_ADDR + 0x100) /* Command reg */
#define EMAC_STATUS          (MAC_BASE_ADDR + 0x104) /* Status reg (RO) */
#define EMAC_RXDESCRIPTOR    (MAC_BASE_ADDR + 0x108) /* Rx descriptor base address reg */
#define EMAC_RXSTATUS        (MAC_BASE_ADDR + 0x10C) /* Rx status base address reg */
#define EMAC_RXDESCRIPTORNUM (MAC_BASE_ADDR + 0x110) /* Rx number of descriptors reg */
#define EMAC_RXPRODUCEINDEX  (MAC_BASE_ADDR + 0x114) /* Rx produce index reg (RO) */
#define EMAC_RXCONSUMEINDEX  (MAC_BASE_ADDR + 0x118) /* Rx consume index reg */
#define EMAC_TXDESCRIPTOR    (MAC_BASE_ADDR + 0x11C) /* Tx descriptor base address reg */
#define EMAC_TXSTATUS        (MAC_BASE_ADDR + 0x120) /* Tx status base address reg */
#define EMAC_TXDESCRIPTORNUM (MAC_BASE_ADDR + 0x124) /* Tx number of descriptors reg */
#define EMAC_TXPRODUCEINDEX  (MAC_BASE_ADDR + 0x128) /* Tx produce index reg */
#define EMAC_TXCONSUMEINDEX  (MAC_BASE_ADDR + 0x12C) /* Tx consume index reg (RO) */

#define EMAC_TSV0            (MAC_BASE_ADDR + 0x158) /* Tx status vector 0 reg (RO) */
#define EMAC_TSV1            (MAC_BASE_ADDR + 0x15C) /* Tx status vector 1 reg (RO) */
#define EMAC_RSV             (MAC_BASE_ADDR + 0x160) /* Rx status vector reg (RO) */

#define EMAC_FLOWCONTROLCNT  (MAC_BASE_ADDR + 0x170) /* Flow control counter reg */
#define EMAC_FLOWCONTROLSTS  (MAC_BASE_ADDR + 0x174) /* Flow control status reg */

#define EMAC_RXFILTERCTRL    (MAC_BASE_ADDR + 0x200) /* Rx filter ctrl reg */
#define EMAC_RXFILTERWOLSTS  (MAC_BASE_ADDR + 0x204) /* Rx filter WoL status reg (RO) */
#define EMAC_RXFILTERWOLCLR  (MAC_BASE_ADDR + 0x208) /* Rx filter WoL clear reg (WO) */

#define EMAC_HASHFILTERL     (MAC_BASE_ADDR + 0x210) /* Hash filter LSBs reg */
#define EMAC_HASHFILTERH     (MAC_BASE_ADDR + 0x214) /* Hash filter MSBs reg */

#define EMAC_INTSTATUS       (MAC_BASE_ADDR + 0xFE0) /* Interrupt status reg (RO) */
#define EMAC_INTENABLE       (MAC_BASE_ADDR + 0xFE4) /* Interrupt enable reg  */
#define EMAC_INTCLEAR        (MAC_BASE_ADDR + 0xFE8) /* Interrupt clear reg (WO) */
#define EMAC_INTSET          (MAC_BASE_ADDR + 0xFEC) /* Interrupt set reg (WO) */

#define EMAC_POWERDOWN       (MAC_BASE_ADDR + 0xFF4) /* Power-down reg */
#define EMAC_MODULEID        (MAC_BASE_ADDR + 0xFFC) /* Module ID reg (RO) */


/* System Control Block(SCB) modules include Memory Accelerator Module,
Phase Locked Loop, VPB divider, Power Control, External Interrupt, 
Reset, and Code Security/Debugging */
#define SCB_BASE_ADDR   0xE01FC000

/* Memory Accelerator Module (MAM) */
#define MAMCR          (SCB_BASE_ADDR + 0x000)
#define MAMTIM         (SCB_BASE_ADDR + 0x004)
#define MEMMAP         (SCB_BASE_ADDR + 0x040)

/* Phase Locked Loop (PLL) */
#define PLLCON         (SCB_BASE_ADDR + 0x080)
#define PLLCFG         (SCB_BASE_ADDR + 0x084)
#define PLLSTAT        (SCB_BASE_ADDR + 0x088)
#define PLLFEED        (SCB_BASE_ADDR + 0x08C)

/* Power Control */
#define PCON           (SCB_BASE_ADDR + 0x0C0)
#define PCONP          (SCB_BASE_ADDR + 0x0C4)

/* Pin Connect Block */
#define PINSEL_BASE_ADDR    0xE002C000
#define PINSEL0        (PINSEL_BASE_ADDR + 0x00)
#define PINSEL1        (PINSEL_BASE_ADDR + 0x04)
#define PINSEL2        (PINSEL_BASE_ADDR + 0x08)
#define PINSEL3        (PINSEL_BASE_ADDR + 0x0C)
#define PINSEL4        (PINSEL_BASE_ADDR + 0x10)
#define PINSEL5        (PINSEL_BASE_ADDR + 0x14)
#define PINSEL6        (PINSEL_BASE_ADDR + 0x18)
#define PINSEL7        (PINSEL_BASE_ADDR + 0x1C)
#define PINSEL8        (PINSEL_BASE_ADDR + 0x20)
#define PINSEL9        (PINSEL_BASE_ADDR + 0x24)
#define PINSEL10       (PINSEL_BASE_ADDR + 0x28)


#define CMD_RX_ENABLE 0x01
#define CMD_TX_ENABLE 0x02

#ifndef IRQ_EXT3
  #define IRQ_EXT3 LPC2294_INTERRUPT_EINT3
#endif



#if 1
  #define DPRINT(args...) printk(args)
#else
  #define DPRINT(args...)
#endif
#define xDPRINT(args...) //printk(args)





#define m_nic_read(reg) (*(volatile unsigned long *)(reg))
#define m_nic_write(reg, data) ((*(volatile unsigned long *)(reg)) = (volatile unsigned long)(data))
#define m_nic_bfs(reg, data) (*(volatile unsigned long *)(reg)) |= (data)
#define m_nic_bfc(reg, data) (*(volatile unsigned long *)(reg)) &= ~(data)

#define printk printf

/******************************************************************************
 * Local variables
 *****************************************************************************/

//static struct eth_device _dev_emac;
static volatile u32 emacDuplex;
static volatile u32 emacSpeed;
static unsigned char macAddr[6];
static int opened = 0;

/******************************************************************************
 * Local functions
 *****************************************************************************/

static void 
writePhy(u32 phyReg, 
         u32 phyData)
{
  //xDPRINT("writePhy\n");

  // write command
  m_nic_write(EMAC_MCMD, 0x0000);              

  // [12:8] == PHY addr, [4:0]=0x00(BMCR) register addr
  m_nic_write(EMAC_MADR, (phyAddr | phyReg)); 
  m_nic_write(EMAC_MWTD,  phyData);
  while ( m_nic_read(EMAC_MIND) != 0 );
}

static u32 
readPhy(u32 phyReg)
{
  //xDPRINT("readPhy\n");

  // read command
  m_nic_write(EMAC_MCMD, 0x0001);

  // [12:8] == PHY addr, [4:0]=0x00(BMCR) register addr
  m_nic_write(EMAC_MADR, (phyAddr | phyReg));

  while ( m_nic_read(EMAC_MIND) != 0 );

  m_nic_write(EMAC_MCMD, 0x0000);

  return m_nic_read(EMAC_MRDD);
}


static int 
emac_start_xmit(void* buf, int length)
{
//  struct emac_private *p_priv = (struct emac_private *)dev->priv;

  u32 txProduceIndex = 0;
  u32 txConsumeIndex = 0;
  u8* pData          = 0;
  u32 len            = length;//skb->len;
  u32 sendLen        = 0;
  u32* tx_desc_addr   = NULL;

/*
        {
                int i = 0;
                u8* pBuf = buf;
                for(i=0; i<length; i++)
                {
                        if(i > 0 && (i % 16) == 0)
                                printf("\n");

                        printf("%x:", pBuf[i]);
                }

                printf("\n\n");
        }
*/

//  spin_lock_irq(&lock);

  txProduceIndex = m_nic_read(EMAC_TXPRODUCEINDEX);
  txConsumeIndex = m_nic_read(EMAC_TXCONSUMEINDEX);

//  printf("emac_start_xmit: pIdx = %d, cIdx=%d\n", 
//               txProduceIndex, txConsumeIndex);


  if (txConsumeIndex != txProduceIndex)
  {
    // TODO why return here? This just means that the transmit array isn't empty
    printk("emac: emac_tx transmit array isn't empty\n");
    return -1;
  }

  if (txProduceIndex == EMAC_TX_DESCRIPTOR_COUNT)
  {
    // should never happen
    // TODO remove
    printk("emac: emac_tx produce index == count\n");
  }



  if (len > 0)
  {
    pData = (u8*)EMAC_TX_BUFFER_ADDR;
    memcpy(pData, buf, length); 


    do
    {
      tx_desc_addr = (u32*) (TX_DESCRIPTOR_ADDR + txProduceIndex * 8);

      sendLen = len;
      if (sendLen > EMAC_BLOCK_SIZE)
      {
        sendLen = EMAC_BLOCK_SIZE;
      }
      else
      {
        // last fragment
        sendLen |= EMAC_TX_DESC_LAST;
      }

      m_nic_write(tx_desc_addr,   (u32)pData);
      tx_desc_addr++;
      m_nic_write(tx_desc_addr, (u32)(EMAC_TX_DESC_INT | (sendLen -1)));

      txProduceIndex++;
      if (txProduceIndex == EMAC_TX_DESCRIPTOR_COUNT)
      {
        txProduceIndex = 0;
      }

      m_nic_write(EMAC_TXPRODUCEINDEX, txProduceIndex);

      len   -= (sendLen & ~EMAC_TX_DESC_LAST);
      pData += (sendLen & ~EMAC_TX_DESC_LAST);
    } while (len > 0);

//      dev->trans_start = jiffies;

//      p_priv->skb = skb;

    // stop upper layer from sending more packages until we are done
//      netif_stop_queue(dev);


  }

//  spin_unlock_irq(&lock);

  return 0;
}

static void emac_tx(void)
{
  xDPRINT("emac_tx\n");
}

static void emac_rx(void)
{
  u32 rxProduceIndex = 0;
  u32 rxConsumeIndex = 0;
  u32* rxStatusAddr  = 0;
  u32 recvSize       = 0;
  u32* recvAddr      = 0;

  xDPRINT("\nemac_rx\n");


  /* the input parameter, EMCBuf, needs to be word aligned */

  rxProduceIndex = m_nic_read(EMAC_RXPRODUCEINDEX);
  rxConsumeIndex = m_nic_read(EMAC_RXCONSUMEINDEX);

  // consume the received packets
  while (rxConsumeIndex != rxProduceIndex)
  {

    rxStatusAddr = (u32*)(RX_STATUS_ADDR + rxConsumeIndex * 8);

    recvSize = m_nic_read(rxStatusAddr);

    if ((recvSize & RX_DESC_STATUS_LAST) == 0)
    {
      // TODO: could this occur when EMAC_BLOCK_SIZE == 0x0600?
      printk("emac_rx: NOT LAST fragment\n");
    }

    recvSize = (recvSize & DESC_SIZE_MASK) + 1;

    recvAddr = (u32*)(RX_DESCRIPTOR_ADDR + rxConsumeIndex * 8);

    // TODO: allocate buffer?
    NetReceive( (char*)(m_nic_read(recvAddr)/*inBuf*/), recvSize);

    rxConsumeIndex++;
    if (rxConsumeIndex == EMAC_RX_DESCRIPTOR_COUNT)
    {
      rxConsumeIndex = 0;
    }

    m_nic_write(EMAC_RXCONSUMEINDEX, rxConsumeIndex);

  }


}

static void emac_interrupt(void)
{
  //struct net_device *dev = dev_id;
//  struct eth_device* dev = &_dev_emac;

  volatile u32 regValue = 0;

  regValue = m_nic_read(EMAC_INTSTATUS);


  do
  {
    if (regValue == 0)
    {
      break;
    }

    if (regValue & EMAC_INT_RXOVERRUN)
    {
      m_nic_write(EMAC_INTCLEAR, EMAC_INT_RXOVERRUN);
      //p_priv->stats.rx_over_erros++;
      printk("rxOverrun\n");
      break;
    }

    if (regValue & EMAC_INT_RXERROR)
    {
      m_nic_write(EMAC_INTCLEAR, EMAC_INT_RXERROR);
      //p_priv->stats.rx_erros++;
      //printf("rxError %x\n", regValue);

/*
            {
                u32 rxConsumeIndex = m_nic_read(EMAC_RXCONSUMEINDEX);
                u32* rxStatusAddr = (tU32*)(RX_STATUS_ADDR + rxConsumeIndex * 8);
                u32 val = m_nic_read(rxStatusAddr);

                printf("val = %x\n", val);
            }
*/




      //break;   
    }

    if (regValue & EMAC_INT_RXFINISHED)
    {
      m_nic_write(EMAC_INTCLEAR, EMAC_INT_RXFINISHED);
//      printf("rxFinished\n");
      //p_priv->stats.rx_erros++;   
      //while ( MAC_RXPRODUCEINDEX != (MAC_RXCONSUMEINDEX - 1) );
    }

    if (regValue & EMAC_INT_RXDONE)
    {
      m_nic_write(EMAC_INTCLEAR, EMAC_INT_RXDONE);                
      emac_rx();   
    }

    if (regValue & EMAC_INT_TXUNDERRUN)
    {
      m_nic_write(EMAC_INTCLEAR, EMAC_INT_TXUNDERRUN);      
//          p_priv->stats.tx_erros++;
      printk("txUnderrun\n");
      break;  
    }

    if (regValue & EMAC_INT_TXERROR)
    {
      m_nic_write(EMAC_INTCLEAR, EMAC_INT_TXERROR);      
//          p_priv->stats.tx_erros++;
      printk("txError\n");

      break;  
    }

    if (regValue & EMAC_INT_TXFINISHED)
    {
      m_nic_write(EMAC_INTCLEAR, EMAC_INT_TXFINISHED); 
    }

    if (regValue & EMAC_INT_TXDONE)
    {
      m_nic_write(EMAC_INTCLEAR, EMAC_INT_TXDONE);      
      emac_tx();
    }

  } while (0);


//  return IRQ_HANDLED;
}


static int emac_open(void)
{

  xDPRINT("emac_open\n");
  opened = 1;

  // enable Rx & Tx
  m_nic_bfs(EMAC_COMMAND, CMD_RX_ENABLE);
  m_nic_bfs(EMAC_COMMAND, CMD_TX_ENABLE);
  m_nic_bfs(EMAC_MAC1,    0x01);

//  netif_start_queue(dev);
  return 0;
}

//static int emac_close(struct net_device *dev)
static void emac_close (void)
{
  xDPRINT("emac_close\n");
  opened = 0;
//  netif_stop_queue(dev);

  // disable Rx6 Tx
  m_nic_bfc(EMAC_COMMAND, CMD_RX_ENABLE);
  m_nic_bfc(EMAC_COMMAND, CMD_TX_ENABLE);
  m_nic_bfc(EMAC_MAC1,    0x01);

//  return 0;
}


static int 
check_Micrel_KSZ8001L(void)
{
  volatile u32 regValue = 0;

  // See Micrel KSZ8001L Users Manual for more details
  regValue = readPhy(PHY_PHYIDR1);
  if ( (regValue & 0xffff) != 0x0022)
  {
    return FALSE;
  }

  regValue = readPhy(PHY_PHYIDR2);
  if ( (regValue & 0xfff0) != 0x1610)
  {
    return FALSE;
  }
  return TRUE;
}


static int 
check_National_DP83848(void)
{
  volatile u32 regValue = 0;

  // See Micrel KSZ8001L Users Manual for more details
  regValue = readPhy(PHY_PHYIDR1);
  if ( (regValue & 0x2000) != 0x2000)
  {
    return FALSE;
  }

  regValue = readPhy(PHY_PHYIDR2);
  if ( (regValue & 0x5C90) != 0x5C90)
  {
    return FALSE;
  }
  return TRUE;
}


static int 
phyInit(void)
{
  int i        = 0;
  u32 regValue = 0;
  u32 regValue2;
  u32 timeout  = 0;

  xDPRINT("\nphyInit\n");


  m_nic_write(MAC_MCFG, 0x8018);	/* host clock divided by 20, no suppress preamble, no scan increment */
  for ( i = 0; i < 0x40; i++ ) asm volatile ("  nop");

  // MII Mgmt. Divided by 20.
  m_nic_write(EMAC_MCFG, 0x0018);
  m_nic_write(MAC_MCMD, 0);

  /* RMII configuration */

// TODO do we really need to set this again?

  m_nic_bfs(EMAC_COMMAND,  0x0200); 
  m_nic_write(EMAC_SUPP,  0x0900);                 /* RMII setting, PHY support: [8]=0 ->10 Mbps mode, =1 -> 100 Mbps mode */
  /*  (note bit 4 was set in original test, although spec says its unused) */
  for ( i = 0; i < 0x20; i++ )                    /* short delay */
    asm volatile ("  nop"); 
  m_nic_write(EMAC_SUPP,  0x0100);

  for ( i = 0; i < 0x100; i++ ) asm volatile ("  nop"); 


  // check PHY IDs.
  phyAddr = 0;
  if (check_Micrel_KSZ8001L() == FALSE)
  {
    phyAddr = 0x100;
    if (check_National_DP83848() == FALSE)
    {
      printk("emac: cannot identify external PHY");
      phyAddr = 0;
      printk(": [0]->(%x, %x)", readPhy(PHY_PHYIDR1), readPhy(PHY_PHYIDR2));
      phyAddr = 0x100;
      printk(": [1]->(%x, %x)", readPhy(PHY_PHYIDR1), readPhy(PHY_PHYIDR2));
      return -1;
    }
    else
    {
      phyType = NATIONAL_PHY;
      printk("emac: found National DP83848 PHY");
    }
  }
  else
  {
    phyType = MICREL_PHY;
    printk("emac: found Micrel KSZ8001L PHY");
  }


  // software reset of PHY. The bit is self-clearing
  writePhy(PHY_BMCR, BMCR_RESET);

  for ( i = 0; i < 0x20; i++ ) asm volatile ("  nop");                 /* short delay */

  // software reset of PHY. The bit is self-clearing
  writePhy(PHY_BMCR, BMCR_RESET);

  for ( i = 0; i < 0x2000; i++ )
    asm volatile ("  nop");                 /* short delay */

  timeout = MII_BMSR_TIMEOUT * 4;
  while (timeout != 0)
  {
    regValue = readPhy(PHY_BMCR);
    if ((regValue & BMCR_RESET) == 0x0000)
    {
      // reset bit has been cleared
      break;
    }
    timeout--;
  }

  if (timeout == 0)
  {
    printk("\nemac: phyInit failed to reset PHY\n");
    return -1;
  }

  
  // auto negotiation
  writePhy(PHY_BMCR, (BMCR_AN | BMCR_RE_AN));

  printk("\remac: started auto negitiation:  ");
  timeout = 10000;
  while ( timeout != 0 )
  {
    if (phyType == MICREL_PHY)
    {
      regValue = readPhy( MIC_PHY_100BASE_PHY_CTRL );
      if (((regValue & 0x001c) != 0x0000) && ((regValue & 0x0080) == 0x0080))
//	    regValue = readPhy( PHY_BMSR );
//	    if ( (regValue & (BMSR_AUTO_DONE|BMSR_LINK_ESTABLISHED))
//         == (BMSR_AUTO_DONE|BMSR_LINK_ESTABLISHED) )	
      {
        // link established when AN status bit (bit 7) is set
        break;
      }
    }
    else
    {
      regValue = readPhy( NSM_PHY_PHYSTS );
      if ( (regValue & 0x0011) == 0x0011 )
      {
        // link established when AN status bit (bit 7) is set
        break;
      }
    }

    timeout--;

    //print progress marker
    switch(timeout%4)
    {
      case 0:  printk("\b\\"); break;
      case 1:  printk("\b-"); break;
      case 2:  printk("\b/"); break;
      case 3:  printk("\b|"); break;
      default: printk("\b "); break;
    }
    for ( i = 0; i < 0x48000; i++ ) asm volatile ("  nop");    /* short delay */
  }

  if ( timeout == 0 )
  {
    printk("\remac: phyInit: NO LINK\n");
    return -1;
  }
  else
    printk("\bdone!");

  printk("\remac: link status = ");
  //check status
  if (phyType == MICREL_PHY)
  {
    regValue = readPhy( MIC_PHY_100BASE_PHY_CTRL );
	  /* successful negotiations; update link info */
	  regValue &= 0x001C;
	  switch ( regValue )
	  {
	  case 0x0004:
		  emacSpeed = SPEED_10;
		  emacDuplex = HALF_DUPLEX;
      printk("10Mbps, half duplex\n");
	    break;
	  case 0x0008:
		  emacSpeed = SPEED_100;
		  emacDuplex = HALF_DUPLEX;
      printk("100Mbps, half duplex\n");
	    break;
	  case 0x0014:
		  emacSpeed = SPEED_10;
		  emacDuplex = FULL_DUPLEX;
      printk("10Mbps, full duplex\n");
	    break;
	  case 0x0018:
		  emacSpeed = SPEED_100;
		  emacDuplex = FULL_DUPLEX;
      printk("100Mbps, full duplex\n");
	    break;
	  default:	// Should not come here, force to set default, 100 FULL_DUPLEX
		  emacSpeed = SPEED_100;
		  emacDuplex = FULL_DUPLEX;
      printk("forced to 100Mbps, full duplex\n");
		  break;
	  }
  }
  else
  {
    regValue = readPhy( NSM_PHY_PHYSTS );

	  /* Link established from here on */
  	if ( regValue & 0x02 )
    {
	    emacSpeed = SPEED_10;
      printk("10Mbps, ");
    }
	  else
    {
	    emacSpeed = SPEED_100;
      printk("100Mbps, ");
    }

	  if ( regValue & 0x04 )
    {
	    emacDuplex = FULL_DUPLEX;
      printk("full duplex\n");
    }
	  else
    {
	    emacDuplex = HALF_DUPLEX;
      printk("half duplex\n");
    }

  }  

  return 0; 

}

static void 
emacTxDescriptorInit(void)
{
  int i = 0;
  u32* txDescAddr   = NULL;
  u32* txStatusAddr = NULL;

  xDPRINT("\nemacTxDescriptorInit\n");


  // base address of tx descriptor array
  m_nic_write(EMAC_TXDESCRIPTOR, TX_DESCRIPTOR_ADDR);

  // base address of tx status
  m_nic_write(EMAC_TXSTATUS, TX_STATUS_ADDR);

  // number of tx descriptors
  m_nic_write(EMAC_TXDESCRIPTORNUM, EMAC_TX_DESCRIPTOR_COUNT-1);

  for (i = 0; i < EMAC_TX_DESCRIPTOR_COUNT; i++)
  {
    txDescAddr = (u32 *)(TX_DESCRIPTOR_ADDR + i * 8);
    m_nic_write(txDescAddr, (EMAC_TX_BUFFER_ADDR + i * EMAC_BLOCK_SIZE));

    // control field in descriptor
    txDescAddr++;
    m_nic_write(txDescAddr, (EMAC_TX_DESC_INT | (EMAC_BLOCK_SIZE-1)));
  }

  for (i = 0; i < EMAC_TX_DESCRIPTOR_COUNT; i++)
  {

    txStatusAddr = (u32 *)(TX_STATUS_ADDR + i * 4);

    // set status info to 0
    m_nic_write(txStatusAddr, 0);
  }

  m_nic_write(EMAC_TXPRODUCEINDEX, 0);
}

static void 
emacRxDescriptorInit(void) 
{
  int i;
  u32* rxDescAddr   = NULL;
  u32* rxStatusAddr = NULL;

  xDPRINT("\nemacRxDescriptorInit\n");

  // base address of rx descriptor array
  m_nic_write(EMAC_RXDESCRIPTOR, RX_DESCRIPTOR_ADDR);

  // base address of rx status
  m_nic_write(EMAC_RXSTATUS, RX_STATUS_ADDR);

  // number of rx descriptors
  m_nic_write(EMAC_RXDESCRIPTORNUM,  EMAC_RX_DESCRIPTOR_COUNT-1);

  for (i = 0; i < EMAC_RX_DESCRIPTOR_COUNT; i++)
  {
    rxDescAddr = (u32 *)(RX_DESCRIPTOR_ADDR + i * 8);
    m_nic_write(rxDescAddr, (EMAC_RX_BUFFER_ADDR + i * EMAC_BLOCK_SIZE));
    rxDescAddr++;
    m_nic_write(rxDescAddr, (EMAC_RX_DESC_INT | ((EMAC_BLOCK_SIZE - 1) & DESC_SIZE_MASK) ) );
  }

  for (i = 0; i < EMAC_RX_DESCRIPTOR_COUNT; i++)
  {
    // RX status, two words, status info. and status hash CRC.
    rxStatusAddr = (u32 *)(RX_STATUS_ADDR + i * 8);  
    m_nic_write(rxStatusAddr, 0);
    rxStatusAddr++;
    m_nic_write(rxStatusAddr, 0);
  }

  m_nic_write(EMAC_RXCONSUMEINDEX, 0);
}

static int 
emacInit(void)
{
  int i = 0;
  u32 regValue = 0;
  u32 retryCnt;

  xDPRINT("\nemacInit\n");


  // turn on the ethernet MAC clock in PCONP, bit 30 
  m_nic_bfs(PCONP, PCONP_EMAC_CLOCK);

  /*------------------------------------------------------------------------------
   * write to PINSEL2/3 to select the PHY functions on P1[17:0]
   *-----------------------------------------------------------------------------*/
  /* documentation needs to be updated */
  regValue = m_nic_read(EMAC_MODULEID);

  if ( regValue == OLD_EMAC_MODULE_ID )
  {
    /* On Rev. '-', MAC_MODULEID should be equal to
       OLD_EMAC_MODULE_ID, P1.6 should be set. 
     */
    
    m_nic_write(PINSEL2, 0x50151105);
    /* selects P1[0,1,4,6,8,9,10,14,15] */
  }
  else
  {
    /* on rev. 'A', MAC_MODULEID should not equal to
       OLD_EMAC_MODULE_ID, P1.6 should not be set. */
    m_nic_write(PINSEL2, 0x50150105);
    /* selects P1[0,1,4,8,9,10,14,15] */
  }
  m_nic_write(PINSEL3, 0x00000005);   /* selects P1[17:16] */


  // reset MAC modules, tx, mcs_tx, rx, mcs_rx, simulation and soft reset 
  m_nic_write(EMAC_MAC1, 0xCF00);

  // reset datapaths and host registers
  m_nic_write(EMAC_COMMAND, 0x0038);

  // short delay after reset
//  udelay(10);
  for ( i = 0; i < 0x40; i++ )
    asm volatile ("  nop");

  m_nic_write(EMAC_MAC1, 0x0);

  // disable Tx
  m_nic_bfc(EMAC_COMMAND, CMD_TX_ENABLE);
  // disable Rx
  m_nic_bfc(EMAC_COMMAND, CMD_RX_ENABLE);

  // initialize MAC2 to default value
  // TODO don't thinkt this is necessary??  m_nic_write(EMAC_MAC2, 0x0);

  // non back-to-back Inter-Packet-Gap register
  // The manual recommends the value 0x12
  m_nic_write(EMAC_IPGR, 0x12);

  // collision window/retry register. Using recommended value from manual.
  m_nic_write(EMAC_CLRT, 0x370F);

  // maximum frame register. 
  // TODO the default reset value is already 0x0600!!  m_nic_write(EMAC_MAXF, 0x0600);

  // intialize PHY. emacSpeed and emacDuplex will be set in phyInit
  for(retryCnt=0; retryCnt<MAX_PHY_INIT_RETRY; retryCnt++)
  {
    if ( phyInit() < 0)
      continue;
    else
      break;
  }
  if(retryCnt==MAX_PHY_INIT_RETRY)
    return -1;

  // write the mac address
  m_nic_write(EMAC_SA0, (macAddr[5] << 8 | macAddr[4]));
  m_nic_write(EMAC_SA1, (macAddr[3] << 8 | macAddr[2]));
  m_nic_write(EMAC_SA2, (macAddr[1] << 8 | macAddr[0]));

  printk("emac: MAC address = %2x:%2x:%2x:%2x:%2x:%2x\n", macAddr[0], macAddr[1], macAddr[2], macAddr[3], macAddr[4], macAddr[5]);

  // 10 Mbps and half-duplex
  if (emacSpeed == SPEED_10 && emacDuplex == HALF_DUPLEX)
  {
    // CRC and PAD enabled
    m_nic_write(EMAC_MAC2, 0x30);

    // 10 Mbps mode
    m_nic_bfc(EMAC_SUPP, 0x0100);

    // PassRuntFrame and RMII
    m_nic_bfc(EMAC_COMMAND, 0x0240);

    // back-to-back Inter-Packet-Gap register. Recommended value from manual.
    m_nic_write(EMAC_IPGT, 0x12);
  }
  else if (emacSpeed == SPEED_100 && emacDuplex == HALF_DUPLEX)
  {
    // CRC and PAD enabled
    m_nic_write(EMAC_MAC2, 0x30);

    // 100 Mbps mode
    m_nic_bfs(EMAC_SUPP, 0x0100);

    // PassRuntFrame and RMII
    m_nic_bfs(EMAC_COMMAND, 0x0240);

    // back-to-back Inter-Packet-Gap register. Recommended value from manual.
    m_nic_write(EMAC_IPGT, 0x12);
  }
  else if (emacSpeed == SPEED_10 && emacDuplex == FULL_DUPLEX)
  {
    // CRC and PAD enabled, Full-Duplex
    m_nic_write(EMAC_MAC2, 0x31);

    // 10 Mbps mode
    m_nic_bfc(EMAC_SUPP, 0x0100);

    // PassRuntFrame, RMII and Full-Duplex
    m_nic_bfs(EMAC_COMMAND, 0x0640);

    // back-to-back Inter-Packet-Gap register. Recommended value from manual.
    m_nic_write(EMAC_IPGT, 0x15);
  }
  else if ( emacSpeed == SPEED_100 && emacDuplex == FULL_DUPLEX )
  {
    // CRC and PAD enabled, Full-Duplex
    m_nic_write(EMAC_MAC2, 0x31);

    // 100 Mbps mode
    m_nic_bfs(EMAC_SUPP, 0x0100);

    // PassRuntFrame, RMII and Full-Duplex
    m_nic_bfs(EMAC_COMMAND, 0x0640);

    // back-to-back Inter-Packet-Gap register. Recommended value from manual.
    m_nic_write(EMAC_IPGT, 0x15);
  }

  emacTxDescriptorInit();
  emacRxDescriptorInit();

  // pass all receive frames
  m_nic_bfs(EMAC_MAC1, 0x0002);


  // set up the Rx filter 
  // [0]-AllUnicast, [1]-AllBroadCast, [2]-AllMulticast, [3]-UnicastHash
  // [4]-MulticastHash, [5]-Perfect, [12]-MagicPacketEnWoL, [13]-RxFilterEnWoL
  m_nic_write(EMAC_RXFILTERCTRL, 0x0022);


  // clear all interrupts
  m_nic_write(EMAC_INTCLEAR, 0xFFFF);

  // TODO the irq is installed here in sample code !!

  // enable interrupts (not SoftInt and WoL)
  m_nic_write(EMAC_INTENABLE, 0x00FF);

  return 0;
}


/* ##############################
 * u-boot interface
 * ##############################
 */
static int eth_initialized = 0;
/*-----------------------------------------------------------------------------
 * Returns 0 when failes otherwise 1
 */
int eth_init(bd_t *bis)
{
  int ret;
  int i = 0;

  //printk("eth_init\n");

  if(!eth_initialized)
  {

    //printf("eth_init: calling emacInit\n");

    for (i=0; i<6; i++)
    {
      //_dev_emac.enetaddr[i] = bis->bi_enetaddr[i];
      macAddr[i] = bis->bi_enetaddr[i];
    }

    ret = emacInit();
    if (ret != 0)
    {
      return ret;
    }

    eth_initialized = 1;
  }

  //eth_register(&_dev_emac);

  emac_open();

  return 0;


}

int eth_send(volatile void *packet, int length)
{
  //printf("eth_send\n");

  emac_start_xmit(packet, length);

  return 0;
}

/*-----------------------------------------------------------------------------
 * Check for received packets. Call NetReceive for each packet. The return
 * value is ignored by the caller.
 */
int eth_rx(void)
{
  //printf("eth_rx\n");
  
  emac_interrupt();

  return 0;
}

void eth_halt(void)
{
  //printf("eth_halt\n");
  emac_close();
}
