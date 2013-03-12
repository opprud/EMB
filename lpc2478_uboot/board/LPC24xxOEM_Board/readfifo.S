   .file        "readfifo.s"

  .text
  .section .text
  .code 32

		@; int MCI_ReadFifo(unsigned int * dest);
		@; int MCI_WriteFifo(unsigned int * src);

        .align 2
        .global MCI_ReadFifo
        .type MCI_ReadFifo,function
MCI_ReadFifo:
		STMFD   sp!,{r4,r5,r12,lr}
		LDR     r4, mci_fifo
		MOV     r5, r0
		LDMIA   r4, {r0-r3}
		STMIA   r5!, {r0-r3}
		LDMIA   r4, {r0-r3}
		STMIA   r5!, {r0-r3}
		MOV     r0, r5
		LDMFD   sp!,{r4,r5,r12,pc}

        .align 2
        .global MCI_WriteFifo
        .type MCI_WriteFifo,function
MCI_WriteFifo:
		STMFD   sp!,{r4,r5, r12,lr}
		LDR     r5, mci_fifo
		MOV     r4, r0
		LDMIA   r4!, {r0-r3}
		STMIA   r5, {r0-r3}
		LDMIA   r4!, {r0-r3}
		STMIA   r5, {r0-r3}
		MOV     r0, r4
		LDMFD   sp!,{r4,r5,r12,pc}

mci_fifo:
    .word   0xE008C080
    .end
