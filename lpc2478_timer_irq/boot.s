#
# *** Startup Code (executed after Reset) ***
#

#include "config.h"

		.equ UND_Stack_Size,  0x00000000
		.equ SVC_Stack_Size,  0x00000100
		.equ ABT_Stack_Size,  0x00000000
		.equ FIQ_Stack_Size,  0x00000100
		.equ IRQ_Stack_Size,  0x00000200
		.equ USR_Stack_Size,  0x00000100

		.equ Stack_Size,      (UND_Stack_Size + SVC_Stack_Size + ABT_Stack_Size + FIQ_Stack_Size + IRQ_Stack_Size + USR_Stack_Size)

# Standard definitions of Mode bits and Interrupt (I & F) flags in PSRs

        .equ    Mode_USR,       0x10
        .equ    Mode_FIQ,       0x11
        .equ    Mode_IRQ,       0x12
        .equ    Mode_SVC,       0x13
        .equ    Mode_ABT,       0x17
        .equ    Mode_UND,       0x1B
        .equ    Mode_SYS,       0x1F

        .equ    I_Bit,          0x80        /* when I bit is set, IRQ is disabled */
        .equ    F_Bit,          0x40        /* when F bit is set, FIQ is disabled */

#        .equ    sram_bottom, SRAM_SADDR
 		.equ    sram_bottom, 0x40000000
        .equ    sram_top,    0x40010000
        .equ    stackTop,    0x40010000


#        .equ    sram_top,    SRAM_TOP
#        .equ    stackTop,    SRAM_TOP



#define VAL_PLLCFG_MSEL  ((PLL_MUL - 1) << 0)
#if (PLL_DIV == 1)
#define PLL_DIV_VALUE 0x00
#elif (PLL_DIV == 2)
#define PLL_DIV_VALUE 0x01
#elif (PLL_DIV == 4)
#define PLL_DIV_VALUE 0x10
#elif (PLL_DIV == 8)
#define PLL_DIV_VALUE 0x11
#endif
#define VAL_PLLCFG_PSEL  (PLL_DIV_VALUE << 5)
#define VAL_PLLCFG       (VAL_PLLCFG_MSEL | VAL_PLLCFG_PSEL)

# Phase Locked Loop (PLL) definitions
        .equ    PLL_BASE,       0xE01FC080  /* PLL Base Address */
        .equ    PLLCON_OFS,     0x00        /* PLL Control Offset*/
        .equ    PLLCFG_OFS,     0x04        /* PLL Configuration Offset */
        .equ    PLLSTAT_OFS,    0x08        /* PLL Status Offset */
        .equ    PLLFEED_OFS,    0x0C        /* PLL Feed Offset */
        .equ    PLLCON_PLLE,    (1<<0)      /* PLL Enable */
        .equ    PLLCON_PLLC,    (1<<1)      /* PLL Connect */
        .equ    PLLSTAT_PLOCK,  (1<<10)     /* PLL Lock Status */


# Starupt Code must be linked first at Address at which it expects to run.

        .text
        .arm
# ******************************************************************************
#   Declare external function
# ******************************************************************************
#        .extern lowLevelInit
#        .extern exceptionHandlerInit

        .global _startup
        .func   _startup
_startup:

# Exception Vectors
# Mapped to Address 0.

Vectors:
_vectors:
# If vectors are in FLASH, starting at 0x00000000
                B handleReset               /* jump to reset code              */
                B HandlerUndef              /* handlerUndef                    */
                B HandlerSWI                /* SWI interrupt handler           */
                B HandlerPabort             /* handlerPAbort                   */
                B HandlerDabort             /* handlerDAbort                   */
                NOP                         /* Reserved Vector                 */
                LDR PC,[PC,#-0x120]         /* jump to address supplied by VIC */
                B HandlerFIQ                /* handlerFIQ                      */

HandlerUndef:
				B	HandlerUndef
HandlerSWI:
     			B	HandlerSWI
HandlerPabort:
  				B	HandlerPabort
HandlerDabort:
	  			B	HandlerDabort
HandlerFIQ:
	  			B	HandlerFIQ

#IRQ_Handler     B       IRQ_Handler


# Reset Handler
handleReset:

#  Call pll initialization
#                BL      pllInit

# Setup Stack for each mode
                LDR     R0, =stackTop

#  Enter Undefined Instruction Mode and set its Stack Pointer
                MSR     CPSR_c, #Mode_UND|I_Bit|F_Bit
                MOV     SP, R0
                SUB     R0, R0, #UND_Stack_Size

#  Enter Abort Mode and set its Stack Pointer
                MSR     CPSR_c, #Mode_ABT|I_Bit|F_Bit
                MOV     SP, R0
                SUB     R0, R0, #ABT_Stack_Size

#  Enter FIQ Mode and set its Stack Pointer
                MSR     CPSR_c, #Mode_FIQ|I_Bit|F_Bit
                MOV     SP, R0
                SUB     R0, R0, #FIQ_Stack_Size

#  Enter IRQ Mode and set its Stack Pointer
                MSR     CPSR_c, #Mode_IRQ|I_Bit|F_Bit
                MOV     SP, R0
                SUB     R0, R0, #IRQ_Stack_Size

#  Enter Supervisor Mode and set its Stack Pointer
                MSR     CPSR_c, #Mode_SVC|I_Bit|F_Bit
                MOV     SP, R0
                SUB     R0, R0, #SVC_Stack_Size

#  Enter System Mode and set its Stack Pointer
                MSR     CPSR_c, #Mode_SYS|I_Bit|F_Bit
                MOV     SP, R0

#  Setup a default Stack Limit (when compiled with "-mapcs-stack-check")
                SUB     SL, SP, #USR_Stack_Size

#  Call low-level initialization
#                BL      lowLevelInit

# Relocate .data section (Copy from ROM to RAM)
                LDR     R1, =_etext
                LDR     R2, =_data
                LDR     R3, =_edata
LoopRel:        CMP     R2, R3
                LDRLO   R0, [R1], #4
                STRLO   R0, [R2], #4
                BLO     LoopRel

# Clear .bss section (Zero init)
                MOV     R0, #0
                LDR     R1, =__bss_start__
                LDR     R2, =__bss_end__
LoopZI:         CMP     R1, R2
                STRLO   R0, [R1], #4
                BLO     LoopZI

# Initialize exception vectors
#                BL      exceptionHandlerInit

# Enter the C code
Jump_To_Main:
                ADR     LR, __Return_from_Main
                MOV     R0, #0
                MOV     R1, #0
                LDR     R2, =main
                BX      R2

__Return_from_Main:
                B       __Return_from_Main

        .size   _startup, . - _startup
        .endfunc

        .end

