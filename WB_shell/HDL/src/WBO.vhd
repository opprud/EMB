--------------------------------------------------------------------------------
--! @file
--! @ingroup	RTL
--! @brief		WB output port
--! @details    General Purpose Wishbone output port. Width identical
--! 			to Wishbone datawidth, with granularity similar to width
--! @author		Morten Opprud Jakobsen \n
--!				AU-HIH \n                                            
--!				morten@hih.au.dk \n 
--! @version  	1.0
----------------------------------------------------------------------------------------------------------------------------------------------------------------
library IEEE;
  use IEEE.std_logic_1164.all;
library work;
  use work.def_pkg.all;

entity WBO is
  port (
	--! Wishbone Slave interface
    clk_i:        in    std_logic;    --! Clock input from SYSCON
    rst_i:        in    std_logic;    --! Reset for Wishbone interface
    cyc_i:        in    std_logic;    --! cycle input, asserted when cycle is in progress
    stb_i:        in    std_logic;    --! Strobe input, similar to Chip Select
    we_i:         in    std_logic;    --! Write Enable: High=WR, Low=RD
    dat_i:        in    wb_dat_typ;   --! data from host
    adr_i:        in    wb_lad_typ;   --! Low address input
    ack_o:        out   std_logic;    --! Slave acknowledge output, asserted after sucessful cycle
    err_o:        out   std_logic;    --! Error output, abnormal cycle termination
    rty_o:        out   std_logic;    --! Retry output, slave not ready
    dat_o:        out   wb_dat_typ;   --! data to host
	--! Output port
    Port_o:       out   wb_dat_typ);
end WBO;

architecture RTL of WBO is

  signal Q:             wb_dat_typ;
  signal r1:             wb_dat_typ;
  signal r2:             wb_dat_typ;

begin

  --!  Concurrent assignments
  err_o <= '0';
  rty_o <= '0';
  dat_o <= Q;
  Port_o <= Q;
  ack_o <= stb_i and cyc_i;  --! asynhronous cycle termination is OK here.

  --!  Processes                                             --
  --! Wishbone write to Q register
  Reg : process(clk_i, rst_i)
	begin
		if(clk_i'event and clk_i = '1') then
      if (rst_i = '1') then
        Q <= Revision_c;          --! Revision readable at reset 
      else
        if ((cyc_i and stb_i ) = '1') then
         	if (we_i = '1') then
    				case adr_i is
    					when A_WBO_REG1 =>
    	    		      r1 <= dat_i;
    	    		      Q  <= r1;
    					when A_WBO_REG2 =>
    	    		      r2 <= dat_i;
    	    		      Q  <= (r1 xor r2);
    					when others =>
    					 --Ack_o <= '0'; 
    				end case;
          end if;
				end if;
      end if;
    end if;
  end process Reg;

end RTL;
