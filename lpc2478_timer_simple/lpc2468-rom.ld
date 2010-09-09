/* ****************************************************************************************************** */
/*   lpc2468-rom.ld			  	            LINKER  SCRIPT                                                */
/*                                                                                                        */
/*                                                                                                        */
/*   The Linker Script defines how the code and data emitted by the GNU C compiler and assembler are  	  */
/*   to be loaded into memory (code goes into FLASH, variables go into RAM).                 			  */
/*                                                                                                        */
/*   Any symbols defined in the Linker Script are automatically global and available to the rest of the   */
/*   program.                                                                                             */
/*                                                                                                        */
/*   To force the linker to use this LINKER SCRIPT, just add the -T lpc2468-rom.ld directive              */
/*   to the linker flags in the makefile.                                                                 */
/*                                                                                                        */
/*                                                                                                        */
/*                              MEMORY MAP                                                                */
/*                      |                                 |0x40010000                                     */
/*            .-------->|---------------------------------|                                               */
/*            .         |                                 |0x4000FFFB   <---------- _stack_end            */
/*            .         |     stack area for user program |                                               */
/*            .         |                                 |                                               */
/*            .         |                                 |                                               */
/*            .         |                                 |                                               */
/*            .         |                                 |                                               */
/*            .         |                                 |                                               */
/*            .         |                                 |                                               */
/*            .         |                                 |                                               */
/*            .         |          free ram               |                                               */
/*           ram        |                                 |                                               */
/*            .         |                                 |                                               */
/*            .         |                                 |                                               */
/*            .         |.................................|0x4000FDFB <---------- _bss_end                */
/*            .         |                                 |                                               */
/*            .         |  .bss   uninitialized variables |                                               */
/*            .-------->|---------------------------------|0x40000000 <---------- _bss_beg                */
/*                      |                                 |                                               */
/*                                                                                                        */
/*                                                                                                        */
/*                                                                                                        */
/*                      |                                 |                                               */
/*           .--------> |---------------------------------|                                               */
/*           .          |                                 |0x0007FFFF                                     */
/*           .          |                                 |                                               */
/*           .          |                                 |                                               */
/*           .          |                                 |                                               */
/*           .          |                                 |                                               */
/*           .          |                                 |                                               */
/*           .          |       unused flash eprom        |                                               */
/*           .          |                                 |                                               */
/*           .          |                                 |                                               */
/*           .          |                                 |                                               */
/*           .          |                                 |                                               */
/*           .          |                                 |                                               */
/*         flash        |                                 |                                               */
/*           .          |                                 |                                               */
/*           .          |                                 |                                               */
/*           .          |---------------------------------|0x000001e4                                     */
/*           .          |                                 |                                               */
/*           .          |                                 |                                               */
/*           .          |                                 |                                               */
/*           .          |            C code               |0x00000178  main                               */
/*           .          |                                 |0x00000144  delay                              */
/*           .          |                                 |0x0000011c  ledOff                             */
/*           .          |                                 |0x000000f4  ledOn                              */
/*           .          |---------------------------------|0x000000a4  ledInit                            */
/*           .          |                                 |0x000000xx                                     */
/*           .          |         Startup Code            |                                               */
/*           .          |         (assembler)             |                                               */
/*           .          |                                 |                                               */
/*           .          |---------------------------------|0x00000020 Start                               */
/*           .          |                                 |0x0000001F                                     */
/*           .          | Interrupt Vector Table (unused) |                                               */
/*           .          |          64 bytes               |                                               */
/*           .--------->|---------------------------------|0x00000000 _startup                            *
/*                                                                                                        */
/*                                                                                                        */
/*                                                                                                        */
/* startup - this output section holds the code in the startup function, defined in boot.s                */
/* .text - this output section holds all other executable code generated by the compiler                  */
/* .data - this output section contains all initialized data generated by the compiler                    */
/* .bss - this output section contains all uninitialized data generated by the compiler                   */
/*                                                                                                        */
/* ****************************************************************************************************** */

MEMORY {
   flash : ORIGIN = 0, LENGTH = 512K
   ram : ORIGIN = 0x40000000, LENGTH = 64K
   }

__stack_end__ = 0x40000000 + 64K - 4;
SECTIONS {
  . = 0;
  startup : { *(.startup)} >flash
  prog : {
         *(.text)
         *(.rodata)
         *(.rodata*)
         *(.glue_7)
         *(.glue_7t)
         } >flash
  __end_of_text__ = .;
  .data : {
         __data_beg__ = .;
         __data_beg_src__ = __end_of_text__;
         *(.data)
         __data_end__ = .;
         } >ram AT>flash
  .bss : {
         __bss_beg__ = .;
         *(.bss)
         } >ram
    /* Align here to ensure that the .bss section occupies space up to
       _end.  Align after .bss to ensure correct alignment even if the
       .bss section disappears because there are no input sections.  */
    . = ALIGN(32 / 8);
   }
   . = ALIGN(32 / 8);
   _end = .;
   _bss_end__ = . ; __bss_end__ = . ; __end__ = . ;
   PROVIDE (end = .);

/*
INPUT( -lc -lgcc)
*/

PROVIDE( undefined_instruction_exception = endless_loop);
PROVIDE( software_interrupt_exception = endless_loop);
PROVIDE( prefetch_abort_exception = endless_loop);
PROVIDE( data_abort_exception = endless_loop);
PROVIDE( reserved_exception = endless_loop);
PROVIDE( interrupt_exception = endless_loop);
PROVIDE( fast_interrupt_exception = endless_loop);
