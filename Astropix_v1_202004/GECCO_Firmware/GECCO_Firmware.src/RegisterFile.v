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
//////////////////////////////////////////////////////////////////////////////////
// Company:     KIT-ADL
// Engineer:    Felix Ehrler, Richard Leys
// 
// Create Date: 21.03.2016
// Design Name: 
// Module Name: H35Demo_rf
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Revision 1.0  - renamed module to "RegisterFile"
//               - simplified the structures for reading and writing
//               - removed double words from inputs/outputs
//              [02.08.2019, Rudolf Schimassek]
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////

module RegisterFile (
    input  wire        clk,
    input  wire        res_n,
    input  wire        read,
    output reg [7:0]   read_data,
    input  wire        write,
    input  wire [7:0]  write_data,
    output reg         done,
    input  wire [7:0]  address,
    
    output wire        ChipConfig_Clock1,
    output wire        ChipConfig_Clock2,
    output wire        ChipConfig_Data,
    output wire        ChipConfig_Load,
    output wire        ChipConfig_Res_n,

    output wire        ChipConfig_LdDAC,
    output wire        ChipConfig_LdConfig,
    output wire        ChipConfig_LdVDAC,
    output wire        ChipConfig_LdTDAC,
    output wire        ChipConfig_LdRow,
    output wire        ChipConfig_LdColumn,
    output wire [3:0]  ChipConfig_WrRAM,
    output wire        ChipConfig_no_sr,
    
    output wire        patgen_Reset,
    output wire        patgen_Suspend,
    output wire        patgen_writeStrobe,
    output wire [3:0]  patgen_address,
    output wire [7:0]  patgen_data,
    output wire        patgen_synced,
	output wire 	   injection_gecco,
	output wire		   injection_chip,
	output wire        patgen_tsoverflow_sync,
	output wire [7:0]  patgen_skipsignals,
	
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
    
    output reg  [7:0]  fastreadout_data,
    //input  wire [7:0]  fastreadout_data_hw,
    output wire        fastreadout_control_enable,
    output wire        fastreadout_control_fifoclear,
    output wire        fastreadout_control_rst,
    output wire        fastreadout_control_trigger,
    output wire        fastreadout_control_debug,
    output wire        fastreadout_control_datamux,
    output wire        fastreadout_control_realign,
    output wire        fastreadout_status_empty,
    output wire        fastreadout_status_full,
    output wire        fastreadout_status_Datasetstart,
    input  wire        fastreadout_status_hw_write,
    //input  wire        fastreadout_status_Datasetstart_hw,
    //input  wire        fastreadout_status_full_hw,
    //input  wire        fastreadout_status_empty_hw,
    input  wire        fastreadout_fifo_reset,
    input  wire [63:0] fastreadout_fifo_din,
    input  wire        fastreadout_fifo_wr_en,
    input  wire        fastreadout_fifo_wr_clk,
    output wire [7:0]  fastreadout_fifo_dout,
    output wire        fastreadout_fifo_full,
    output wire        fastreadout_fifo_empty,
    output wire        fastreadout_fifo_progfull,
    input  wire        fastreadout_fifo_rd_en,
    output wire [7:0]  fastreadout_trig_delay,
    output wire [7:0]  fastreadout_trig_window,
    output wire [7:0]  fastreadout_clockspeed,
    output wire [7:0]  fastreadout_deser_delay,
    output wire [7:0]  fastreadout_tsdiv,
    output wire [7:0]  fastreadout_ts2div,
    output wire [7:0]  fastreadout_tsphase,
    
    output wire [15:0] fastreadout_trigger_numsignals,
    output wire [7:0]  fastreadout_trigger_length,
    output wire [15:0] fastreadout_trigger_distance,
    output wire [15:0] fastreadout_trigger_initdelay,
    output wire        fastreadout_trigger_synced,
    
	output wire        VoltageBoard_Clock,
	output wire        VoltageBoard_Data,
	output wire        VoltageBoard_Load,
	
    output wire [47:0] ethernet_mac,
    output wire [31:0] ethernet_ip,
    output wire [31:0] ethernet_subnetmask,
    output wire [31:0] ethernet_gateway,
    output wire [15:0] ethernet_port,
    output wire [7:0]  ethernet_config,
    output wire [15:0] ethernet_delay,
    output wire [23:0] ethernet_timeout,
    
    output wire        trigger_edge,
    output wire        trigger_FTDI_trigger,
    output wire [2:0]  trigger_source,
    output wire [2:0]  trigger_noise_suppression,
    output wire [23:0] trigger_delay,
    output wire [23:0] trigger_duration,
    output wire        triggerid_reset,
    
    output wire        spi_config_reset,
    output wire [7:0]  spi_clock_divider,
    output wire [31:0]  spi_write_fifo_dout,
    input  wire        spi_write_fifo_rd_clk,
    input  wire        spi_write_fifo_rd_en,
    output wire        spi_write_fifo_empty,
    
    input  wire [63:0] spi_read_fifo_din,
    input  wire        spi_read_fifo_wr_clk,
    input  wire        spi_read_fifo_wr_en,
    output wire        spi_read_fifo_full,
    input  wire        spi_read_fifo_rd_en,  //connect to ordersorter_read
    output wire        spi_config_readback_en,
    
    output wire        cmd_enable,
    output wire [7:0]  cmd_data,
    input  wire        cmd_rd_clk,
    input  wire        cmd_rd_en,
    output wire        cmd_fifo_empty,
    output wire        cmd_fifo_6entries,
    output wire        cmd_reset
);


