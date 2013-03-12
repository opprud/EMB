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
--! @todo Full in decoding of control signals and sampling of the Add/Data busses		
--!				Calculate and simulate and verify number of nessecery CPU Waitstates
--------------------------------------------------------------------------------
library ieee;
  use ieee.std_logic_1164.ALL;
library work;
  use work.def_pkg.ALL;
  
entity CpuInterface is
  generic( 
    AddrWidth : integer := 10;   --DataWidth of Address bus
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
---- ADD HERE !!!!!!!!!!!!!


begin

--------------------------------------------------------------------------------
--                         Synchronize input signals                          --
--------------------------------------------------------------------------------


---- ADD HERE !!!!!!!!!!!!!




end;
