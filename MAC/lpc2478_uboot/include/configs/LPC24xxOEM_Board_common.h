/*
 * (C) Copyright 2007
 * Embedded Artists AB, Sweden <www.EmbeddedArtists.com>
 *
 * Configuation settings for the LPC2468 OEM Board.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
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

#ifndef __CONFIG_H
#define __CONFIG_H

/*
 * Special config for handling layout feature on v1.1 of the
 * LPC2468 OEM Board.
 * v1.1 of the board supported different brands of nor flash memories
 * but the trade-off was to treat the 4 MBytes as 4 x 1 MByte areas.
 * v2.x of the board treat the nor flash as one single area.
 *
 * Note that the bank offsets are defined as follow:
 * 1st bank: 0x00000000 (images at 0x00100000, 0x00800000, 0x00900000)
 * 2nd bank: 0x00400000 (images at 0x00500000, 0x00C00000, 0x00D00000)
 * 3rd bank: 0x00200000 (images at 0x00300000, 0x00A00000, 0x00B00000)
 * 4th bank: 0x00600000 (images at 0x00700000, 0x00E00000, 0x00F00000)
 */
#undef EA_CONFIG_NOR_ON_V1_1


/*
 * If we are developing, we might want to start armboot from ram
 * so we MUST NOT initialize critical regs like mem-timing ...
 */
#if 0
#define CONFIG_INIT_CRITICAL		/* undef for developing */
#endif

#undef CONFIG_SKIP_LOWLEVEL_INIT


/*
 * High Level Configuration Options
 * (easy to change)
 */
#define CONFIG_ARM7		1	/* This is a ARM7 CPU	*/
#define CONFIG_ARM_THUMB	1	/* this is an ARM720TDMI */
#undef  CONFIG_ARM7_REVD	 	/* disable ARM720 REV.D Workarounds */

#define CONFIG_USE_IRQ			/* use irq for mci interface */

/*
 * Size of malloc() pool
 */
#define CFG_MALLOC_LEN		(CFG_ENV_SIZE + 128*1024)
#define CFG_GBL_DATA_SIZE	128	/* size in bytes reserved for initial data */

/*
 * Hardware drivers
 */

/*
 * select serial console configuration
 */
#define CONFIG_SERIAL1		1	/* we use Serial line 1 */

/* allow to overwrite serial and ethaddr */
#define CONFIG_ENV_OVERWRITE

#define CONFIG_BAUDRATE		115200
//#define CONFIG_BAUDRATE		38400

#define CONFIG_BOOTP_MASK       (CONFIG_BOOTP_DEFAULT|CONFIG_BOOTP_BOOTFILESIZE)

#define CONFIG_USB_STORAGE 1
#define CONFIG_USB_OHCI 1

//#define CONFIG_COMMANDS		(CONFIG_CMD_DFL | CFG_CMD_FAT | CFG_CMD_NET | CFG_CMD_MMC | CFG_CMD_NAND | CFG_CMD_DHCP | CFG_CMD_PING)
#define CONFIG_COMMANDS		(CONFIG_CMD_DFL | CFG_CMD_FAT | CFG_CMD_NET | CFG_CMD_MMC | CFG_CMD_NAND | CFG_CMD_DHCP | CFG_CMD_PING | CFG_CMD_I2C | CFG_CMD_USB)

/* this must be included AFTER the definition of CONFIG_COMMANDS (if any) */
#include <cmd_confdefs.h>

#define CONFIG_BOOTDELAY	  3
#define CONFIG_BOOTARGS    	"root=/dev/ram initrd=0xa1800000,4000k console=ttyS0,115200N8"
//#define CONFIG_BOOTARGS     "root=/dev/mtdblock0 console=ttyS0,38400N8"
//#define CONFIG_BOOTCOMMAND	"run nand_boot"
#define CONFIG_BOOTCOMMAND	"echo ;echo Booting from NOR FLASH (may take some seconds);" \
                            "echo First loads 'uLinux.bin' and then 'cramfs.img';" \
                            "run nor_boot"

/*
 * Miscellaneous configurable options
 */
#define	CFG_LONGHELP				/* undef to save memory		*/
#define	CFG_CBSIZE		256		/* Console I/O Buffer Size	*/
#define	CFG_PBSIZE (CFG_CBSIZE+sizeof(CFG_PROMPT)+16) /* Print Buffer Size */
#define	CFG_MAXARGS		16		/* max number of command args	*/
#define CFG_BARGSIZE		CFG_CBSIZE	/* Boot Argument Buffer Size	*/

#define CFG_MEMTEST_START	0xA0000000	/* memtest works on	*/
#define CFG_MEMTEST_END		0xA1F80000	/* 31.5 MB in DRAM	*/
#define CFG_ALT_MEMTEST

#undef  CFG_CLKS_IN_HZ		/* everything, incl board info, in Hz */

#define	CFG_LOAD_ADDR		0xA0008000	/* default load address	for kernel img is here*/


#define CLK_FCCO ((2*CONFIG_PLL_MVALUE*CONFIG_FOSC) / CONFIG_PLL_NVALUE)

#define CFG_SYS_CLK_FREQ        (CLK_FCCO / CONFIG_PLL_CLKDIV)/* Hz */
#define	CFG_HZ			              200000		/* decrementer freq in Hz */

						/* valid baudrates */
#define CFG_BAUDRATE_TABLE	{ 9600, 19200, 38400, 57600, 115200 }