//---------------
// Signaling
//---------------
reg   [7:0]  ChipConfig;
reg   [7:0]  ChipConfig_Lds = 8'b0000_0000;
reg   [7:0]  ChipConfig_RAMwr;
reg   [7:0]  reset_reg;
wire  [7:0]  reset_wire;
reg   [7:0]  config_mode;
reg   [7:0]  work_mode;

reg   [7:0]  fastreadout_control;
reg   [7:0]  fastreadout_status;
wire  [7:0]  fastreadout_status_hw;
reg   [7:0]  fastreadout_tsdiv_reg;
reg   [7:0]  fastreadout_ts2div_reg;
reg   [7:0]  fastreadout_tsphase_reg;
reg   [7:0]  VoltageBoard;
reg   [7:0]  patgen_address_reg;
reg   [7:0]  patgen_data_reg;
reg   [7:0]  patgen_Reset_reg;
reg   [7:0]  patgen_Suspend_reg;
reg   [7:0]  patgen_writeStrobe_reg;
reg   [7:0]  patgen_config;
reg   [7:0]  patgen_skipsignals_reg;
reg   [7:0]  TrigDelay;
reg   [7:0]  TrigWindow;
reg   [7:0]  fastclkspeed;
reg   [7:0]  fastro_deser_delay;

reg   [15:0] fastreadout_trigger_numsignals_reg = 16'd0;
reg          fastreadout_trigger_numsignals_readpos = 1'b0;
reg   [7:0]  fastreadout_trigger_length_reg = 8'd1;
reg   [15:0] fastreadout_trigger_distance_reg = 16'hffff;
reg          fastreadout_trigger_distance_readpos = 1'b0;
reg   [15:0] fastreadout_trigger_initdelay_reg = 16'd0;
reg          fastreadout_trigger_initdelay_readpos = 1'b0;

reg   [47:0] ethernet_reg_mac            = 48'h02_00_00_00_00_00;
reg   [2:0]  ethernet_mac_readpos        =  3'd0;
reg   [31:0] ethernet_reg_ip             = {8'd192, 8'd168, 8'd1, 8'd128};
reg   [2:0]  ethernet_ip_readpos         =  3'd0;
reg   [31:0] ethernet_reg_gateway        = {8'd192, 8'd168, 8'd1, 8'd1};
reg   [1:0]  ethernet_gateway_readpos    =  2'd0;
reg   [31:0] ethernet_reg_subnetmask     = {8'd255, 8'd255, 8'd255, 8'd0};
reg   [1:0]  ethernet_subnetmask_readpos =  2'd0;
reg   [15:0] ethernet_reg_port           = 16'd1234;
reg   [1:0]  ethernet_port_readpos       =  2'b0;
reg   [7:0]  ethernet_reg_config         =  8'd0;
reg   [15:0] ethernet_reg_delay          = 16'd10;
reg   [1:0]  ethernet_delay_readpos      =  2'd0;
reg   [23:0] ethernet_reg_timeout        = 24'hff_ff_ff;
reg   [1:0]  ethernet_timeout_readpos    =  2'd0;

reg   [7:0]  trigger_config;
reg   [23:0] trigger_delay_reg;
reg   [1:0]  trigger_delay_readpos;
reg   [23:0] trigger_duration_reg;
reg   [1:0]  trigger_duration_readpos;
reg   [7:0]  trigger_config_2;

