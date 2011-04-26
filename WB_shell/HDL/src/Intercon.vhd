--------------------------------------------------------------------------------
--! @file
--! @ingroup	RTL
--! @brief		Wishbone Interconnect
--! @details    Connects a number of masters to a number of slaves, 
--!				using shared bus interconnect.\n
--!				Number of slaves and the associated types are declared
--!				in def_p  package\n
--!				Signals between master and slaves are connected by multiplexers.
--! @section 	Wishbone Wishbone revision:  3.0
--! @author		Morten Opprud Jakobsen \n
--!				AU-HIH \n                                            
--!				morten@hih.au.dk \n 
--! @version  	1.0
--------------------------------------------------------------------------------
library IEEE;
  use IEEE.std_logic_1164.all;
library work;
  use work.def_pkg.all;

entity Intercon is
  port( 
    --! General signals
    clk_i   : in     std_logic;
    rst_i   : in     std_logic;
    --! Master(m) cycle control connections
    ack_o   : out    std_logic;
    adr_i   : in     wb_adr_typ;
    cyc_i   : in     std_logic;
    cti_i   : in     wb_cti_typ;
    err_o   : out    std_logic;
    rty_o   : out    std_logic;
    stb_i   : in     std_logic;
    we_i    : in     std_logic;
    --! Master data connections
    m_dat_i : in     wb_dat_typ;
    m_dat_o : out    wb_dat_typ;
    --! Slave cycle control connections
    ack_i   : in     wb_s_size;
    adr_o   : out    wb_lad_typ;
    cyc_o   : out    std_logic;
    cti_o   : out    wb_cti_typ;
    err_i   : in     wb_s_size;
    rty_i   : in     wb_s_size;
    stb_o   : out    wb_s_size;
    we_o    : out    std_logic;
    --! Slave data connetions
    s_dat_i : in     wb_s_dat_typ;
    s_dat_o : out    wb_dat_typ
  );

end Intercon ;

architecture RTL of Intercon is
  signal s:               integer range 0 to Num_s; -- Pointer to active slave
  signal Timer_en:        boolean;
  signal Timer:           int8;
  signal err_s:           std_logic;
  signal adr_s:           wb_adr_typ;
  constant Timeout:       positive:= 4;  -- Number of clockcycles before a response is expected from a slave
begin

--! Connect selected slave and selected master          
--! This will only work for systems with a single master

  --! Data connections
  s_dat_o    <= m_dat_i;

  --! Process for Master return signals
  ack_o_gen :process (s, ack_i)
  begin
    ack_o    <= '0';
    if (ack_i(s) = '1') then
      ack_o<= '1';
    else
      ack_o<= '0';
    end if;
  end process ack_o_gen;

  err_o_gen : process (s, err_i, err_s)
  begin
    err_o    <= '0';
    if ((err_i(s) or err_s) = '1') then
      err_o<= '1';
    else
      err_o<= '0';
    end if;
  end process err_o_gen;

  rty_o_gen : process (s, rty_i)
  begin
    rty_o    <= '0';
    rty_o <= rty_i(s);
  end process rty_o_gen;

  m_dat_o_gen : process (s, s_dat_i)
  begin
    m_dat_o <= (others => '0');
    m_dat_o <= s_dat_i(s);
  end process m_dat_o_gen;

  SlaveMux : process (s, stb_i)
  begin
  --! default values
  --! Cycle control: Assign slave outputs
    stb_o   <= (others => '0');
    if (stb_i = '1') then
      stb_o(s)<= '1';
    else
      stb_o(s)<= '0';
    end if;
  end process SlaveMux;

  adr_o   <= adr_i(LAddrRange-1 downto 0);
  --had_o   <= adr_i(m)(AddrRange-1 downto LAddrRange);
  cyc_o   <= cyc_i;
  cti_o   <= cti_i;
  we_o    <= we_i;
  adr_s   <= adr_i;
  
  --! Slave select from the current addressS
  Slave_sel : process (adr_s)
  begin
    case adr_s(AddrRange-1 downto AddrRange-HAddrRange) is
      when BA_WBO   => s <= 1;
      when BA_WBI   => s <= 2;
      when others   => s <= 0; --! Slave 0 is reserved for dummy addressing, when the address is not valid
    end case;
  end process Slave_sel;

  --! Timeout. wb_err signal asserted, when no slave responds within Timeout
  tmout : process (clk_i, rst_i)
  begin
    if (clk_i'event and clk_i = '1') then
      if (rst_i = '1') then
        err_s <= '0';
        Timer_en <= false;
      else
        if (stb_i = '1' and err_s = '0') then --! Start timer when master starts a cycle
          Timer_en <= true;
        elsif (stb_i = '0') then
          err_s <= '0';
          Timer_en <= false;  --! Stop timer
        end if;
        if (ack_i(s) = '1') then
          Timer_en <= false;  --! Stop timer when an acknowledge is received
        end if;
        if (Timer = Timeout-1) then
          err_s <= '1';
          Timer_en <= false;  --! Stop timer
        end if;
      end if;
    end if;
  end process tmout;

  tick : process (clk_i, rst_i)
  begin
    if (clk_i'event and clk_i = '1') then
      if (rst_i = '1') then
        Timer <= 0;
      else
        if Timer_en then
          Timer <= Timer + 1;
        else
          Timer <= 0;
        end if;
      end if;
    end if;
  end process tick;

end RTL;

