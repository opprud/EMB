--------------------------------------------------------------------------------
--! @file
--! @ingroup	RTL
--! @brief		Definitions package
--! @details    Defines constants and types for use in the project
--! @author		Morten Opprud Jakobsen \n
--!				AU-HIH \n                                            
--!				morten@hih.au.dk \n 
--! @version  	1.0
--! @version  	see svn log for details
--------------------------------------------------------------------------------
library IEEE;
  use IEEE.std_logic_1164.all;

package def_pkg is

  --! Common type declarations in the project
  subtype Word      is    std_logic_vector (15 downto 0);
  subtype Byte      is    std_logic_vector (7 downto 0);
  subtype Int8      is    integer range 0 to 255;  -- 8 bit integer
  subtype Int10     is    integer range 0 to 1023; -- 10 bit integer

--! Project Constants
--! FPGA revision: x.yy.z
  constant Revision_c:        Word        := X"03E8";	--rev 1.00.0  
   
--! Wishbone Constants and types
  constant Num_s:         integer := 2;  --! Number of slaves  connected to the Intercon
  constant Num_m:         integer := 1;  --! Number of masters connected to the Intercon
  constant Datawidth:     integer := 16; --! Number of bits in datainterface
  constant AddrRange:     integer := 10; --! Number of bits in Address space (was 16)
  constant LAddrRange:    integer := 8;  --! Number of bits in Low Address space, which connects to slaves
  constant HAddrRange:    integer := 2;  --! Number of bits in High Address space, used for decoding in Intercon
  constant Num_bytes:     integer := 2;  --! Number of databytes
  constant cti_width:     integer := 3;  --! Number of bits in cti vector (always 3)

  --! Type definition for sizing vectors
  subtype wb_adr_typ is   std_logic_vector (AddrRange-1 downto 0);  --! Full Address range
  subtype wb_lad_typ is   std_logic_vector (LAddrRange-1 downto 0); --! Address range for slave connections
  subtype wb_had_typ is   std_logic_vector (HAddrRange-1 downto 0); --! Address range for decoding in Intercon
  subtype wb_sel_typ is   std_logic_vector (Num_bytes-1 downto 0);	--! 
  subtype wb_dat_typ is   std_logic_vector (Datawidth-1 downto 0);	--! WB datawidth is 16
  subtype wb_cti_typ is   std_logic_vector (cti_width-1 downto 0);	--!

  --! Slave interface type definitions  - arrays of vectors from above
  subtype wb_s_size is    std_logic_vector (Num_s downto 0);
  type wb_s_dat_typ is    array (natural range <>) of wb_dat_typ;
  constant cti_classic:   wb_cti_typ := "000";

--! High Address constants
  constant BA_WBO:  wb_had_typ    := "01";  --! 0x?00 Base addresses for Test Output port
  constant BA_WBI:  wb_had_typ    := "00";  --! 0x?00 Base addresses for Tst input port

--! Low Address constants (Memory map) 
  constant A_WBO_REG1:       	wb_lad_typ := "00000000"; -- 0x0
  constant A_WBO_REG2:          wb_lad_typ := "00000001"; -- 0x2

end def_pkg; 