wire         fifo_rd_en;

reg   [2:0]  fastreadout_rdcount;
reg          fifoemptyatstart = 0;
wire         loadfromfifo;

//SPI FIFOs:
reg   [7:0]  spi_config;
wire  [7:0]  spi_config_wire;
reg   [7:0]  spi_clock_divider_reg;
//  write FIFO
wire         spi_config_wr_fifo_reset;
wire  [7:0]  spi_write_fifo_din;
wire         spi_write_fifo_wr_en;
wire         spi_write_fifo_full;
//  read FIFO
wire         spi_config_rd_fifo_reset;
wire  [7:0]  spi_read_fifo_dout;
reg   [2:0]  spi_read_fifo_rdcount;
reg          spi_read_fifo_empty_at_start = 0;
wire         spi_read_fifo_load_from_fifo;
wire         spi_read_fifo_empty;
wire         spi_read_fifo_rd_en_real;

//CMD decoder FIFO:
reg   [7:0]  cmd_config;
wire  [7:0]  cmd_config_wire;
wire         cmd_fifo_reset;
wire  [7:0]  cmd_datain;
wire         cmd_wr_en;
wire         cmd_fifo_full;

//---------------
// Assigments
//---------------
assign ChipConfig_Clock1 	 = ChipConfig[0:0];
assign ChipConfig_Clock2 	 = ChipConfig[1:1];
assign ChipConfig_Data 		 = ChipConfig[2:2];
assign ChipConfig_Load 		 = ChipConfig[3:3];
assign ChipConfig_Res_n      = ChipConfig[4:4];

assign ChipConfig_LdDAC      = ChipConfig_Lds[0];
assign ChipConfig_LdConfig   = ChipConfig_Lds[1];
assign ChipConfig_LdVDAC     = ChipConfig_Lds[2];
assign ChipConfig_LdTDAC     = ChipConfig_Lds[3];
assign ChipConfig_LdRow      = ChipConfig_Lds[4];
assign ChipConfig_LdColumn   = ChipConfig_Lds[5];
assign ChipConfig_WrRAM[3:0] = ChipConfig_RAMwr[3:0];
assign ChipConfig_no_sr      = ChipConfig_Lds[7];

assign reset_autoreset_analog  = reset_reg[0];
assign reset_reset_analog_b    = reset_reg[2];
assign reset_autoreset_digital = reset_reg[3];
assign reset_reset_digital_b   = reset_reg[5];
assign reset_autoreset_combine = reset_reg[7];
assign reset_wire = {reset_reg[7], reset_por_test_reset, reset_reg[5], reset_por, 
                        reset_reg[3:2], reset_regulator_reset_out, reset_reg[0]};
                        
assign config_mode_use_spi          = config_mode[0];         
assign config_mode_bypass_cmd       = config_mode[1];
assign config_mode_encdr            = config_mode[2];
assign config_mode_en_pll           = config_mode[3];
assign config_mode_cmd_clock_invert = config_mode[4];
assign config_mode_interface_speed  = config_mode[5];
assign config_mode_take_fast        = config_mode[6];

assign work_mode_sel_ext            = work_mode[0];
assign work_mode_always_enable_b    = work_mode[1];
assign work_mode_untriggered_ro_en  = work_mode[2];
assign work_mode_trig_ro_reset_n    = work_mode[3];

assign patgen_Reset 		     = patgen_Reset_reg[0:0];
assign patgen_Suspend 		     = patgen_Suspend_reg[0:0];
assign patgen_writeStrobe 	     = patgen_writeStrobe_reg[0:0];
assign patgen_address 		     = patgen_address_reg[3:0];
assign patgen_data 			     = patgen_data_reg[7:0];
assign injection_gecco 		     = patgen_config[0];
assign injection_chip  		     = patgen_config[1];
assign patgen_synced		     = patgen_config[2];
assign patgen_tsoverflow_sync    = patgen_config[3];
assign patgen_skipsignals        = patgen_skipsignals_reg[7:0];

