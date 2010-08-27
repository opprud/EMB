/* Sample initialization file */

         .extern main
         .extern exit

  /* .text is used instead of .section .text so it works with arm-aout too. */

         .text
         .code 32


         .align  0

         .extern __bss_beg__
         .extern __bss_end__
         .extern __stack_end__
         .extern __data_beg__
         .extern __data_end__
         .extern __data+beg_src__

         .global start
/*       .global _mainCRTStartup */
/*       .global _start */
         .global endless_loop

start:
_start:
_mainCRTStartup:
/* Start by setting up a stack */
         /*  Set up the stack pointer to end of bss */
         ldr             r3, .LC6
         mov     sp, r3

         sub     sl, sp, #512    /* Still assumes 512 bytes below sl */

         mov     a2, #0          /* Second arg: fill value */
         mov     fp, a2          /* Null frame pointer */
         mov     r7, a2          /* Null frame pointer for Thumb */

/*       ldr     r1, #__bss_beg__*/      /* First arg: start of memory block */

         ldr     r1, .LC1                /* First arg: start of memory block */
/*       ldr     r3, #__bss_end__*/      /* Second arg: end of memory block */
         ldr     r3, .LC2                /* Second arg: end of memory block */
         subs    r3, r3, r1              /* Third arg: length of block */
         beq     .end_clear_loop
         mov     r2, #0

.clear_loop:
         strb    r2, [r1], #1
         subs    r3, r3, #1
         bgt     .clear_loop

.end_clear_loop:

         ldr     r1, .LC3                /* First arg: start of memory block */
         ldr     r2, .LC4                /* Second arg: end of memory block */
         ldr     r3, .LC5
         subs    r3, r3, r1              /* Third arg: length of block */
         beq     .end_set_loop

.set_loop:
         ldrb    r4, [r2], #1
         strb    r4, [r1], #1
         subs    r3, r3, #1
         bgt     .set_loop

.end_set_loop:


         mov             r0, #0          /*  no arguments  */
         mov             r1, #0          /* no argv either */

         bl              main

endless_loop:
         b               endless_loop

/*
         bl              exit            Should not return
*/         

         /* For Thumb, constants must be after the code since only
         positive offsets are supported for PC relative addresses. */

         .align 0
.LC1:
.word   __bss_beg__
.LC2:
.word   __bss_end__
.LC3:
.word   __data_beg__
.LC4:
.word   __data_beg_src__
.LC5:
.word   __data_end__
.LC6:
.word   __stack_end__

.section .startup,"ax"
         .code 32
         .align 0

         b       start
         b       undefined_instruction_exception
         b       software_interrupt_exception
         b       prefetch_abort_exception
         b       data_abort_exception
         b       reserved_exception
         b       interrupt_exception
         b       fast_interrupt_exception
