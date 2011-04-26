--------------------------------------------------------------------------------
--! @file
--! @ingroup	RTL
--! @brief		Cpu interface
--! @details    This block synchronizes all signals from the CPU to the FPGA's 
--!				clock domain
--! @author		Morten Opprud Jakobsen \n
--!				AU-HIH \n                                            
--!				morten@hih.au.dk \n 
--! @version  	1.0
--! @todo  		Calculate and simulate and verify number of nessecery CPU Waitstates
--------------------------------------------------------------------------------
library ieee;
  use ieee.std_logic_1164.ALL;
library work;
  use work.def_pkg.ALL;
  
entity CpuInterface is
  generic( 
    AddrWidth : integer := 10;    --DataWidth of Address bus
    DataWidth : integer := 16    --DataWidth of Databus in and out
  );
  port( 
    Clk     : in     std_logic;                                  -- Clock signal synchronous to CPU signals
    Rst     : in     std_logic;                                  -- Asynchronous Reset
    CpuA_i  : in     std_logic_vector ( AddrWidth-1 downto 0 );  -- CPU Address Input
    CpuD_i  : in     std_logic_vector ( DataWidth-1 downto 0 );  -- CPU Data Input
    CpuD_o  : out    std_logic_vector ( DataWidth-1 downto 0 );  -- CPU Data Output
    CpuRd_i : in     std_logic;                                  -- CPU Read strobe
    CpuWr_i : in     std_logic;                                  -- CPU Write strobe
    CpuCs_i : in     std_logic;                                  -- CPU Chip Select
    A_o     : out    std_logic_vector ( AddrWidth-1 downto 0 );  -- Address bus to application 
    D_o     : out    std_logic_vector ( DataWidth-1 downto 0 );  -- Data bus to application
    D_i     : in     std_logic_vector ( DataWidth-1 downto 0 );  -- Data bus from application
    Wr_o    : out    std_logic;                                  -- Write strobe (single clock) to application
    Rd_o    : out    std_logic                                   -- Read strobe (single clock) to application
  );
end CpuInterface ;

architecture RTL of CpuInterface is

  signal A_r:       std_logic_vector (AddrWidth-1 downto 0);
  signal D_r:       std_logic_vector (DataWidth-1 downto 0);
  signal Wr_r:      std_logic;
  signal Wr_rr:     std_logic;
  signal Rd_r:      std_logic;

begin

--------------------------------------------------------------------------------
--                         Synchronize input signals                          --
--------------------------------------------------------------------------------

  ProcCpuSync1 : process(Clk)
  begin
    if(Clk'event and Clk = '1') then
      --register inputs      
	  A_r   <= CpuA_i; 
      D_r   <= CpuD_i; 
      Wr_r  <= CpuWr_i and CpuCs_i;
      Wr_rr <= Wr_r;
      Rd_r  <= CpuRd_i and CpuCs_i;
    end if;
  end process ProcCpuSync1;

  ProcCpuSync2 : process(Clk, Rst)
  begin
    if(Clk'event and Clk = '1') then
      if (Rst = '1') then
        A_o  <= (others => '0');
        D_o  <= (others => '0');
        Rd_o  <= '0';
      else
		--latch only when selected
        if (Wr_r = '1') or (Rd_r = '1') then
          A_o <= A_r; 
          D_o <= D_r; 
        end if;
        Rd_o  <= Rd_r;
      end if;
    end if;
  end process ProcCpuSync2;

  Wr_o <= Wr_rr and not Wr_r;   -- Generate a write strobe at the end of write (single clock)
  CpuD_o <= D_i;

end;