assign fastreadout_control_enable 		= fastreadout_control[0:0];
assign fastreadout_control_fifoclear 	= fastreadout_control[1:1];
assign fastreadout_control_rst 			= fastreadout_control[2:2];
assign fastreadout_control_trigger 		= fastreadout_control[3:3];
assign fastreadout_control_debug        = fastreadout_control[4:4];
assign fastreadout_control_datamux      = fastreadout_control[5:5];
assign fastreadout_trigger_synced       = fastreadout_control[6:6];
assign fastreadout_control_realign      = fastreadout_control[7:7];
assign fastreadout_status_hw            = {fastreadout_status[7:3], fastreadout_fifo_full, 
                                            fastreadout_fifo_progfull, fastreadout_fifo_empty};
assign fastreadout_status_empty 		= fastreadout_fifo_empty;
assign fastreadout_status_full 			= fastreadout_fifo_progfull;
assign fastreadout_status_Datasetstart 	= (fastreadout_rdcount == 0);
assign fastreadout_trig_delay[7:0] 		= TrigDelay[7:0];
assign fastreadout_trig_window[7:0] 	= TrigWindow[7:0];
assign fastreadout_clockspeed[7:0] 		= fastclkspeed[7:0];
assign fastreadout_deser_delay[7:0] 	= fastro_deser_delay[7:0];
assign fastreadout_tsdiv                = fastreadout_tsdiv_reg;
assign fastreadout_ts2div               = fastreadout_ts2div_reg;
assign fastreadout_tsphase              = fastreadout_tsphase_reg;

assign fastreadout_trigger_numsignals   = fastreadout_trigger_numsignals_reg[15:0];
assign fastreadout_trigger_length       = fastreadout_trigger_length_reg[7:0];
assign fastreadout_trigger_distance     = fastreadout_trigger_distance_reg[15:0];
assign fastreadout_trigger_initdelay    = fastreadout_trigger_initdelay_reg[15:0];

assign VoltageBoard_Clock = VoltageBoard[0:0];
assign VoltageBoard_Data  = VoltageBoard[1:1];
assign VoltageBoard_Load  = VoltageBoard[2:2];

//  Fast Readout FIFO Readout signals
assign fifo_rd_en = fastreadout_fifo_rd_en && address == 11 && loadfromfifo;
assign loadfromfifo = (!fifoemptyatstart       && fastreadout_rdcount != 0) 
                   || (!fastreadout_fifo_empty && fastreadout_rdcount == 0);

assign ethernet_mac 			= ethernet_reg_mac;
assign ethernet_ip 				= ethernet_reg_ip;
assign ethernet_gateway 		= ethernet_reg_gateway;
assign ethernet_subnetmask 		= ethernet_reg_subnetmask;
assign ethernet_port 			= ethernet_reg_port;
assign ethernet_config 			= ethernet_reg_config;
assign ethernet_delay 			= ethernet_reg_delay;
assign ethernet_timeout[23:0] 	= ethernet_reg_timeout[23:0];

assign trigger_edge                     = trigger_config[0:0];
assign trigger_FTDI_trigger             = trigger_config[1:1];
assign trigger_source[2:0]              = trigger_config[4:2];
assign trigger_noise_suppression[2:0]   = trigger_config[7:5];
assign trigger_delay[23:0]              = trigger_delay_reg[23:0];
assign trigger_duration[23:0]           = trigger_duration_reg[23:0];
assign triggerid_reset                  = trigger_config_2[0:0];

assign spi_config_wr_fifo_reset = spi_config[0];
assign spi_config_rd_fifo_reset = spi_config[3];
assign spi_config_readback_en   = spi_config[6];
assign spi_config_reset         = spi_config[7];
assign spi_write_fifo_din = write_data;
assign spi_config_wire = {spi_config[7], spi_config[6],
                            spi_read_fifo_full, spi_read_fifo_empty, spi_config[3],
                            spi_write_fifo_full, spi_write_fifo_empty, spi_config[0]};
assign spi_clock_divider = spi_clock_divider_reg;

assign spi_write_fifo_wr_en = write & (address == 23);
assign spi_read_fifo_rd_en_real = spi_read_fifo_rd_en && address == 23 && spi_read_fifo_load_from_fifo;
assign spi_read_fifo_load_from_fifo = (!spi_read_fifo_empty_at_start && spi_read_fifo_rdcount != 0)
                                    || (!spi_read_fifo_empty && spi_read_fifo_rdcount == 0);

assign cmd_enable       = cmd_config[0];
assign cmd_fifo_reset   = cmd_config[1];
assign cmd_reset        = cmd_config[4];
assign cmd_datain       = write_data[7:0];
assign cmd_config_wire  = {cmd_config[7:5], cmd_config[4], cmd_fifo_full, cmd_fifo_empty, cmd_config[1], cmd_config[0]};
assign cmd_wr_en = write & (address == 51);

