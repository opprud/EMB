#include <common.h>
#include <nand.h>

/*
 * CLE at A20
 * ALE at A19
 */
#define MASK_CLE (1l<<20)
#define MASK_ALE (1l<<19)

static void hwcontrol(struct mtd_info *mtd, int cmd)
{
  struct nand_chip *this = mtd->priv;
  ulong IO_ADDR_W = (ulong) this->IO_ADDR_W;
  
  IO_ADDR_W &= ~(MASK_ALE|MASK_CLE);

  switch (cmd) {
  case NAND_CTL_SETCLE: 
    IO_ADDR_W |= MASK_CLE; 
    break;
  case NAND_CTL_SETALE: 
    IO_ADDR_W |= MASK_ALE; 
    break;
  }

  this->IO_ADDR_W = (void *) IO_ADDR_W;
}

void board_nand_init(struct nand_chip *nand)
{
  /* IO_ADDR_R and IO_ADDR_W set by nand-driver using CFG_NAND_BASE */
  nand->hwcontrol = hwcontrol;
  nand->dev_ready = 0;
  nand->eccmode = NAND_ECC_SOFT;
  nand->chip_delay = 25;	/* us */
  nand->options = NAND_SAMSUNG_LP_OPTIONS;
}
