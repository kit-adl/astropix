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
// Create Date: 09.03.2020
// Design Name: 
// Module Name: Data Alignment and Decoding for triggered Readout on ATLASPix3
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

module triggered_data_aligner(
	input  wire 		clock,
	input  wire 		reset,
	input  wire         realign,
	input  wire         shortsearch,
	input  wire         debug,
	
	output reg  		block_update, //indicates update 
	
	input  wire         datain, 	//unaligned data input
	output reg  [63:0]  dataout, 	//aligned & decoded data output
	output reg        	alignment_found,
	output wire         phasemsb//,
	//input  wire [2:0]   bitselect
);


reg  [65:0] shiftphase; //replaced counter by shifting register with one set bit as the counter did not work at 320MHz
//wire [7:0]  phase;
//reg  [1:0]  phase1;
//reg  [4:0]  phase2;

//assign phase = {phase2, phase1};
//reg  [6:0]  align;
reg         align;

assign phasemsb = shiftphase[0];
//assign phasemsb =      (bitselect == 3'd1 && phase1[0])
//                    || (bitselect == 3'd2 && phase1[1])
//                    || (bitselect == 3'd3 && phase2[0])
//                    || (bitselect == 3'd4 && phase2[1])
//                    || (bitselect == 3'd5 && phase2[2])
//                    || (bitselect == 3'd6 && phase2[3])
//                    || (bitselect == 3'd7 && phase2[4]);

reg         shift_data;
reg  [63:0] decoded_data;

reg xorbit;
reg  [57:0] poly;

//parameter testsize = 8'd127;  // <= max_value{match}
reg [7:0] match;
reg [7:0] count;

reg [1:0] retestphase;
reg datain_reg;

always @(posedge clock) begin
    if(reset) begin
        shiftphase      <= 66'd1;
		//phase           <=  7'd0;
		//phase1          <=  2'd0;
		//phase2          <=  5'd0;
		align           <=  1'b0; //7'd0;
		shift_data      <=  1'd0;
		decoded_data    <= 64'd0;
		block_update    <=  1'b0;
		dataout         <= 64'd0;
        alignment_found <=  1'd0;
        
        xorbit          <=  1'b1;
        poly            <= 58'h155_5555_5555_5555;
        
        match           <=  8'd0;
        count           <=  8'd0;
        
        retestphase     <=  2'b10;
    end
    else begin
        datain_reg <= datain;
        
        if(align != 0)
            align      <= 0;
        else
            shiftphase <= {shiftphase[64:0],shiftphase[65]};
    
//        //if(phase == 7'd65) begin
//        if({phase2[4:0],phase1[1:0]} == 7'd65) begin
//            //phase <= 7'd0;
//            phase1 <= 2'd0;
//            phase2 <= 5'd0;
//        end
//        //delay the phase by the value set in aligned_phase:
//        else if(align != 0) 
//            align <= 0; //align - 7'd1;
//        else begin
//            //phase <= phase + 7'd1;
//            //phase1 <= phase1 + 3'd1;
//            phase1[0] <= ~phase1[0];
//            phase1[1] <= phase1[1] ^ phase1[0];
//            //if(phase1[1:0] == 2'b11)
//            //    phase1[2] <= ~phase1[2];
//            //if(phase1[2:0] == 3'b111)
//            if(phase1[1:0] == 2'b11)
//                phase2 <= phase2 + 5'd1;
//        end
        
        
        //search for alignment:
        if(!alignment_found) begin
            shift_data <= datain_reg;
            //if(phase == 7'd1) begin
            if(shiftphase[1] == 1) begin
                if((count[7] == 1'b1 && ~shortsearch) || (count[5] == 1'b1 && shortsearch)) begin
                //if(count[7] == 1'b1) begin
                    if(match == count)
                        alignment_found <= 1'b1;
                    else begin
                        match <= 8'b0;
                        count <= 8'b0;
                        align <= 1'd1; //7'd1; //shift phase by 1 bit
                    end
                end
                else begin
                    count <= count + 8'b1;
                    if(shift_data ^ datain_reg)
                        match <= match + 8'b1;
                end
                
                if(debug)
                    dataout <= {8'd0, count, 8'hf0, match, 15'd0, align, 8'h0f, 7'd0, shift_data ^ datain_reg};
                    //dataout <= {8'd0, count, 8'hf0, match, 9'd0, align, 8'h0f, 7'd0, shift_data ^ datain};
                else
                    dataout <= 64'd0;
            end
            //else if(phase == 7'd2)
            //else if(phase == 7'd2)
//            else if(shiftphase[2] == 1)
//                block_update <= debug;
            //else if(phase == 7'd12) // 7'd4)
            //else if(phase == 7'd12) // 7'd4)
//            else if(shiftphase[12] == 1)
//                block_update <= 0;
                
            else if(shiftphase[8:4] != 0)
                block_update <= debug;
            else
                block_update <= 0;
        end
        //update data containers according to found alignment:
        else begin
            //if(phase >= 7'd2) begin
            if(shiftphase[1:0] == 2'b0) begin
                xorbit       <= datain_reg ^ poly[57] ^ poly[38];
                poly         <= {poly[56:0], datain_reg};
                decoded_data <= {decoded_data[62:0], xorbit};
            end
            // store the alignment bits in a dedicated register:
            else begin
                retestphase  <= {datain_reg, retestphase[1]};
            end
            
            //retest the alignment after a new block started:
            //if(realign && phase == 7'd2 && retestphase[1] == retestphase[0])
            if(realign && shiftphase[2] == 1 && retestphase[1] == retestphase[0])
                alignment_found <= 0;
            
            //if(phase == 7'd3)
            if(shiftphase[3] == 1)
                dataout[63:0] <= decoded_data[63:0];
                
            //if(phase == 7'd4)
//            if(shiftphase[4] == 1)
//                block_update <= 1'b1;
            //else if(phase >= 7'd14) //!= 7'd5)
//            else if(shiftphase[14] == 1)
//                block_update <= 1'b0;
                
            block_update <= (shiftphase[15:4] != 0);//min six times
        end
    end
end

endmodule
