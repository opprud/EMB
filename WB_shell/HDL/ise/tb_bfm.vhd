--------------------------------------------------------------------------------
-- Company: 
-- Engineer:
--
-- Create Date:   10:51:23 04/05/2011
-- Design Name:   
-- Module Name:   C:/Workspace/EMB/WB_shell/HDL/ise/tb_bfm.vhd
-- Project Name:  WB_shell
-- Target Device:  
-- Tool versions:  
-- Description:   
-- 
-- VHDL Test Bench Created by ISE for module: Wrapper
-- 
-- Dependencies:
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
--
-- Notes: 
-- This testbench has been automatically generated using types std_logic and
-- std_logic_vector for the ports of the unit under test.  Xilinx recommends
-- that these types always be used for the top-level I/O of a design in order
-- to guarantee that the testbench will bind correctly to the post-implementation 
-- simulation model.
--------------------------------------------------------------------------------
LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
 
-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--USE ieee.numeric_std.ALL;
 
ENTITY tb_bfm IS
END tb_bfm;
 
ARCHITECTURE behavior OF tb_bfm IS 
 
    -- Component Declaration for the Unit Under Test (UUT)
 
    COMPONENT Wrapper
    PORT(
         Clock : IN  std_logic;
         Rst_i : IN  std_logic;
         nCpuCs_i : IN  std_logic;
         nCpuRd_i : IN  std_logic;
         nCpuWr_i : IN  std_logic;
         CpuA_i : IN  std_logic_vector(9 downto 0);
         CpuD : INOUT  std_logic_vector(15 downto 0);
         Port_o : OUT  std_logic_vector(3 downto 0);
         Dbus_En : OUT  std_logic;
         Abus_En : OUT  std_logic
        );
    END COMPONENT;
    

   --Inputs
   signal Clock : std_logic := '0';
   signal Rst_i : std_logic := '0';
   signal nCpuCs_i : std_logic := '0';
   signal nCpuRd_i : std_logic := '0';
   signal nCpuWr_i : std_logic := '0';
   signal CpuA_i : std_logic_vector(9 downto 0) := (others => '0');

	--BiDirs
   signal CpuD : std_logic_vector(15 downto 0);

 	--Outputs
   signal Port_o : std_logic_vector(3 downto 0);
   signal Dbus_En : std_logic;
   signal Abus_En : std_logic;

   -- Clock period definitions
   constant Clock_period : time := 10 ns;
 
BEGIN
 
	-- Instantiate the Unit Under Test (UUT)
   uut: Wrapper PORT MAP (
          Clock => Clock,
          Rst_i => Rst_i,
          nCpuCs_i => nCpuCs_i,
          nCpuRd_i => nCpuRd_i,
          nCpuWr_i => nCpuWr_i,
          CpuA_i => CpuA_i,
          CpuD => CpuD,
          Port_o => Port_o,
          Dbus_En => Dbus_En,
          Abus_En => Abus_En
        );

   -- Clock process definitions
   Clock_process :process
   begin
		Clock <= '0';
		wait for Clock_period/2;
		Clock <= '1';
		wait for Clock_period/2;
   end process;
 

   -- Stimulus process
   stim_proc: process
   begin		
      -- hold reset state for 100 ns.
      wait for 100 ns;	

      wait for Clock_period*10;

      -- insert stimulus here 

      wait;
   end process;

END;
