/*
 * type.h
 *
 *  Created on: 27/08/2010
 *      Author: Morten
 */

#ifndef TYPE_H_
#define TYPE_H_

#define TRUE	1
#define FALSE	0
#define NULL	0

#define OK		0
#define FAIL	-1

#define tU8   unsigned char
#define tU16  unsigned short
#define tU32  unsigned int
#define tS8   signed char
#define tS16  signed short
#define tS32  int
#define tBool unsigned char

#define OK		0
#define FAIL	-1

/** Simple defines */
#define OFF             0
#define ON              1
#define FALSE           0
#define TRUE            1

#define NOT             !
#define AND             &&
#define bwAND           &
#define OR              ||
#define bwOR            |
#define exOR            ^

#define BIT0            0x00000001
#define BIT1            0x00000002
#define BIT2            0x00000004
#define BIT3            0x00000008
#define BIT4            0x00000010
#define BIT5            0x00000020
#define BIT6            0x00000040
#define BIT7            0x00000080
#define BIT8            0x00000100
#define BIT9            0x00000200
#define BIT10           0x00000400
#define BIT11           0x00000800
#define BIT12           0x00001000
#define BIT13           0x00002000
#define BIT14           0x00004000
#define BIT15           0x00008000
#define BIT16           0x00010000
#define BIT17           0x00020000
#define BIT18           0x00040000
#define BIT19           0x00080000
#define BIT20           0x00100000
#define BIT21           0x00200000
#define BIT22           0x00400000
#define BIT23           0x00800000
#define BIT24           0x01000000
#define BIT25           0x02000000
#define BIT26           0x04000000
#define BIT27           0x08000000
#define BIT28           0x10000000
#define BIT29           0x20000000
#define BIT30           0x40000000
#define BIT31           0x80000000

/* Status type */
//typedef int STATUS;

/* NO_ERROR macro */
#define _NO_ERROR           (int)(0)
/* ERROR macro */
#define _ERROR              (int)(-1)
/* Device unknown macro */
#define LPC_DEV_UNKNOWN     (int)(-2)
/* Device not supported macro */
#define LPC_NOT_SUPPORTED   (int)(-3)
/* Device not open macro */
#define LPC_NOT_OPEN        (int)(-4)
/* Device in use macro */
#define LPC_IN_USE          (int)(-5)
/* Device oin conflict macro */
#define LPC_PIN_CONFLICT    (int)(-6)
/* Device bad paramaters macro */
#define LPC_BAD_PARAMS      (int)(-7)
/* Bad device handle macro */
#define LPC_BAD_HANDLE      (int)(-8)
/* Bad device clock macro */
#define LPC_BAD_CLK         (int)(-9)
/* Device can't start macro */
#define LPC_CANT_START      (int)(-10)
/* Device can't stop macro */
#define LPC_CANT_STOP       (int)(-11)

/* following are legacy defines which are OBSELETE. DONOT USE. */
#define SMA_DEV_UNKNOWN     LPC_DEV_UNKNOWN
#define SMA_NOT_SUPPORTED   LPC_NOT_SUPPORTED
#define SMA_NOT_OPEN        LPC_NOT_OPEN
#define SMA_IN_USE          LPC_IN_USE
#define SMA_PIN_CONFLICT    LPC_PIN_CONFLICT
#define SMA_BAD_PARAMS      LPC_BAD_PARAMS
#define SMA_BAD_HANDLE      LPC_BAD_HANDLE
#define SMA_BAD_CLK         LPC_BAD_CLK
#define SMA_CANT_START      LPC_CANT_START
#define SMA_CANT_STOP       LPC_CANT_STOP

#endif /* TYPE_H_ */
