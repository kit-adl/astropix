/*
 * ATLASPix3_SoftAndFirmware
 * Copyright (C) 2019  Rudolf Schimassek (rudolf.schimassek@kit.edu)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
`timescale 1ns / 1ps
/*********************************************************
 * Module to wrap the ethernet communication between the *
 * FPGA and the readout computer via UDP                 *
 * It is based on the "verilog-ethernet" project of      *
 *    alexforencich hosted on github                     *
 *                                                       *
 *  Author: Rudolf Schimassek                            *
 *  Version: 1.0 (19.06.19)                              *
 *           1.1 (08.07.19)                              *
 *              added input for sending packages not as  *
 *                answer to a received package           *                             
 *           1.2 (09.07.19)                              *
 *              added a 125MHz clock output for writing  *
 *                data to the FIFO (result of actual     *
 *                testing)                               *                           
 *********************************************************/
 
 
module ethernet_udp_com(
    input  wire clk100mhz,
    input  wire reset,
    
    output wire FIFOclk,
    /*
     * Ethernet: 1000BASE-T RGMII
     */
    input  wire       phy_rx_clk,
    input  wire [3:0] phy_rxd,
    input  wire       phy_rx_ctl,
    output wire       phy_tx_clk,
    output wire [3:0] phy_txd,
    output wire       phy_tx_ctl,
    output wire       phy_reset_n,
    input  wire       phy_int_n,
    input  wire       phy_pme_n,
    /*
     * Address configuration:
     */
    input wire [47:0] local_mac,   // = 48'h02_00_00_00_00_00;
    input wire [31:0] local_ip,    // = {8'd192, 8'd168, 8'd1,   8'd128};
    input wire [31:0] gateway_ip,  // = {8'd192, 8'd168, 8'd1,   8'd1};
    input wire [31:0] subnet_mask, // = {8'd255, 8'd255, 8'd255, 8'd0};
    input wire [15:0] port,

    /*
     *  Data Output (receive from Ethernet)
     */
    output wire [7:0] rec_data,
    output wire       rec_valid,
    input  wire       rec_ready,
    output wire       rec_lastbyte,
     
    /*
     * Data Input (send to Ethernet)
     */
    input  wire [7:0] send_data,
    input  wire       send_valid,
    output wire       send_ready,
    input  wire       send_lastbyte,
    input  wire       send_package,
    
    output wire       pll_lock
);

// Clock and reset

wire clk_ibufg;
wire clk_bufg;
wire clk_mmcm_out;

// Internal 125 MHz clock
wire clk_int;
wire rst_int;   

wire mmcm_rst = reset;
wire mmcm_locked;
wire mmcm_clkfb;

// use an external IBUFG as the clock might be used somewhere else in the project:
//IBUFG
//clk_ibufg_inst(
//    .I(clk100mhz),
//    .O(clk_ibufg)
//);
assign clk_ibufg = clk100mhz;

wire clk90_mmcm_out;
wire clk90_int;

wire clk_200_mmcm_out;
wire clk_200_int;

// MMCM instance
// 100 MHz in, 125 MHz out
// PFD range: 10 MHz to 550 MHz
// VCO range: 600 MHz to 1200 MHz
// M = 10, D = 1 sets Fvco = 1000 MHz (in range)
// Divide by 8 to get output frequency of 125 MHz
// Need two 125 MHz outputs with 90 degree offset
// Also need 200 MHz out for IODELAY
// 1000 / 5 = 200 MHz
MMCME2_BASE #(
    .BANDWIDTH("OPTIMIZED"),
    .CLKOUT0_DIVIDE_F(8),
    .CLKOUT0_DUTY_CYCLE(0.5),
    .CLKOUT0_PHASE(0),
    .CLKOUT1_DIVIDE(8),
    .CLKOUT1_DUTY_CYCLE(0.5),
    .CLKOUT1_PHASE(90),
    .CLKOUT2_DIVIDE(5),
    .CLKOUT2_DUTY_CYCLE(0.5),
    .CLKOUT2_PHASE(0),
    .CLKOUT3_DIVIDE(1),
    .CLKOUT3_DUTY_CYCLE(0.5),
    .CLKOUT3_PHASE(0),
    .CLKOUT4_DIVIDE(1),
    .CLKOUT4_DUTY_CYCLE(0.5),
    .CLKOUT4_PHASE(0),
    .CLKOUT5_DIVIDE(1),
    .CLKOUT5_DUTY_CYCLE(0.5),
    .CLKOUT5_PHASE(0),
    .CLKOUT6_DIVIDE(1),
    .CLKOUT6_DUTY_CYCLE(0.5),
    .CLKOUT6_PHASE(0),
    .CLKFBOUT_MULT_F(10),
    .CLKFBOUT_PHASE(0),
    .DIVCLK_DIVIDE(1),
    .REF_JITTER1(0.010),
    .CLKIN1_PERIOD(10.0),
    .STARTUP_WAIT("FALSE"),
    .CLKOUT4_CASCADE("FALSE")
)
clk_mmcm_inst (
    .CLKIN1(clk_ibufg),
    .CLKFBIN(mmcm_clkfb),
    .RST(mmcm_rst),
    .PWRDWN(1'b0),
    .CLKOUT0(clk_mmcm_out),
    .CLKOUT0B(),
    .CLKOUT1(clk90_mmcm_out),
    .CLKOUT1B(),
    .CLKOUT2(clk_200_mmcm_out),
    .CLKOUT2B(),
    .CLKOUT3(),
    .CLKOUT3B(),
    .CLKOUT4(),
    .CLKOUT5(),
    .CLKOUT6(),
    .CLKFBOUT(mmcm_clkfb),
    .CLKFBOUTB(),
    .LOCKED(mmcm_locked)
);

assign pll_lock = mmcm_locked;

BUFG
clk_bufg_inst (
    .I(clk_mmcm_out),
    .O(clk_int)
);
assign FIFOclk = clk_int;

BUFG
clk90_bufg_inst (
    .I(clk90_mmcm_out),
    .O(clk90_int)
);

BUFG
clk_200_bufg_inst (
    .I(clk_200_mmcm_out),
    .O(clk_200_int)
);

sync_reset #(
    .N(4)
)
sync_reset_inst (
    .clk(clk_int),
    .rst(~mmcm_locked),
    .sync_reset_out(rst_int)
);

