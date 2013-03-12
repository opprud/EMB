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
USE std.textio.all;
USE work.txt_util.all;

-- package declaration similar to "entity"
package arm_emc_package is
  -- cycle @ 16Mhz
  constant CYCLE  	: time := 62.5 ns;

	-- EMC timing, adjust to fit electricaldatasheet
	-- Read timing
	constant WST_RD  		: integer := 3;
	constant Tr_CSLAV  		: time := 5 ns;
	constant Tr_CSLOEL  	: time := 5 ns;
	constant Tr_OELOEH  	: time := ((WST_RD*CYCLE)+30ns);
	constant Tr_OEHANV  	: time := 5 ns;
	constant Tr_CSHOEH  	: time := 5 ns;
	
	-- Write timing
	constant WST_WR  		: integer := 3;
	constant Tw_CSLAV  		: time := 30 ns;
	constant Tw_CSLWEL  	: time := 30 ns;
	constant Tw_CSLDW  		: time := 30 ns;
	constant Tw_WELWEH  	: time := ((WST_RD*CYCLE)+30ns);
	constant Tw_WEHANV  	: time := 30 ns;
	constant Tw_WEHDNV  	: time := 30 ns;

  -- ARM EMC interface
  type arm_emc_t is record
      nCpuCs_i	: std_logic;
      nCpuRd_i	: std_logic;
      nCpuWr_i	: std_logic;
      CpuA_i  	: std_logic_vector(15 downto 0);
      CpuD    	: std_logic_vector(15 downto 0);
  end record;

  -- Write EMC
  procedure arm_16bit_write (	signal clk: in std_logic;
                        		signal uio: out arm_emc_t;
                        		addr      : in std_logic_vector (15 downto 0);
                        		data      : in std_logic_vector (15 downto 0);
                        		file log  : TEXT
                       	   );

  -- Read EMC
  procedure arm_16bit_read (	signal clk: in std_logic;
                        		signal uio: out arm_emc_t;
                        		addr      : in std_logic_vector (15 downto 0);
                        		--data      : in std_logic_vector (15 downto 0);
                        		file log  : TEXT
                       	   );


  -- Compare two std_logic_vectors (handles don't-care)
  function compare (d1 : std_logic_vector; d2 : std_logic_vector) return boolean;

end arm_emc_package;

-- implementation of functional units
package body arm_emc_package is

    -- Write EMC
    procedure arm_16bit_write (	signal clk: in std_logic;
                          		signal uio: out arm_emc_t;
                          		addr      : in std_logic_vector (15 downto 0);
                          		data      : in std_logic_vector (15 downto 0);
                          		file log  : TEXT
                         	   ) is

    begin
        --print (log, "EMC write: 0x" & hstr(addr) & " : 0x" & hstr(data));
		    -- start cycle;
        -- assert CS
        uio.nCpuCs_i <= '0'; 
        wait for Tw_CSLAV;
        -- setup addr
        uio.CpuA_i <= addr;
        wait for (Tw_CSLAV-Tw_CSLDW);
        -- setup data
        uio.CpuD <= data;
        -- wait for WE to be set
        wait for (Tw_CSLWEL-Tw_CSLAV);
        uio.nCpuWr_i <= '0';
        -- write happens
        wait for (Tw_WELWEH);
        wait for Tw_WEHDNV;
        --invalidate data and addr
        uio.CpuA_i <= (others => 'U');
        uio.CpuD  <= (others => 'U');
		    -- deassert cs and WE
        uio.nCpuCs_i <= '1';
        uio.nCpuWr_i <= '1';

    end arm_16bit_write;

    -- Read EMC
    procedure arm_16bit_read (	signal clk: in std_logic;
                          		signal uio: out arm_emc_t;
                          		addr      : in std_logic_vector (15 downto 0);
                          		--data      : out std_logic_vector (15 downto 0);
                          		file log  : TEXT
                         	   ) is
      variable data : std_logic_vector(15 downto 0);
    begin
        --  print (log, "EMC read: 0x" & hstr(addr) & " : 0x" & hstr(data));
		    -- start cycle;
        -- assert CS
        uio.nCpuCs_i <= '0'; 
        wait for Tr_CSLOEL;
        -- assert RD (OE)
        uio.nCpuRd_i <= '0';
        wait for (Tr_CSLAV-Tr_CSLOEL);
        -- setup address
        uio.CpuA_i <= addr;
        wait for (Tr_OELOEH-Tr_CSHOEH);
        --sample data
        --data  <= uio.CpuD;
        --print (log, "EMC read: 0x" & hstr(addr) & " : 0x" & hstr(uio.CpuD));
        --deassert CS
        uio.nCpuCs_i <= '1';
        wait for Tr_CSHOEH;
        --deassert RD (OE)
        uio.nCpuRd_i <= '1';
        wait for Tr_OEHANV;
        -- invalidate Address
        uio.CpuA_i <= (others => 'U');

    end arm_16bit_read;

    -- Compare two std_logig_vectors (handles don't-care)
    function compare (d1 : std_logic_vector; d2 : std_logic_vector) return boolean is
        variable i : natural;
    begin
        for i in d1'range loop
            if (not (d1(i)='-' or d2(i)='-')) then
                if (d1(i)/=d2(i)) then
                    return false;
                end if;
            end if;
        end loop;
        return true;
    end compare;

end arm_emc_package;