// Instances
//---------------
reg resfifo = 0;//ivan
fifo_fastreadout fastreadout_fifo(
    .rst(resfifo),//fastreadout_fifo_reset),//resfifo
    .din(fastreadout_fifo_din),
    .wr_en(fastreadout_fifo_wr_en),
    .wr_clk(fastreadout_fifo_wr_clk),
    .dout(fastreadout_fifo_dout),
    .rd_en(fifo_rd_en | fastreadout_fifo_reset),
    .rd_clk(clk),
    .full(fastreadout_fifo_full),
    .empty(fastreadout_fifo_empty),
    .prog_full(fastreadout_fifo_progfull)
);

reg res_spi_write_fifo = 0;
spi_write_fifo spi_write_fifo_i(
    .rst(res_spi_write_fifo),
    .din(spi_write_fifo_din),
    .wr_en(spi_write_fifo_wr_en),
    .wr_clk(clk),
    .full(),
    .prog_full(spi_write_fifo_full),
    .dout(spi_write_fifo_dout),
    .rd_clk(spi_write_fifo_rd_clk),
    .rd_en(spi_write_fifo_rd_en),
    .empty(spi_write_fifo_empty)
);

reg res_spi_read_fifo = 0;
spi_read_fifo spi_read_fifo_i(
    .rst(res_spi_read_fifo),
    .din(spi_read_fifo_din),
    .wr_en(spi_read_fifo_wr_en),
    .wr_clk(spi_read_fifo_wr_clk),
    .full(spi_read_fifo_full),
    .dout(spi_read_fifo_dout),
    .rd_clk(clk),
    .rd_en(spi_read_fifo_rd_en_real),
    .empty(spi_read_fifo_empty)
);

reg res_cmd_fifo = 0;
cmd_fifo cmd_fifo_i(
    .rst(res_cmd_fifo),
    .din(cmd_datain),
    .wr_en(cmd_wr_en),
    .wr_clk(clk),
    .full(cmd_fifo_full),
    .dout(cmd_data),
    .rd_clk(cmd_rd_clk),
    .rd_en(cmd_rd_en),
    .empty(cmd_fifo_empty),
    .prog_empty(cmd_fifo_6entries)
);

//FIFO reset synchronisation:
always @ clk begin
    if((!res_n) | fastreadout_fifo_reset) begin
        resfifo <= 1;
    end
    else begin
        resfifo <= 0;
    end
    if((!res_n) | spi_config_wr_fifo_reset) begin
        res_spi_write_fifo <= 1;
    end
    else begin
        res_spi_write_fifo <= 0;
    end
    if((!res_n) | spi_config_rd_fifo_reset) begin
        res_spi_read_fifo <= 1;
    end
    else begin
        res_spi_read_fifo <= 0;
    end
    if((!res_n) | cmd_fifo_reset) begin
        res_cmd_fifo <= 1;
    end
    else begin
        res_cmd_fifo <= 0;
    end
end


