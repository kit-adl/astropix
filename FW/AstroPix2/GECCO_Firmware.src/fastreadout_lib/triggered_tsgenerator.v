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
// Module Name: Time Stamp Generator for untriggered Readout on ATLASPix3
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

module triggered_TSGenerator(
	input  wire 		clock,    //640 MHz input clock
	input  wire 		reset,
	input  wire 		enable,
	
	input  wire [7:0] 	tsdiv,
	input  wire [7:0] 	ts2div,
	input  wire [7:0] 	tsphase,
	
	output wire [39:0] 	tsout,
	output wire [31:0]	ts2out,
	output wire 		syncReset,
	output reg          overflowsync,
	output reg trigger,
	output reg injtrigger
);


//generate 40MHz clock from 640MHz input clock -> divide by 16
//  -> i.e. count to 7 for toggling


//actual TS and TS2 generation:
reg [ 7: 0] ts1div_cnt;
reg [ 7: 0] ts2div_cnt;

reg [ 9: 0] ts1_cnt;
reg [ 7: 0] overflows1_1;
reg [21: 0] overflows1_2;

reg [ 6: 0] ts2_cnt;
reg [ 7: 0] overflows2_1;
reg [16: 0] overflows2_2;

reg [ 7: 0] bincnt1;
reg [ 7: 0] bincnt2;
reg [ 7: 0] bincnt3;

reg binoverflow_posedge;

wire bincntoverflow;

assign bincntoverflow = ({bincnt3, bincnt2, bincnt1} == 24'hffffff);

//syncReset generation
reg syncrst;

assign syncReset = syncrst; // && !lastsyncrst; //short pulse on rising edge of syncrst


always @(posedge clock or posedge reset) begin
    if(reset) begin
        ts1div_cnt   <=  8'd0;
        ts2div_cnt   <=  8'd0;
        
        ts1_cnt      <= 10'd0;
        overflows1_1 <=  8'd0;
        overflows1_2 <= 22'd0;
        
        ts2_cnt      <=  7'd0;
        overflows2_1 <=  8'd0;
        overflows2_2 <= 17'd0;
        
        overflowsync <=  1'b0;
        
        bincnt1      <=  8'd0;
        bincnt2      <=  8'd0;
        bincnt3      <=  8'd0;
        binoverflow_posedge <= 0;
    end
    else if(enable) begin
        // TS1 incrementing:
        if(ts1div_cnt >= tsdiv) begin
            ts1div_cnt <= 8'd0;
            ts1_cnt    <= ts1_cnt + 10'd1;
            if(overflows1_1[1:0] == 2'b00) if(ts1_cnt == 10'd200) trigger <= 1;
            if(overflows1_1[1:0] == 2'b00) if(ts1_cnt == 10'd208) trigger <= 0;//ivan changed from 16!
            
            if(overflows1_1[1:0] == 2'b00) if(ts1_cnt == 10'd0) injtrigger <= 1;
            //if(ts1_cnt == 10'd0) injtrigger <= 1;
            if(overflows1_1[1:0] == 2'b00) if(ts1_cnt == 10'd16) injtrigger <= 0;
            
            
            if(ts1_cnt == 10'h3ff) begin
                overflows1_1 <= overflows1_1 + 8'd1;
                if(overflows1_1 == 8'hff) begin
                    overflows1_2 <= overflows1_2 + 22'd1;
                end    
                overflowsync <= 1'b1;
                if(overflows1_1[1:0] == 2'b00) syncrst <= 1'b1;
                
            end
            else begin
                overflowsync <= 1'b0;
                syncrst <= 1'b0;
            end
        end
        else
            ts1div_cnt <= ts1div_cnt + 8'd1;

        // TS2 incrementing:
        if(ts2div_cnt >= ts2div) begin
            ts2div_cnt <= 8'd0;
            ts2_cnt    <= ts2_cnt + 7'd1;
            if(ts2_cnt == 7'h7f) begin
                overflows2_1 <= overflows2_1 + 8'd1;
                if(overflows2_1 == 8'hff)
                    overflows2_2 <= overflows2_2 + 17'd1;
            end
        end
        else
            ts2div_cnt <= ts2div_cnt + 8'd1;
        
        //binary counter for syncreset generation:
        //no TS divider here
        bincnt1 <= bincnt1 + 8'd1;
        if(bincnt1 == 8'hff) begin
            bincnt2 <= bincnt2 + 8'd1;
            if(bincnt2 == 8'hff)
                bincnt3 <= bincnt3 + 8'd1;
        end
        binoverflow_posedge <= bincntoverflow;
        //  syncReset generation on negative edge at full speed clock
    end
    else begin
        ts1_cnt      <= 10'd0;
        overflows1_1 <=  8'd0;
        overflows1_2 <= 22'd0;
        
        ts2_cnt      <=  7'd0;
        overflows2_1 <=  8'd0;
        overflows2_2 <= 17'd0;
        
        overflowsync <=  1'b0;
        
        bincnt1      <=  8'd0;
        bincnt2      <=  8'd0;
        bincnt3      <=  8'd0;
        binoverflow_posedge <= 0;
    end
end

assign tsout[39:0]  = {overflows1_2[21:0], overflows1_1[7:0], ts1_cnt[9:0]};
assign ts2out[31:0] = {overflows2_2[16:0], overflows2_1[7:0], ts2_cnt[6:0]};




endmodule