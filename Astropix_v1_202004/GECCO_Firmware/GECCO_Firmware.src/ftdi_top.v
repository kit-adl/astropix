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
 *
 * This module was initially developed by Felix Ehrler and Richard Leys
 */
`timescale 1ns / 1ps
module ftdi_top (
    input  wire			clk,
    input  wire 		res_n,
    input  wire 		prog_clko,
    input  wire 		FTDI_TXE_N,
    input  wire 		FTDI_RXF_N,
    output wire 		FTDI_RD_N,
    output wire 		FTDI_OE_N,
    output wire 		FTDI_WR_N,
    inout  wire [7:0] 	FTDI_DATA,
    
    output wire 		ChipConfig_Clock1,
    output wire 		ChipConfig_Clock2,
    output wire 		ChipConfig_Data,
    output wire 		ChipConfig_Load,
    output wire         ChipConfig_LdDAC,
    output wire         ChipConfig_LdConfig,
    output wire         ChipConfig_LdVDAC,
    output wire         ChipConfig_LdTDAC,
    output wire         ChipConfig_LdRow,
    output wire         ChipConfig_LdColumn,
    output wire [3:0]   ChipConfig_WrRAM,
    output wire         ChipConfig_no_sr,
    
    output wire        reset_autoreset_analog,
    input  wire        reset_regulator_reset_out,
    output wire        reset_reset_analog_b,
    output wire        reset_autoreset_digital,
    input  wire        reset_por,
    output wire        reset_reset_digital_b,
    input  wire        reset_por_test_reset,
    output wire        reset_autoreset_combine,
    
    output wire        config_mode_use_spi,
    output wire        config_mode_bypass_cmd,
    output wire        config_mode_encdr,
    output wire        config_mode_en_pll,
    output wire        config_mode_cmd_clock_invert,
    output wire        config_mode_interface_speed,
    output wire        config_mode_take_fast,
    
    output wire        work_mode_sel_ext,
    output wire        work_mode_always_enable_b,
    output wire        work_mode_untriggered_ro_en,
    output wire        work_mode_trig_ro_reset_n,
    
    output wire 		patgen_Reset,
    output wire 		patgen_Suspend,
    output wire 		patgen_writeStrobe,
    output wire [7:0] 	patgen_address,
    output wire [7:0] 	patgen_data,
	output wire 		injection_gecco,
	output wire 		injection_chip,
	output wire 		patgen_synced,
	output wire         patgen_tsoverflow_sync,
	output wire [7:0]   patgen_skipsignals,
    
    output wire [7:0] 	fastreadout_data,
    //input wire 		fastreadout_data_hw_write,
    //input wire [7:0] 	fastreadout_data_hw,
    output wire 		fastreadout_control_enable,
    output wire 		fastreadout_control_fifoclear,
    output wire 		fastreadout_control_rst,
    output wire 		fastreadout_control_trigger,
    output wire         fastreadout_control_debug,
    output wire         fastreadout_control_datamux,
    output wire         fastreadout_control_realign,
    output wire 		fastreadout_status_empty,
    output wire 		fastreadout_status_full,
    output wire 		fastreadout_status_Datasetstart,
    input  wire 		fastreadout_status_hw_write,
    //input  wire 		fastreadout_status_Datasetstart_hw,
    //input  wire 		fastreadout_status_full_hw,
    //input  wire 		fastreadout_status_empty_hw,
    input  wire 	   	fastreadout_fifo_reset,
    input  wire [63:0] 	fastreadout_fifo_din,
    input  wire 	   	fastreadout_fifo_wr_en,
    input  wire 	   	fastreadout_fifo_wr_clk,
    output wire [7:0]  	fastreadout_fifo_dout,
    output wire 	   	fastreadout_fifo_full,
    output wire 	   	fastreadout_fifo_empty,
    output wire        	fastreadout_fifo_progfull,
    output wire [7:0]  	fastreadout_trig_delay,
    output wire [7:0]  	fastreadout_trig_window,
    output wire [7:0]  	fastreadout_clockspeed,
    output wire [7:0]  	fastreadout_deser_delay,
    output wire [7:0]   fastreadout_tsdiv,
    output wire [7:0]   fastreadout_ts2div,
    output wire [7:0]   fastreadout_tsphase,
    
    output wire [15:0]  fastreadout_trigger_numsignals,
    output wire [7:0]   fastreadout_trigger_length,
    output wire [15:0]  fastreadout_trigger_distance,
    output wire [15:0]  fastreadout_trigger_initdelay,
    output wire         fastreadout_trigger_synced,
    
    output wire 		VoltageBoard_Clock,
    output wire 		VoltageBoard_Data,
    output wire 		VoltageBoard_Load,
	
    output wire [47:0] 	ethernet_mac,
    output wire [31:0] 	ethernet_ip,
    output wire [31:0] 	ethernet_subnetmask,
    output wire [31:0] 	ethernet_gateway,
    output wire [15:0] 	ethernet_port,
    output wire [7:0]  	ethernet_config,
    output wire [15:0] 	ethernet_delay,
    output wire [23:0] 	ethernet_timeout,
    
    output wire        	trigger_edge,
    output wire        	trigger_FTDI_trigger,
    output wire [2:0]  	trigger_source,
    output wire [2:0]  	trigger_noise_suppression,
    output wire [23:0] 	trigger_delay,
    output wire [23:0] 	trigger_duration,
    output wire         triggerid_reset,
    
    output wire        spi_config_reset,
    output wire [7:0]  spi_clock_divider,
    output wire [7:0]  spi_write_fifo_dout,
    input  wire        spi_write_fifo_rd_clk,
    input  wire        spi_write_fifo_rd_en,
    output wire        spi_write_fifo_empty,
    
    input  wire [63:0] spi_read_fifo_din,
    input  wire        spi_read_fifo_wr_clk,
    input  wire        spi_read_fifo_wr_en,
    output wire        spi_read_fifo_full,
    output wire        spi_config_readback_en,
    
    output wire        cmd_enable,
    output wire [7:0]  cmd_data,
    input  wire        cmd_rd_clk,
    input  wire        cmd_rd_en,
    output wire        cmd_fifo_empty,
    output wire        cmd_fifo_6entries,
    output wire        cmd_reset,
    
    output wire [7:0] 	ordersorter_data
);
    // Parametersrs
    //---------------


    // Signaling
    //---------------
    wire  [7:0] rfg_read_data;
    wire  rfg_read_done;


    wire [3:0] ordersorter_state;



//-- FIFO Connection
wire            wi_clk;
wire            wi_wr;
wire    [7:0]   wi_data;
wire            wi_full;
wire            wi_almost_full;

//-- FIFO interface for reading data from the FTDI
wire            ri_read;
wire    [7:0]   ri_data;
wire            ri_empty;
wire            ri_almost_empty;

// Assigments
//---------------
assign ordersorter_data = {ordersorter_state [3:0], ri_data [3:0]};


//-- Order Sorter
wire [7:0] ordersorter_header;
wire [7:0] ordersorter_address;
wire [15:0] ordersorter_length;
wire [7:0] ordersorter_value;
wire ordersorter_read;
wire  ordersorter_write;
wire  writeFIFO_prog_full;

OrderSorter ordersorter_I (
    .ri_data(ri_data),
    .ri_empty(ri_empty),
    .ri_read(ri_read),
    .pcreadfifofull(writeFIFO_prog_full),
    .clk(clk),
    .res_n(res_n),
    .header(ordersorter_header),
    .address(ordersorter_address),
    .length(ordersorter_length),
    .value(ordersorter_value),
    .read(ordersorter_read),
    .write(ordersorter_write),
    .state(ordersorter_state [3:0])
);

//Parameter definition
parameter   PRIORITY = 1'b0; // 1'b0 = read priority,  1'b1 = write priority



//wire and register definitions

//fifo control signals
wire            writeFIFO_almost_empty;
wire            writeFIFO_empty;
wire            writeFIFO_rd_en;
wire            readFIFO_almost_full;
//wire            readFIFO_full;
wire            readFIFO_wr_en;

//wires for connecting the FIFO data busses to the IO buffer
wire    [7:0]   data2ftdi;
wire    [7:0]   data2fifo;


//Asynchronous assignments
assign writeFIFO_rd_en  = ~FTDI_TXE_N && ~FTDI_WR_N;
assign readFIFO_wr_en   = ~FTDI_RXF_N && ~FTDI_RD_N;
 
// FIFO for writing to the FTDI
//----------------------
async_fifo_ftdi writeFIFO_I (
	.rd_clk(prog_clko),
	.rd_en(writeFIFO_rd_en || ~res_n),
	.rst(~res_n),
    .dout(data2ftdi),
	
    // Write to FIFO when there is a read_done from RFG
    .wr_clk(clk),
	.wr_en(rfg_read_done),
    .din(rfg_read_data),
	.almost_empty(writeFIFO_almost_empty),
	
	.empty(writeFIFO_empty),
	.full(wi_full),
	.almost_full(wi_almost_full),
	.prog_full(writeFIFO_prog_full)
);


//FIFO for reading data from the FTDI
//---------------------
async_fifo_ftdi readFIFO_I (
	.din(data2fifo),
	.rd_clk(clk),
	.rd_en(ri_read),
	.rst(~res_n),
	.wr_clk(prog_clko),
	.wr_en(readFIFO_wr_en),
	.almost_empty(ri_almost_empty),
	.almost_full(readFIFO_almost_full),
	.dout(ri_data),
	.empty(ri_empty),
	.full()
);

//controlling FSM
ftdi_interface_control_fsm fsm_I ( 
    .clk(prog_clko), 
    .res_n(res_n), 
    .rxf_n(FTDI_RXF_N), 
    .txe_n(FTDI_TXE_N), 
    .rf_almost_full(readFIFO_almost_full), 
    .wf_almost_empty(writeFIFO_almost_empty), 
    .wf_empty(writeFIFO_empty), 
    .prio(PRIORITY), 
    .rd_n(FTDI_RD_N), 
    .oe_n(FTDI_OE_N), 
    .wr_n(FTDI_WR_N)
);

//IO Buffer for the bidirectional data bus to the FTDI
genvar i;
generate
    for (i = 0; i < 8; i = i + 1) begin: buffer
        IOBUF ftdi_data_obuft_I (
            .I(data2ftdi[i]),
            .O(data2fifo[i]),
            .T(~FTDI_OE_N),
            .IO(FTDI_DATA[i])
        );
    end
endgenerate



assign patgen_address[7:4] = 0;

    // Instances
    //---------------
RegisterFile rfg_I (
    .clk(clk),
    .res_n(res_n),
    .read(ordersorter_read),
    .read_data(rfg_read_data),
    .write(ordersorter_write),
    .write_data(ordersorter_value),
    .done(rfg_read_done),
    .address(ordersorter_address[7:0]),
    
    .ChipConfig_Clock1(ChipConfig_Clock1),
    .ChipConfig_Clock2(ChipConfig_Clock2),
    .ChipConfig_Data(ChipConfig_Data),
    .ChipConfig_Load(ChipConfig_Load),
    .ChipConfig_LdDAC(ChipConfig_LdDAC),
    .ChipConfig_LdConfig(ChipConfig_LdConfig),
    .ChipConfig_LdVDAC(ChipConfig_LdVDAC),
    .ChipConfig_LdTDAC(ChipConfig_LdTDAC),
    .ChipConfig_LdRow(ChipConfig_LdRow),
    .ChipConfig_LdColumn(ChipConfig_LdColumn),
    .ChipConfig_WrRAM(ChipConfig_WrRAM),
    .ChipConfig_no_sr(ChipConfig_no_sr),
    
    .reset_autoreset_analog(reset_autoreset_analog),      
    .reset_regulator_reset_out(reset_regulator_reset_out),
    .reset_reset_analog_b(reset_reset_analog_b),
    .reset_autoreset_digital(reset_autoreset_digital),
    .reset_por(reset_por),
    .reset_reset_digital_b(reset_reset_digital_b),
    .reset_por_test_reset(reset_por_test_reset),
    .reset_autoreset_combine(reset_autoreset_combine),
                                 
    .config_mode_use_spi(config_mode_use_spi),
    .config_mode_bypass_cmd(config_mode_bypass_cmd),
    .config_mode_encdr(config_mode_encdr),
    .config_mode_en_pll(config_mode_en_pll),
    .config_mode_cmd_clock_invert(config_mode_cmd_clock_invert),
    .config_mode_interface_speed(config_mode_interface_speed),
    .config_mode_take_fast(config_mode_take_fast),
    
    .work_mode_sel_ext(work_mode_sel_ext),
    .work_mode_always_enable_b(work_mode_always_enable_b),
    .work_mode_untriggered_ro_en(work_mode_untriggered_ro_en),
    .work_mode_trig_ro_reset_n(work_mode_trig_ro_reset_n),
    
    .patgen_Reset(patgen_Reset),
    .patgen_Suspend(patgen_Suspend),
    .patgen_writeStrobe(patgen_writeStrobe),
    .patgen_address(patgen_address[3:0]),
    .patgen_data(patgen_data),
	.patgen_synced(patgen_synced),
	.injection_gecco(injection_gecco),
	.injection_chip(injection_chip),
	.patgen_tsoverflow_sync(patgen_tsoverflow_sync),
	.patgen_skipsignals(patgen_skipsignals),
    
    .fastreadout_data(fastreadout_data),
    //.fastreadout_data_hw(fastreadout_data_hw),
    .fastreadout_control_enable(fastreadout_control_enable),
    .fastreadout_control_fifoclear(fastreadout_control_fifoclear),
    .fastreadout_control_rst(fastreadout_control_rst),
    .fastreadout_control_trigger(fastreadout_control_trigger),
    .fastreadout_control_debug(fastreadout_control_debug),
    .fastreadout_control_datamux(fastreadout_control_datamux),
    .fastreadout_control_realign(fastreadout_control_realign),
    .fastreadout_status_empty(fastreadout_status_empty),
    .fastreadout_status_full(fastreadout_status_full),
    .fastreadout_status_Datasetstart(fastreadout_status_Datasetstart),
    .fastreadout_status_hw_write(fastreadout_status_hw_write),
    //.fastreadout_status_Datasetstart_hw(fastreadout_status_Datasetstart_hw),
    //.fastreadout_status_full_hw(fastreadout_status_full_hw),
    //.fastreadout_status_empty_hw(fastreadout_status_empty_hw),
    .fastreadout_fifo_reset(fastreadout_fifo_reset),
    .fastreadout_fifo_din(fastreadout_fifo_din),
    .fastreadout_fifo_wr_en(fastreadout_fifo_wr_en),
    .fastreadout_fifo_wr_clk(fastreadout_fifo_wr_clk),
    .fastreadout_fifo_dout(fastreadout_fifo_dout),
    .fastreadout_fifo_full(fastreadout_fifo_full),
    .fastreadout_fifo_empty(fastreadout_fifo_empty),
    .fastreadout_fifo_progfull(fastreadout_fifo_progfull),
    .fastreadout_fifo_rd_en(ordersorter_read),
    .fastreadout_trig_delay(fastreadout_trig_delay[7:0]),
    .fastreadout_trig_window(fastreadout_trig_window[7:0]),
    .fastreadout_clockspeed(fastreadout_clockspeed[7:0]),
    .fastreadout_deser_delay(fastreadout_deser_delay[7:0]),
    .fastreadout_tsdiv(fastreadout_tsdiv),
    .fastreadout_ts2div(fastreadout_ts2div),
    .fastreadout_tsphase(fastreadout_tsphase),
    
    .fastreadout_trigger_numsignals(fastreadout_trigger_numsignals),
    .fastreadout_trigger_length(fastreadout_trigger_length),
    .fastreadout_trigger_distance(fastreadout_trigger_distance),
    .fastreadout_trigger_initdelay(fastreadout_trigger_initdelay),
    .fastreadout_trigger_synced(fastreadout_trigger_synced),
    
    .VoltageBoard_Clock(VoltageBoard_Clock),
    .VoltageBoard_Data(VoltageBoard_Data),
    .VoltageBoard_Load(VoltageBoard_Load),
    
    .ethernet_mac(ethernet_mac),
    .ethernet_ip(ethernet_ip),
    .ethernet_subnetmask(ethernet_subnetmask),
    .ethernet_gateway(ethernet_gateway),
    .ethernet_port(ethernet_port),
    .ethernet_config(ethernet_config),
    .ethernet_delay(ethernet_delay),
    .ethernet_timeout(ethernet_timeout),
    
    .trigger_edge(trigger_edge),
    .trigger_FTDI_trigger(trigger_FTDI_trigger),
    .trigger_source(trigger_source),
    .trigger_noise_suppression(trigger_noise_suppression),
    .trigger_delay(trigger_delay),
    .trigger_duration(trigger_duration),
    .triggerid_reset(triggerid_reset),
    
    .spi_config_reset(spi_config_reset),
    .spi_clock_divider(spi_clock_divider),
    .spi_write_fifo_dout(spi_write_fifo_dout),
    .spi_write_fifo_rd_clk(spi_write_fifo_rd_clk),
    .spi_write_fifo_rd_en(spi_write_fifo_rd_en),
    .spi_write_fifo_empty(spi_write_fifo_empty),
    
    .spi_read_fifo_din(spi_read_fifo_din),
    .spi_read_fifo_wr_clk(spi_read_fifo_wr_clk),
    .spi_read_fifo_wr_en(spi_read_fifo_wr_en),
    .spi_read_fifo_full(spi_read_fifo_full),
    .spi_read_fifo_rd_en(ordersorter_read),
    .spi_config_readback_en(spi_config_readback_en),
    
    .cmd_enable(cmd_enable),    
    .cmd_data(cmd_data),
    .cmd_rd_clk(cmd_rd_clk),    
    .cmd_rd_en(cmd_rd_en),
    .cmd_fifo_empty(cmd_fifo_empty),
    .cmd_fifo_6entries(cmd_fifo_6entries),
    .cmd_reset(cmd_reset)
);




endmodule
