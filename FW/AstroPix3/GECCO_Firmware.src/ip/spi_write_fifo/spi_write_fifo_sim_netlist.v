// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2022.1 (win64) Build 3526262 Mon Apr 18 15:48:16 MDT 2022
// Date        : Mon Aug 22 17:15:59 2022
// Host        : DESKTOP-UNSGLV7 running 64-bit major release  (build 9200)
// Command     : write_verilog -force -mode funcsim
//               c:/Users/nicolas/Documents/GitHub/astropix_github/FW/AstroPix2/GECCO_Firmware.src/ip/spi_write_fifo/spi_write_fifo_sim_netlist.v
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
