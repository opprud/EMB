void UART16C750_isr (UART16C750CONFIG* ptUARTConfig) {
   ptUARTConfig->Interrupts++;

   /* Process all interrupt bits*/
   do
   {
      // First clear interrupt status bit
      ptUARTConfig->ClearMCUInterruptStatus(); <-----Armér H8SX flanketrig

      // Read the UART interrupt status register
      switch (0xF & EXT_IIR)
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
.
.
.
      }
   } while ((0x1 & EXT_IIR) != NO_PENDING_INT);