// IODELAY elements for RGMII interface to PHY
wire [3:0] phy_rxd_delay;
wire       phy_rx_ctl_delay;

IDELAYCTRL
idelayctrl_inst
(
    .REFCLK(clk_200_int),
    .RST(rst_int),
    .RDY()
);

IDELAYE2 #(
    .IDELAY_TYPE("FIXED")
)
phy_rxd_idelay_0
(
    .IDATAIN(phy_rxd[0]),
    .DATAOUT(phy_rxd_delay[0]),
    .DATAIN(1'b0),
    .C(1'b0),
    .CE(1'b0),
    .INC(1'b0),
    .CINVCTRL(1'b0),
    .CNTVALUEIN(5'd0),
    .CNTVALUEOUT(),
    .LD(1'b0),
    .LDPIPEEN(1'b0),
    .REGRST(1'b0)
);

IDELAYE2 #(
    .IDELAY_TYPE("FIXED")
)
phy_rxd_idelay_1
(
    .IDATAIN(phy_rxd[1]),
    .DATAOUT(phy_rxd_delay[1]),
    .DATAIN(1'b0),
    .C(1'b0),
    .CE(1'b0),
    .INC(1'b0),
    .CINVCTRL(1'b0),
    .CNTVALUEIN(5'd0),
    .CNTVALUEOUT(),
    .LD(1'b0),
    .LDPIPEEN(1'b0),
    .REGRST(1'b0)
);

IDELAYE2 #(
    .IDELAY_TYPE("FIXED")
)
phy_rxd_idelay_2
(
    .IDATAIN(phy_rxd[2]),
    .DATAOUT(phy_rxd_delay[2]),
    .DATAIN(1'b0),
    .C(1'b0),
    .CE(1'b0),
    .INC(1'b0),
    .CINVCTRL(1'b0),
    .CNTVALUEIN(5'd0),
    .CNTVALUEOUT(),
    .LD(1'b0),
    .LDPIPEEN(1'b0),
    .REGRST(1'b0)
);

IDELAYE2 #(
    .IDELAY_TYPE("FIXED")
)
phy_rxd_idelay_3
(
    .IDATAIN(phy_rxd[3]),
    .DATAOUT(phy_rxd_delay[3]),
    .DATAIN(1'b0),
    .C(1'b0),
    .CE(1'b0),
    .INC(1'b0),
    .CINVCTRL(1'b0),
    .CNTVALUEIN(5'd0),
    .CNTVALUEOUT(),
    .LD(1'b0),
    .LDPIPEEN(1'b0),
    .REGRST(1'b0)
);

IDELAYE2 #(
    .IDELAY_TYPE("FIXED")
)
phy_rx_ctl_idelay
(
    .IDATAIN(phy_rx_ctl),
    .DATAOUT(phy_rx_ctl_delay),
    .DATAIN(1'b0),
    .C(1'b0),
    .CE(1'b0),
    .INC(1'b0),
    .CINVCTRL(1'b0),
    .CNTVALUEIN(5'd0),
    .CNTVALUEOUT(),
    .LD(1'b0),
    .LDPIPEEN(1'b0),
    .REGRST(1'b0)
);

fpga_core
core_inst (
    /*
     * Clock: 125MHz
     * Synchronous reset
     */
    .clk(clk_int),
    .clk90(clk90_int),
    .rst(rst_int),
    
    //.led(led),
    /*
     * Ethernet: 1000BASE-T RGMII
     */
    .phy_rx_clk(phy_rx_clk),
    .phy_rxd(phy_rxd_delay),
    .phy_rx_ctl(phy_rx_ctl_delay),
    .phy_tx_clk(phy_tx_clk),
    .phy_txd(phy_txd),
    .phy_tx_ctl(phy_tx_ctl),
    .phy_reset_n(phy_reset_n),
    .phy_int_n(phy_int_n),
    .phy_pme_n(phy_pme_n),
    /*
     * Address configuration:
     */
    .local_mac(local_mac),
    .local_ip(local_ip),
    .gateway_ip(gateway_ip),
    .subnet_mask(subnet_mask),
    .port(port),
    
    /*
     * Data input from Ethernet
     */
    .rec_data(rec_data),
    .rec_valid(rec_valid),
    .rec_ready(rec_ready),
    .rec_lastbyte(rec_lastbyte),
    .rec_user(),
    
    /*
     * Data output to Ethernet
     */
    .send_data(send_data),
    .send_valid(send_valid),
    .send_ready(send_ready),
    .send_lastbyte(send_lastbyte),
    .send_user(0), //data_user),
    .send_package(send_package)
);

endmodule