--------------------------------------------------------------------------------
--! @file
--! @ingroup	RTL
--! @brief		Wishbone master
--! @details    Wishbone Master, generates Wishbone Bus cycles
--!				when asserted by input strobes.
--! @section WD 	Wishbone Datasheet 
--! 				Wishbone revision:  3.0\n
--!					Interface type: 	Master
--! @author		Morten Opprud Jakobsen \n
--!				AU-HIH \n                                            
--!				morten@hih.au.dk \n 
--! @author		Opencores.org \n
--! @version  	1.0
--------------------------------------------------------------------------------
library ieee;
	use ieee.std_logic_1164.all;
library work;
	use work.def_pkg.all;


entity WB_Master is
  port(
--!  Interface to protocol
    A_i     : in     wb_adr_typ;
    wr_D    : in     wb_dat_typ;
    wr_strb : in     std_logic;
    rd_D    : out    wb_dat_typ;
    rd_strb : in     std_logic;
    RD_Ack  : out    std_logic;
    WR_Ack  : out    std_logic;
    Err     : out    std_logic;
--!  Wishbone interface
    clk_i   : in     std_logic;
    rst_i   : in     std_logic;
    stb_o   : out    std_logic;
    cyc_o   : out    std_logic;
    we_o    : out    std_logic;
    adr_o   : out    wb_adr_typ;
    dat_o   : out    wb_dat_typ;
    cti_o   : out    wb_cti_typ;
    ack_i   : in     std_logic;
    err_i   : in     std_logic;
    rty_i   : in     std_logic;
    dat_i   : in     wb_dat_typ);
end WB_Master ;

architecture RTL of WB_Master is

--! Signal Declarations                                    --
  type   wb_cycle_typ is     (Idle,wb_write, wb_read, wait_end);
  signal cs_wb_cycle:         wb_cycle_typ;
  signal nx_wb_cycle:         wb_cycle_typ;
  signal nx_cyc_o:            std_logic;
  signal cyc_o_s:             std_logic;
  signal nx_stb_o:            std_logic;
  signal nx_we_o:             std_logic;
  signal we_o_s:              std_logic;
  signal dat_o_s:             wb_dat_typ;
  

--! Architecture....                                      
begin

--! Continous assignments                                 
  dat_o   <= dat_o_s;
  we_o    <= we_o_s;
  RD_Ack  <= ack_i and cyc_o_s and not we_o_s;
  WR_Ack  <= ack_i and cyc_o_s and we_o_s;
  Err     <= err_i;
  cyc_o   <= cyc_o_s;
  rd_D    <= dat_i;

  --! Misc processes                                        
  wb_set_dat_o : process(clk_i, rst_i)  --! (synchronous reset)
  begin
    if (clk_i'event and clk_i = '1') then
      if (rst_i = '1') then
        dat_o_s <= (others => '0');
      else
        if(wr_strb = '1') then
          dat_o_s <= wr_D;
        else
          dat_o_s <= dat_o_s;
        end if;
      end if;
    end if;
  end process wb_set_dat_o;
  
  --! This process samples the Address, when strobed
  wb_sample_address : process(clk_i, rst_i)
  begin
    if(clk_i'event and clk_i = '1') then
      if (rst_i = '1') then
        adr_o <= (others => '0');
      elsif (wr_strb = '1' or rd_strb = '1') then
        adr_o <= A_i;
      end if;
    end if;
  end process wb_sample_address;
  
  --!  Wishbone bus cycles
  --! next state logic                                  
  wb_cycle_state_logic : process(cs_wb_cycle, wr_strb, rd_strb, ack_i, err_i)
  begin
    case cs_wb_cycle is
  
      when Idle =>
        if (wr_strb = '1' and err_i = '0') then        --! Write data
          nx_wb_cycle <= wb_write;
        elsif (rd_strb = '1' and err_i = '0') then     --! read data
          nx_wb_cycle <= wb_read;
        else
          nx_wb_cycle <= Idle;
        end if;
  
      when wb_write =>
        if (ack_i = '1' or err_i = '1') then
          nx_wb_cycle <= Idle;
        else
          nx_wb_cycle <= wb_write;
        end if;
  
      when wb_read =>
        if (ack_i = '1' or err_i = '1') then
          nx_wb_cycle <= wait_end;
        else
          nx_wb_cycle <= wb_Read;
        end if;
        
      when wait_end =>
        if (rd_strb = '0') then
          nx_wb_cycle <= Idle;
        else
          nx_wb_cycle <= wait_end;
        end if;
  
      when others =>
        nx_wb_cycle <= Idle;
    end case;
  end process wb_cycle_state_logic;
  
  --! WB output logic
  wb_cycle_output_logic : process(nx_wb_cycle)
  begin
  --!default assignments
    nx_cyc_o  <= '0';
    nx_stb_o  <= '0';
    nx_we_o   <= '0';
    
    case nx_wb_cycle is
      when idle =>

      when wb_write =>
        nx_cyc_o  <= '1';
        nx_stb_o  <= '1';
        nx_we_o   <= '1';

      when wb_read =>
        nx_cyc_o  <= '1';
        nx_stb_o  <= '1';
        nx_we_o   <= '0';

      when wait_end =>

      when others => null;

    end case;
  end process wb_cycle_output_logic;
  
  --! WB state registers
  wb_cycle_state_reg : process(clk_i, rst_i)  --! (synchronous reset)
  begin
    if (clk_i'event and clk_i = '1') then
      if (rst_i = '1') then
        cs_wb_cycle <= Idle;
      else
        cs_wb_cycle <= nx_wb_cycle;
      end if;
    end if;
  end process wb_cycle_state_reg;
  
  --! WB output register logic
  wb_cycle_output_reg : process(clk_i,rst_i)
  begin
    if (clk_i'event and clk_i = '1') then
      if (rst_i = '1') then
        cyc_o_s<= '0';
        stb_o  <= '0';
        we_o_s <= '0';
        cti_o  <= (others => '0');
	  else
        cyc_o_s<= nx_cyc_o;
        stb_o  <= nx_stb_o;
        we_o_s <= nx_we_o;
        cti_o  <= cti_classic;
      end if;
    end if;
  end process wb_cycle_output_reg;

end RTL;