/*-----------------------------------------------------------------------
 * Stack sizes
 *
 * The stack sizes are set up in start.S using the settings below
 */
#define CONFIG_STACKSIZE	(128*1024)	/* regular stack */
#ifdef CONFIG_USE_IRQ
#define CONFIG_STACKSIZE_IRQ	(8*1024)	/* IRQ stack */
#define CONFIG_STACKSIZE_FIQ	(8*1024)	/* FIQ stack */
#endif

/*-----------------------------------------------------------------------
 * Physical Memory Map
 */
#define CONFIG_NR_DRAM_BANKS	1	   /* we have 1 bank of DRAM */
#define PHYS_SDRAM_1		  0xA0000000 /* SDRAM Bank #1 */
#define PHYS_SDRAM_1_SIZE	0x02000000 /* 32 MB SDRAM */

#define PHYS_FLASH_1		  0x80000000 /* Flash Bank #1 */
#define PHYS_FLASH_SIZE		0x00400000 /* 4 MB */

#define CFG_FLASH_BASE		PHYS_FLASH_1

/*-----------------------------------------------------------------------
 * FLASH and environment organization
 */
#if EA_CONFIG_NOR_ON_V1_1
#define CFG_MAX_FLASH_BANKS	5	/* max number of memory banks		*/
#else
#define CFG_MAX_FLASH_BANKS	2	/* max number of memory banks		*/
#endif
#define CFG_MAX_FLASH_SECT	1024	/* max number of sectors on one chip	*/

/* timeout values are in ticks */
#define CFG_FLASH_ERASE_TOUT	(2*CFG_HZ) /* Timeout for Flash Erase */
#define CFG_FLASH_WRITE_TOUT	(2*CFG_HZ) /* Timeout for Flash Write */

/*
 * Linux tags
 */
#define CONFIG_CMDLINE_TAG
#define CONFIG_SETUP_MEMORY_TAGS
#define CONFIG_INITRD_TAG

#define CONFIG_MMC 1
#define CONFIG_DOS_PARTITION 1
#define CONFIG_MAC_PARTITION 1


/*
 * NAND Flash
 */
#define CFG_MAX_NAND_DEVICE	1
#define NAND_MAX_CHIPS		1
#define CFG_NAND_BASE	0x81000000


/*
 * Default environment settings
 */
#define CONFIG_EXTRA_ENV_SETTINGS \
          "tftp_boot=tftpboot a1500000 uLinux.bin;tftpboot a1800000 romfs.img;bootm a1500000\0" \
          "nand_boot=nboot a1500000 0\;bootm a1500000\0" \
          "nor_boot=cp.b 80000000 a1500000 200000;cp.b 80200000 a1800000 200000;bootm a1500000\0" \
          "mmc_boot=mmc;fatload mmc 0 a1500000 uLinux.bin;fatload mmc 0 a1800000 romfs.img;bootm a1500000\0" \
          "update_uboot=tftpboot a1000000 u-boot.bin;protect off 0 2ffff;erase 0 2ffff;cp.b a1000000 0 $(filesize)\0" \
          "update_nor=erase bank 2;tftpboot a0000000 uLinux.bin;cp.b $(fileaddr) 80000000 $(filesize);tftpboot a0000000 cramfs.img;cp.b $(fileaddr) 80200000 $(filesize)\0" \
          "update_nor_usb=erase bank 2;usb start;fatload usb 0:1 a0000000 uLinux.bin;cp.b a0000000 80000000 200000;fatload usb 0:1 a0000000 cramfs.img;cp.b a0000000 80200000 200000;usb stop\0" \
          "update_nand=nand erase;tftpboot a0000000 uLinux.bin;nand write $(fileaddr) 0 0x00300000;tftpboot a0000000 jffs2.img;nand write $(fileaddr) 0x00300000 $(filesize)\0" \
          "usb_boot=usb start;fatload usb 0:1 a1500000 uLinux.bin;fatload usb 0:1 a1800000 romfs.img;usb stop;bootm a1500000\0" \
          "ipaddr=192.168.5.233\0" \
          "netmask=255.255.255.0\0" \
          "serverip=192.168.5.10\0" \
          "ethaddr=00:1a:f1:00:00:00\0"

/*
 * Control where environment is stored
 */
#if 1
#define	CFG_ENV_IS_IN_FLASH	1
#define CFG_ENV_ADDR		(0x0 + 0x7C000)	/* Addr of Environment Sector	*/
#define CFG_ENV_SIZE		0x1000 /* Total Size of Environment Sector(4k) */
#else
//#define CFG_ENV_IS_IN_EEPROM 1
//#define CFG_ENV_OFFSET  0x7000           /* Offset within EEPROM */
//#define CFG_ENV_SIZE    0x1000           /* Total Size of Environment Sector(4k) */
#endif

#define CFG_I2C_EEPROM_ADDR 0x50
#define CFG_EEPROM_PAGE_WRITE_BITS 6     /* 64 byte pages */
#define CFG_EEPROM_PAGE_WRITE_DELAY_MS 5
#define CFG_I2C_EEPROM_ADDR_LEN 2
#define CFG_EEPROM_SIZE 32768

#define CONFIG_HARD_I2C
#define CFG_I2C_SPEED 400000   //400kHz
#define CFG_I2C_SLAVE 0

#endif	/* __CONFIG_H */
