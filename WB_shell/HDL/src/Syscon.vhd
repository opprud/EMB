--------------------------------------------------------------------------------
--! @file
--! @ingroup	RTL
--! @brief		Wishbone syscon module
--! @details    Standard Wishbone syscon module\n
--!				Generates teh system & Wishbone clock signal\n
--!				Reset is asserted asynchronously by a input reset signal and 
--!				deasserted by the clock								
--! @author		Morten Opprud Jakobsen \n
--!				AU-HIH \n                                            
--!				morten@hih.au.dk \n 
--! @version  	1.0
--------------------------------------------------------------------------------
library IEEE;
  use IEEE.std_logic_1164.all;
library unisim;
  use unisim.vcomponents.all;	

entity Syscon is
  port (
    Reset:        in    std_logic;    --! Asynchronous Reset input
    clk_i:        in    std_logic;    --! System clock input
    clk_o:        out   std_logic;    --! Clock output for all Wishbone interface signals. 
    rst_o:        out   std_logic);   --! Reset  output for Wishbone interface
end Syscon;

architecture RTL of Syscon is

  signal clk_s:         std_logic;
  signal clk_r:         std_logic;
  signal rst_r:         std_logic;

begin

  clk_o <= clk_r;

  -- DCM instantiation for the system clock.
  --! Input 12. MHz, output 44.2368 MHz
  DCM_Sys:   DCM
    generic map (
      CLKFX_DIVIDE          => 1,                     -- Can be any integer from 1 to 32
      CLKFX_MULTIPLY        => 4)                     -- Can be any integer from 1 to 32
    port map (
      CLK0                  => clk_s,                 -- 0 degree DCM CLK ouptput
      CLKFX                 => clk_r,                 -- DCM CLK synthesis out (M/D)
      CLKFB                 => clk_s,                 -- DCM clock feedback
      CLKIN                 => clk_i,              	  -- Clock input (from IBUFG, BUFG or DCM)
      RST                   => '0');                  -- DCM asynchronous reset input

  --! Generates a synchronous reset signal
  rst_o_gen : process (clk_r, Reset)            
  begin
    if(rising_edge(clk_r) ) then
      if (Reset = '1') then
        rst_o <= '1';
        rst_r <= '1';
	  	else
      	rst_r <= '0';               
      	rst_o <= rst_r;
      end if;
    end if;
  end process rst_o_gen;


end RTL;
