// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2022.1 (lin64) Build 3526262 Mon Apr 18 15:47:01 MDT 2022
// Date        : Wed May 24 11:00:13 2023
// Host        : nstriebig-desktop running 64-bit Ubuntu 22.04.2 LTS
// Command     : write_verilog -force -mode synth_stub
//               /home/nstriebig/Documents/git/astropix/FW/AstroPix2_telescope/GECCO_Firmware.src/ip/clk_wiz_0/clk_wiz_0_stub.v
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
