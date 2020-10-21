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

//////////////////////////////////////////////////////////////////////
//  Verilog Module: Ethernet_Top									//
//	Author: Rudolf Schimassek										//
//  Version: 0.9 (01.08.19)											//
//																	//
//  This module hides the communication over ethernet using UDP		//
//  protocol from the rest of the FPGA code. It also implements		//
//  a debugging mode for the ethernet using the answerbot module.	//
//	The debugging is stopped with an input (debug_mode_n).			//
//  																//
//  Internally, the data to send off the FPGA is written to a FIFO	//
//  crossing the clock domain border to the 125MHz of the UDP 		//
//	module and there it is passed to the UDP transmission module	//
//	into a special FIFO by the ethernet_data_sender from where the	//
//	UDP module takes over, packages the data and sends it over the  //
//	ethernet PHY.													//
//																	//
//	TODOs:															//
//		- add a FIFO for receiving data via ethernet for 		 	//
//			processing on the FPGA									//
//////////////////////////////////////////////////////////////////////

module ethernet_top
(
	input  wire 	   clk100mhz,
	input  wire 	   reset,
	input  wire		   reset_data_sender,
	input  wire		   reset_send_fifo,
	output wire 	   clk125mhz,
	
	/*
     * Ethernet: 1000BASE-T RGMII
     */
    input  wire        phy_rx_clk,
    input  wire [3:0]  phy_rxd,
    input  wire        phy_rx_ctl,
    output wire        phy_tx_clk,
    output wire [3:0]  phy_txd,
    output wire        phy_tx_ctl,
    output wire        phy_reset_n,
    input  wire        phy_int_n,
    input  wire        phy_pme_n,
    /*
     * Address configuration:
     */
    input  wire [47:0] local_mac,   // = 48'h02_00_00_00_00_00;
    input  wire [31:0] local_ip,    // = {8'd192, 8'd168, 8'd1,   8'd128};
    input  wire [31:0] subnet_mask, // = {8'd255, 8'd255, 8'd255, 8'd0};
    input  wire [31:0] gateway_ip,  // = {8'd192, 8'd168, 8'd1,   8'd1};
    input  wire [15:0] port,
	/*
	 * further configuration:
	 */
	input  wire [15:0] send_delay,	//delay before a new package can be sent to slow down sending 
									//  speed useful in case the receiving device cannot cope with 
									//  the sending speed
	input  wire [23:0] timeout,
	
	/*
	 * Debug ports:
	 */
	output wire 	   module_ready,
	input  wire  	   debug_mode_n,
	
	input  wire		   debug_send_package,		//manually send a package in debug mode
	input  wire [7:0]  debug_data_to_send,		//data to use for the debug package
	
	/*
	 * Data input for sending (buffer FIFO):
	 */
	input  wire [63:0] send_fifo_data_in,
	input  wire		   send_fifo_wr_en,
	input  wire		   send_fifo_wr_clk,
	output wire		   send_fifo_full,
	/*
	 * Data output (TODO) for data received via ethernet / UDP:
	 */
	output wire [7:0]  receive_fifo_data_out,
	input  wire 	   receive_fifo_rd_en,
	input  wire		   receive_fifo_rd_clk,
	output wire		   receive_fifo_empty
);

wire pll_lock;
assign module_ready = pll_lock && ~reset && ~debug_mode_n;

// debug mode receive data lines for UDP:
wire  [7:0] rec_data;
wire        rec_valid;
wire        rec_ready;
wire        rec_lastbyte;

// debug mode send data lines for UDP:
wire  [7:0] send_data;
wire        send_valid;
wire        send_ready;
wire        send_lastbyte;
wire        send_package;

// work mode send data lines for FIFO:
wire [7:0] send_fifo_data_out;
wire       send_fifo_rd_en;
wire       send_fifo_rd_clk;
//wire       send_fifo_full;
wire       send_fifo_empty;

// work mode send data lines for UDP:
wire [7:0] ethernet_send_dout;
wire       ethernet_send_valid;
wire       ethernet_send_ready;
wire       ethernet_send_lastbyte;
wire       ethernet_send_package;

ethernet_udp_com network(
    .clk100mhz(clk100mhz),
    .reset(reset),
    .FIFOclk(clk125mhz),
    
    .phy_rx_clk(phy_rx_clk),
    .phy_rxd(phy_rxd),
    .phy_rx_ctl(phy_rx_ctl),
    .phy_tx_clk(phy_tx_clk),
    .phy_txd(phy_txd),
    .phy_tx_ctl(phy_tx_ctl),
    .phy_reset_n(phy_reset_n),
    .phy_int_n(phy_int_n),
    .phy_pme_n(phy_pme_n),
    
    .local_mac(local_mac),
    .local_ip(local_ip),
    .gateway_ip(gateway_ip),
    .subnet_mask(subnet_mask),
    .port(port),
    
    .rec_data(rec_data),	//TODO: changes here for data receiving
    .rec_valid(rec_valid),
    .rec_ready(rec_ready),
    .rec_lastbyte(rec_lastbyte),
    
    .send_data((debug_mode_n)? ethernet_send_dout : send_data),
    .send_valid((debug_mode_n)? ethernet_send_valid : send_valid),
    .send_ready(ethernet_send_ready),
    .send_lastbyte((debug_mode_n)? ethernet_send_lastbyte : send_lastbyte),
    .send_package((debug_mode_n)? ethernet_send_package : send_package),
    
    .pll_lock(pll_lock)
);

assign send_ready = ethernet_send_ready;
    
answerbot networktester(
    .clk(clk125mhz),
    .reset(reset),
    
    .datain(rec_data),
    .datain_valid(rec_valid),
    .datain_ready(rec_ready),
    .datain_lastbyte(rec_lastbyte),
    
    .dataout(send_data),
    .dataout_valid(send_valid),
    .dataout_ready(send_ready),
    .dataout_lastbyte(send_lastbyte),
    .send(send_package),
    
    .status(),
    
    .byte(debug_data_to_send),
    .debug(debug_send_package)
);

ethernet_data_sender fastreadoutOverEthernet(
    .clk(clk125mhz),
    .reset(reset_data_sender),
    
    .packagesize(8'd127),
    .restartdelay(send_delay),
    .enable(debug_mode_n),
    .timeout(timeout),
    
    .datain_clk(send_fifo_rd_clk),
    .datain(send_fifo_data_out),
    .datain_valid(~send_fifo_empty),
    .datain_rd_en(send_fifo_rd_en),
    
    .dataout(ethernet_send_dout),
    .dataout_valid(ethernet_send_valid),
    .dataout_ready(ethernet_send_ready),
    .dataout_lastbyte(ethernet_send_lastbyte),
    .dataout_package(ethernet_send_package)
);


ethernet_fifo_buffer ethernet_fastreadout_fifo(
    .rst(reset_send_fifo),
	
    .din(send_fifo_data_in),
    .wr_en(send_fifo_wr_en),
    .wr_clk(send_fifo_wr_clk),
    .full(send_fifo_full),
	
    .dout(send_fifo_data_out),
    .rd_en(send_fifo_rd_en),
    .rd_clk(send_fifo_rd_clk),
    .empty(send_fifo_empty)
);

endmodule