/* 
    This code was original written by Ulrich Radig and modified by 
    Embedded Artists AB (www.embeddedartists.com).

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
    
#include <config.h>
#include <common.h>
#include <asm/arch/hardware.h>
#include "spi.h"

#define MMC_Enable() PUT32(IO1CLR, 1l<<22)
#define MMC_Disable() PUT32(IO1SET, 1l<<22)
#define mmc_spi_cfg() spi_set_clock(8); spi_set_cfg(0,1,0); 

static unsigned char Write_Command_MMC (unsigned char *CMD);
static void MMC_Read_Block(unsigned char *CMD,
		    unsigned char *Buffer,
		    unsigned short int Bytes);
#if 0
static unsigned char Read_CID_MMC (unsigned char *Buffer);
#endif

/* initialize the hardware */
int mmc_hw_init(void)
{
  unsigned long a;
  unsigned short int Timeout = 0;
  unsigned char b ;
  unsigned char CMD[] = {0x40,0x00,0x00,0x00,0x00,0x95};

  /* set-up GPIO and SPI */
  (*((volatile unsigned long *)PINSEL2)) &= ~(1l<<3); /* clear bit 3 */
  (*((volatile unsigned long *)IO1DIR)) |= (1l<<22); /* set bit 22 (output) */

//printf("\nEnter mmc_hx_init()\n");
  MMC_Disable();
  
  spi_lock();
  spi_set_clock(248);
  spi_set_cfg(0,1,0);
  MMC_Enable();

  for(a=0;a<20000;a++){
    asm("nop");
  }
  
  //Init MMC/SD in SPI-mode
  for (b = 0;b<10;b++){
    spi_write(0xff);
  }	

  //Send command CMD0
  while(Write_Command_MMC (CMD) !=1){
    if (Timeout++ > 200){
      MMC_Disable();
      spi_unlock();
      return(1);
    }
  }	
  //Send command CMD1
  Timeout = 0;
  CMD[0] = 0x41;
  CMD[5] = 0xFF;
  
  while( Write_Command_MMC (CMD) !=0){
    if (Timeout++ > 200){
      MMC_Disable();
      spi_unlock();
      return(2);
    }
  }	

  MMC_Disable();
  spi_unlock();
  
  return 0;
}

//############################################################################
//Sendet ein Commando an die MMC/SD-Karte
static unsigned char Write_Command_MMC (unsigned char *CMD)
//############################################################################
{
  unsigned char a, tmp = 0xff;
  unsigned short int Timeout = 0;

  MMC_Disable();
  spi_write(0xFF);
  MMC_Enable();
	
  for (a = 0;a<0x06;a++){
    spi_write(*CMD++);
  }
  while (tmp == 0xff){
    tmp = spi_read();
    if (Timeout++ > 5000){
      break;
    }
  }

  return(tmp);
}

//############################################################################
//Routine zum lesen des CID Registers von der MMC/SD-Karte (16Bytes)
void MMC_Read_Block(unsigned char *CMD,unsigned char *Buffer,unsigned short int Bytes)
//############################################################################
{
  unsigned short int a;
  spi_lock();
  mmc_spi_cfg();
  MMC_Enable();

  if (Write_Command_MMC (CMD) != 0){
    MMC_Disable();
    spi_unlock();
    return;
  }	
  while (spi_read() != 0xfe){};
  for (a=0;a<Bytes;a++){
    *Buffer++ = spi_read();
  }
  //CRC-Byte auslesen
  spi_read();//CRC - Byte wird nicht ausgewertet
  spi_read();//CRC - Byte wird nicht ausgewertet	
  //set MMC_Chip_Select to high (MMC/SD-Karte Inaktiv)
  MMC_Disable();
  spi_unlock();
  return;
}

