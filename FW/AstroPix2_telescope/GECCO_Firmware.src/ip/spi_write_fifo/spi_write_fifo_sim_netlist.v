// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2022.1 (lin64) Build 3526262 Mon Apr 18 15:47:01 MDT 2022
// Date        : Wed May 24 11:00:18 2023
// Host        : nstriebig-desktop running 64-bit Ubuntu 22.04.2 LTS
// Command     : write_verilog -force -mode funcsim
//               /home/nstriebig/Documents/git/astropix/FW/AstroPix2_telescope/GECCO_Firmware.src/ip/spi_write_fifo/spi_write_fifo_sim_netlist.v
// Design      : spi_write_fifo
// Purpose     : This verilog netlist is a functional simulation representation of the design and should not be modified
//               or synthesized. This netlist cannot be used for SDF annotated simulation.
// Device      : xc7a200tsbg484-1
// --------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

(* CHECK_LICENSE_TYPE = "spi_write_fifo,fifo_generator_v13_2_7,{}" *) (* downgradeipidentifiedwarnings = "yes" *) (* x_core_info = "fifo_generator_v13_2_7,Vivado 2022.1" *) 
(* NotValidForBitStream *)
module spi_write_fifo
   (rst,
    wr_clk,
    rd_clk,
    din,
    wr_en,
    rd_en,
    dout,
    full,
    empty,
    prog_full,
    prog_empty);
  input rst;
  (* x_interface_info = "xilinx.com:signal:clock:1.0 write_clk CLK" *) (* x_interface_parameter = "XIL_INTERFACENAME write_clk, FREQ_HZ 100000000, FREQ_TOLERANCE_HZ 0, PHASE 0.0, INSERT_VIP 0" *) input wr_clk;
  (* x_interface_info = "xilinx.com:signal:clock:1.0 read_clk CLK" *) (* x_interface_parameter = "XIL_INTERFACENAME read_clk, FREQ_HZ 100000000, FREQ_TOLERANCE_HZ 0, PHASE 0.0, INSERT_VIP 0" *) input rd_clk;
  (* x_interface_info = "xilinx.com:interface:fifo_write:1.0 FIFO_WRITE WR_DATA" *) input [7:0]din;
  (* x_interface_info = "xilinx.com:interface:fifo_write:1.0 FIFO_WRITE WR_EN" *) input wr_en;
  (* x_interface_info = "xilinx.com:interface:fifo_read:1.0 FIFO_READ RD_EN" *) input rd_en;
  (* x_interface_info = "xilinx.com:interface:fifo_read:1.0 FIFO_READ RD_DATA" *) output [7:0]dout;
  (* x_interface_info = "xilinx.com:interface:fifo_write:1.0 FIFO_WRITE FULL" *) output full;
  (* x_interface_info = "xilinx.com:interface:fifo_read:1.0 FIFO_READ EMPTY" *) output empty;
  output prog_full;
  output prog_empty;

  wire [7:0]din;
  wire [7:0]dout;
  wire empty;
  wire full;
  wire prog_empty;
  wire prog_full;
  wire rd_clk;
  wire rd_en;
  wire rst;
  wire wr_clk;
  wire wr_en;
  wire NLW_U0_almost_empty_UNCONNECTED;
  wire NLW_U0_almost_full_UNCONNECTED;
  wire NLW_U0_axi_ar_dbiterr_UNCONNECTED;
  wire NLW_U0_axi_ar_overflow_UNCONNECTED;
  wire NLW_U0_axi_ar_prog_empty_UNCONNECTED;
  wire NLW_U0_axi_ar_prog_full_UNCONNECTED;
  wire NLW_U0_axi_ar_sbiterr_UNCONNECTED;
  wire NLW_U0_axi_ar_underflow_UNCONNECTED;
  wire NLW_U0_axi_aw_dbiterr_UNCONNECTED;
  wire NLW_U0_axi_aw_overflow_UNCONNECTED;
  wire NLW_U0_axi_aw_prog_empty_UNCONNECTED;
  wire NLW_U0_axi_aw_prog_full_UNCONNECTED;
  wire NLW_U0_axi_aw_sbiterr_UNCONNECTED;
  wire NLW_U0_axi_aw_underflow_UNCONNECTED;
  wire NLW_U0_axi_b_dbiterr_UNCONNECTED;
  wire NLW_U0_axi_b_overflow_UNCONNECTED;
  wire NLW_U0_axi_b_prog_empty_UNCONNECTED;
  wire NLW_U0_axi_b_prog_full_UNCONNECTED;
  wire NLW_U0_axi_b_sbiterr_UNCONNECTED;
  wire NLW_U0_axi_b_underflow_UNCONNECTED;
  wire NLW_U0_axi_r_dbiterr_UNCONNECTED;
  wire NLW_U0_axi_r_overflow_UNCONNECTED;
  wire NLW_U0_axi_r_prog_empty_UNCONNECTED;
  wire NLW_U0_axi_r_prog_full_UNCONNECTED;
  wire NLW_U0_axi_r_sbiterr_UNCONNECTED;
  wire NLW_U0_axi_r_underflow_UNCONNECTED;
  wire NLW_U0_axi_w_dbiterr_UNCONNECTED;
  wire NLW_U0_axi_w_overflow_UNCONNECTED;
  wire NLW_U0_axi_w_prog_empty_UNCONNECTED;
  wire NLW_U0_axi_w_prog_full_UNCONNECTED;
  wire NLW_U0_axi_w_sbiterr_UNCONNECTED;
  wire NLW_U0_axi_w_underflow_UNCONNECTED;
  wire NLW_U0_axis_dbiterr_UNCONNECTED;
  wire NLW_U0_axis_overflow_UNCONNECTED;
  wire NLW_U0_axis_prog_empty_UNCONNECTED;
  wire NLW_U0_axis_prog_full_UNCONNECTED;
  wire NLW_U0_axis_sbiterr_UNCONNECTED;
  wire NLW_U0_axis_underflow_UNCONNECTED;
  wire NLW_U0_dbiterr_UNCONNECTED;
  wire NLW_U0_m_axi_arvalid_UNCONNECTED;
  wire NLW_U0_m_axi_awvalid_UNCONNECTED;
  wire NLW_U0_m_axi_bready_UNCONNECTED;
  wire NLW_U0_m_axi_rready_UNCONNECTED;
  wire NLW_U0_m_axi_wlast_UNCONNECTED;
  wire NLW_U0_m_axi_wvalid_UNCONNECTED;
  wire NLW_U0_m_axis_tlast_UNCONNECTED;
  wire NLW_U0_m_axis_tvalid_UNCONNECTED;
  wire NLW_U0_overflow_UNCONNECTED;
  wire NLW_U0_rd_rst_busy_UNCONNECTED;
  wire NLW_U0_s_axi_arready_UNCONNECTED;
  wire NLW_U0_s_axi_awready_UNCONNECTED;
  wire NLW_U0_s_axi_bvalid_UNCONNECTED;
  wire NLW_U0_s_axi_rlast_UNCONNECTED;
  wire NLW_U0_s_axi_rvalid_UNCONNECTED;
  wire NLW_U0_s_axi_wready_UNCONNECTED;
  wire NLW_U0_s_axis_tready_UNCONNECTED;
  wire NLW_U0_sbiterr_UNCONNECTED;
  wire NLW_U0_underflow_UNCONNECTED;
  wire NLW_U0_valid_UNCONNECTED;
  wire NLW_U0_wr_ack_UNCONNECTED;
  wire NLW_U0_wr_rst_busy_UNCONNECTED;
  wire [4:0]NLW_U0_axi_ar_data_count_UNCONNECTED;
  wire [4:0]NLW_U0_axi_ar_rd_data_count_UNCONNECTED;
  wire [4:0]NLW_U0_axi_ar_wr_data_count_UNCONNECTED;
  wire [4:0]NLW_U0_axi_aw_data_count_UNCONNECTED;
  wire [4:0]NLW_U0_axi_aw_rd_data_count_UNCONNECTED;
  wire [4:0]NLW_U0_axi_aw_wr_data_count_UNCONNECTED;
  wire [4:0]NLW_U0_axi_b_data_count_UNCONNECTED;
  wire [4:0]NLW_U0_axi_b_rd_data_count_UNCONNECTED;
  wire [4:0]NLW_U0_axi_b_wr_data_count_UNCONNECTED;
  wire [10:0]NLW_U0_axi_r_data_count_UNCONNECTED;
  wire [10:0]NLW_U0_axi_r_rd_data_count_UNCONNECTED;
  wire [10:0]NLW_U0_axi_r_wr_data_count_UNCONNECTED;
  wire [10:0]NLW_U0_axi_w_data_count_UNCONNECTED;
  wire [10:0]NLW_U0_axi_w_rd_data_count_UNCONNECTED;
  wire [10:0]NLW_U0_axi_w_wr_data_count_UNCONNECTED;
  wire [10:0]NLW_U0_axis_data_count_UNCONNECTED;
  wire [10:0]NLW_U0_axis_rd_data_count_UNCONNECTED;
  wire [10:0]NLW_U0_axis_wr_data_count_UNCONNECTED;
  wire [9:0]NLW_U0_data_count_UNCONNECTED;
  wire [31:0]NLW_U0_m_axi_araddr_UNCONNECTED;
  wire [1:0]NLW_U0_m_axi_arburst_UNCONNECTED;
  wire [3:0]NLW_U0_m_axi_arcache_UNCONNECTED;
  wire [0:0]NLW_U0_m_axi_arid_UNCONNECTED;
  wire [7:0]NLW_U0_m_axi_arlen_UNCONNECTED;
  wire [0:0]NLW_U0_m_axi_arlock_UNCONNECTED;
  wire [2:0]NLW_U0_m_axi_arprot_UNCONNECTED;
  wire [3:0]NLW_U0_m_axi_arqos_UNCONNECTED;
  wire [3:0]NLW_U0_m_axi_arregion_UNCONNECTED;
  wire [2:0]NLW_U0_m_axi_arsize_UNCONNECTED;
  wire [0:0]NLW_U0_m_axi_aruser_UNCONNECTED;
  wire [31:0]NLW_U0_m_axi_awaddr_UNCONNECTED;
  wire [1:0]NLW_U0_m_axi_awburst_UNCONNECTED;
  wire [3:0]NLW_U0_m_axi_awcache_UNCONNECTED;
  wire [0:0]NLW_U0_m_axi_awid_UNCONNECTED;
  wire [7:0]NLW_U0_m_axi_awlen_UNCONNECTED;
  wire [0:0]NLW_U0_m_axi_awlock_UNCONNECTED;
  wire [2:0]NLW_U0_m_axi_awprot_UNCONNECTED;
  wire [3:0]NLW_U0_m_axi_awqos_UNCONNECTED;
  wire [3:0]NLW_U0_m_axi_awregion_UNCONNECTED;
  wire [2:0]NLW_U0_m_axi_awsize_UNCONNECTED;
  wire [0:0]NLW_U0_m_axi_awuser_UNCONNECTED;
  wire [63:0]NLW_U0_m_axi_wdata_UNCONNECTED;
  wire [0:0]NLW_U0_m_axi_wid_UNCONNECTED;
  wire [7:0]NLW_U0_m_axi_wstrb_UNCONNECTED;
  wire [0:0]NLW_U0_m_axi_wuser_UNCONNECTED;
  wire [7:0]NLW_U0_m_axis_tdata_UNCONNECTED;
  wire [0:0]NLW_U0_m_axis_tdest_UNCONNECTED;
  wire [0:0]NLW_U0_m_axis_tid_UNCONNECTED;
  wire [0:0]NLW_U0_m_axis_tkeep_UNCONNECTED;
  wire [0:0]NLW_U0_m_axis_tstrb_UNCONNECTED;
  wire [3:0]NLW_U0_m_axis_tuser_UNCONNECTED;
  wire [9:0]NLW_U0_rd_data_count_UNCONNECTED;
  wire [0:0]NLW_U0_s_axi_bid_UNCONNECTED;
  wire [1:0]NLW_U0_s_axi_bresp_UNCONNECTED;
  wire [0:0]NLW_U0_s_axi_buser_UNCONNECTED;
  wire [63:0]NLW_U0_s_axi_rdata_UNCONNECTED;
  wire [0:0]NLW_U0_s_axi_rid_UNCONNECTED;
  wire [1:0]NLW_U0_s_axi_rresp_UNCONNECTED;
  wire [0:0]NLW_U0_s_axi_ruser_UNCONNECTED;
  wire [9:0]NLW_U0_wr_data_count_UNCONNECTED;

  (* C_ADD_NGC_CONSTRAINT = "0" *) 
  (* C_APPLICATION_TYPE_AXIS = "0" *) 
  (* C_APPLICATION_TYPE_RACH = "0" *) 
  (* C_APPLICATION_TYPE_RDCH = "0" *) 
  (* C_APPLICATION_TYPE_WACH = "0" *) 
  (* C_APPLICATION_TYPE_WDCH = "0" *) 
  (* C_APPLICATION_TYPE_WRCH = "0" *) 
  (* C_AXIS_TDATA_WIDTH = "8" *) 
  (* C_AXIS_TDEST_WIDTH = "1" *) 
  (* C_AXIS_TID_WIDTH = "1" *) 
  (* C_AXIS_TKEEP_WIDTH = "1" *) 
  (* C_AXIS_TSTRB_WIDTH = "1" *) 
  (* C_AXIS_TUSER_WIDTH = "4" *) 
  (* C_AXIS_TYPE = "0" *) 
  (* C_AXI_ADDR_WIDTH = "32" *) 
  (* C_AXI_ARUSER_WIDTH = "1" *) 
  (* C_AXI_AWUSER_WIDTH = "1" *) 
  (* C_AXI_BUSER_WIDTH = "1" *) 
  (* C_AXI_DATA_WIDTH = "64" *) 
  (* C_AXI_ID_WIDTH = "1" *) 
  (* C_AXI_LEN_WIDTH = "8" *) 
  (* C_AXI_LOCK_WIDTH = "1" *) 
  (* C_AXI_RUSER_WIDTH = "1" *) 
  (* C_AXI_TYPE = "1" *) 
  (* C_AXI_WUSER_WIDTH = "1" *) 
  (* C_COMMON_CLOCK = "0" *) 
  (* C_COUNT_TYPE = "0" *) 
  (* C_DATA_COUNT_WIDTH = "10" *) 
  (* C_DEFAULT_VALUE = "BlankString" *) 
  (* C_DIN_WIDTH = "8" *) 
  (* C_DIN_WIDTH_AXIS = "1" *) 
  (* C_DIN_WIDTH_RACH = "32" *) 
  (* C_DIN_WIDTH_RDCH = "64" *) 
  (* C_DIN_WIDTH_WACH = "1" *) 
  (* C_DIN_WIDTH_WDCH = "64" *) 
  (* C_DIN_WIDTH_WRCH = "2" *) 
  (* C_DOUT_RST_VAL = "0" *) 
  (* C_DOUT_WIDTH = "8" *) 
  (* C_ENABLE_RLOCS = "0" *) 
  (* C_ENABLE_RST_SYNC = "1" *) 
  (* C_EN_SAFETY_CKT = "0" *) 
  (* C_ERROR_INJECTION_TYPE = "0" *) 
  (* C_ERROR_INJECTION_TYPE_AXIS = "0" *) 
  (* C_ERROR_INJECTION_TYPE_RACH = "0" *) 
  (* C_ERROR_INJECTION_TYPE_RDCH = "0" *) 
  (* C_ERROR_INJECTION_TYPE_WACH = "0" *) 
  (* C_ERROR_INJECTION_TYPE_WDCH = "0" *) 
  (* C_ERROR_INJECTION_TYPE_WRCH = "0" *) 
  (* C_FAMILY = "artix7" *) 
  (* C_FULL_FLAGS_RST_VAL = "1" *) 
  (* C_HAS_ALMOST_EMPTY = "0" *) 
  (* C_HAS_ALMOST_FULL = "0" *) 
  (* C_HAS_AXIS_TDATA = "1" *) 
  (* C_HAS_AXIS_TDEST = "0" *) 
  (* C_HAS_AXIS_TID = "0" *) 
  (* C_HAS_AXIS_TKEEP = "0" *) 
  (* C_HAS_AXIS_TLAST = "0" *) 
  (* C_HAS_AXIS_TREADY = "1" *) 
  (* C_HAS_AXIS_TSTRB = "0" *) 
  (* C_HAS_AXIS_TUSER = "1" *) 
  (* C_HAS_AXI_ARUSER = "0" *) 
  (* C_HAS_AXI_AWUSER = "0" *) 
  (* C_HAS_AXI_BUSER = "0" *) 
  (* C_HAS_AXI_ID = "0" *) 
  (* C_HAS_AXI_RD_CHANNEL = "1" *) 
  (* C_HAS_AXI_RUSER = "0" *) 
  (* C_HAS_AXI_WR_CHANNEL = "1" *) 
  (* C_HAS_AXI_WUSER = "0" *) 
  (* C_HAS_BACKUP = "0" *) 
  (* C_HAS_DATA_COUNT = "0" *) 
  (* C_HAS_DATA_COUNTS_AXIS = "0" *) 
  (* C_HAS_DATA_COUNTS_RACH = "0" *) 
  (* C_HAS_DATA_COUNTS_RDCH = "0" *) 
  (* C_HAS_DATA_COUNTS_WACH = "0" *) 
  (* C_HAS_DATA_COUNTS_WDCH = "0" *) 
  (* C_HAS_DATA_COUNTS_WRCH = "0" *) 
  (* C_HAS_INT_CLK = "0" *) 
  (* C_HAS_MASTER_CE = "0" *) 
  (* C_HAS_MEMINIT_FILE = "0" *) 
  (* C_HAS_OVERFLOW = "0" *) 
  (* C_HAS_PROG_FLAGS_AXIS = "0" *) 
  (* C_HAS_PROG_FLAGS_RACH = "0" *) 
  (* C_HAS_PROG_FLAGS_RDCH = "0" *) 
  (* C_HAS_PROG_FLAGS_WACH = "0" *) 
  (* C_HAS_PROG_FLAGS_WDCH = "0" *) 
  (* C_HAS_PROG_FLAGS_WRCH = "0" *) 
  (* C_HAS_RD_DATA_COUNT = "0" *) 
  (* C_HAS_RD_RST = "0" *) 
  (* C_HAS_RST = "1" *) 
  (* C_HAS_SLAVE_CE = "0" *) 
  (* C_HAS_SRST = "0" *) 
  (* C_HAS_UNDERFLOW = "0" *) 
  (* C_HAS_VALID = "0" *) 
  (* C_HAS_WR_ACK = "0" *) 
  (* C_HAS_WR_DATA_COUNT = "0" *) 
  (* C_HAS_WR_RST = "0" *) 
  (* C_IMPLEMENTATION_TYPE = "2" *) 
  (* C_IMPLEMENTATION_TYPE_AXIS = "1" *) 
  (* C_IMPLEMENTATION_TYPE_RACH = "1" *) 
  (* C_IMPLEMENTATION_TYPE_RDCH = "1" *) 
  (* C_IMPLEMENTATION_TYPE_WACH = "1" *) 
  (* C_IMPLEMENTATION_TYPE_WDCH = "1" *) 
  (* C_IMPLEMENTATION_TYPE_WRCH = "1" *) 
  (* C_INIT_WR_PNTR_VAL = "0" *) 
  (* C_INTERFACE_TYPE = "0" *) 
  (* C_MEMORY_TYPE = "1" *) 
  (* C_MIF_FILE_NAME = "BlankString" *) 
  (* C_MSGON_VAL = "1" *) 
  (* C_OPTIMIZATION_MODE = "0" *) 
  (* C_OVERFLOW_LOW = "0" *) 
  (* C_POWER_SAVING_MODE = "0" *) 
  (* C_PRELOAD_LATENCY = "0" *) 
  (* C_PRELOAD_REGS = "1" *) 
  (* C_PRIM_FIFO_TYPE = "1kx18" *) 
  (* C_PRIM_FIFO_TYPE_AXIS = "1kx18" *) 
  (* C_PRIM_FIFO_TYPE_RACH = "512x36" *) 
  (* C_PRIM_FIFO_TYPE_RDCH = "1kx36" *) 
  (* C_PRIM_FIFO_TYPE_WACH = "512x36" *) 
  (* C_PRIM_FIFO_TYPE_WDCH = "1kx36" *) 
  (* C_PRIM_FIFO_TYPE_WRCH = "512x36" *) 
  (* C_PROG_EMPTY_THRESH_ASSERT_VAL = "4" *) 
  (* C_PROG_EMPTY_THRESH_ASSERT_VAL_AXIS = "1022" *) 
  (* C_PROG_EMPTY_THRESH_ASSERT_VAL_RACH = "1022" *) 
  (* C_PROG_EMPTY_THRESH_ASSERT_VAL_RDCH = "1022" *) 
  (* C_PROG_EMPTY_THRESH_ASSERT_VAL_WACH = "1022" *) 
  (* C_PROG_EMPTY_THRESH_ASSERT_VAL_WDCH = "1022" *) 
  (* C_PROG_EMPTY_THRESH_ASSERT_VAL_WRCH = "1022" *) 
  (* C_PROG_EMPTY_THRESH_NEGATE_VAL = "5" *) 
  (* C_PROG_EMPTY_TYPE = "1" *) 
  (* C_PROG_EMPTY_TYPE_AXIS = "0" *) 
  (* C_PROG_EMPTY_TYPE_RACH = "0" *) 
  (* C_PROG_EMPTY_TYPE_RDCH = "0" *) 
  (* C_PROG_EMPTY_TYPE_WACH = "0" *) 
  (* C_PROG_EMPTY_TYPE_WDCH = "0" *) 
  (* C_PROG_EMPTY_TYPE_WRCH = "0" *) 
  (* C_PROG_FULL_THRESH_ASSERT_VAL = "1019" *) 
  (* C_PROG_FULL_THRESH_ASSERT_VAL_AXIS = "1023" *) 
  (* C_PROG_FULL_THRESH_ASSERT_VAL_RACH = "1023" *) 
  (* C_PROG_FULL_THRESH_ASSERT_VAL_RDCH = "1023" *) 
  (* C_PROG_FULL_THRESH_ASSERT_VAL_WACH = "1023" *) 
  (* C_PROG_FULL_THRESH_ASSERT_VAL_WDCH = "1023" *) 
  (* C_PROG_FULL_THRESH_ASSERT_VAL_WRCH = "1023" *) 
  (* C_PROG_FULL_THRESH_NEGATE_VAL = "1018" *) 
  (* C_PROG_FULL_TYPE = "1" *) 
  (* C_PROG_FULL_TYPE_AXIS = "0" *) 
  (* C_PROG_FULL_TYPE_RACH = "0" *) 
  (* C_PROG_FULL_TYPE_RDCH = "0" *) 
  (* C_PROG_FULL_TYPE_WACH = "0" *) 
  (* C_PROG_FULL_TYPE_WDCH = "0" *) 
  (* C_PROG_FULL_TYPE_WRCH = "0" *) 
  (* C_RACH_TYPE = "0" *) 
  (* C_RDCH_TYPE = "0" *) 
  (* C_RD_DATA_COUNT_WIDTH = "10" *) 
  (* C_RD_DEPTH = "1024" *) 
  (* C_RD_FREQ = "1" *) 
  (* C_RD_PNTR_WIDTH = "10" *) 
  (* C_REG_SLICE_MODE_AXIS = "0" *) 
  (* C_REG_SLICE_MODE_RACH = "0" *) 
  (* C_REG_SLICE_MODE_RDCH = "0" *) 
  (* C_REG_SLICE_MODE_WACH = "0" *) 
  (* C_REG_SLICE_MODE_WDCH = "0" *) 
  (* C_REG_SLICE_MODE_WRCH = "0" *) 
  (* C_SELECT_XPM = "0" *) 
  (* C_SYNCHRONIZER_STAGE = "2" *) 
  (* C_UNDERFLOW_LOW = "0" *) 
  (* C_USE_COMMON_OVERFLOW = "0" *) 
  (* C_USE_COMMON_UNDERFLOW = "0" *) 
  (* C_USE_DEFAULT_SETTINGS = "0" *) 
  (* C_USE_DOUT_RST = "1" *) 
  (* C_USE_ECC = "0" *) 
  (* C_USE_ECC_AXIS = "0" *) 
  (* C_USE_ECC_RACH = "0" *) 
  (* C_USE_ECC_RDCH = "0" *) 
  (* C_USE_ECC_WACH = "0" *) 
  (* C_USE_ECC_WDCH = "0" *) 
  (* C_USE_ECC_WRCH = "0" *) 
  (* C_USE_EMBEDDED_REG = "0" *) 
  (* C_USE_FIFO16_FLAGS = "0" *) 
  (* C_USE_FWFT_DATA_COUNT = "0" *) 
  (* C_USE_PIPELINE_REG = "0" *) 
  (* C_VALID_LOW = "0" *) 
  (* C_WACH_TYPE = "0" *) 
  (* C_WDCH_TYPE = "0" *) 
  (* C_WRCH_TYPE = "0" *) 
  (* C_WR_ACK_LOW = "0" *) 
  (* C_WR_DATA_COUNT_WIDTH = "10" *) 
  (* C_WR_DEPTH = "1024" *) 
  (* C_WR_DEPTH_AXIS = "1024" *) 
  (* C_WR_DEPTH_RACH = "16" *) 
  (* C_WR_DEPTH_RDCH = "1024" *) 
  (* C_WR_DEPTH_WACH = "16" *) 
  (* C_WR_DEPTH_WDCH = "1024" *) 
  (* C_WR_DEPTH_WRCH = "16" *) 
  (* C_WR_FREQ = "1" *) 
  (* C_WR_PNTR_WIDTH = "10" *) 
  (* C_WR_PNTR_WIDTH_AXIS = "10" *) 
  (* C_WR_PNTR_WIDTH_RACH = "4" *) 
  (* C_WR_PNTR_WIDTH_RDCH = "10" *) 
  (* C_WR_PNTR_WIDTH_WACH = "4" *) 
  (* C_WR_PNTR_WIDTH_WDCH = "10" *) 
  (* C_WR_PNTR_WIDTH_WRCH = "4" *) 
  (* C_WR_RESPONSE_LATENCY = "1" *) 
  (* is_du_within_envelope = "true" *) 
  spi_write_fifo_fifo_generator_v13_2_7 U0
       (.almost_empty(NLW_U0_almost_empty_UNCONNECTED),
        .almost_full(NLW_U0_almost_full_UNCONNECTED),
        .axi_ar_data_count(NLW_U0_axi_ar_data_count_UNCONNECTED[4:0]),
        .axi_ar_dbiterr(NLW_U0_axi_ar_dbiterr_UNCONNECTED),
        .axi_ar_injectdbiterr(1'b0),
        .axi_ar_injectsbiterr(1'b0),
        .axi_ar_overflow(NLW_U0_axi_ar_overflow_UNCONNECTED),
        .axi_ar_prog_empty(NLW_U0_axi_ar_prog_empty_UNCONNECTED),
        .axi_ar_prog_empty_thresh({1'b0,1'b0,1'b0,1'b0}),
        .axi_ar_prog_full(NLW_U0_axi_ar_prog_full_UNCONNECTED),
        .axi_ar_prog_full_thresh({1'b0,1'b0,1'b0,1'b0}),
        .axi_ar_rd_data_count(NLW_U0_axi_ar_rd_data_count_UNCONNECTED[4:0]),
        .axi_ar_sbiterr(NLW_U0_axi_ar_sbiterr_UNCONNECTED),
        .axi_ar_underflow(NLW_U0_axi_ar_underflow_UNCONNECTED),
        .axi_ar_wr_data_count(NLW_U0_axi_ar_wr_data_count_UNCONNECTED[4:0]),
        .axi_aw_data_count(NLW_U0_axi_aw_data_count_UNCONNECTED[4:0]),
        .axi_aw_dbiterr(NLW_U0_axi_aw_dbiterr_UNCONNECTED),
        .axi_aw_injectdbiterr(1'b0),
        .axi_aw_injectsbiterr(1'b0),
        .axi_aw_overflow(NLW_U0_axi_aw_overflow_UNCONNECTED),
        .axi_aw_prog_empty(NLW_U0_axi_aw_prog_empty_UNCONNECTED),
        .axi_aw_prog_empty_thresh({1'b0,1'b0,1'b0,1'b0}),
        .axi_aw_prog_full(NLW_U0_axi_aw_prog_full_UNCONNECTED),
        .axi_aw_prog_full_thresh({1'b0,1'b0,1'b0,1'b0}),
        .axi_aw_rd_data_count(NLW_U0_axi_aw_rd_data_count_UNCONNECTED[4:0]),
        .axi_aw_sbiterr(NLW_U0_axi_aw_sbiterr_UNCONNECTED),
        .axi_aw_underflow(NLW_U0_axi_aw_underflow_UNCONNECTED),
        .axi_aw_wr_data_count(NLW_U0_axi_aw_wr_data_count_UNCONNECTED[4:0]),
        .axi_b_data_count(NLW_U0_axi_b_data_count_UNCONNECTED[4:0]),
        .axi_b_dbiterr(NLW_U0_axi_b_dbiterr_UNCONNECTED),
        .axi_b_injectdbiterr(1'b0),
        .axi_b_injectsbiterr(1'b0),
        .axi_b_overflow(NLW_U0_axi_b_overflow_UNCONNECTED),
        .axi_b_prog_empty(NLW_U0_axi_b_prog_empty_UNCONNECTED),
        .axi_b_prog_empty_thresh({1'b0,1'b0,1'b0,1'b0}),
        .axi_b_prog_full(NLW_U0_axi_b_prog_full_UNCONNECTED),
        .axi_b_prog_full_thresh({1'b0,1'b0,1'b0,1'b0}),
        .axi_b_rd_data_count(NLW_U0_axi_b_rd_data_count_UNCONNECTED[4:0]),
        .axi_b_sbiterr(NLW_U0_axi_b_sbiterr_UNCONNECTED),
        .axi_b_underflow(NLW_U0_axi_b_underflow_UNCONNECTED),
        .axi_b_wr_data_count(NLW_U0_axi_b_wr_data_count_UNCONNECTED[4:0]),
        .axi_r_data_count(NLW_U0_axi_r_data_count_UNCONNECTED[10:0]),
        .axi_r_dbiterr(NLW_U0_axi_r_dbiterr_UNCONNECTED),
        .axi_r_injectdbiterr(1'b0),
        .axi_r_injectsbiterr(1'b0),
        .axi_r_overflow(NLW_U0_axi_r_overflow_UNCONNECTED),
        .axi_r_prog_empty(NLW_U0_axi_r_prog_empty_UNCONNECTED),
        .axi_r_prog_empty_thresh({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .axi_r_prog_full(NLW_U0_axi_r_prog_full_UNCONNECTED),
        .axi_r_prog_full_thresh({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .axi_r_rd_data_count(NLW_U0_axi_r_rd_data_count_UNCONNECTED[10:0]),
        .axi_r_sbiterr(NLW_U0_axi_r_sbiterr_UNCONNECTED),
        .axi_r_underflow(NLW_U0_axi_r_underflow_UNCONNECTED),
        .axi_r_wr_data_count(NLW_U0_axi_r_wr_data_count_UNCONNECTED[10:0]),
        .axi_w_data_count(NLW_U0_axi_w_data_count_UNCONNECTED[10:0]),
        .axi_w_dbiterr(NLW_U0_axi_w_dbiterr_UNCONNECTED),
        .axi_w_injectdbiterr(1'b0),
        .axi_w_injectsbiterr(1'b0),
        .axi_w_overflow(NLW_U0_axi_w_overflow_UNCONNECTED),
        .axi_w_prog_empty(NLW_U0_axi_w_prog_empty_UNCONNECTED),
        .axi_w_prog_empty_thresh({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .axi_w_prog_full(NLW_U0_axi_w_prog_full_UNCONNECTED),
        .axi_w_prog_full_thresh({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .axi_w_rd_data_count(NLW_U0_axi_w_rd_data_count_UNCONNECTED[10:0]),
        .axi_w_sbiterr(NLW_U0_axi_w_sbiterr_UNCONNECTED),
        .axi_w_underflow(NLW_U0_axi_w_underflow_UNCONNECTED),
        .axi_w_wr_data_count(NLW_U0_axi_w_wr_data_count_UNCONNECTED[10:0]),
        .axis_data_count(NLW_U0_axis_data_count_UNCONNECTED[10:0]),
        .axis_dbiterr(NLW_U0_axis_dbiterr_UNCONNECTED),
        .axis_injectdbiterr(1'b0),
        .axis_injectsbiterr(1'b0),
        .axis_overflow(NLW_U0_axis_overflow_UNCONNECTED),
        .axis_prog_empty(NLW_U0_axis_prog_empty_UNCONNECTED),
        .axis_prog_empty_thresh({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .axis_prog_full(NLW_U0_axis_prog_full_UNCONNECTED),
        .axis_prog_full_thresh({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .axis_rd_data_count(NLW_U0_axis_rd_data_count_UNCONNECTED[10:0]),
        .axis_sbiterr(NLW_U0_axis_sbiterr_UNCONNECTED),
        .axis_underflow(NLW_U0_axis_underflow_UNCONNECTED),
        .axis_wr_data_count(NLW_U0_axis_wr_data_count_UNCONNECTED[10:0]),
        .backup(1'b0),
        .backup_marker(1'b0),
        .clk(1'b0),
        .data_count(NLW_U0_data_count_UNCONNECTED[9:0]),
        .dbiterr(NLW_U0_dbiterr_UNCONNECTED),
        .din(din),
        .dout(dout),
        .empty(empty),
        .full(full),
        .injectdbiterr(1'b0),
        .injectsbiterr(1'b0),
        .int_clk(1'b0),
        .m_aclk(1'b0),
        .m_aclk_en(1'b0),
        .m_axi_araddr(NLW_U0_m_axi_araddr_UNCONNECTED[31:0]),
        .m_axi_arburst(NLW_U0_m_axi_arburst_UNCONNECTED[1:0]),
        .m_axi_arcache(NLW_U0_m_axi_arcache_UNCONNECTED[3:0]),
        .m_axi_arid(NLW_U0_m_axi_arid_UNCONNECTED[0]),
        .m_axi_arlen(NLW_U0_m_axi_arlen_UNCONNECTED[7:0]),
        .m_axi_arlock(NLW_U0_m_axi_arlock_UNCONNECTED[0]),
        .m_axi_arprot(NLW_U0_m_axi_arprot_UNCONNECTED[2:0]),
        .m_axi_arqos(NLW_U0_m_axi_arqos_UNCONNECTED[3:0]),
        .m_axi_arready(1'b0),
        .m_axi_arregion(NLW_U0_m_axi_arregion_UNCONNECTED[3:0]),
        .m_axi_arsize(NLW_U0_m_axi_arsize_UNCONNECTED[2:0]),
        .m_axi_aruser(NLW_U0_m_axi_aruser_UNCONNECTED[0]),
        .m_axi_arvalid(NLW_U0_m_axi_arvalid_UNCONNECTED),
        .m_axi_awaddr(NLW_U0_m_axi_awaddr_UNCONNECTED[31:0]),
        .m_axi_awburst(NLW_U0_m_axi_awburst_UNCONNECTED[1:0]),
        .m_axi_awcache(NLW_U0_m_axi_awcache_UNCONNECTED[3:0]),
        .m_axi_awid(NLW_U0_m_axi_awid_UNCONNECTED[0]),
        .m_axi_awlen(NLW_U0_m_axi_awlen_UNCONNECTED[7:0]),
        .m_axi_awlock(NLW_U0_m_axi_awlock_UNCONNECTED[0]),
        .m_axi_awprot(NLW_U0_m_axi_awprot_UNCONNECTED[2:0]),
        .m_axi_awqos(NLW_U0_m_axi_awqos_UNCONNECTED[3:0]),
        .m_axi_awready(1'b0),
        .m_axi_awregion(NLW_U0_m_axi_awregion_UNCONNECTED[3:0]),
        .m_axi_awsize(NLW_U0_m_axi_awsize_UNCONNECTED[2:0]),
        .m_axi_awuser(NLW_U0_m_axi_awuser_UNCONNECTED[0]),
        .m_axi_awvalid(NLW_U0_m_axi_awvalid_UNCONNECTED),
        .m_axi_bid(1'b0),
        .m_axi_bready(NLW_U0_m_axi_bready_UNCONNECTED),
        .m_axi_bresp({1'b0,1'b0}),
        .m_axi_buser(1'b0),
        .m_axi_bvalid(1'b0),
        .m_axi_rdata({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .m_axi_rid(1'b0),
        .m_axi_rlast(1'b0),
        .m_axi_rready(NLW_U0_m_axi_rready_UNCONNECTED),
        .m_axi_rresp({1'b0,1'b0}),
        .m_axi_ruser(1'b0),
        .m_axi_rvalid(1'b0),
        .m_axi_wdata(NLW_U0_m_axi_wdata_UNCONNECTED[63:0]),
        .m_axi_wid(NLW_U0_m_axi_wid_UNCONNECTED[0]),
        .m_axi_wlast(NLW_U0_m_axi_wlast_UNCONNECTED),
        .m_axi_wready(1'b0),
        .m_axi_wstrb(NLW_U0_m_axi_wstrb_UNCONNECTED[7:0]),
        .m_axi_wuser(NLW_U0_m_axi_wuser_UNCONNECTED[0]),
        .m_axi_wvalid(NLW_U0_m_axi_wvalid_UNCONNECTED),
        .m_axis_tdata(NLW_U0_m_axis_tdata_UNCONNECTED[7:0]),
        .m_axis_tdest(NLW_U0_m_axis_tdest_UNCONNECTED[0]),
        .m_axis_tid(NLW_U0_m_axis_tid_UNCONNECTED[0]),
        .m_axis_tkeep(NLW_U0_m_axis_tkeep_UNCONNECTED[0]),
        .m_axis_tlast(NLW_U0_m_axis_tlast_UNCONNECTED),
        .m_axis_tready(1'b0),
        .m_axis_tstrb(NLW_U0_m_axis_tstrb_UNCONNECTED[0]),
        .m_axis_tuser(NLW_U0_m_axis_tuser_UNCONNECTED[3:0]),
        .m_axis_tvalid(NLW_U0_m_axis_tvalid_UNCONNECTED),
        .overflow(NLW_U0_overflow_UNCONNECTED),
        .prog_empty(prog_empty),
        .prog_empty_thresh({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .prog_empty_thresh_assert({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .prog_empty_thresh_negate({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .prog_full(prog_full),
        .prog_full_thresh({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .prog_full_thresh_assert({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .prog_full_thresh_negate({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .rd_clk(rd_clk),
        .rd_data_count(NLW_U0_rd_data_count_UNCONNECTED[9:0]),
        .rd_en(rd_en),
        .rd_rst(1'b0),
        .rd_rst_busy(NLW_U0_rd_rst_busy_UNCONNECTED),
        .rst(rst),
        .s_aclk(1'b0),
        .s_aclk_en(1'b0),
        .s_aresetn(1'b0),
        .s_axi_araddr({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .s_axi_arburst({1'b0,1'b0}),
        .s_axi_arcache({1'b0,1'b0,1'b0,1'b0}),
        .s_axi_arid(1'b0),
        .s_axi_arlen({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .s_axi_arlock(1'b0),
        .s_axi_arprot({1'b0,1'b0,1'b0}),
        .s_axi_arqos({1'b0,1'b0,1'b0,1'b0}),
        .s_axi_arready(NLW_U0_s_axi_arready_UNCONNECTED),
        .s_axi_arregion({1'b0,1'b0,1'b0,1'b0}),
        .s_axi_arsize({1'b0,1'b0,1'b0}),
        .s_axi_aruser(1'b0),
        .s_axi_arvalid(1'b0),
        .s_axi_awaddr({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .s_axi_awburst({1'b0,1'b0}),
        .s_axi_awcache({1'b0,1'b0,1'b0,1'b0}),
        .s_axi_awid(1'b0),
        .s_axi_awlen({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .s_axi_awlock(1'b0),
        .s_axi_awprot({1'b0,1'b0,1'b0}),
        .s_axi_awqos({1'b0,1'b0,1'b0,1'b0}),
        .s_axi_awready(NLW_U0_s_axi_awready_UNCONNECTED),
        .s_axi_awregion({1'b0,1'b0,1'b0,1'b0}),
        .s_axi_awsize({1'b0,1'b0,1'b0}),
        .s_axi_awuser(1'b0),
        .s_axi_awvalid(1'b0),
        .s_axi_bid(NLW_U0_s_axi_bid_UNCONNECTED[0]),
        .s_axi_bready(1'b0),
        .s_axi_bresp(NLW_U0_s_axi_bresp_UNCONNECTED[1:0]),
        .s_axi_buser(NLW_U0_s_axi_buser_UNCONNECTED[0]),
        .s_axi_bvalid(NLW_U0_s_axi_bvalid_UNCONNECTED),
        .s_axi_rdata(NLW_U0_s_axi_rdata_UNCONNECTED[63:0]),
        .s_axi_rid(NLW_U0_s_axi_rid_UNCONNECTED[0]),
        .s_axi_rlast(NLW_U0_s_axi_rlast_UNCONNECTED),
        .s_axi_rready(1'b0),
        .s_axi_rresp(NLW_U0_s_axi_rresp_UNCONNECTED[1:0]),
        .s_axi_ruser(NLW_U0_s_axi_ruser_UNCONNECTED[0]),
        .s_axi_rvalid(NLW_U0_s_axi_rvalid_UNCONNECTED),
        .s_axi_wdata({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .s_axi_wid(1'b0),
        .s_axi_wlast(1'b0),
        .s_axi_wready(NLW_U0_s_axi_wready_UNCONNECTED),
        .s_axi_wstrb({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .s_axi_wuser(1'b0),
        .s_axi_wvalid(1'b0),
        .s_axis_tdata({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .s_axis_tdest(1'b0),
        .s_axis_tid(1'b0),
        .s_axis_tkeep(1'b0),
        .s_axis_tlast(1'b0),
        .s_axis_tready(NLW_U0_s_axis_tready_UNCONNECTED),
        .s_axis_tstrb(1'b0),
        .s_axis_tuser({1'b0,1'b0,1'b0,1'b0}),
        .s_axis_tvalid(1'b0),
        .sbiterr(NLW_U0_sbiterr_UNCONNECTED),
        .sleep(1'b0),
        .srst(1'b0),
        .underflow(NLW_U0_underflow_UNCONNECTED),
        .valid(NLW_U0_valid_UNCONNECTED),
        .wr_ack(NLW_U0_wr_ack_UNCONNECTED),
        .wr_clk(wr_clk),
        .wr_data_count(NLW_U0_wr_data_count_UNCONNECTED[9:0]),
        .wr_en(wr_en),
        .wr_rst(1'b0),
        .wr_rst_busy(NLW_U0_wr_rst_busy_UNCONNECTED));
endmodule

(* DEF_VAL = "1'b0" *) (* DEST_SYNC_FF = "2" *) (* INIT_SYNC_FF = "0" *) 
(* INV_DEF_VAL = "1'b1" *) (* ORIG_REF_NAME = "xpm_cdc_async_rst" *) (* RST_ACTIVE_HIGH = "1" *) 
(* VERSION = "0" *) (* XPM_MODULE = "TRUE" *) (* is_du_within_envelope = "true" *) 
(* keep_hierarchy = "true" *) (* xpm_cdc = "ASYNC_RST" *) 
module spi_write_fifo_xpm_cdc_async_rst
   (src_arst,
    dest_clk,
    dest_arst);
  input src_arst;
  input dest_clk;
  output dest_arst;

  (* RTL_KEEP = "true" *) (* async_reg = "true" *) (* xpm_cdc = "ASYNC_RST" *) wire [1:0]arststages_ff;
  wire dest_clk;
  wire src_arst;

  assign dest_arst = arststages_ff[1];
  (* ASYNC_REG *) 
  (* KEEP = "true" *) 
  (* XPM_CDC = "ASYNC_RST" *) 
  FDPE #(
    .INIT(1'b0)) 
    \arststages_ff_reg[0] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(1'b0),
        .PRE(src_arst),
        .Q(arststages_ff[0]));
  (* ASYNC_REG *) 
  (* KEEP = "true" *) 
  (* XPM_CDC = "ASYNC_RST" *) 
  FDPE #(
    .INIT(1'b0)) 
    \arststages_ff_reg[1] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(arststages_ff[0]),
        .PRE(src_arst),
        .Q(arststages_ff[1]));
endmodule

(* DEF_VAL = "1'b0" *) (* DEST_SYNC_FF = "2" *) (* INIT_SYNC_FF = "0" *) 
(* INV_DEF_VAL = "1'b1" *) (* ORIG_REF_NAME = "xpm_cdc_async_rst" *) (* RST_ACTIVE_HIGH = "1" *) 
(* VERSION = "0" *) (* XPM_MODULE = "TRUE" *) (* is_du_within_envelope = "true" *) 
(* keep_hierarchy = "true" *) (* xpm_cdc = "ASYNC_RST" *) 
module spi_write_fifo_xpm_cdc_async_rst__1
   (src_arst,
    dest_clk,
    dest_arst);
  input src_arst;
  input dest_clk;
  output dest_arst;

  (* RTL_KEEP = "true" *) (* async_reg = "true" *) (* xpm_cdc = "ASYNC_RST" *) wire [1:0]arststages_ff;
  wire dest_clk;
  wire src_arst;

  assign dest_arst = arststages_ff[1];
  (* ASYNC_REG *) 
  (* KEEP = "true" *) 
  (* XPM_CDC = "ASYNC_RST" *) 
  FDPE #(
    .INIT(1'b0)) 
    \arststages_ff_reg[0] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(1'b0),
        .PRE(src_arst),
        .Q(arststages_ff[0]));
  (* ASYNC_REG *) 
  (* KEEP = "true" *) 
  (* XPM_CDC = "ASYNC_RST" *) 
  FDPE #(
    .INIT(1'b0)) 
    \arststages_ff_reg[1] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(arststages_ff[0]),
        .PRE(src_arst),
        .Q(arststages_ff[1]));
endmodule

(* DEST_SYNC_FF = "2" *) (* INIT_SYNC_FF = "0" *) (* ORIG_REF_NAME = "xpm_cdc_gray" *) 
(* REG_OUTPUT = "1" *) (* SIM_ASSERT_CHK = "0" *) (* SIM_LOSSLESS_GRAY_CHK = "0" *) 
(* VERSION = "0" *) (* WIDTH = "10" *) (* XPM_MODULE = "TRUE" *) 
(* is_du_within_envelope = "true" *) (* keep_hierarchy = "true" *) (* xpm_cdc = "GRAY" *) 
module spi_write_fifo_xpm_cdc_gray
   (src_clk,
    src_in_bin,
    dest_clk,
    dest_out_bin);
  input src_clk;
  input [9:0]src_in_bin;
  input dest_clk;
  output [9:0]dest_out_bin;

  wire [9:0]async_path;
  wire [8:0]binval;
  wire dest_clk;
  (* RTL_KEEP = "true" *) (* async_reg = "true" *) (* xpm_cdc = "GRAY" *) wire [9:0]\dest_graysync_ff[0] ;
  (* RTL_KEEP = "true" *) (* async_reg = "true" *) (* xpm_cdc = "GRAY" *) wire [9:0]\dest_graysync_ff[1] ;
  wire [9:0]dest_out_bin;
  wire [8:0]gray_enc;
  wire src_clk;
  wire [9:0]src_in_bin;

  (* ASYNC_REG *) 
  (* KEEP = "true" *) 
  (* XPM_CDC = "GRAY" *) 
  FDRE \dest_graysync_ff_reg[0][0] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(async_path[0]),
        .Q(\dest_graysync_ff[0] [0]),
        .R(1'b0));
  (* ASYNC_REG *) 
  (* KEEP = "true" *) 
  (* XPM_CDC = "GRAY" *) 
  FDRE \dest_graysync_ff_reg[0][1] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(async_path[1]),
        .Q(\dest_graysync_ff[0] [1]),
        .R(1'b0));
  (* ASYNC_REG *) 
  (* KEEP = "true" *) 
  (* XPM_CDC = "GRAY" *) 
  FDRE \dest_graysync_ff_reg[0][2] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(async_path[2]),
        .Q(\dest_graysync_ff[0] [2]),
        .R(1'b0));
  (* ASYNC_REG *) 
  (* KEEP = "true" *) 
  (* XPM_CDC = "GRAY" *) 
  FDRE \dest_graysync_ff_reg[0][3] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(async_path[3]),
        .Q(\dest_graysync_ff[0] [3]),
        .R(1'b0));
  (* ASYNC_REG *) 
  (* KEEP = "true" *) 
  (* XPM_CDC = "GRAY" *) 
  FDRE \dest_graysync_ff_reg[0][4] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(async_path[4]),
        .Q(\dest_graysync_ff[0] [4]),
        .R(1'b0));
  (* ASYNC_REG *) 
  (* KEEP = "true" *) 
  (* XPM_CDC = "GRAY" *) 
  FDRE \dest_graysync_ff_reg[0][5] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(async_path[5]),
        .Q(\dest_graysync_ff[0] [5]),
        .R(1'b0));
  (* ASYNC_REG *) 
  (* KEEP = "true" *) 
  (* XPM_CDC = "GRAY" *) 
  FDRE \dest_graysync_ff_reg[0][6] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(async_path[6]),
        .Q(\dest_graysync_ff[0] [6]),
        .R(1'b0));
  (* ASYNC_REG *) 
  (* KEEP = "true" *) 
  (* XPM_CDC = "GRAY" *) 
  FDRE \dest_graysync_ff_reg[0][7] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(async_path[7]),
        .Q(\dest_graysync_ff[0] [7]),
        .R(1'b0));
  (* ASYNC_REG *) 
  (* KEEP = "true" *) 
  (* XPM_CDC = "GRAY" *) 
  FDRE \dest_graysync_ff_reg[0][8] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(async_path[8]),
        .Q(\dest_graysync_ff[0] [8]),
        .R(1'b0));
  (* ASYNC_REG *) 
  (* KEEP = "true" *) 
  (* XPM_CDC = "GRAY" *) 
  FDRE \dest_graysync_ff_reg[0][9] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(async_path[9]),
        .Q(\dest_graysync_ff[0] [9]),
        .R(1'b0));
  (* ASYNC_REG *) 
  (* KEEP = "true" *) 
  (* XPM_CDC = "GRAY" *) 
  FDRE \dest_graysync_ff_reg[1][0] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(\dest_graysync_ff[0] [0]),
        .Q(\dest_graysync_ff[1] [0]),
        .R(1'b0));
  (* ASYNC_REG *) 
  (* KEEP = "true" *) 
  (* XPM_CDC = "GRAY" *) 
  FDRE \dest_graysync_ff_reg[1][1] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(\dest_graysync_ff[0] [1]),
        .Q(\dest_graysync_ff[1] [1]),
        .R(1'b0));
  (* ASYNC_REG *) 
  (* KEEP = "true" *) 
  (* XPM_CDC = "GRAY" *) 
  FDRE \dest_graysync_ff_reg[1][2] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(\dest_graysync_ff[0] [2]),
        .Q(\dest_graysync_ff[1] [2]),
        .R(1'b0));
  (* ASYNC_REG *) 
  (* KEEP = "true" *) 
  (* XPM_CDC = "GRAY" *) 
  FDRE \dest_graysync_ff_reg[1][3] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(\dest_graysync_ff[0] [3]),
        .Q(\dest_graysync_ff[1] [3]),
        .R(1'b0));
  (* ASYNC_REG *) 
  (* KEEP = "true" *) 
  (* XPM_CDC = "GRAY" *) 
  FDRE \dest_graysync_ff_reg[1][4] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(\dest_graysync_ff[0] [4]),
        .Q(\dest_graysync_ff[1] [4]),
        .R(1'b0));
  (* ASYNC_REG *) 
  (* KEEP = "true" *) 
  (* XPM_CDC = "GRAY" *) 
  FDRE \dest_graysync_ff_reg[1][5] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(\dest_graysync_ff[0] [5]),
        .Q(\dest_graysync_ff[1] [5]),
        .R(1'b0));
  (* ASYNC_REG *) 
  (* KEEP = "true" *) 
  (* XPM_CDC = "GRAY" *) 
  FDRE \dest_graysync_ff_reg[1][6] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(\dest_graysync_ff[0] [6]),
        .Q(\dest_graysync_ff[1] [6]),
        .R(1'b0));
  (* ASYNC_REG *) 
  (* KEEP = "true" *) 
  (* XPM_CDC = "GRAY" *) 
  FDRE \dest_graysync_ff_reg[1][7] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(\dest_graysync_ff[0] [7]),
        .Q(\dest_graysync_ff[1] [7]),
        .R(1'b0));
  (* ASYNC_REG *) 
  (* KEEP = "true" *) 
  (* XPM_CDC = "GRAY" *) 
  FDRE \dest_graysync_ff_reg[1][8] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(\dest_graysync_ff[0] [8]),
        .Q(\dest_graysync_ff[1] [8]),
        .R(1'b0));
  (* ASYNC_REG *) 
  (* KEEP = "true" *) 
  (* XPM_CDC = "GRAY" *) 
  FDRE \dest_graysync_ff_reg[1][9] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(\dest_graysync_ff[0] [9]),
        .Q(\dest_graysync_ff[1] [9]),
        .R(1'b0));
  LUT5 #(
    .INIT(32'h96696996)) 
    \dest_out_bin_ff[0]_i_1 
       (.I0(\dest_graysync_ff[1] [0]),
        .I1(\dest_graysync_ff[1] [2]),
        .I2(binval[4]),
        .I3(\dest_graysync_ff[1] [3]),
        .I4(\dest_graysync_ff[1] [1]),
        .O(binval[0]));
  LUT4 #(
    .INIT(16'h6996)) 
    \dest_out_bin_ff[1]_i_1 
       (.I0(\dest_graysync_ff[1] [1]),
        .I1(\dest_graysync_ff[1] [3]),
        .I2(binval[4]),
        .I3(\dest_graysync_ff[1] [2]),
        .O(binval[1]));
  LUT3 #(
    .INIT(8'h96)) 
    \dest_out_bin_ff[2]_i_1 
       (.I0(\dest_graysync_ff[1] [2]),
        .I1(binval[4]),
        .I2(\dest_graysync_ff[1] [3]),
        .O(binval[2]));
  LUT2 #(
    .INIT(4'h6)) 
    \dest_out_bin_ff[3]_i_1 
       (.I0(\dest_graysync_ff[1] [3]),
        .I1(binval[4]),
        .O(binval[3]));
  LUT6 #(
    .INIT(64'h6996966996696996)) 
    \dest_out_bin_ff[4]_i_1 
       (.I0(\dest_graysync_ff[1] [4]),
        .I1(\dest_graysync_ff[1] [6]),
        .I2(\dest_graysync_ff[1] [8]),
        .I3(\dest_graysync_ff[1] [9]),
        .I4(\dest_graysync_ff[1] [7]),
        .I5(\dest_graysync_ff[1] [5]),
        .O(binval[4]));
  LUT5 #(
    .INIT(32'h96696996)) 
    \dest_out_bin_ff[5]_i_1 
       (.I0(\dest_graysync_ff[1] [5]),
        .I1(\dest_graysync_ff[1] [7]),
        .I2(\dest_graysync_ff[1] [9]),
        .I3(\dest_graysync_ff[1] [8]),
        .I4(\dest_graysync_ff[1] [6]),
        .O(binval[5]));
  LUT4 #(
    .INIT(16'h6996)) 
    \dest_out_bin_ff[6]_i_1 
       (.I0(\dest_graysync_ff[1] [6]),
        .I1(\dest_graysync_ff[1] [8]),
        .I2(\dest_graysync_ff[1] [9]),
        .I3(\dest_graysync_ff[1] [7]),
        .O(binval[6]));
  LUT3 #(
    .INIT(8'h96)) 
    \dest_out_bin_ff[7]_i_1 
       (.I0(\dest_graysync_ff[1] [7]),
        .I1(\dest_graysync_ff[1] [9]),
        .I2(\dest_graysync_ff[1] [8]),
        .O(binval[7]));
  LUT2 #(
    .INIT(4'h6)) 
    \dest_out_bin_ff[8]_i_1 
       (.I0(\dest_graysync_ff[1] [8]),
        .I1(\dest_graysync_ff[1] [9]),
        .O(binval[8]));
  FDRE \dest_out_bin_ff_reg[0] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(binval[0]),
        .Q(dest_out_bin[0]),
        .R(1'b0));
  FDRE \dest_out_bin_ff_reg[1] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(binval[1]),
        .Q(dest_out_bin[1]),
        .R(1'b0));
  FDRE \dest_out_bin_ff_reg[2] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(binval[2]),
        .Q(dest_out_bin[2]),
        .R(1'b0));
  FDRE \dest_out_bin_ff_reg[3] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(binval[3]),
        .Q(dest_out_bin[3]),
        .R(1'b0));
  FDRE \dest_out_bin_ff_reg[4] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(binval[4]),
        .Q(dest_out_bin[4]),
        .R(1'b0));
  FDRE \dest_out_bin_ff_reg[5] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(binval[5]),
        .Q(dest_out_bin[5]),
        .R(1'b0));
  FDRE \dest_out_bin_ff_reg[6] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(binval[6]),
        .Q(dest_out_bin[6]),
        .R(1'b0));
  FDRE \dest_out_bin_ff_reg[7] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(binval[7]),
        .Q(dest_out_bin[7]),
        .R(1'b0));
  FDRE \dest_out_bin_ff_reg[8] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(binval[8]),
        .Q(dest_out_bin[8]),
        .R(1'b0));
  FDRE \dest_out_bin_ff_reg[9] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(\dest_graysync_ff[1] [9]),
        .Q(dest_out_bin[9]),
        .R(1'b0));
  (* SOFT_HLUTNM = "soft_lutpair4" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \src_gray_ff[0]_i_1 
       (.I0(src_in_bin[1]),
        .I1(src_in_bin[0]),
        .O(gray_enc[0]));
  (* SOFT_HLUTNM = "soft_lutpair4" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \src_gray_ff[1]_i_1 
       (.I0(src_in_bin[2]),
        .I1(src_in_bin[1]),
        .O(gray_enc[1]));
  (* SOFT_HLUTNM = "soft_lutpair5" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \src_gray_ff[2]_i_1 
       (.I0(src_in_bin[3]),
        .I1(src_in_bin[2]),
        .O(gray_enc[2]));
  (* SOFT_HLUTNM = "soft_lutpair5" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \src_gray_ff[3]_i_1 
       (.I0(src_in_bin[4]),
        .I1(src_in_bin[3]),
        .O(gray_enc[3]));
  (* SOFT_HLUTNM = "soft_lutpair6" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \src_gray_ff[4]_i_1 
       (.I0(src_in_bin[5]),
        .I1(src_in_bin[4]),
        .O(gray_enc[4]));
  (* SOFT_HLUTNM = "soft_lutpair6" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \src_gray_ff[5]_i_1 
       (.I0(src_in_bin[6]),
        .I1(src_in_bin[5]),
        .O(gray_enc[5]));
  (* SOFT_HLUTNM = "soft_lutpair7" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \src_gray_ff[6]_i_1 
       (.I0(src_in_bin[7]),
        .I1(src_in_bin[6]),
        .O(gray_enc[6]));
  (* SOFT_HLUTNM = "soft_lutpair7" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \src_gray_ff[7]_i_1 
       (.I0(src_in_bin[8]),
        .I1(src_in_bin[7]),
        .O(gray_enc[7]));
  LUT2 #(
    .INIT(4'h6)) 
    \src_gray_ff[8]_i_1 
       (.I0(src_in_bin[9]),
        .I1(src_in_bin[8]),
        .O(gray_enc[8]));
  FDRE \src_gray_ff_reg[0] 
       (.C(src_clk),
        .CE(1'b1),
        .D(gray_enc[0]),
        .Q(async_path[0]),
        .R(1'b0));
  FDRE \src_gray_ff_reg[1] 
       (.C(src_clk),
        .CE(1'b1),
        .D(gray_enc[1]),
        .Q(async_path[1]),
        .R(1'b0));
  FDRE \src_gray_ff_reg[2] 
       (.C(src_clk),
        .CE(1'b1),
        .D(gray_enc[2]),
        .Q(async_path[2]),
        .R(1'b0));
  FDRE \src_gray_ff_reg[3] 
       (.C(src_clk),
        .CE(1'b1),
        .D(gray_enc[3]),
        .Q(async_path[3]),
        .R(1'b0));
  FDRE \src_gray_ff_reg[4] 
       (.C(src_clk),
        .CE(1'b1),
        .D(gray_enc[4]),
        .Q(async_path[4]),
        .R(1'b0));
  FDRE \src_gray_ff_reg[5] 
       (.C(src_clk),
        .CE(1'b1),
        .D(gray_enc[5]),
        .Q(async_path[5]),
        .R(1'b0));
  FDRE \src_gray_ff_reg[6] 
       (.C(src_clk),
        .CE(1'b1),
        .D(gray_enc[6]),
        .Q(async_path[6]),
        .R(1'b0));
  FDRE \src_gray_ff_reg[7] 
       (.C(src_clk),
        .CE(1'b1),
        .D(gray_enc[7]),
        .Q(async_path[7]),
        .R(1'b0));
  FDRE \src_gray_ff_reg[8] 
       (.C(src_clk),
        .CE(1'b1),
        .D(gray_enc[8]),
        .Q(async_path[8]),
        .R(1'b0));
  FDRE \src_gray_ff_reg[9] 
       (.C(src_clk),
        .CE(1'b1),
        .D(src_in_bin[9]),
        .Q(async_path[9]),
        .R(1'b0));
endmodule

(* DEST_SYNC_FF = "2" *) (* INIT_SYNC_FF = "0" *) (* ORIG_REF_NAME = "xpm_cdc_gray" *) 
(* REG_OUTPUT = "1" *) (* SIM_ASSERT_CHK = "0" *) (* SIM_LOSSLESS_GRAY_CHK = "0" *) 
(* VERSION = "0" *) (* WIDTH = "10" *) (* XPM_MODULE = "TRUE" *) 
(* is_du_within_envelope = "true" *) (* keep_hierarchy = "true" *) (* xpm_cdc = "GRAY" *) 
module spi_write_fifo_xpm_cdc_gray__2
   (src_clk,
    src_in_bin,
    dest_clk,
    dest_out_bin);
  input src_clk;
  input [9:0]src_in_bin;
  input dest_clk;
  output [9:0]dest_out_bin;

  wire [9:0]async_path;
  wire [8:0]binval;
  wire dest_clk;
  (* RTL_KEEP = "true" *) (* async_reg = "true" *) (* xpm_cdc = "GRAY" *) wire [9:0]\dest_graysync_ff[0] ;
  (* RTL_KEEP = "true" *) (* async_reg = "true" *) (* xpm_cdc = "GRAY" *) wire [9:0]\dest_graysync_ff[1] ;
  wire [9:0]dest_out_bin;
  wire [8:0]gray_enc;
  wire src_clk;
  wire [9:0]src_in_bin;

  (* ASYNC_REG *) 
  (* KEEP = "true" *) 
  (* XPM_CDC = "GRAY" *) 
  FDRE \dest_graysync_ff_reg[0][0] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(async_path[0]),
        .Q(\dest_graysync_ff[0] [0]),
        .R(1'b0));
  (* ASYNC_REG *) 
  (* KEEP = "true" *) 
  (* XPM_CDC = "GRAY" *) 
  FDRE \dest_graysync_ff_reg[0][1] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(async_path[1]),
        .Q(\dest_graysync_ff[0] [1]),
        .R(1'b0));
  (* ASYNC_REG *) 
  (* KEEP = "true" *) 
  (* XPM_CDC = "GRAY" *) 
  FDRE \dest_graysync_ff_reg[0][2] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(async_path[2]),
        .Q(\dest_graysync_ff[0] [2]),
        .R(1'b0));
  (* ASYNC_REG *) 
  (* KEEP = "true" *) 
  (* XPM_CDC = "GRAY" *) 
  FDRE \dest_graysync_ff_reg[0][3] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(async_path[3]),
        .Q(\dest_graysync_ff[0] [3]),
        .R(1'b0));
  (* ASYNC_REG *) 
  (* KEEP = "true" *) 
  (* XPM_CDC = "GRAY" *) 
  FDRE \dest_graysync_ff_reg[0][4] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(async_path[4]),
        .Q(\dest_graysync_ff[0] [4]),
        .R(1'b0));
  (* ASYNC_REG *) 
  (* KEEP = "true" *) 
  (* XPM_CDC = "GRAY" *) 
  FDRE \dest_graysync_ff_reg[0][5] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(async_path[5]),
        .Q(\dest_graysync_ff[0] [5]),
        .R(1'b0));
  (* ASYNC_REG *) 
  (* KEEP = "true" *) 
  (* XPM_CDC = "GRAY" *) 
  FDRE \dest_graysync_ff_reg[0][6] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(async_path[6]),
        .Q(\dest_graysync_ff[0] [6]),
        .R(1'b0));
  (* ASYNC_REG *) 
  (* KEEP = "true" *) 
  (* XPM_CDC = "GRAY" *) 
  FDRE \dest_graysync_ff_reg[0][7] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(async_path[7]),
        .Q(\dest_graysync_ff[0] [7]),
        .R(1'b0));
  (* ASYNC_REG *) 
  (* KEEP = "true" *) 
  (* XPM_CDC = "GRAY" *) 
  FDRE \dest_graysync_ff_reg[0][8] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(async_path[8]),
        .Q(\dest_graysync_ff[0] [8]),
        .R(1'b0));
  (* ASYNC_REG *) 
  (* KEEP = "true" *) 
  (* XPM_CDC = "GRAY" *) 
  FDRE \dest_graysync_ff_reg[0][9] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(async_path[9]),
        .Q(\dest_graysync_ff[0] [9]),
        .R(1'b0));
  (* ASYNC_REG *) 
  (* KEEP = "true" *) 
  (* XPM_CDC = "GRAY" *) 
  FDRE \dest_graysync_ff_reg[1][0] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(\dest_graysync_ff[0] [0]),
        .Q(\dest_graysync_ff[1] [0]),
        .R(1'b0));
  (* ASYNC_REG *) 
  (* KEEP = "true" *) 
  (* XPM_CDC = "GRAY" *) 
  FDRE \dest_graysync_ff_reg[1][1] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(\dest_graysync_ff[0] [1]),
        .Q(\dest_graysync_ff[1] [1]),
        .R(1'b0));
  (* ASYNC_REG *) 
  (* KEEP = "true" *) 
  (* XPM_CDC = "GRAY" *) 
  FDRE \dest_graysync_ff_reg[1][2] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(\dest_graysync_ff[0] [2]),
        .Q(\dest_graysync_ff[1] [2]),
        .R(1'b0));
  (* ASYNC_REG *) 
  (* KEEP = "true" *) 
  (* XPM_CDC = "GRAY" *) 
  FDRE \dest_graysync_ff_reg[1][3] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(\dest_graysync_ff[0] [3]),
        .Q(\dest_graysync_ff[1] [3]),
        .R(1'b0));
  (* ASYNC_REG *) 
  (* KEEP = "true" *) 
  (* XPM_CDC = "GRAY" *) 
  FDRE \dest_graysync_ff_reg[1][4] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(\dest_graysync_ff[0] [4]),
        .Q(\dest_graysync_ff[1] [4]),
        .R(1'b0));
  (* ASYNC_REG *) 
  (* KEEP = "true" *) 
  (* XPM_CDC = "GRAY" *) 
  FDRE \dest_graysync_ff_reg[1][5] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(\dest_graysync_ff[0] [5]),
        .Q(\dest_graysync_ff[1] [5]),
        .R(1'b0));
  (* ASYNC_REG *) 
  (* KEEP = "true" *) 
  (* XPM_CDC = "GRAY" *) 
  FDRE \dest_graysync_ff_reg[1][6] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(\dest_graysync_ff[0] [6]),
        .Q(\dest_graysync_ff[1] [6]),
        .R(1'b0));
  (* ASYNC_REG *) 
  (* KEEP = "true" *) 
  (* XPM_CDC = "GRAY" *) 
  FDRE \dest_graysync_ff_reg[1][7] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(\dest_graysync_ff[0] [7]),
        .Q(\dest_graysync_ff[1] [7]),
        .R(1'b0));
  (* ASYNC_REG *) 
  (* KEEP = "true" *) 
  (* XPM_CDC = "GRAY" *) 
  FDRE \dest_graysync_ff_reg[1][8] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(\dest_graysync_ff[0] [8]),
        .Q(\dest_graysync_ff[1] [8]),
        .R(1'b0));
  (* ASYNC_REG *) 
  (* KEEP = "true" *) 
  (* XPM_CDC = "GRAY" *) 
  FDRE \dest_graysync_ff_reg[1][9] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(\dest_graysync_ff[0] [9]),
        .Q(\dest_graysync_ff[1] [9]),
        .R(1'b0));
  LUT5 #(
    .INIT(32'h96696996)) 
    \dest_out_bin_ff[0]_i_1 
       (.I0(\dest_graysync_ff[1] [0]),
        .I1(\dest_graysync_ff[1] [2]),
        .I2(binval[4]),
        .I3(\dest_graysync_ff[1] [3]),
        .I4(\dest_graysync_ff[1] [1]),
        .O(binval[0]));
  LUT4 #(
    .INIT(16'h6996)) 
    \dest_out_bin_ff[1]_i_1 
       (.I0(\dest_graysync_ff[1] [1]),
        .I1(\dest_graysync_ff[1] [3]),
        .I2(binval[4]),
        .I3(\dest_graysync_ff[1] [2]),
        .O(binval[1]));
  LUT3 #(
    .INIT(8'h96)) 
    \dest_out_bin_ff[2]_i_1 
       (.I0(\dest_graysync_ff[1] [2]),
        .I1(binval[4]),
        .I2(\dest_graysync_ff[1] [3]),
        .O(binval[2]));
  LUT2 #(
    .INIT(4'h6)) 
    \dest_out_bin_ff[3]_i_1 
       (.I0(\dest_graysync_ff[1] [3]),
        .I1(binval[4]),
        .O(binval[3]));
  LUT6 #(
    .INIT(64'h6996966996696996)) 
    \dest_out_bin_ff[4]_i_1 
       (.I0(\dest_graysync_ff[1] [4]),
        .I1(\dest_graysync_ff[1] [6]),
        .I2(\dest_graysync_ff[1] [8]),
        .I3(\dest_graysync_ff[1] [9]),
        .I4(\dest_graysync_ff[1] [7]),
        .I5(\dest_graysync_ff[1] [5]),
        .O(binval[4]));
  LUT5 #(
    .INIT(32'h96696996)) 
    \dest_out_bin_ff[5]_i_1 
       (.I0(\dest_graysync_ff[1] [5]),
        .I1(\dest_graysync_ff[1] [7]),
        .I2(\dest_graysync_ff[1] [9]),
        .I3(\dest_graysync_ff[1] [8]),
        .I4(\dest_graysync_ff[1] [6]),
        .O(binval[5]));
  LUT4 #(
    .INIT(16'h6996)) 
    \dest_out_bin_ff[6]_i_1 
       (.I0(\dest_graysync_ff[1] [6]),
        .I1(\dest_graysync_ff[1] [8]),
        .I2(\dest_graysync_ff[1] [9]),
        .I3(\dest_graysync_ff[1] [7]),
        .O(binval[6]));
  LUT3 #(
    .INIT(8'h96)) 
    \dest_out_bin_ff[7]_i_1 
       (.I0(\dest_graysync_ff[1] [7]),
        .I1(\dest_graysync_ff[1] [9]),
        .I2(\dest_graysync_ff[1] [8]),
        .O(binval[7]));
  LUT2 #(
    .INIT(4'h6)) 
    \dest_out_bin_ff[8]_i_1 
       (.I0(\dest_graysync_ff[1] [8]),
        .I1(\dest_graysync_ff[1] [9]),
        .O(binval[8]));
  FDRE \dest_out_bin_ff_reg[0] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(binval[0]),
        .Q(dest_out_bin[0]),
        .R(1'b0));
  FDRE \dest_out_bin_ff_reg[1] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(binval[1]),
        .Q(dest_out_bin[1]),
        .R(1'b0));
  FDRE \dest_out_bin_ff_reg[2] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(binval[2]),
        .Q(dest_out_bin[2]),
        .R(1'b0));
  FDRE \dest_out_bin_ff_reg[3] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(binval[3]),
        .Q(dest_out_bin[3]),
        .R(1'b0));
  FDRE \dest_out_bin_ff_reg[4] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(binval[4]),
        .Q(dest_out_bin[4]),
        .R(1'b0));
  FDRE \dest_out_bin_ff_reg[5] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(binval[5]),
        .Q(dest_out_bin[5]),
        .R(1'b0));
  FDRE \dest_out_bin_ff_reg[6] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(binval[6]),
        .Q(dest_out_bin[6]),
        .R(1'b0));
  FDRE \dest_out_bin_ff_reg[7] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(binval[7]),
        .Q(dest_out_bin[7]),
        .R(1'b0));
  FDRE \dest_out_bin_ff_reg[8] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(binval[8]),
        .Q(dest_out_bin[8]),
        .R(1'b0));
  FDRE \dest_out_bin_ff_reg[9] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(\dest_graysync_ff[1] [9]),
        .Q(dest_out_bin[9]),
        .R(1'b0));
  (* SOFT_HLUTNM = "soft_lutpair0" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \src_gray_ff[0]_i_1 
       (.I0(src_in_bin[1]),
        .I1(src_in_bin[0]),
        .O(gray_enc[0]));
  (* SOFT_HLUTNM = "soft_lutpair0" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \src_gray_ff[1]_i_1 
       (.I0(src_in_bin[2]),
        .I1(src_in_bin[1]),
        .O(gray_enc[1]));
  (* SOFT_HLUTNM = "soft_lutpair1" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \src_gray_ff[2]_i_1 
       (.I0(src_in_bin[3]),
        .I1(src_in_bin[2]),
        .O(gray_enc[2]));
  (* SOFT_HLUTNM = "soft_lutpair1" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \src_gray_ff[3]_i_1 
       (.I0(src_in_bin[4]),
        .I1(src_in_bin[3]),
        .O(gray_enc[3]));
  (* SOFT_HLUTNM = "soft_lutpair2" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \src_gray_ff[4]_i_1 
       (.I0(src_in_bin[5]),
        .I1(src_in_bin[4]),
        .O(gray_enc[4]));
  (* SOFT_HLUTNM = "soft_lutpair2" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \src_gray_ff[5]_i_1 
       (.I0(src_in_bin[6]),
        .I1(src_in_bin[5]),
        .O(gray_enc[5]));
  (* SOFT_HLUTNM = "soft_lutpair3" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \src_gray_ff[6]_i_1 
       (.I0(src_in_bin[7]),
        .I1(src_in_bin[6]),
        .O(gray_enc[6]));
  (* SOFT_HLUTNM = "soft_lutpair3" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \src_gray_ff[7]_i_1 
       (.I0(src_in_bin[8]),
        .I1(src_in_bin[7]),
        .O(gray_enc[7]));
  LUT2 #(
    .INIT(4'h6)) 
    \src_gray_ff[8]_i_1 
       (.I0(src_in_bin[9]),
        .I1(src_in_bin[8]),
        .O(gray_enc[8]));
  FDRE \src_gray_ff_reg[0] 
       (.C(src_clk),
        .CE(1'b1),
        .D(gray_enc[0]),
        .Q(async_path[0]),
        .R(1'b0));
  FDRE \src_gray_ff_reg[1] 
       (.C(src_clk),
        .CE(1'b1),
        .D(gray_enc[1]),
        .Q(async_path[1]),
        .R(1'b0));
  FDRE \src_gray_ff_reg[2] 
       (.C(src_clk),
        .CE(1'b1),
        .D(gray_enc[2]),
        .Q(async_path[2]),
        .R(1'b0));
  FDRE \src_gray_ff_reg[3] 
       (.C(src_clk),
        .CE(1'b1),
        .D(gray_enc[3]),
        .Q(async_path[3]),
        .R(1'b0));
  FDRE \src_gray_ff_reg[4] 
       (.C(src_clk),
        .CE(1'b1),
        .D(gray_enc[4]),
        .Q(async_path[4]),
        .R(1'b0));
  FDRE \src_gray_ff_reg[5] 
       (.C(src_clk),
        .CE(1'b1),
        .D(gray_enc[5]),
        .Q(async_path[5]),
        .R(1'b0));
  FDRE \src_gray_ff_reg[6] 
       (.C(src_clk),
        .CE(1'b1),
        .D(gray_enc[6]),
        .Q(async_path[6]),
        .R(1'b0));
  FDRE \src_gray_ff_reg[7] 
       (.C(src_clk),
        .CE(1'b1),
        .D(gray_enc[7]),
        .Q(async_path[7]),
        .R(1'b0));
  FDRE \src_gray_ff_reg[8] 
       (.C(src_clk),
        .CE(1'b1),
        .D(gray_enc[8]),
        .Q(async_path[8]),
        .R(1'b0));
  FDRE \src_gray_ff_reg[9] 
       (.C(src_clk),
        .CE(1'b1),
        .D(src_in_bin[9]),
        .Q(async_path[9]),
        .R(1'b0));
endmodule

(* DEST_SYNC_FF = "4" *) (* INIT_SYNC_FF = "0" *) (* ORIG_REF_NAME = "xpm_cdc_single" *) 
(* SIM_ASSERT_CHK = "0" *) (* SRC_INPUT_REG = "0" *) (* VERSION = "0" *) 
(* XPM_MODULE = "TRUE" *) (* is_du_within_envelope = "true" *) (* keep_hierarchy = "true" *) 
(* xpm_cdc = "SINGLE" *) 
module spi_write_fifo_xpm_cdc_single
   (src_clk,
    src_in,
    dest_clk,
    dest_out);
  input src_clk;
  input src_in;
  input dest_clk;
  output dest_out;

  wire dest_clk;
  wire src_in;
  (* RTL_KEEP = "true" *) (* async_reg = "true" *) (* xpm_cdc = "SINGLE" *) wire [3:0]syncstages_ff;

  assign dest_out = syncstages_ff[3];
  (* ASYNC_REG *) 
  (* KEEP = "true" *) 
  (* XPM_CDC = "SINGLE" *) 
  FDRE \syncstages_ff_reg[0] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(src_in),
        .Q(syncstages_ff[0]),
        .R(1'b0));
  (* ASYNC_REG *) 
  (* KEEP = "true" *) 
  (* XPM_CDC = "SINGLE" *) 
  FDRE \syncstages_ff_reg[1] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(syncstages_ff[0]),
        .Q(syncstages_ff[1]),
        .R(1'b0));
  (* ASYNC_REG *) 
  (* KEEP = "true" *) 
  (* XPM_CDC = "SINGLE" *) 
  FDRE \syncstages_ff_reg[2] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(syncstages_ff[1]),
        .Q(syncstages_ff[2]),
        .R(1'b0));
  (* ASYNC_REG *) 
  (* KEEP = "true" *) 
  (* XPM_CDC = "SINGLE" *) 
  FDRE \syncstages_ff_reg[3] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(syncstages_ff[2]),
        .Q(syncstages_ff[3]),
        .R(1'b0));
endmodule

(* DEST_SYNC_FF = "4" *) (* INIT_SYNC_FF = "0" *) (* ORIG_REF_NAME = "xpm_cdc_single" *) 
(* SIM_ASSERT_CHK = "0" *) (* SRC_INPUT_REG = "0" *) (* VERSION = "0" *) 
(* XPM_MODULE = "TRUE" *) (* is_du_within_envelope = "true" *) (* keep_hierarchy = "true" *) 
(* xpm_cdc = "SINGLE" *) 
module spi_write_fifo_xpm_cdc_single__2
   (src_clk,
    src_in,
    dest_clk,
    dest_out);
  input src_clk;
  input src_in;
  input dest_clk;
  output dest_out;

  wire dest_clk;
  wire src_in;
  (* RTL_KEEP = "true" *) (* async_reg = "true" *) (* xpm_cdc = "SINGLE" *) wire [3:0]syncstages_ff;

  assign dest_out = syncstages_ff[3];
  (* ASYNC_REG *) 
  (* KEEP = "true" *) 
  (* XPM_CDC = "SINGLE" *) 
  FDRE \syncstages_ff_reg[0] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(src_in),
        .Q(syncstages_ff[0]),
        .R(1'b0));
  (* ASYNC_REG *) 
  (* KEEP = "true" *) 
  (* XPM_CDC = "SINGLE" *) 
  FDRE \syncstages_ff_reg[1] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(syncstages_ff[0]),
        .Q(syncstages_ff[1]),
        .R(1'b0));
  (* ASYNC_REG *) 
  (* KEEP = "true" *) 
  (* XPM_CDC = "SINGLE" *) 
  FDRE \syncstages_ff_reg[2] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(syncstages_ff[1]),
        .Q(syncstages_ff[2]),
        .R(1'b0));
  (* ASYNC_REG *) 
  (* KEEP = "true" *) 
  (* XPM_CDC = "SINGLE" *) 
  FDRE \syncstages_ff_reg[3] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(syncstages_ff[2]),
        .Q(syncstages_ff[3]),
        .R(1'b0));
endmodule
`pragma protect begin_protected
`pragma protect version = 1
`pragma protect encrypt_agent = "XILINX"
`pragma protect encrypt_agent_info = "Xilinx Encryption Tool 2022.1"
`pragma protect key_keyowner="Synopsys", key_keyname="SNPS-VCS-RSA-2", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=128)
`pragma protect key_block
RgPKnWr9n0dGgttm3akiFhAlfB96usOQYxnEmPhGyTGg1AbizYAjGPWLXBWl50n/d0IA71ci4aJB
wt6mtfyNADm3ZReK7D3mKu037BOgxryoEwwf1kiC6q/PllxsdAgEMfQrfHJ3E2AzSpdYjoxVYito
y0JW6CUDcWvWa4WV0EA=

`pragma protect key_keyowner="Aldec", key_keyname="ALDEC15_001", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=256)
`pragma protect key_block
M0l6KpOGH3jL8eRt3NCD7e2USYnkg5H9GAnE1PKmnjiouFN3Y8kjWA2PZDAQLm9UW+TsC1HeVlzO
WjNCHkjR/6ubCsIcWfpPZWdIuAenlsyq8Y9l6b8vMj8JSbDEOiFF/GHSbKsn22MJdDJKEhHFK6GV
s8gR2vywRFwG69gIRE4qGhVB+WIg8GJrDpDMYH6lCjMkTrjXuKDUcNlJN3NPLuhJ7tsditwf1pr5
moJRmGpJnip/rGm0g4o4A6ev4CtePjoao8C1wFtzHkERX9oenhh7cGjDMejU5IrLv8NxFnLj1FpB
9MuF1beTU20NI5oAn6zLiLiOtXjf0ghU3AN4DA==

`pragma protect key_keyowner="Mentor Graphics Corporation", key_keyname="MGC-VELOCE-RSA", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=128)
`pragma protect key_block
hAsrUfp6Qgjm8yBjNYTEtQmVQmMxzL8TE/3oiQSxSI3+yEkXAbQCXkT9mo+LCdv+fGECOB0istHd
eLtbsiYbxjxNxYkXiUrRE5O+aSxynIray+uF9DJigTEUZu8JJXUbzxK4DDUu1Lm9tpGps4+Prz1m
0gkj13RT/Y/418s2VTw=

`pragma protect key_keyowner="Mentor Graphics Corporation", key_keyname="MGC-VERIF-SIM-RSA-2", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=256)
`pragma protect key_block
BP/54Wm/GJmb1jy0bxWJJX4aiKyiWPVh4X3VL30BQrmX4PlEsNKzBJH3Qu8IIYERfnFP0ifAgboa
vypMQ5Ed0BrMePGkWIgT6I8hxJCMFpHdkSK7m1giSKyZzFfTOrVqoNFXE+qdzLfY1J5hBWCvouYo
jllavK4N3gF9FLScH2AUWYVMcVth2QPaTAU2NLnAUNH8kgtBjBfc8/KbPPTznD1QNVqvFstzcbTA
hGQ1ETVPvINQ0KqxxAG5PRhtQD4+pC+hr/Tvk+RSvGyBOfy9zE86OXkJiYs9dSFhNiMFmCPL9DBO
se4OxNNC0/7aBtb1mkSEA9YFDYEb9jS7Jasy2A==

`pragma protect key_keyowner="Real Intent", key_keyname="RI-RSA-KEY-1", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=256)
`pragma protect key_block
SnzT9DVH3xlEN8nrr2YrlvTO5qj7L22d7WaBcuKyTaiHoIwwFHrC4HQbfs0TAwkdWcOgmJoATPSF
F6qm0KiddbrlERF3MfKUldeGBJtqLdX+zGw7+3JD7S+HB9dIMOFOHy+IiCZp1/Pz8epKpi238cel
rcVoJQKz406wmXDvOo8KsT+XhRLs9BVCrBErPGGXKYDk6NXAp0duOgQE9DbslzMU83M/kUC7uERV
tQW02240peKQFp2elEZC7Tetvgp0TaFTtJiKN45REi8GQUCKGa85JjNIk1qb/+k95TIIP1xrHirc
6iX7qbwnPetv8TVu2NjkZ0WDEK5RXdOXcxBwHA==

`pragma protect key_keyowner="Xilinx", key_keyname="xilinxt_2021_07", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=256)
`pragma protect key_block
erkR82V0jX8ytva+9MzEs0c75Z7j7TsgxWRLNAUfbbU28i+U9YzuZlSfYU75M0f9jx1gvxtBrfKv
cNHVdkR+i5zfHDZsDwfMEEBhs8wzDCKqe+eex6BBEvlIOesCPXrr2RozQgaQ1PBh/os2Arfu+873
BjsVxFJkbhpzIqlddOo/XZV9Yi+eih7A7pXXEBR6IL7Poo4Ka49MiVQU0xJrDTm+ddOuMPDRRD7g
dsxS/uzdcBcO2myV6g/7YH/C2Ce9s6+UywJN/0JeXSqwA7bsBqqnfFicVAT0lckLopMLiuzK7dsN
EwhFeqoetciFrDIj9+o0xDMWBZhgNP1u68vURA==

`pragma protect key_keyowner="Metrics Technologies Inc.", key_keyname="DSim", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=256)
`pragma protect key_block
R+BI65BKLT0I9hEtsxGnDyM5XY9gzULeTPOSDXvd3KWOzZJAx6C0xlbyZcFZhAEG/QIK2yd0wAi6
IUWxyF/sx3HsqKjhVi5KxnpuXDBOZVoj811O7JukedFVmDW7OHGtBkuiJ5X5irw7mfsEKRQmF/1i
V6lj9HYHZEjxtDeZjACsLY4y1QxWalSKT4HIMOHznBLL8dLbGMlS+ZmFuFn0gcwZavVl7gTkTtkf
W0gn01A9ru7NKsf+iLX0kj4dgItPu9N2g02M1vWQ9UUQEVvfV7lUc7GY1suibrD8aEkhH9S7lZ7n
bFsT4qxyvzg8ML6v4g4v7N6VuyhEtgFgNd725Q==

`pragma protect key_keyowner="Atrenta", key_keyname="ATR-SG-RSA-1", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=384)
`pragma protect key_block
jNUVKiwH68vHsU54idgvKwaVJcoxTUuxfgrQpbpmM/IpesWA3wHsGzYClwAxkKzw3KRnFyQqTWcZ
yj1EQ2CMBxlJ0kyNbZW8OK8pXzeigToZ0U6Aq3Gy+j7wBbhe83wE1Ygn82sK8dHTEulvaRLn/c5r
ispy1s7jMKIvYNzoUuZrgyBQyfaYmdqUia8XlQjFd+VwzhTXKwzvmaqHWyaHjfBKeCooO7+oUxMG
OJg83W54EVe9ronFQ8Wr9EOL8ia7qelCAgyQe/bC0HHCoMAm8apI7sX23iMR/wMiPP5V2bQzycy+
rBX/+SWkqSeIE1FLm+muFPvrE7iLwJaW8d1fzdFFjAZ5aIXArbWNfwbK8S0TczXc9lEzmpb69rwA
UJIrs4alo81qGQ32UFhjuMQjX75O9Od1HWHDj5PFaT/Ja5Ly+bK8Cc3gfO6dCE81m7d+B2JBZ/Hl
tBA19QuOAYwT2EIPOdpaVtCULb33cWODWu3qQFhZMmDzKTb3kwpcr0LL

`pragma protect key_keyowner="Cadence Design Systems.", key_keyname="CDS_RSA_KEY_VER_1", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=256)
`pragma protect key_block
MnzcA2swaxH1LRacKDou0ZmiiMRETbWIdHgeBgyQz7ILronsXLoB/C20WuFNGEVSiL2/51EZ6MXZ
vMHI8fFcMQCJcuTBDBibUMKv6bXI9s8fTbtrBZppbF/R1icG5JYhqmX4aRnv4W/dxJRjI2L35nLN
Y48E9OfgfkD1sr+IRwx8WEKFmUhuk8dLe0VOK7ywe3XEcneYvrz+HhPj16bGmNfMwNnDgZ3gKKZD
hRnys+jzvAX3HyISrErWXhMKrhWMxXeTNFJCqNQ0LWAVHQYwyKnF5xVpyXSuGNSIrva+QXqOrZBG
3VNLirNVtMRiKLfwZeMaqvswkqBDAa53utlAAA==

`pragma protect key_keyowner="Synplicity", key_keyname="SYNP15_1", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=256)
`pragma protect key_block
d0xXkKtsKM8GvXhDZr16p1+oE3uBtD04BJ76eGGIEj/CFECfHVy9qsJ43oSKjas0+AJr4GFFnVb6
X7gJV6MmX/OboC9ier5joUCGz0mxVzkRZK9a+LPEDcg0K6+cLE36kr+FfxW9Uk2816EHBCMCf5mK
A4eAhSmAb5Nq74F/q0quiG416npbny7faiQ+xmPDfYYiM3UuMKaD4iE8ODlz1w5xThPllWESf3LZ
NTkw6fozyTqZ47vvE21O3dgIGAY1v+C6BwlCK24VwPJa1xs9csY+qTk31j5jjAc1ExlB6QF7t9UH
lk70qdNPWxT87OH7kFT8UvPO1D6BTC3/WkDZfA==

`pragma protect key_keyowner="Mentor Graphics Corporation", key_keyname="MGC-PREC-RSA", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=256)
`pragma protect key_block
LYhdzKTvo93uJrAaiP5OfCDuOnu2BSvPnxlv6I7h2n1+xHtj34LNNKzWEgOg9dUV9cYDaHYUjjEt
DKdWcz6mZ61d5qyxAhpv67fc90v7JVgtOAcT94/Yb+AuLxXFcGA6Gic7uoJtUgz6JmTnb22Dxdjc
KuIewDj4IOTfP8XGXKTaF+cNp0CFrQgTAcVSQFyLFxr0I/9h3S+GZLecA7ntEeHEOfCJzPvy0ddi
7MCdQWECLb+fXC0IAn8V95TumcpINiRAX1BHi9IGJ4QoMrb3jOCrPkFhDMTJj2aiImUWdi/l/0QE
d7wcXlgIEYVeoKYUOJ4mqy+zZPUbLNeOPADUDQ==

`pragma protect data_method = "AES128-CBC"
`pragma protect encoding = (enctype = "BASE64", line_length = 76, bytes = 128736)
`pragma protect data_block
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
`pragma protect end_protected
`ifndef GLBL
`define GLBL
`timescale  1 ps / 1 ps

module glbl ();

    parameter ROC_WIDTH = 100000;
    parameter TOC_WIDTH = 0;
    parameter GRES_WIDTH = 10000;
    parameter GRES_START = 10000;

//--------   STARTUP Globals --------------
    wire GSR;
    wire GTS;
    wire GWE;
    wire PRLD;
    wire GRESTORE;
    tri1 p_up_tmp;
    tri (weak1, strong0) PLL_LOCKG = p_up_tmp;

    wire PROGB_GLBL;
    wire CCLKO_GLBL;
    wire FCSBO_GLBL;
    wire [3:0] DO_GLBL;
    wire [3:0] DI_GLBL;
   
    reg GSR_int;
    reg GTS_int;
    reg PRLD_int;
    reg GRESTORE_int;

//--------   JTAG Globals --------------
    wire JTAG_TDO_GLBL;
    wire JTAG_TCK_GLBL;
    wire JTAG_TDI_GLBL;
    wire JTAG_TMS_GLBL;
    wire JTAG_TRST_GLBL;

    reg JTAG_CAPTURE_GLBL;
    reg JTAG_RESET_GLBL;
    reg JTAG_SHIFT_GLBL;
    reg JTAG_UPDATE_GLBL;
    reg JTAG_RUNTEST_GLBL;

    reg JTAG_SEL1_GLBL = 0;
    reg JTAG_SEL2_GLBL = 0 ;
    reg JTAG_SEL3_GLBL = 0;
    reg JTAG_SEL4_GLBL = 0;

    reg JTAG_USER_TDO1_GLBL = 1'bz;
    reg JTAG_USER_TDO2_GLBL = 1'bz;
    reg JTAG_USER_TDO3_GLBL = 1'bz;
    reg JTAG_USER_TDO4_GLBL = 1'bz;

    assign (strong1, weak0) GSR = GSR_int;
    assign (strong1, weak0) GTS = GTS_int;
    assign (weak1, weak0) PRLD = PRLD_int;
    assign (strong1, weak0) GRESTORE = GRESTORE_int;

    initial begin
	GSR_int = 1'b1;
	PRLD_int = 1'b1;
	#(ROC_WIDTH)
	GSR_int = 1'b0;
	PRLD_int = 1'b0;
    end

    initial begin
	GTS_int = 1'b1;
	#(TOC_WIDTH)
	GTS_int = 1'b0;
    end

    initial begin 
	GRESTORE_int = 1'b0;
	#(GRES_START);
	GRESTORE_int = 1'b1;
	#(GRES_WIDTH);
	GRESTORE_int = 1'b0;
    end

endmodule
`endif
