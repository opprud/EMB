void UART16C750_isr (UART16C750CONFIG* ptUARTConfig) {
   ptUARTConfig->Interrupts++;
	
	 char UartIIR;

	 // Arm H8, only once, unless we support nested UART IRQ's - FKL/CFR ???
	 ptUARTConfig->ClearMCUInterruptStatus(); <-----Armér H8SX flanketrig

	 UartIIR = EXT_IIR;	//MOJ: copy initial IIR register to avoid multiple "clear on read" 

	 
   /* Process all interrupt bits*/
   do
   {
      // First clear interrupt status bit
			// MOJ:This doesn't seem to be nescecerry to repeat
      //ptUARTConfig->ClearMCUInterruptStatus(); <-----Armér H8SX flanketrig

      // Use the previously read UART interrupt status register
      //switch (0xF & EXT_IIR)
			switch (0xF & UartIIR) //Use copied value
      {
         /*############### Transmit interrupt ################*/
         case TX_REGISTER_EMPTY_INT:

            if (ptUARTConfig->sTxQueue.wNbrOfBytes)
            {
               // de-refer Tx queue to gain execution speed.
               DU8* pnTxBuffer = ptUARTConfig->sTxQueue.pnBuffer;
               DU16* pwNbrOfBytes = &(ptUARTConfig->sTxQueue.wNbrOfBytes);
               DU16* pwOutIndex = &(ptUARTConfig->sTxQueue.wOutIndex);
               DU16  wSize = ptUARTConfig->sTxQueue.wSize;
               char byteCount = *pwNbrOfBytes < ptUARTConfig->nFifoSize ? *pwNbrOfBytes: ptUARTConfig->nFifoSize;
               DUFF_DEVICE_16(byteCount,
                             EXT_THR = pnTxBuffer[*pwOutIndex]; // Move the byte
                             (*pwNbrOfBytes)--;    // Dec # of bytes in the queue
                             if (++(*pwOutIndex) == wSize)  // Inc the output index pointer
                                *pwOutIndex = 0; // If the pointer is at the end of the buffer
                                );
               /* Increment number of bytes transmitted */
               ptUARTConfig->TxBytes += byteCount; 

               /* Disable interrupt when queue is empty */
               if (ptUARTConfig->sTxQueue.wNbrOfBytes == 0)
                  BITCLEAR(EXT_IER, 0x02);
            }
            else
            {
               /* Disable TX interrupt if queue empty */
               BITCLEAR(EXT_IER, 0x02);
            }
            break;
      }
			//MOJ: only read IIR register here to avoid multiple "clears on read" of THRE flag.
			// An extra read without switch case (above) can clear an actual pending THRE flag.
			// see table 5, page 27 in Uart datasheet
			// Writing the EXT_THR register clears the current THRE flag. 
			UartIIR = EXT_IIR;	 
		} while ((0x1 & UartIIR) != NO_PENDING_INT); 
			//   } while ((0x1 & EXT_IIR) != NO_PENDING_INT);

