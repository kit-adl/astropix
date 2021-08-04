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
//////////////////////////////////////////////////////////////////////////////////
// Company:     KIT-ADL
// Engineer:    Rudolf Schimassek
// 
// Create Date: 05.09.2019
// Design Name: 
// Module Name: State Machine for triggered Readout on ATLASPix3
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
//
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////

module triggered_statemachine(
	input  wire 		clock,
	input  wire         block_update,
	input  wire 		reset,
	input  wire 		enable,
	input  wire         debug,
	//input  wire         keepback_bod,
	input  wire         disable_hitword1,
	input  wire         disable_hitword2,
	
	input  wire [63:0] 	datain,
	input  wire [39:0]	ts_from_SM,
	input  wire [31:0]  ts2_from_SM,
	input  wire [39:0]  trigger_ts,
	input  wire [23:0]  trigger_id,
	
	//FIFO output:
	output wire [63:0] 	dataset,
	output wire 		FIFO_wr_clk,
	output wire			FIFO_wr_enable,
	//state machine debug:
	output wire			debug_found_empty_output,
	output wire			debug_found_data_start
);

reg  [63:0] dataset_internal;
//wire [63:0] dataset_bod;
assign dataset = dataset_internal; //(keepback_bod)?dataset_bod:dataset_internal;

//wire        FIFO_wr_clk;
reg         FIFO_wr_enable_internal;
//wire        wr_en_bod;
assign FIFO_wr_enable = FIFO_wr_enable_internal; //(keepback_bod)?wr_en_bod:FIFO_wr_enable_internal;

//triggered_keepback correctbod(
//    .clock(FIFO_wr_clk),
//    .reset(reset),
//    .enable(keepback_bod),
//    
//    .data_in(dataset_internal),
//    .wr_en_in(FIFO_wr_enable_internal),
//    
//    .data_out(dataset_bod),
//    .wr_en_out(wr_en_bod)
//);

reg  [3:0]  clockdiv;
reg         fifo_clock;
reg  [2:0] 	state;
reg			empty_output;
reg         newevent;
reg         newline;
reg         filledevent;
reg  [31:0] decode;

reg reg_block_update;
reg reg_block_update2;

//reg  [7:0]  debug_counter;
//reg         lastdebug;

assign debug_found_empty_output = empty_output;
assign debug_found_data_start   = newline;

//assign FIFO_wr_clk = fifo_clock; //enable & fifo_clock;

assign FIFO_wr_clk = clock; //Ivan


//Gray Decoding:
//wire [9:0] ts_decoded;
//assign ts_decoded = {data_ts[9],
//					 data_ts[9]^data_ts[8],
//					 data_ts[9]^data_ts[8]^data_ts[7],
//					 data_ts[9]^data_ts[8]^data_ts[7]^data_ts[6],
//					 data_ts[9]^data_ts[8]^data_ts[7]^data_ts[6]^data_ts[5],
//					 data_ts[9]^data_ts[8]^data_ts[7]^data_ts[6]^data_ts[5]^data_ts[4],
//					 data_ts[9]^data_ts[8]^data_ts[7]^data_ts[6]^data_ts[5]^data_ts[4]^data_ts[3],
//					 data_ts[9]^data_ts[8]^data_ts[7]^data_ts[6]^data_ts[5]^data_ts[4]^data_ts[3]^data_ts[2],
//					 data_ts[9]^data_ts[8]^data_ts[7]^data_ts[6]^data_ts[5]^data_ts[4]^data_ts[3]^data_ts[2]^data_ts[1],
//					 data_ts[9]^data_ts[8]^data_ts[7]^data_ts[6]^data_ts[5]^data_ts[4]^data_ts[3]^data_ts[2]^data_ts[1]^data_ts[0]};
//wire [6:0] ts2_decoded;
//assign ts2_decoded = {data_ts2[6],
//					  data_ts2[6]^data_ts2[5],
//					  data_ts2[6]^data_ts2[5]^data_ts2[4],
//					  data_ts2[6]^data_ts2[5]^data_ts2[4]^data_ts2[3],
//					  data_ts2[6]^data_ts2[5]^data_ts2[4]^data_ts2[3]^data_ts2[2],
//					  data_ts2[6]^data_ts2[5]^data_ts2[4]^data_ts2[3]^data_ts2[2]^data_ts2[1],
//					  data_ts2[6]^data_ts2[5]^data_ts2[4]^data_ts2[3]^data_ts2[2]^data_ts2[1]^data_ts2[0]};

