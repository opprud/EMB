--------------------------------------------------------------------------------
--! @file
--! @defgroup	RTL RTL - All files that compromise the physical (FPGA) design
--! @brief		Cputest_structure
--! @details    This module connects all the internal slaves, the host and the  
--!				syscon with the interconn. 
--! @image 		html CpuTestSch.JPG "Cpu Test structure connections"
--! @author		Morten Opprud Jakobsen \n
--!				morten@opprud.dk \n 
--! @version  	see svn log for details
--------------------------------------------------------------------------------
library ieee;
  use ieee.std_logic_1164.ALL;
library work;
  use work.def_pkg.ALL;

entity Wrapper is
  port( 
    Clock     		  : in     std_logic;                        --! Clock signal synchronous to CPU signals
    Rst_i     		  : in     std_logic;                        --! Asynchronous Reset
	nCpuCs_i 		  : in     std_logic;                        --! CPU Chip Select
	nCpuRd_i 		  : in     std_logic;                        --! CPU Read strobe
	nCpuWr_i 		  : in     std_logic;                        --! CPU Write strobe
    CpuA_i  		  : in     wb_adr_typ;                       --! CPU Address Input
    CpuD     		  : inout  wb_dat_typ;                       --! External Bi-directional databus
    Port_o  		  : out    std_logic_vector ( 3 downto 0 );  --! Test output port
    Dbus_En  		  : out    std_logic;  						 --! Enable signal for DATABUS latches on LPC2478 board, connect to CS
    Abus_En  		  : out    std_logic   						 --! Enable signal for ADDRESSBUS latches on LPC2478 board, connect to gnd
  );
end Wrapper ;

architecture RTL of Wrapper is

  --! Architecture declarations

  --! Internal signal declarations
  --active high CPU signals
  signal CpuCs_i 		 : std_logic;                            -- CPU Chip Select
  signal CpuD_i  		 : wb_dat_typ;                           -- CPU Data Input
  signal CpuRd_i 		 : std_logic;                            -- CPU Read strobe
  signal CpuWr_i 		 : std_logic;                            -- CPU Write strobe
  signal CpuD_o  		 : wb_dat_typ;                           -- CPU Data Output
  
  -- WB signals
  signal clk_o           : std_logic;							 -- system clock
  signal rst_o           : std_logic;							 -- system reset
  signal ack_i           : std_logic;                            -- Aknowledge input from selected slave
  signal err_i           : std_logic;                            -- Error input from selected slave
  signal rty_i           : std_logic;                            -- Retry input from selected slave
  signal dat_i           : wb_dat_typ;                           -- Data input from selected slave
  signal adr_o           : wb_adr_typ;                           -- Address bus output from Master
  signal stb_o           : std_logic;                            -- Strobe signal
  signal cti_o           : wb_cti_typ;                           -- cyle type identifier
  signal cyc_o           : std_logic;                            -- Valid bus cycle in progress
  signal we_o            : std_logic;                            -- Write indicator
  signal dat_o           : wb_dat_typ;                           -- Databus from Master
  signal adr_i           : wb_lad_typ;                           -- Lower Address bus for selecting address space inside slave
  signal cti_i           : wb_cti_typ;                           -- Cycle Type Identifier
  signal cyc_i           : std_logic;                            -- Active during a valid bus cycle
  signal we_i            : std_logic;                            -- Write indicator
  signal s_dat_i         : wb_dat_typ;                           -- Databus common to all slaves
  signal stb_i           : std_logic_vector( Num_s downto 0 );
  signal ack_o           : std_logic_vector( Num_s downto 0 );   -- Bus of Ack responses from all Slaves
  signal err_o           : std_logic_vector( Num_s downto 0 );   -- Bus of Err responses from all Slaves
  signal rty_o           : std_logic_vector( Num_s downto 0 );   -- Bus of Rty responses from all Slaves
  signal s_dat_o         : wb_s_dat_typ( Num_s downto 0 );       -- Array of Databusses from all Slaves
  
  -- Out test port
  signal Port_o_int      : std_logic_vector( 15 downto 0 );   -- Bus of Rty responses from all Slaves
  

begin
  --! Architecture concurrent statements
  --! Drive Databus during read
  CpuD <= CpuD_o when nCpuCs_i = '0' and nCpuRd_i = '0' else (others => 'Z');
  CpuCs_i <= not nCpuCs_i;
  CpuRd_i <= not nCpuRd_i;
  CpuWr_i <= not nCpuWr_i;
  
  --! Enable Databus latches when CS is set (see LPC2478 OEM manual)
  Dbus_En <= nCpuCs_i;
  Abus_En <= '0';
  	
  --! LED Port o
  Port_o <= Port_o_int(3 downto 0);
  	
  --! Instantiate and port map
  Host1 : entity work.host(RTL)
    port map (
    --! Sys signals 
      Clk     => clk_o,
      clk_i   => clk_o,
      Rst     => rst_o,
      rst_i   => rst_o,
	--! to CPU signals 
      CpuA_i  => CpuA_i,
      CpuCs_i => CpuCs_i,
    --CpuD_i  => CpuD_i,
      CpuD_i  => CpuD,
      CpuRd_i => CpuRd_i,
      CpuWr_i => CpuWr_i,
      CpuD_o  => CpuD_o,
     --! to Interconn signals
      ack_i   => ack_i,
      err_i   => err_i,
      rty_i   => rty_i,
      dat_i   => dat_i,
      adr_o   => adr_o,
      stb_o   => stb_o,
      cti_o   => cti_o,
      cyc_o   => cyc_o,
      we_o    => we_o,
      dat_o   => dat_o
    );
  Intercon1 : entity work.Intercon(RTL)
    port map (
    --! System signals 
      clk_i   => clk_o,
      rst_i   => rst_o,
	--! to/from Host signals
      ack_o   => ack_i,
      err_o   => err_i,
      rty_o   => rty_i,
      m_dat_o => dat_i,
      adr_i   => adr_o,
      stb_i   => stb_o,
      cti_i   => cti_o,
      cyc_i   => cyc_o,
      we_i    => we_o,
      m_dat_i => dat_o,
	--! to/from WB slaves signals
      adr_o   => adr_i,
      cti_o   => cti_i,
      cyc_o   => cyc_i,
      we_o    => we_i,
      s_dat_o => s_dat_i,
      stb_o   => stb_i,
      ack_i   => ack_o,
      err_i   => err_o,
      rty_i   => rty_o,
      s_dat_i => s_dat_o
    );
     
    
  Syscon1 : entity work.Syscon(RTL)
    port map (
      Reset => Rst_i,
      clk_i => Clock,
      rst_o => rst_o,
      clk_o => clk_o
    );
  S1 : entity work.WBO(RTL)
    port map (
      clk_i  => clk_o,
      rst_i  => rst_o,
      cyc_i  => cyc_i,
      we_i   => we_i,
      dat_i  => s_dat_i,
      adr_i  => adr_i,
      stb_i  => stb_i(1),
      ack_o  => ack_o(1),
      err_o  => err_o(1),
      rty_o  => rty_o(1),
      dat_o  => s_dat_o(1),
      Port_o => Port_o_int
    );

end RTL;