//############################################################################
//Routine zum lesen eines Blocks(512Byte) von der MMC/SD-Karte
unsigned char mmc_read_sector (unsigned long addr,unsigned char *Buffer)
//############################################################################
{	
  //Commando 16 zum lesen eines Blocks von der MMC/SD - Karte
  unsigned char CMD[] = {0x51,0x00,0x00,0x00,0x00,0xFF}; 
  
  /*Die Adressierung der MMC/SD-Karte wird in Bytes angegeben,
    addr wird von Blocks zu Bytes umgerechnet danach werden 
    diese in das Commando eingefgt*/
  
  addr = addr << 9; //addr = addr * 512
  
  CMD[1] = ((addr & 0xFF000000) >>24);
  CMD[2] = ((addr & 0x00FF0000) >>16);
  CMD[3] = ((addr & 0x0000FF00) >>8 );
  
  MMC_Read_Block(CMD,Buffer,512);
  
  return(0);
}

//############################################################################
//Routine zum schreiben eines Blocks(512Byte) auf die MMC/SD-Karte
unsigned char mmc_write_sector (unsigned long addr,unsigned char *Buffer)
//############################################################################
{
  unsigned char tmp,a;
  unsigned short int b;
  //Commando 24 zum schreiben eines Blocks auf die MMC/SD - Karte
  unsigned char CMD[] = {0x58,0x00,0x00,0x00,0x00,0xFF}; 
  
  /*Die Adressierung der MMC/SD-Karte wird in Bytes angegeben,
    addr wird von Blocks zu Bytes umgerechnet danach werden 
    diese in das Commando eingefgt*/
#if 0  
  addr = addr << 9; //addr = addr * 512
  
  CMD[1] = ((addr & 0xFF000000) >>24);
  CMD[2] = ((addr & 0x00FF0000) >>16);
  CMD[3] = ((addr & 0x0000FF00) >>8 );
#else
  CMD[1] = ((addr & 0xFF000000) >>15);
  CMD[2] = ((addr & 0x00FF0000) >>7);
  CMD[3] = ((addr & 0x0000FF00) <<1);
#endif

  spi_lock();
  mmc_spi_cfg();
  MMC_Enable();
  
  //Sendet Commando CMD24 an MMC/SD-Karte (Write 1 Block/512 Bytes)
  tmp = Write_Command_MMC (CMD);
  if (tmp != 0){
    MMC_Disable();
    spi_unlock();
    return(tmp);
  }	

  //Wartet einen Moment und sendet einen Clock an die MMC/SD-Karte
  for (a=0;a<100;a++){
    spi_read();
  }
  //Sendet Start Byte an MMC/SD-Karte
  spi_write(0xFE);	
  
  //Schreiben des Bolcks (512Bytes) auf MMC/SD-Karte
  for (b=0;b<512;b++){
    spi_write(*Buffer++);
  }
  
  //CRC-Byte schreiben
  spi_write(0xFF); //Schreibt Dummy CRC
  spi_write(0xFF); //CRC Code wird nicht benutzt
  
  //Wartet auf MMC/SD-Karte Bussy
  while (spi_read() != 0xff){};
  
  //set MMC_Chip_Select to high (MMC/SD-Karte Inaktiv)
  MMC_Disable();
  spi_unlock();
  return(0);
}

#if 0
//############################################################################
//Routine zum lesen des CID Registers von der MMC/SD-Karte (16Bytes)
static unsigned char Read_CID_MMC (unsigned char *Buffer)
//############################################################################
{
  //Commando zum lesen des CID Registers
  unsigned char CMD[] = {0x4A,0x00,0x00,0x00,0x00,0xFF}; 
  
  MMC_Read_Block(CMD,Buffer,16);
  
  return(0);
}
#endif

//############################################################################
//Routine zum lesen des CSD Registers von der MMC/SD-Karte (16Bytes)
unsigned char mmc_read_csd (unsigned char *Buffer)
//############################################################################
{	
  //Commando zum lesen des CSD Registers
  unsigned char CMD[] = {0x49,0x00,0x00,0x00,0x00,0xFF};
  
  MMC_Read_Block(CMD,Buffer,16);
  
  return(0);
}
