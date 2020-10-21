-- Copyright 1986-2020 Xilinx, Inc. All Rights Reserved.
-- --------------------------------------------------------------------------------
-- Tool Version: Vivado v.2020.1 (win64) Build 2902540 Wed May 27 19:54:49 MDT 2020
-- Date        : Thu Aug 20 16:35:05 2020
-- Host        : DESKTOP-07NDF46 running 64-bit major release  (build 9200)
-- Command     : write_vhdl -force -mode synth_stub
--               c:/Users/leysr/git/adl/richard-chaos-club/2020/Xilinx-LVDS/atlaspix3_loopback_richardivan/GECCO_Firmware/GECCO_Firmware.src/ip/clk_wiz_0/clk_wiz_0_stub.vhdl
-- Design      : clk_wiz_0
-- Purpose     : Stub declaration of top-level module interface
-- Device      : xc7a200tsbg484-1
-- --------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity clk_wiz_0 is
  Port ( 
    clk_out600p90 : out STD_LOGIC;
    clk_out600 : out STD_LOGIC;
    clk_out150 : out STD_LOGIC;
    clk_out200 : out STD_LOGIC;
    clk_out_66slow : out STD_LOGIC;
    reset : in STD_LOGIC;
    locked : out STD_LOGIC;
    clk_in1 : in STD_LOGIC
  );

end clk_wiz_0;

architecture stub of clk_wiz_0 is
attribute syn_black_box : boolean;
attribute black_box_pad_pin : string;
attribute syn_black_box of stub : architecture is true;
attribute black_box_pad_pin of stub : architecture is "clk_out600p90,clk_out600,clk_out150,clk_out200,clk_out_66slow,reset,locked,clk_in1";
begin
end;
