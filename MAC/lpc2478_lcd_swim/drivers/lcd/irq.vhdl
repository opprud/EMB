


//in driver 'EXT_THR = pnTxBuffer[*pwOutIndex]; // Move the byte'
// clears the THRinterrupt

if (iLSR_THRERE = '1' or iFCR_TXFIFOReset = '1' or 
   (iIERWrite = '1' and iDIN(1) = '1' and iLSR_THRE = '1')) then
		iTHRInterrupt <= '1';           -- Set on THRE, TX FIFO reset (FIFO enable) or ETBEI enable
elsif ((iIIRRead = '1' and iIIR(3 downto 1) = "001") or iTHRWrite = '1') then
		iTHRInterrupt <= '0';           -- Clear on IIR read (if source of interrupt) or THR write
end if;


-----------
iLSR_THRERE <= rising_edge(iLSR_THRE)

iLSR_THRE <= '1' when iTXFIFOEmpty = '1' else '0';