//------------------------     
// Reading Registers:
//------------------------
always @(posedge clk) begin
    if (~ res_n) begin
        read_data                   <= 0;
        done                        <= 0;
        fastreadout_data            <= 17;
        
        fastreadout_rdcount         <= 0;
        fifoemptyatstart            <= 0;
        
        ethernet_mac_readpos        <= 0;
        ethernet_ip_readpos         <= 0;
        ethernet_port_readpos       <= 0;
        ethernet_gateway_readpos    <= 0;
        ethernet_subnetmask_readpos <= 0;
        ethernet_delay_readpos      <= 0;
        ethernet_timeout_readpos    <= 0;
        
        trigger_delay_readpos       <= 0;
        trigger_duration_readpos    <= 0;
        
        fastreadout_trigger_numsignals_readpos <= 0;
        fastreadout_trigger_distance_readpos   <= 0;
        fastreadout_trigger_initdelay_readpos  <= 0;

        spi_read_fifo_rdcount        <= 0;
        spi_read_fifo_empty_at_start <= 0;
    end
    else begin
    if (read == 1) begin
        done <= 1;
        case(address)
            0:  read_data <= ChipConfig;
            1:  read_data <= fastclkspeed;
            2:  read_data <= patgen_Reset_reg;
            3:  read_data <= patgen_Suspend_reg;
            4:  read_data <= patgen_writeStrobe_reg;
			5:  read_data <= patgen_config;
            6:  read_data <= patgen_address_reg;
            7:  read_data <= patgen_data_reg;
            8: begin
                read_data <= fastreadout_data;
                fastreadout_data <= fastreadout_data + 8'b1;                              
            end
            9:  read_data <= fastreadout_control;
            10: read_data <= fastreadout_status_hw;
            11: begin
                fastreadout_rdcount <= fastreadout_rdcount + 3'd1;
            
                //store empty state at beginning of the data set:
                if(fastreadout_rdcount == 0)
                    fifoemptyatstart <= fastreadout_fifo_empty;
            
                //load data if it was already present at beginning of the data set:
                if(loadfromfifo)
                    read_data <= fastreadout_fifo_dout;
                else
                    read_data <= 8'hff;
            end
            12: read_data <= VoltageBoard;
            13: read_data <= TrigDelay;
            14: read_data <= TrigWindow;
            15: read_data <= fastro_deser_delay;
            16: read_data <= ChipConfig_Lds;
            17: read_data <= ChipConfig_RAMwr;
            18: read_data <= reset_wire;
            19: read_data <= config_mode;
            20: read_data <= work_mode;
            21: read_data <= spi_config_wire;
            22: read_data <= spi_clock_divider_reg;
            //23 writing FIFO, no read possible
            24: begin
                spi_read_fifo_rdcount <= spi_read_fifo_rdcount + 3'd1;
            
                //store empty state at beginning of the data set:
                if(spi_read_fifo_rdcount == 0)
                    spi_read_fifo_empty_at_start <= spi_read_fifo_empty;
            
                //load data if it was already present at beginning of the data set:
                if(spi_read_fifo_load_from_fifo)
                    read_data <= spi_read_fifo_dout;
                else
                    read_data <= 8'hff;
            end
            25: read_data <= fastreadout_tsdiv_reg;
            26: read_data <= fastreadout_ts2div_reg;
            27: read_data <= fastreadout_tsphase_reg;
            28: read_data <= patgen_skipsignals_reg;
            
            30: begin
                ethernet_ip_readpos <= ethernet_ip_readpos + 3'b1;
                case(ethernet_ip_readpos)
                    0: read_data[7:0] <= ethernet_reg_ip[31:24];
                    1: read_data[7:0] <= ethernet_reg_ip[23:16];
                    2: read_data[7:0] <= ethernet_reg_ip[15:8];
                    3: read_data[7:0] <= ethernet_reg_ip[7:0];
                    default:
                        read_data[7:0] <= {5'b0,ethernet_ip_readpos[2:0]};
                endcase
            end
            31: begin
                ethernet_port_readpos <= ethernet_port_readpos + 2'b1;
                case(ethernet_port_readpos)
                    0: read_data[7:0] <= ethernet_reg_port[15:8];
                    1: read_data[7:0] <= ethernet_reg_port[7:0];
                    default:
                        read_data[7:0] <= {6'b0, ethernet_port_readpos[1:0]};
                endcase
            end
            32: begin
                ethernet_mac_readpos <= ethernet_mac_readpos + 3'b1;
                case(ethernet_mac_readpos)
                    0: read_data[7:0] <= ethernet_reg_mac[47:40];
                    1: read_data[7:0] <= ethernet_reg_mac[39:32];
                    2: read_data[7:0] <= ethernet_reg_mac[31:24];
                    3: read_data[7:0] <= ethernet_reg_mac[23:16];
                    4: read_data[7:0] <= ethernet_reg_mac[15:8];
                    5: read_data[7:0] <= ethernet_reg_mac[7:0];
                    default:
                        read_data[7:0] <= {5'b0, ethernet_mac_readpos[2:0]};
                endcase
            end
            33: begin
                read_data[7:0] <= ethernet_reg_config[7:0];
            end
            34: begin
                ethernet_delay_readpos <= ethernet_delay_readpos + 2'b1;
                case(ethernet_delay_readpos)
                    0: read_data <= ethernet_reg_delay[15:8];    
                    1: read_data <= ethernet_reg_delay[7:0];
                    default:
                        read_data[7:0] <= {6'b0, ethernet_delay_readpos[1:0]};
                endcase
            end
            35: begin
                ethernet_timeout_readpos <= ethernet_timeout_readpos[1:0] + 2'b1;
                case(ethernet_timeout_readpos)
                    0: read_data[7:0] <= ethernet_reg_timeout[23:16];
                    1: read_data[7:0] <= ethernet_reg_timeout[15:8];
                    2: read_data[7:0] <= ethernet_reg_timeout[7:0];
                    default:
                        read_data[7:0] <= {6'b0, ethernet_timeout_readpos[1:0]};
                endcase
            end
            36: begin
                ethernet_subnetmask_readpos <= ethernet_subnetmask_readpos + 2'b1;
                case(ethernet_subnetmask_readpos)
                    0: read_data[7:0] <= ethernet_reg_subnetmask[31:24];
                    1: read_data[7:0] <= ethernet_reg_subnetmask[23:16];
                    2: read_data[7:0] <= ethernet_reg_subnetmask[15:8];
                    3: read_data[7:0] <= ethernet_reg_subnetmask[7:0];
                endcase
            end
            40: read_data[7:0] <= trigger_config[7:0];
            41: begin
                trigger_delay_readpos[1:0] <= trigger_delay_readpos[1:0] + 2'b1;
                case(trigger_delay_readpos)
                    0: read_data[7:0] <= trigger_delay_reg[23:16];
                    1: read_data[7:0] <= trigger_delay_reg[15:8];
                    2: read_data[7:0] <= trigger_delay_reg[7:0];
                    default:
                        read_data[7:0] <= {6'b0, trigger_delay_readpos[1:0]};
                endcase
            end
            42: begin
                trigger_duration_readpos[1:0] <= trigger_duration_readpos[1:0] + 2'b1;
                case(trigger_duration_readpos)
                    0: read_data[7:0] <= trigger_duration_reg[23:16];
                    1: read_data[7:0] <= trigger_duration_reg[15:8];
                    2: read_data[7:0] <= trigger_duration_reg[7:0];
                    default:
                        read_data[7:0] <= {6'b0, trigger_duration_readpos[1:0]};
                endcase
            end
            43: read_data[7:0] <= trigger_config_2[7:0];
            44: begin
                fastreadout_trigger_numsignals_readpos <= ~fastreadout_trigger_numsignals_readpos;
                if(fastreadout_trigger_numsignals_readpos)
                    read_data[7:0] <= fastreadout_trigger_numsignals_reg[15:8];
                else
                    read_data[7:0] <= fastreadout_trigger_numsignals_reg[7:0];
            end
            45: read_data[7:0] <= fastreadout_trigger_length_reg[7:0];
            46: begin
                fastreadout_trigger_distance_readpos <= ~fastreadout_trigger_distance_readpos;
                if(fastreadout_trigger_distance_readpos)
                    read_data[7:0] <= fastreadout_trigger_distance_reg[15:8];
                else
                    read_data[7:0] <= fastreadout_trigger_distance_reg[7:0];
            end
            47: begin
                fastreadout_trigger_initdelay_readpos <= ~fastreadout_trigger_initdelay_readpos;
                if(fastreadout_trigger_initdelay_readpos)
                    read_data[7:0] <= fastreadout_trigger_initdelay_reg[15:8];
                else
                    read_data[7:0] <= fastreadout_trigger_initdelay_reg[7:0];
            end
            
            50: read_data[7:0] <= cmd_config_wire[7:0];
            //51: fifo only for writing
            
            // Test Register
            80: read_data[7:0] <= 8'hAB;
        endcase
    end
    else begin
         done <= 0;
    end
end
end

//------------------------
// Writing Registers:
//------------------------
always @(posedge clk) begin
    if (~ res_n) begin
        ChipConfig          <= 0;
        ChipConfig_Lds      <= 8'b0000_0000;
        ChipConfig_RAMwr    <= 0;
        
        reset_reg           <= 8'b00100100; //the resets are active low
        config_mode         <= 0;
        work_mode           <= 0;
        
        fastclkspeed <= 0;
        
        patgen_Reset_reg        <= 1;
        patgen_Suspend_reg      <= 0;
        patgen_writeStrobe_reg  <= 0;
        patgen_address_reg      <= 0;
        patgen_data_reg         <= 0;
		patgen_config           <= 0;
		patgen_skipsignals_reg  <= 0;
        
        fastreadout_control     <= 8'h80;
        fastreadout_status      <= 0;
        fastreadout_tsdiv_reg   <= 0;
        fastreadout_ts2div_reg  <= 0;
        fastreadout_tsphase_reg <= 0;
        
        fastreadout_trigger_numsignals_reg <= 1;
        fastreadout_trigger_length_reg     <= 15;
        fastreadout_trigger_distance_reg   <= 0;
        fastreadout_trigger_initdelay_reg  <= 0;
        
        VoltageBoard <= 0;
        
        TrigDelay <= 0;
        TrigWindow <= 0;
        
        fastro_deser_delay <= 8'd1;

        ethernet_reg_ip      <= {8'd192, 8'd168, 8'd1, 8'd128};
        ethernet_reg_port    <= 16'd1234;
        ethernet_reg_mac     <= 48'h02_00_00_00_00_00;
        ethernet_reg_config  <= 8'd0;
        ethernet_reg_delay   <= 16'd10;
        ethernet_reg_timeout <= 24'hff_ff_ff;
        
        trigger_config       <= 16;
        trigger_delay_reg    <= 0;
        trigger_duration_reg <= 0;
        
        spi_config           <= 128;
        cmd_config           <= 0;
    end
    else begin
        if(write == 1) begin
            case(address)
                0:  ChipConfig                    <= write_data;
                1:  fastclkspeed                  <= write_data;
                2:  patgen_Reset_reg              <= write_data;
                3:  patgen_Suspend_reg            <= write_data;
                4:  patgen_writeStrobe_reg        <= write_data;
				5:  patgen_config				  <= write_data;
                6:  patgen_address_reg            <= write_data;
                7:  patgen_data_reg               <= write_data;
                9:  fastreadout_control           <= write_data;
                10: fastreadout_status            <= write_data;
                //11: is the fast readout FIFO, no writing possible
                12: VoltageBoard                  <= write_data;
                13: TrigDelay                     <= write_data;
                14: TrigWindow                    <= write_data;
                15: fastro_deser_delay[7:0]       <= write_data[7:0];
                16: ChipConfig_Lds[7:0]           <= write_data[7:0];
                17: ChipConfig_RAMwr[7:0]         <= write_data[7:0];
                18: reset_reg[7:0]                <= write_data[7:0];
                19: config_mode[7:0]              <= write_data[7:0];
                20: work_mode[7:0]                <= write_data[7:0];
                21: spi_config                    <= write_data[7:0];
                22: spi_clock_divider_reg         <= write_data[7:0];
                //23 writing to FIFO, done using assignments
                //24: reading FIFO for SPI, no writing possible
                25: fastreadout_tsdiv_reg         <= write_data[7:0];
                26: fastreadout_ts2div_reg        <= write_data[7:0];
                27: fastreadout_tsphase_reg       <= write_data[7:0];
                28: patgen_skipsignals_reg        <= write_data[7:0];

                30: ethernet_reg_ip[31:0]         <= {ethernet_reg_ip[23:0], write_data[7:0]};
                31: ethernet_reg_port[15:0]       <= {ethernet_reg_port[7:0], write_data[7:0]};
                32: ethernet_reg_mac[47:0]        <= {ethernet_reg_mac[39:0], write_data[7:0]};
                33: ethernet_reg_config[7:0]      <= write_data[7:0];
                34: ethernet_reg_delay[15:0]      <= {ethernet_reg_delay[7:0], write_data[7:0]};
                35: ethernet_reg_timeout[23:0]    <= {ethernet_reg_timeout[15:0], write_data[7:0]};
                36: ethernet_reg_subnetmask[31:0] <= {ethernet_reg_subnetmask[23:0], write_data[7:0]};
                
                40: trigger_config[7:0]           <= write_data[7:0];
                41: trigger_delay_reg[23:0]       <= {trigger_delay_reg[15:0], write_data[7:0]};
                42: trigger_duration_reg[23:0]    <= {trigger_duration_reg[15:0], write_data[7:0]};
                43: trigger_config_2[7:0]         <= write_data[7:0];
                44: fastreadout_trigger_numsignals_reg <= {fastreadout_trigger_numsignals_reg[7:0], write_data[7:0]};
                45: fastreadout_trigger_length_reg     <= write_data[7:0];
                46: fastreadout_trigger_distance_reg   <= {fastreadout_trigger_distance_reg[7:0], write_data[7:0]};
                47: fastreadout_trigger_initdelay_reg  <= {fastreadout_trigger_initdelay_reg[7:0], write_data[7:0]};
                
                50: cmd_config[7:0]               <= write_data[7:0];
                //51: writing CMD fifo done using assignments
            endcase
        end
    end
end

endmodule
