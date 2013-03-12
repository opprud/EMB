--------------------------------------------------------------------------------
--! @file
--! @ingroup	RTL
--! @brief		Host structure
--! @details    Implements the host structure, including an instantiation of the
--!				WB master.\n
--!				The module is used for bridging the registered CPU signals to
--!				the internal Wishbone bus 
--! @author		Morten Opprud Jakobsen \n
--!				AU-HIH \n                                            
--!				morten@hih.au.dk \n 
--! @version  	1.0
--------------------------------------------------------------------------------
library ieee;
	use ieee.std_logic_1164.all;
library work;
	use work.def_pkg.all;

--library work;

entity host is
  port( 
    Clk     : in     std_logic;    --! Clock signal synchronous to CPU signals
    Rst     : in     std_logic;    --! Asynchronous Reset
    CpuA_i  : in     wb_adr_typ;   --! CPU Address Input
    CpuD_i  : in     wb_dat_typ;   --! CPU Data Input
    CpuD_o  : out    wb_dat_typ;   --! CPU Data Output
    CpuRd_i : in     std_logic;    --! CPU Read strobe
    CpuWr_i : in     std_logic;    --! CPU Write strobe
    CpuCs_i : in     std_logic;    --! CPU Chip Select
    clk_i   : in     std_logic;    --! WB clk input 
    rst_i   : in     std_logic;    --! System reset input
    stb_o   : out    std_logic;    --! WB strobe out 
    cyc_o   : out    std_logic;    --! WB cycle out  
    we_o    : out    std_logic;    --! write enable
    adr_o   : out    wb_adr_typ;   --! address to slaves 
    dat_o   : out    wb_dat_typ;   --! data to slaves 
    cti_o   : out    wb_cti_typ;   --! @todo not used ? 
    ack_i   : in     std_logic;    --! ack signal from slaves
    err_i   : in     std_logic;    --! error signal from slaves
    rty_i   : in     std_logic;    --! retry signal from slaves
    dat_i   : in     wb_dat_typ    --! data from slaves
  );

end host ;

architecture RTL of host is

  --! Architecture declarations

  --! Internal signal declarations
  signal A_o    : wb_adr_typ;   --! Address bus to application 
  signal D_o    : wb_dat_typ;   --! Data bus to application
  signal D_i    : wb_dat_typ;   --! Data bus from application
  signal Wr_o   : std_logic;    --! Write strobe (single clock) to application
  signal Rd_o   : std_logic;    --! Read strobe (single clock) to application
  signal Err    : std_logic;    --! Error for ?
  signal RD_Ack : std_logic;    --! Read acknowledge signal used between host and WB master
  signal WR_Ack : std_logic;    --! Write acknowledge signal used between host and WB master

begin

  --! Instance and do port mappings.
  CPU : entity work.CpuInterface
    generic map (
      AddrWidth => AddrRange, --! Width of Address bus
      DataWidth => Datawidth  --! DataWidth of Databus in and out
    )
    port map (
	  --! System signals
      Clk     => Clk,
      Rst     => Rst,
      --! CPU conections
      CpuA_i  => CpuA_i, 
      CpuD_i  => CpuD_i,
      CpuD_o  => CpuD_o,
      CpuRd_i => CpuRd_i,
      CpuWr_i => CpuWr_i,
      CpuCs_i => CpuCs_i,
      --! WB master connections
      A_o     => A_o,
      D_o     => D_o,
      D_i     => D_i,
      Wr_o    => Wr_o,
      Rd_o    => Rd_o
    );

  WB_Master : entity work.WB_Master
    port map (
      --! CPU I/F signals
      A_i     => A_o,
      wr_D    => D_o,
      wr_strb => Wr_o,
      rd_D    => D_i,
      rd_strb => Rd_o,
      RD_Ack  => RD_Ack,
      WR_Ack  => WR_Ack,
      Err     => Err,
      --! Wishbone signals 
      clk_i   => clk_i,
      rst_i   => rst_i,
      stb_o   => stb_o,
      cyc_o   => cyc_o,
      we_o    => we_o,
      adr_o   => adr_o,
      dat_o   => dat_o,
      cti_o   => cti_o,
      ack_i   => ack_i,
      err_i   => err_i,
      rty_i   => rty_i,
      dat_i   => dat_i
    );

end struct;
