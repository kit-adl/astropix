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
// Module Name: Byte Clock Generation for untriggered Readout on ATLASPix3
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

module ByteClockGenerator(
	input  wire 	  clock,
	input  wire 	  reset,
	
	input  wire       phaseshift_chip_clock,
	//input  wire [6:0] phaseshift_block_clock,
	
	//output wire		  bit_clock,    //full speed clock for receiving bits
	output wire       chip_clock,   // /2 speed clock for sending to ATLASPix3
	//output wire		  block_clock,  // /66 speed clock for the received data 
	//output reg 		  block_clock_posedge //synchronous with block_clock but a pulse as long as a bit_clock pulse
	//output wire [8:0] delayed_clock
	output wire       sm_clock
);

//clock phase shifting for data alignment:
//clock divider for 10bit output
//reg deser_block_clock = 0;
//reg deser_block_clock_edge = 0;  //edge marker for clk40
//reg [6:0] clock_div = 7'b0;
reg posbitclk;
reg negbitclk;
//reg blockclk;

reg [1:0] quarterclock;

//clock division:
always @(posedge clock) begin
    if(reset) begin
        posbitclk <= 0;
        //blockclk <= (phaseshift_block_clock[6:0] > 65 || phaseshift_block_clock[6:0] < 33);
        //if(phaseshift_block_clock[6:0] > 65)
        //    clock_div <= 7'd0; 
        //else
        //    clock_div <= phaseshift_block_clock[6:0];
        quarterclock <= 0;
    end
    else begin
        posbitclk <= ~posbitclk;
        //if(clock_div == 65) begin
        //    blockclk <= 1; // ~block_clock;
        //    clock_div <= 0;
        //    block_clock_posedge <= 1; //~deser_block_clock;
        //end
        //else if(clock_div == 32) begin
        //    clock_div <= 33;
        //    block_clock_posedge <= 0;
        //    blockclk <= 0; //~block_clock;
        //end
        //else begin
        //    clock_div <= clock_div + 7'b1;
        //    block_clock_posedge <= 0;
        //end
        
        quarterclock <= quarterclock + 2'd1;
    end
end

always @(negedge clock) begin
    if(reset)
        negbitclk <= 0;
    else
        negbitclk <= ~negbitclk;
end

/*
wire block_clock_w;
assign block_clock_w = blockclk ^ 0;
wire bit_clock_w;
assign bit_clock_w   = posbitclk ^ negbitclk;

BUFIO bbit(
    .I(bit_clock_w),
    .O(bit_clock)
);

BUFIO bblock(
    .I(block_clock_w),
    .O(block_clock)
);*/

//assign bit_clock = posbitclk ^ negbitclk;
assign chip_clock = ((phaseshift_chip_clock)?posbitclk:negbitclk); // ^ ~phaseshift_chip_clock[1]; 
//assign block_clock = blockclk ^ 0;


BUFG inst_smclock(
    .I(quarterclock[1]),
    .O(sm_clock)
);
//assign sm_clock = posbitclk;

endmodule