//actual data receiving:
always @(posedge clock) begin
	if(reset) begin
		state  				   <= 0;
		clockdiv               <= 4'b0;
		fifo_clock             <= 0;
		newevent               <= 1;
		newline                <= 0;
		filledevent            <= 0;
		decode                 <= 32'd0;
		reg_block_update      <= 0;
		reg_block_update2    <= 0;
		
		//debug_counter          <= 8'd0;
		
		empty_output		   <= 0;
		FIFO_wr_enable_internal <= 0;
		
		dataset_internal       <= 64'b0;
	end
	else if(enable) begin
        
        reg_block_update <= block_update;
        reg_block_update2 <= reg_block_update;
        
        //clock divider to enable writing to FIFO:
        if(clockdiv < 4'd2) //4) //9)
            clockdiv <= clockdiv + 4'b1;
        else
            clockdiv <= 4'd0;
        if(clockdiv == 0)
            fifo_clock <= 1;
        else if(clockdiv == 1) //3)//5)
            fifo_clock <= 0;
        
        //rising edge detection for all output:
        //lastdebug <= debug;
        //if(debug && ~lastdebug)
        //    debug_counter <= 8'd64;            

        if(block_update & ~reg_block_update)
        //if(block_update)
            state <= 0;
        //else if(clockdiv == 2) begin //4) begin //6) begin
        else begin //ivan
            if(state < 3)
                state <= state + 3'b1;
            case(state)
                1: decode <= datain[31:0];
                2: decode <= datain[63:32];
                default: decode <= 32'd0; //if no key word is recognised, nothing is written
            endcase

            //actual writing of the data:
            //End-of-Event word:
            if(!debug && decode[31:24] == 8'hEE) begin
                newevent    <= 1;
                filledevent <= 0;
                dataset_internal[63:0] <= {4'd1, 2'd0, trigger_ts[39:0], decode[22:16],decode[10],decode[9:0]};
                                                            //   trigger tag, fifo full, TS(gray)
                FIFO_wr_enable_internal <= (filledevent || debug); //only write for non-empty events or debug mode
                empty_output   <= 0;
            end
            //Beginning-of-Data word:
            else if(!debug && decode[31:24] == 8'hAB) begin
                newevent <= 0;
                newline  <= 1;
                dataset_internal[63:0] <= {4'd2, trigger_id[19:0], ts_from_SM[39:0]};
                FIFO_wr_enable_internal <= newevent; //only write this for the first occurrence of the BoD per event
                empty_output   <= 0;
            end
            //Hit word 1:
            else if(!debug && decode[31:24] == 8'hDA && ~disable_hitword1) begin
                newline     <= 0;
                filledevent <= 1;
                dataset_internal[63:0] <= {4'd3, newline, 3'b0, ts2_from_SM[31:0], decode[15:9], decode[8:0], decode[23:16]};
                                                                      //    TS2(gray),    row,         column
                //dataset_internal[63:0] <= {4'd3, newline, 3'b0, decode[31:0], decode[15:9], decode[8:0], decode[23:16]};//ivan
                FIFO_wr_enable_internal <= 1; 
                empty_output   <= 0;
            end
            //Hit word 2:
            else if(!debug && decode[31:28] == 4'hC && ~disable_hitword2) begin
                newline     <= 0;
                filledevent <= 1;
                dataset_internal[63:0] <= {4'd4, newline, ts2_from_SM[31:0], decode[9:0], decode[26:18], decode[17:10]};
                            //changed order for easier decoding:      TS,           row,            col
                FIFO_wr_enable_internal <= 1;
                empty_output   <= 0;
            end
            //debug output:
            //else if(debug && decode[31:0] != 32'd0) begin //(debug_counter != 0) begin
            else if(debug && (decode[31:24] == 8'hEE || decode[31:24] == 8'hDA  || decode[31:24] == 8'hAB  || decode[31:28] == 4'hC)) begin //(debug_counter != 0) begin
                dataset_internal[63:0] <= {4'b1000, 1'd0, state[2:0], 24'b0, decode[31:0]};
                FIFO_wr_enable_internal <= 1;
                empty_output <= 0;
                //if(debug == lastdebug)
                //    debug_counter <= debug_counter - 8'd1;
            end
            else begin
                FIFO_wr_enable_internal <= 0;
                empty_output <= 1;
            end        
        end
	end
end

endmodule
