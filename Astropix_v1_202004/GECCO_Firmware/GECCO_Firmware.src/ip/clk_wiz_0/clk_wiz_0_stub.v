// Copyright 1986-2020 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2020.1 (win64) Build 2902540 Wed May 27 19:54:49 MDT 2020
// Date        : Thu Aug 20 16:35:05 2020
// Host        : DESKTOP-07NDF46 running 64-bit major release  (build 9200)
// Command     : write_verilog -force -mode synth_stub
//               c:/Users/leysr/git/adl/richard-chaos-club/2020/Xilinx-LVDS/atlaspix3_loopback_richardivan/GECCO_Firmware/GECCO_Firmware.src/ip/clk_wiz_0/clk_wiz_0_stub.v
// Design      : clk_wiz_0
// Purpose     : Stub declaration of top-level module interface
// Device      : xc7a200tsbg484-1
// --------------------------------------------------------------------------------

// This empty module with port declaration file causes synthesis tools to infer a black box for IP.
// The synthesis directives are for Synopsys Synplify support to prevent IO buffer insertion.
// Please paste the declaration into a Verilog source file or add the file as an additional source.
module clk_wiz_0(clk_out600p90, clk_out600, clk_out150, 
  clk_out200, clk_out_66slow, reset, locked, clk_in1)
/* synthesis syn_black_box black_box_pad_pin="clk_out600p90,clk_out600,clk_out150,clk_out200,clk_out_66slow,reset,locked,clk_in1" */;
  output clk_out600p90;
  output clk_out600;
  output clk_out150;
  output clk_out200;
  output clk_out_66slow;
  input reset;
  output locked;
  input clk_in1;
endmodule
