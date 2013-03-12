/*
 * (C) Copyright 2000-2003
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
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

/*
 * Misc boot support
 */
#include <common.h>
#include <command.h>
#include <net.h>
#include <asm/arch/hardware.h>

#if defined(CONFIG_I386)
DECLARE_GLOBAL_DATA_PTR;
#endif

int do_go (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	ulong	addr, rc;
	int     rcode = 0;

	if (argc < 2) {
		printf ("Usage:\n%s\n", cmdtp->usage);
		return 1;
	}

	addr = simple_strtoul(argv[1], NULL, 16);

	printf ("## Starting application at 0x%08lX ...\n", addr);

	/*
	 * pass address parameter as argv[0] (aka command name),
	 * and all remaining args
	 */
#if defined(CONFIG_I386)
	/*
	 * x86 does not use a dedicated register to pass the pointer
	 * to the global_data
	 */
	argv[0] = (char *)gd;
#endif
#if !defined(CONFIG_NIOS)

#if defined(CONFIG_LPC2292)
{
volatile unsigned long dummy, i;

disable_interrupts();
PUT32(VPBDIV, 1);
PUT32(MEMMAP, 1);

PUT32(U0IER, 0);
PUT32(U0FCR, 0);
for (i=0; i<16; i++)
{
  dummy = GET32(U0RBR);
  dummy = GET32(U0IIR);
  dummy = GET32(U0LSR);
}

PUT32(IO1SET, 0x00010000);
PUT32(IO1CLR, 0x00020000);
udelay(10000);
PUT32(IO1SET, 0x00020000);
udelay(10000);
PUT32(S0SPCCR, 30);
PUT32(S0SPCR, 0x20);
dummy = GET32(S0SPDR);
dummy = GET32(S0SPDR);

/* reset VIC */
PUT32(VICIntEnClr, 0xffffffff);
PUT32(VICProtection, 0);
PUT32(VICDefVectAddr, 0x81000018);
PUT32(VICVectAddr0, 0);
PUT32(VICVectAddr1, 0);
PUT32(VICVectAddr2, 0);
PUT32(VICVectAddr3, 0);
PUT32(VICVectAddr4, 0);
PUT32(VICVectAddr5, 0);
PUT32(VICVectAddr6, 0);
PUT32(VICVectAddr7, 0);
PUT32(VICVectAddr8, 0);
PUT32(VICVectAddr9, 0);
PUT32(VICVectAddr10, 0);
PUT32(VICVectAddr11, 0);
PUT32(VICVectAddr12, 0);
PUT32(VICVectAddr13, 0);
PUT32(VICVectAddr14, 0);
PUT32(VICVectAddr15, 0);
PUT32(VICVectCntl0, 0);
PUT32(VICVectCntl1, 0);
PUT32(VICVectCntl2, 0);
PUT32(VICVectCntl3, 0);
PUT32(VICVectCntl4, 0);
PUT32(VICVectCntl5, 0);
PUT32(VICVectCntl6, 0);
PUT32(VICVectCntl7, 0);
PUT32(VICVectCntl8, 0);
PUT32(VICVectCntl9, 0);
PUT32(VICVectCntl10, 0);
PUT32(VICVectCntl11, 0);
PUT32(VICVectCntl12, 0);
PUT32(VICVectCntl13, 0);
PUT32(VICVectCntl14, 0);
PUT32(VICVectCntl15, 0);

//enable_interrupts();
}
#endif

#if defined (CONFIG_LPC2468) || defined (CONFIG_LPC2478)
  disable_interrupts();
  {
    volatile unsigned char dummy, i;

    U0IER = 0;
    U1IER = 0;
    for(i=0; i<16; i++)
    {
      dummy = U0RBR;
      dummy = U0LSR;
      dummy = U0IIR;
      dummy = U1RBR;
      dummy = U1LSR;
      dummy = U1IIR;
    }
  }
#endif


	rc = ((ulong (*)(int, char *[]))addr) (--argc, &argv[1]);
#else
	/*
	 * Nios function pointers are address >> 1
	 */
	rc = ((ulong (*)(int, char *[]))(addr>>1)) (--argc, &argv[1]);
#endif
	if (rc != 0) rcode = 1;

	printf ("## Application terminated, rc = 0x%lX\n", rc);
	return rcode;
}

/* -------------------------------------------------------------------- */

U_BOOT_CMD(
	go, CFG_MAXARGS, 1,	do_go,
	"go      - start application at address 'addr'\n",
	"addr [arg ...]\n    - start application at address 'addr'\n"
	"      passing 'arg' as arguments\n"
);

extern int do_reset (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[]);

U_BOOT_CMD(
	reset, CFG_MAXARGS, 1,	do_reset,
	"reset   - Perform RESET of the CPU\n",
	NULL
);
