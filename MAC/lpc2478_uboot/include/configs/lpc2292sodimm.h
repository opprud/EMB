/*
 * (C) Copyright 2000
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Marius Groeger <mgroeger@sysgo.de>
 *
 * Configuation settings for the EP7312 board.
 *
 * Modified to work on Armadillo HT1070 ARM720T board
 * (C) Copyright 2005 Rowel Atienza rowel@diwalabs.com
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
 * If we are developing, we might want to start armboot from ram
 * so we MUST NOT initialize critical regs like mem-timing ...
 */
#if 0
#define CONFIG_INIT_CRITICAL		/* undef for developing */
#endif

#undef CONFIG_SKIP_LOWLEVEL_INIT
//#define CONFIG_SKIP_RELOCATE_UBOOT

/*
 * High Level Configuration Options
 * (easy to change)
 */
#define CONFIG_ARM7		1	/* This is a ARM7 CPU	*/
#define CONFIG_ARM_THUMB	1	/* this is an ARM720TDMI */
#define CONFIG_LPC2292
#undef  CONFIG_ARM7_REVD	 	/* disable ARM720 REV.D Workarounds */

#undef CONFIG_USE_IRQ			/* don't need them anymore */

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

#define CONFIG_BOOTP_MASK       (CONFIG_BOOTP_DEFAULT|CONFIG_BOOTP_BOOTFILESIZE)

//#define CONFIG_COMMANDS		((CONFIG_CMD_DFL | CFG_CMD_FAT | CFG_CMD_MMC) & ~CFG_CMD_NET)
#define CONFIG_COMMANDS		(CONFIG_CMD_DFL | CFG_CMD_FAT | CFG_CMD_MMC | CFG_CMD_NET | CFG_CMD_DHCP | CFG_CMD_PING)
//#define CONFIG_COMMANDS		(CONFIG_CMD_DFL | CFG_CMD_FAT | CFG_CMD_MMC)
#define CONFIG_MAC_PARTITION
#define CONFIG_DOS_PARTITION

/* this must be included AFTER the definition of CONFIG_COMMANDS (if any) */
#include <cmd_confdefs.h>

#define CONFIG_BOOTARGS    	"root=/dev/ram initrd=0x81500000,1800k console=ttyS0,115200N8"
//#define CONFIG_BOOTCOMMAND	"run nor_boot"
#define CONFIG_BOOTCOMMAND	"echo ;echo Booting from NOR FLASH;" \
                            "echo First loads 'linux.bin' and then 'romfs.bin';" \
                            "run nor_boot"
#define CONFIG_BOOTDELAY 3

/*
 * Miscellaneous configurable options
 */
#define	CFG_LONGHELP				/* undef to save memory		*/
#define	CFG_PROMPT		"LPC2292SODIMM # " /* Monitor Command Prompt	*/
#define	CFG_CBSIZE		512		/* Console I/O Buffer Size	*/
#define	CFG_PBSIZE (CFG_CBSIZE+sizeof(CFG_PROMPT)+16) /* Print Buffer Size */
#define	CFG_MAXARGS		16		/* max number of command args	*/
#define CFG_BARGSIZE		CFG_CBSIZE	/* Boot Argument Buffer Size	*/

#define CFG_MEMTEST_START	0x81008000	/* memtest works on	*/
#define CFG_MEMTEST_END		0x8173FFFF	/* 8 MB in DRAM	*/

#undef  CFG_CLKS_IN_HZ		/* everything, incl board info, in Hz */

#define	CFG_LOAD_ADDR		0x81008000	/* default load address	for armadillo: kernel img is here*/

#define CFG_SYS_CLK_FREQ        58982400        /* Hz */
#define	CFG_HZ			2048		/* decrementer freq in Hz */

						/* valid baudrates */
#define CFG_BAUDRATE_TABLE	{ 9600, 19200, 38400, 57600, 115200 }


/*
 * Default environment settings
 */
#define CONFIG_EXTRA_ENV_SETTINGS \
          "tftp_boot=tftpboot 81008000 linux.bin;tftpboot 81500000 romfs.bin;go 81008000\0" \
          "nor_boot=cp.b 80000000 81008000 100000;cp.b 80100000 81500000 100000;go 81008000\0" \
          "mmc_boot=mmc;fatload mmc 0 81008000 linux.bin;fatload mmc 0 81500000 romfs.bin;go 81008000\0" \
          "update_uboot=tftpboot 81000000 u-boot.bin;protect off 0 2ffff;erase 0 2ffff;cp.b 81000000 0 $(filesize)\0" \
          "update_nor=tftpboot 81000000 linux.bin;protect off 80000000 803fffff;erase 80000000 803fffff;cp.b 81000000 80000000 $(filesize);tftpboot 81000000 romfs.bin;cp.b 81000000 80100000 $(filesize)\0" \
          "ipaddr=192.168.1.100\0" \
          "netmask=255.255.255.0\0" \
          "serverip=192.168.1.3\0" \
          "ethaddr=00:1a:f1:00:00:00\0"

/*-----------------------------------------------------------------------
 * Stack sizes
 *
 * The stack sizes are set up in start.S using the settings below
 */
#define CONFIG_STACKSIZE	(128*1024)	/* regular stack */
#ifdef CONFIG_USE_IRQ
#define CONFIG_STACKSIZE_IRQ	(6*1024)	/* IRQ stack */
#define CONFIG_STACKSIZE_FIQ	(6*1024)	/* FIQ stack */
#endif

/*-----------------------------------------------------------------------
 * Physical Memory Map
 */
#define CONFIG_NR_DRAM_BANKS	1	   /* we have 1 bank of DRAM */
#define PHYS_SDRAM_1		  0x81000000 /* SDRAM Bank #1 */
#define PHYS_SDRAM_1_SIZE	0x00800000 /* 8 MB SDRAM */

#define PHYS_FLASH_1		  0x80000000 /* Flash Bank #1 */
#define PHYS_FLASH_SIZE		0x00200000 /* 2 MB */

#define CFG_FLASH_BASE		PHYS_FLASH_1

/*-----------------------------------------------------------------------
 * FLASH and environment organization
 */
#define CFG_MAX_FLASH_BANKS	2	/* max number of memory banks		*/
#define CFG_MAX_FLASH_SECT	1024	/* max number of sectors on one chip	*/

/* timeout values are in ticks */
#define CFG_FLASH_ERASE_TOUT	(2*CFG_HZ) /* Timeout for Flash Erase */
#define CFG_FLASH_WRITE_TOUT	(2*CFG_HZ) /* Timeout for Flash Write */

#define	CFG_ENV_IS_IN_FLASH	1
#define CFG_ENV_ADDR		(0x0 + 0x3C000)	/* Addr of Environment Sector	*/
#define CFG_ENV_SIZE		0x2000 /* Total Size of Environment Sector	*/

#define CONFIG_CMDLINE_TAG
#define CONFIG_SETUP_MEMORY_TAGS
#define CONFIG_INITRD_TAG
#define CONFIG_MMC 1

#define CONFIG_DISPLAY_CPUINFO

#endif	/* __CONFIG_H */
