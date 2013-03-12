-- ARM7 Bus functional model
--
-- Author:  Morten@HIH.AU.DK
-- Version: 0.1
-- Date:    04.04.2011
--
-- This code is free software; you can redistribute it and/or
-- modify it under the terms of the GNU Lesser General Public
-- License as published by the Free Software Foundation; either
-- version 2.1 of the License, or (at your option) any later version.
--
-- This code is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
-- Lesser General Public License for more details.
--
-- You should have received a copy of the GNU Lesser General Public
-- License along with this library; if not, write to the
-- Free Software  Foundation, Inc., 59 Temple Place, Suite 330,
-- Boston, MA  02111-1307  USA
--

LIBRARY IEEE;
USE IEEE.std_logic_1164.all;

use std.textio.all;
use work.arm_emc_package.all;
--use work.def_p.all;
use work.txt_util.all;

entity tb_bfm is
    generic (
                stim_file    : string := "../sim/stim/arm7_bfm_stim.dat";     -- Stimulus input file
                log_file     : string := "../sim/log/arm7_bfm_log.txt"        -- Log file

            );
end tb_bfm;

architecture tb of tb_bfm is
    file stimulus : TEXT open read_mode is stim_file;           -- Open stimulus file for read
    file log      : TEXT open write_mode is log_file;           -- Open log file for write

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

 	--Outputs
   signal Port_o : std_logic_vector(3 downto 0);
   signal Dbus_En : std_logic;
   signal Abus_En : std_logic;

   -- Clock period definitions
   constant Clock_period : time := 10 ns;
 
   -- ARM bus IF
   signal arm_bus_if               : arm_emc_t;
   
 
BEGIN		
 
	-- Instantiate the Unit Under Test (UUT)
   uut: Wrapper PORT MAP (
          Clock => Clock,
          Rst_i => Rst_i,
          nCpuCs_i => arm_bus_if.nCpuCs_i,
          nCpuRd_i => arm_bus_if.nCpuRd_i,
          nCpuWr_i => arm_bus_if.nCpuWr_i,
          CpuA_i   => arm_bus_if.CpuA_i(9 downto 0),
          CpuD     => arm_bus_if.CpuD,
          Port_o   => Port_o,
          Dbus_En  => Dbus_En,
          Abus_En  => Abus_En
        );

   -- Clock process definitions
   Clock_process :process
   begin
		Clock <= '0';
		wait for Clock_period/2;
		Clock <= '1';
		wait for Clock_period/2;
   end process;

    -- Main transaction process
    TRANPROC: process
        variable s          : string(1 to 100);
        variable address    : std_logic_vector(15 downto 0);
        variable data       : std_logic_vector(15 downto 0);
        variable data2      : std_logic_vector(15 downto 0);
    begin

    -- Default values
		Rst_i  <=  '1';
		arm_bus_if.nCpuCs_i  <=  '1';
		arm_bus_if.nCpuRd_i  <=  '1';
		arm_bus_if.nCpuWr_i  <=  '1';

    -- hold reset
    wait for Clock_period*20;
		Rst_i  <=  '0';
				
    -- Get commands from stimulus file
    while not endfile(stimulus) loop
      str_read(stimulus, s);                                  -- Read line into string

      if (s(1 to 5) = "#WAIT") then                        	  -- Wait n cycles
           wait for integer'value(s(7 to 12))*cycle;
      elsif (s(1 to 3) = "#RD") then                          -- Read from UART and compare
          address := to_std_logic_vector(s(5 to 20));
					arm_16bit_read (Clock, arm_bus_if, address, log);
      elsif (s(1 to 3) = "#WR") then                          -- Write to LPC
          address := to_std_logic_vector(s(5 to 20));
          data := to_std_logic_vector(s(22 to 37));
          arm_16bit_write (Clock, arm_bus_if, address, data, log);
          --uart_write (clk, uart_if_in, uart_if_out, address, data, log);
      elsif (s(1 to 4) = "#LOG") then                         -- Write message to log
          print (log, time'image(now) & ": " & s(6 to 80));
      elsif (s(1 to 4) = "#END") then -- Sim end
          print (log, "*** Simulation END ***");
          assert FALSE report "*** Simulation END ***" severity failure;
      else
          print ("log, Unknown command: " & s);
      end if;
    end loop;

    wait;
  end process;

end tb;

