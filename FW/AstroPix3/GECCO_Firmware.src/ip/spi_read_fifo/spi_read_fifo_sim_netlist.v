// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2022.1 (win64) Build 3526262 Mon Apr 18 15:48:16 MDT 2022
// Date        : Mon Aug 22 17:16:00 2022
// Host        : DESKTOP-UNSGLV7 running 64-bit major release  (build 9200)
// Command     : write_verilog -force -mode funcsim
//               c:/Users/nicolas/Documents/GitHub/astropix_github/FW/AstroPix2/GECCO_Firmware.src/ip/spi_read_fifo/spi_read_fifo_sim_netlist.v
// Design      : spi_read_fifo
// Purpose     : This verilog netlist is a functional simulation representation of the design and should not be modified
//               or synthesized. This netlist cannot be used for SDF annotated simulation.
// Device      : xc7a200tsbg484-1
// --------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

(* CHECK_LICENSE_TYPE = "spi_read_fifo,fifo_generator_v13_2_7,{}" *) (* downgradeipidentifiedwarnings = "yes" *) (* x_core_info = "fifo_generator_v13_2_7,Vivado 2022.1" *) 
(* NotValidForBitStream *)
module spi_read_fifo
   (rst,
    wr_clk,
    rd_clk,
    din,
    wr_en,
    rd_en,
    dout,
    full,
    empty);
  input rst;
  (* x_interface_info = "xilinx.com:signal:clock:1.0 write_clk CLK" *) (* x_interface_parameter = "XIL_INTERFACENAME write_clk, FREQ_HZ 100000000, FREQ_TOLERANCE_HZ 0, PHASE 0.0, INSERT_VIP 0" *) input wr_clk;
  (* x_interface_info = "xilinx.com:signal:clock:1.0 read_clk CLK" *) (* x_interface_parameter = "XIL_INTERFACENAME read_clk, FREQ_HZ 100000000, FREQ_TOLERANCE_HZ 0, PHASE 0.0, INSERT_VIP 0" *) input rd_clk;
  (* x_interface_info = "xilinx.com:interface:fifo_write:1.0 FIFO_WRITE WR_DATA" *) input [63:0]din;
  (* x_interface_info = "xilinx.com:interface:fifo_write:1.0 FIFO_WRITE WR_EN" *) input wr_en;
  (* x_interface_info = "xilinx.com:interface:fifo_read:1.0 FIFO_READ RD_EN" *) input rd_en;
  (* x_interface_info = "xilinx.com:interface:fifo_read:1.0 FIFO_READ RD_DATA" *) output [7:0]dout;
  (* x_interface_info = "xilinx.com:interface:fifo_write:1.0 FIFO_WRITE FULL" *) output full;
  (* x_interface_info = "xilinx.com:interface:fifo_read:1.0 FIFO_READ EMPTY" *) output empty;

  wire [63:0]din;
  wire [7:0]dout;
  wire empty;
  wire full;
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
  wire NLW_U0_prog_empty_UNCONNECTED;
  wire NLW_U0_prog_full_UNCONNECTED;
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
  wire [12:0]NLW_U0_rd_data_count_UNCONNECTED;
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
  (* C_DIN_WIDTH = "64" *) 
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
  (* C_PRIM_FIFO_TYPE = "1kx36" *) 
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
  (* C_PROG_EMPTY_TYPE = "0" *) 
  (* C_PROG_EMPTY_TYPE_AXIS = "0" *) 
  (* C_PROG_EMPTY_TYPE_RACH = "0" *) 
  (* C_PROG_EMPTY_TYPE_RDCH = "0" *) 
  (* C_PROG_EMPTY_TYPE_WACH = "0" *) 
  (* C_PROG_EMPTY_TYPE_WDCH = "0" *) 
  (* C_PROG_EMPTY_TYPE_WRCH = "0" *) 
  (* C_PROG_FULL_THRESH_ASSERT_VAL = "1021" *) 
  (* C_PROG_FULL_THRESH_ASSERT_VAL_AXIS = "1023" *) 
  (* C_PROG_FULL_THRESH_ASSERT_VAL_RACH = "1023" *) 
  (* C_PROG_FULL_THRESH_ASSERT_VAL_RDCH = "1023" *) 
  (* C_PROG_FULL_THRESH_ASSERT_VAL_WACH = "1023" *) 
  (* C_PROG_FULL_THRESH_ASSERT_VAL_WDCH = "1023" *) 
  (* C_PROG_FULL_THRESH_ASSERT_VAL_WRCH = "1023" *) 
  (* C_PROG_FULL_THRESH_NEGATE_VAL = "1020" *) 
  (* C_PROG_FULL_TYPE = "0" *) 
  (* C_PROG_FULL_TYPE_AXIS = "0" *) 
  (* C_PROG_FULL_TYPE_RACH = "0" *) 
  (* C_PROG_FULL_TYPE_RDCH = "0" *) 
  (* C_PROG_FULL_TYPE_WACH = "0" *) 
  (* C_PROG_FULL_TYPE_WDCH = "0" *) 
  (* C_PROG_FULL_TYPE_WRCH = "0" *) 
  (* C_RACH_TYPE = "0" *) 
  (* C_RDCH_TYPE = "0" *) 
  (* C_RD_DATA_COUNT_WIDTH = "13" *) 
  (* C_RD_DEPTH = "8192" *) 
  (* C_RD_FREQ = "1" *) 
  (* C_RD_PNTR_WIDTH = "13" *) 
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
  spi_read_fifo_fifo_generator_v13_2_7 U0
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
        .prog_empty(NLW_U0_prog_empty_UNCONNECTED),
        .prog_empty_thresh({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .prog_empty_thresh_assert({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .prog_empty_thresh_negate({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .prog_full(NLW_U0_prog_full_UNCONNECTED),
        .prog_full_thresh({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .prog_full_thresh_assert({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .prog_full_thresh_negate({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .rd_clk(rd_clk),
        .rd_data_count(NLW_U0_rd_data_count_UNCONNECTED[12:0]),
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
module spi_read_fifo_xpm_cdc_async_rst
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
module spi_read_fifo_xpm_cdc_async_rst__1
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
module spi_read_fifo_xpm_cdc_gray
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

(* DEST_SYNC_FF = "2" *) (* INIT_SYNC_FF = "0" *) (* ORIG_REF_NAME = "xpm_cdc_gray" *) 
(* REG_OUTPUT = "1" *) (* SIM_ASSERT_CHK = "0" *) (* SIM_LOSSLESS_GRAY_CHK = "0" *) 
(* VERSION = "0" *) (* WIDTH = "13" *) (* XPM_MODULE = "TRUE" *) 
(* is_du_within_envelope = "true" *) (* keep_hierarchy = "true" *) (* xpm_cdc = "GRAY" *) 
module spi_read_fifo_xpm_cdc_gray__parameterized1
   (src_clk,
    src_in_bin,
    dest_clk,
    dest_out_bin);
  input src_clk;
  input [12:0]src_in_bin;
  input dest_clk;
  output [12:0]dest_out_bin;

  wire [12:0]async_path;
  wire [11:0]binval;
  wire dest_clk;
  (* RTL_KEEP = "true" *) (* async_reg = "true" *) (* xpm_cdc = "GRAY" *) wire [12:0]\dest_graysync_ff[0] ;
  (* RTL_KEEP = "true" *) (* async_reg = "true" *) (* xpm_cdc = "GRAY" *) wire [12:0]\dest_graysync_ff[1] ;
  wire [12:0]dest_out_bin;
  wire [11:0]gray_enc;
  wire src_clk;
  wire [12:0]src_in_bin;

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
  FDRE \dest_graysync_ff_reg[0][10] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(async_path[10]),
        .Q(\dest_graysync_ff[0] [10]),
        .R(1'b0));
  (* ASYNC_REG *) 
  (* KEEP = "true" *) 
  (* XPM_CDC = "GRAY" *) 
  FDRE \dest_graysync_ff_reg[0][11] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(async_path[11]),
        .Q(\dest_graysync_ff[0] [11]),
        .R(1'b0));
  (* ASYNC_REG *) 
  (* KEEP = "true" *) 
  (* XPM_CDC = "GRAY" *) 
  FDRE \dest_graysync_ff_reg[0][12] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(async_path[12]),
        .Q(\dest_graysync_ff[0] [12]),
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
  FDRE \dest_graysync_ff_reg[1][10] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(\dest_graysync_ff[0] [10]),
        .Q(\dest_graysync_ff[1] [10]),
        .R(1'b0));
  (* ASYNC_REG *) 
  (* KEEP = "true" *) 
  (* XPM_CDC = "GRAY" *) 
  FDRE \dest_graysync_ff_reg[1][11] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(\dest_graysync_ff[0] [11]),
        .Q(\dest_graysync_ff[1] [11]),
        .R(1'b0));
  (* ASYNC_REG *) 
  (* KEEP = "true" *) 
  (* XPM_CDC = "GRAY" *) 
  FDRE \dest_graysync_ff_reg[1][12] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(\dest_graysync_ff[0] [12]),
        .Q(\dest_graysync_ff[1] [12]),
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
  LUT3 #(
    .INIT(8'h96)) 
    \dest_out_bin_ff[0]_i_1 
       (.I0(\dest_graysync_ff[1] [0]),
        .I1(binval[2]),
        .I2(\dest_graysync_ff[1] [1]),
        .O(binval[0]));
  LUT3 #(
    .INIT(8'h96)) 
    \dest_out_bin_ff[10]_i_1 
       (.I0(\dest_graysync_ff[1] [10]),
        .I1(\dest_graysync_ff[1] [12]),
        .I2(\dest_graysync_ff[1] [11]),
        .O(binval[10]));
  LUT2 #(
    .INIT(4'h6)) 
    \dest_out_bin_ff[11]_i_1 
       (.I0(\dest_graysync_ff[1] [11]),
        .I1(\dest_graysync_ff[1] [12]),
        .O(binval[11]));
  LUT2 #(
    .INIT(4'h6)) 
    \dest_out_bin_ff[1]_i_1 
       (.I0(\dest_graysync_ff[1] [1]),
        .I1(binval[2]),
        .O(binval[1]));
  LUT6 #(
    .INIT(64'h6996966996696996)) 
    \dest_out_bin_ff[2]_i_1 
       (.I0(\dest_graysync_ff[1] [2]),
        .I1(\dest_graysync_ff[1] [4]),
        .I2(\dest_graysync_ff[1] [6]),
        .I3(binval[7]),
        .I4(\dest_graysync_ff[1] [5]),
        .I5(\dest_graysync_ff[1] [3]),
        .O(binval[2]));
  LUT5 #(
    .INIT(32'h96696996)) 
    \dest_out_bin_ff[3]_i_1 
       (.I0(\dest_graysync_ff[1] [3]),
        .I1(\dest_graysync_ff[1] [5]),
        .I2(binval[7]),
        .I3(\dest_graysync_ff[1] [6]),
        .I4(\dest_graysync_ff[1] [4]),
        .O(binval[3]));
  LUT4 #(
    .INIT(16'h6996)) 
    \dest_out_bin_ff[4]_i_1 
       (.I0(\dest_graysync_ff[1] [4]),
        .I1(\dest_graysync_ff[1] [6]),
        .I2(binval[7]),
        .I3(\dest_graysync_ff[1] [5]),
        .O(binval[4]));
  LUT3 #(
    .INIT(8'h96)) 
    \dest_out_bin_ff[5]_i_1 
       (.I0(\dest_graysync_ff[1] [5]),
        .I1(binval[7]),
        .I2(\dest_graysync_ff[1] [6]),
        .O(binval[5]));
  LUT2 #(
    .INIT(4'h6)) 
    \dest_out_bin_ff[6]_i_1 
       (.I0(\dest_graysync_ff[1] [6]),
        .I1(binval[7]),
        .O(binval[6]));
  LUT6 #(
    .INIT(64'h6996966996696996)) 
    \dest_out_bin_ff[7]_i_1 
       (.I0(\dest_graysync_ff[1] [7]),
        .I1(\dest_graysync_ff[1] [9]),
        .I2(\dest_graysync_ff[1] [11]),
        .I3(\dest_graysync_ff[1] [12]),
        .I4(\dest_graysync_ff[1] [10]),
        .I5(\dest_graysync_ff[1] [8]),
        .O(binval[7]));
  LUT5 #(
    .INIT(32'h96696996)) 
    \dest_out_bin_ff[8]_i_1 
       (.I0(\dest_graysync_ff[1] [8]),
        .I1(\dest_graysync_ff[1] [10]),
        .I2(\dest_graysync_ff[1] [12]),
        .I3(\dest_graysync_ff[1] [11]),
        .I4(\dest_graysync_ff[1] [9]),
        .O(binval[8]));
  LUT4 #(
    .INIT(16'h6996)) 
    \dest_out_bin_ff[9]_i_1 
       (.I0(\dest_graysync_ff[1] [9]),
        .I1(\dest_graysync_ff[1] [11]),
        .I2(\dest_graysync_ff[1] [12]),
        .I3(\dest_graysync_ff[1] [10]),
        .O(binval[9]));
  FDRE \dest_out_bin_ff_reg[0] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(binval[0]),
        .Q(dest_out_bin[0]),
        .R(1'b0));
  FDRE \dest_out_bin_ff_reg[10] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(binval[10]),
        .Q(dest_out_bin[10]),
        .R(1'b0));
  FDRE \dest_out_bin_ff_reg[11] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(binval[11]),
        .Q(dest_out_bin[11]),
        .R(1'b0));
  FDRE \dest_out_bin_ff_reg[12] 
       (.C(dest_clk),
        .CE(1'b1),
        .D(\dest_graysync_ff[1] [12]),
        .Q(dest_out_bin[12]),
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
        .D(binval[9]),
        .Q(dest_out_bin[9]),
        .R(1'b0));
  (* SOFT_HLUTNM = "soft_lutpair4" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \src_gray_ff[0]_i_1 
       (.I0(src_in_bin[1]),
        .I1(src_in_bin[0]),
        .O(gray_enc[0]));
  (* SOFT_HLUTNM = "soft_lutpair9" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \src_gray_ff[10]_i_1 
       (.I0(src_in_bin[11]),
        .I1(src_in_bin[10]),
        .O(gray_enc[10]));
  (* SOFT_HLUTNM = "soft_lutpair9" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \src_gray_ff[11]_i_1 
       (.I0(src_in_bin[12]),
        .I1(src_in_bin[11]),
        .O(gray_enc[11]));
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
  (* SOFT_HLUTNM = "soft_lutpair8" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \src_gray_ff[8]_i_1 
       (.I0(src_in_bin[9]),
        .I1(src_in_bin[8]),
        .O(gray_enc[8]));
  (* SOFT_HLUTNM = "soft_lutpair8" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \src_gray_ff[9]_i_1 
       (.I0(src_in_bin[10]),
        .I1(src_in_bin[9]),
        .O(gray_enc[9]));
  FDRE \src_gray_ff_reg[0] 
       (.C(src_clk),
        .CE(1'b1),
        .D(gray_enc[0]),
        .Q(async_path[0]),
        .R(1'b0));
  FDRE \src_gray_ff_reg[10] 
       (.C(src_clk),
        .CE(1'b1),
        .D(gray_enc[10]),
        .Q(async_path[10]),
        .R(1'b0));
  FDRE \src_gray_ff_reg[11] 
       (.C(src_clk),
        .CE(1'b1),
        .D(gray_enc[11]),
        .Q(async_path[11]),
        .R(1'b0));
  FDRE \src_gray_ff_reg[12] 
       (.C(src_clk),
        .CE(1'b1),
        .D(src_in_bin[12]),
        .Q(async_path[12]),
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
        .D(gray_enc[9]),
        .Q(async_path[9]),
        .R(1'b0));
endmodule

(* DEST_SYNC_FF = "4" *) (* INIT_SYNC_FF = "0" *) (* ORIG_REF_NAME = "xpm_cdc_single" *) 
(* SIM_ASSERT_CHK = "0" *) (* SRC_INPUT_REG = "0" *) (* VERSION = "0" *) 
(* XPM_MODULE = "TRUE" *) (* is_du_within_envelope = "true" *) (* keep_hierarchy = "true" *) 
(* xpm_cdc = "SINGLE" *) 
module spi_read_fifo_xpm_cdc_single
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
module spi_read_fifo_xpm_cdc_single__2
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
`pragma protect encoding = (enctype = "BASE64", line_length = 76, bytes = 152992)
`pragma protect data_block
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
SF+1Hw==
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
