// Copyright 1986-2020 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2020.1.1 (win64) Build 2960000 Wed Aug  5 22:57:20 MDT 2020
// Date        : Sat Dec 25 22:30:31 2021
// Host        : DESKTOP-UNSGLV7 running 64-bit major release  (build 9200)
// Command     : write_verilog -force -mode synth_stub
//               c:/Users/nicolas/Documents/GitHub/astropix_github/FW/AstroPix2/GECCO_Firmware.src/ip/clk_wiz_0/clk_wiz_0_stub.v
// Design      : clk_wiz_0
// Purpose     : Stub declaration of top-level module interface
// Device      : xc7a200tsbg484-1
// --------------------------------------------------------------------------------

// This empty module with port declaration file causes synthesis tools to infer a black box for IP.
// The synthesis directives are for Synopsys Synplify support to prevent IO buffer insertion.
// Please paste the declaration into a Verilog source file or add the file as an additional source.
module clk_wiz_0(clk_out_sampleclk, clk_out_timestamp, reset, 
  locked, clk_in1)
/* synthesis syn_black_box black_box_pad_pin="clk_out_sampleclk,clk_out_timestamp,reset,locked,clk_in1" */;
  output clk_out_sampleclk;
  output clk_out_timestamp;
  input reset;
  output locked;
  input clk_in1;
endmodule
