#include <config.h>
#include <common.h>
#include <mmc.h>
#include <asm/errno.h>
#include <asm/arch/hardware.h>
#include <part.h>
#include <fat.h>

#include "irqVec.h"
#include "mci.h"
#include "dma.h"

#ifdef CONFIG_MMC

int hw_size;		/* in kbytes */
int hw_nr_sects;
int hw_sect_size;	/* in bytes */

extern volatile unsigned long MCI_CardType;

/* Below buffer allocation could be simplier, if the buffer used for DMA and non-DMA 
are shared. Future clear up will be needed. For now, leave as it's. */
#if MCI_DMA_ENABLED
unsigned long *src_addr; 
unsigned long *dest_addr;
#else
volatile tU8 WriteBlock[BLOCK_LENGTH], ReadBlock[BLOCK_LENGTH];
volatile tU32 TXBlockCounter, RXBlockCounter;
#endif

static block_dev_desc_t mmc_dev;

block_dev_desc_t * mmc_get_dev(int dev)
{
  return (block_dev_desc_t *)(&mmc_dev);
}

unsigned long mmc_block_read(int dev, 
			     unsigned long start, 
			     lbaint_t blkcnt,
			     unsigned long *buffer)
{
  unsigned long rc = 0;
  unsigned long *p32 = buffer;
  unsigned long i,j;
  unsigned long addr = start;

//  printf("mmc_block_read: start=%lu, blkcnt=%lu\n", start, 
//	 (unsigned long)blkcnt);

  for(i = 0; i < (unsigned long)blkcnt; i++) {
/*
    printf("mmc_read_sector: addr=%lu, buffer=%p\n", 
	   addr, p32);
*/


if ( MCI_Read_Block( addr ) != TRUE )
{
  printf("\nMCI ERROR when calling MCI_Read_Block!!!\n");
  return ( 0 );		/* Fatal error */
}


/* When RX_ACTIVE is not set, it indicates RX is done from 
   Interrupt_Read_Block, now, validation of RX and TX buffer can start. */
while ( MCI_STATUS & MCI_RX_ACTIVE );

/* Note RXEnable() is called in the Interrupt_Read_Block(). */
MCI_RXDisable();

dest_addr = (unsigned long *)DMA_DST;
for ( j = 0; j < hw_sect_size; j+=4 )
{
  *p32++ = *dest_addr;
//printf("\noffset[%x] = %x", j, *dest_addr);
dest_addr++;
}

    rc++;
    addr++;

printf(".");
  } 

  return rc;
}

int mmc_init(int verbose)
{
  int ret = -ENODEV;
  int error = FALSE;
  int i;

  printf("mmc_init\n");

  DMA_Init();

  if ( MCI_Init() != TRUE )
  {
    printf("\nMCI ERROR from MCI_Init()!\n");
    error = TRUE;
  }

  if ( FALSE == error)
  {
    if ( (MCI_CardType = MCI_CardInit()) == CARD_UNKNOWN )
    {
      printf("\nMCI ERROR from MCI_CardInit()!\n");
      error = TRUE;
    }
  }

  if ( FALSE == error)
  {
    if ( MCI_Check_CID() != TRUE )
    {
      printf("\nMCI ERROR from MCI_Check_CID()!\n");
      error = TRUE;
    }
  }

  if ( FALSE == error)
  {
    if ( MCI_Set_Address() != TRUE )
    {
      printf("\nMCI ERROR from MCI_Set_Address()!\n");
      error = TRUE;
    }
  }

  if ( FALSE == error)
  {
    if ( MCI_Send_CSD() != TRUE )
    {
      printf("\nMCI ERROR from MCI_Send_CSD()!\n");
      error = TRUE;
    }
  }

  if ( FALSE == error)
  {
    if ( MCI_Select_Card() != TRUE )
    {
      printf("\nMCI ERROR from MCI_Select_Card()!\n");
      error = TRUE;
    }
  }

  if ( FALSE == error)
  {
    if ( MCI_CardType == SD_CARD )
    {
//      printf("\nMCI INFO memory card is of SD-type!\n");

      MCI_Set_MCIClock( NORMAL_RATE );
//      MCI_CLOCK |= (1 << 11);		/* Use wide bus for SD */
//      for ( i = 0; i < 0x20; i++ );
//      if ( MCI_Send_ACMD_Bus_Width( BUS_WIDTH_4BITS ) == FALSE )
      if (SD_Set_BusWidth( SD_4_BIT ) != TRUE )
      {
        printf("\nMCI ERROR from SD_Set_BusWidth()!\n");
        error = TRUE;
      }
    }
  }

  if ( FALSE == error)
  {
    if ( MCI_Set_BlockLen( BLOCK_LENGTH ) != TRUE )
    {
      printf("\nMCI ERROR from MCI_Set_BlockLen()!\n");
      error = TRUE;
    }
  }

//  if ( FALSE == error)
//    printf("\nMCI INFO passed initialization sequence!!!\n");

  if ( FALSE == error)
  {

    mmc_dev.if_type = IF_TYPE_MMC;
    mmc_dev.part_type = PART_TYPE_DOS;
    mmc_dev.dev = 0;
    mmc_dev.lun = 0;
    mmc_dev.type = 0;
    mmc_dev.blksz = hw_sect_size;
    mmc_dev.lba = hw_nr_sects;
    sprintf(mmc_dev.vendor, "Unknown vendor");
    sprintf(mmc_dev.product, "Unknown product");
    sprintf(mmc_dev.revision, "N/A");
    mmc_dev.removable = 0;	/* should be true??? */
    mmc_dev.block_read = mmc_block_read;

    printf("\nReturn %d after fat_register_device\n", fat_register_device(&mmc_dev, 1));


    ret = 0;
  }

  return ret;
}

int mmc_write(uchar * src, ulong dst, int size)
{
  printf("mmc_write: src=%p, dst=%lu, size=%u\n", src, dst, size);
  /* Since mmc2info always returns 0 this function will never be called */
  return 0;
}

int mmc_read(ulong src, uchar * dst, int size)
{
  printf("mmc_read: src=%lu, dst=%p, size=%u\n", src, dst, size);
  /* Since mmc2info always returns 0 this function will never be called */
  return 0;
}

int mmc2info(ulong addr)
{
  /* This function is used by cmd_cp to determine if source or destination
     address resides on MMC-card or not. We do not support copy to and from
     MMC-card so we always return 0. */
  return 0;
}



#endif /* CONFIG_MMC */
