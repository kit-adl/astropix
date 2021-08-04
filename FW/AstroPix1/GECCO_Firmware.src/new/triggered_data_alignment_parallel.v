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
// Engineer:    Ivan Peric
// 
// Create Date: 28.07.2020
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

module triggered_data_alignment_parallel(
	input  wire 		clock,	
	input  wire 		reset,
	input  wire         realign,
	input  wire         shortsearch,
	input  wire         debug,	
	output reg  		block_update, //indicates update 	
	input  wire [5:0]        datain, 	//par
	output reg  [63:0]  dataout, 	//aligned & decoded data output
	output reg        	alignment_found,
	output wire         phasemsb,
	input wire sync
);



reg[2:0] phase_pointer;//0->5
reg[3:0] phase;//0->10
reg [5:0] datain_reg;
reg [5:0] datain_reg_del;
reg [5:0] datain_reg_del2;
reg [5:0] datain_aligned;
//reg  [65:0] decoded_data;
//reg [5:0] xorbit;
reg  [65:0] poly;
reg  [127:0] polyreg;
reg [1:0] extrabits;
reg [7:0] match;
reg [7:0] count;
reg align;
wire [63:0] descrambled_data;

genvar i;
generate 
for (i=0; i<=63; i=i+1) begin:blabla
    assign descrambled_data[i] = polyreg[i] ^ polyreg[i+39] ^ polyreg[i+58];  
end
endgenerate


always @(posedge clock) begin
    if(reset) begin
    
        phase_pointer <= 0;
        phase <= 0;
        block_update <= 0;
        //poly            <= 66'h3_5555_5555_5555_5555;
        //poly[58:1] <= 58'h155_5555_5555_5555;
        //poly[0] <= 0;
        //poly[65:59] <= 0;
        //poly[65:1] <= 58'h1_5555_5555_5555_5555;
        
        
        
        extrabits <= 0;
        alignment_found <= 0;
        align           <=  1'b0; //7'd0;
        //decoded_data    <= 66'd0;
        dataout         <= 64'd0;         
        //xorbit          <=  6'b111111;         
        match           <=  0;
        count           <=  0;
        extrabits <= 2'b00;        
           
    end
    else begin
    
        datain_reg <= datain;
        datain_reg_del <= datain_reg;
        datain_reg_del2 <= datain_reg_del;
        
        //if for writing in dataout
        
        
        
        
        //if((phase == 4)&&(alignment_found == 1)) begin //was 4
          //  dataout <= decoded_data[63:0];
            //block_update <= 1;
        //end
        
        if((phase == 2)&&(alignment_found == 1)) begin
            polyreg[63:0] <= poly[63:0];
            polyreg[127:64] <= polyreg[63:0];
            dataout <= descrambled_data;
            block_update <= 1; 
        end
        
        
        else begin
            dataout <= dataout;
            if(phase == 5) block_update <= 0;             
        end 
        
        /*
        if(sync) phase <= 1;//normal
        else begin        
            if(phase < 10) phase <= phase + 1;//normal
            else phase <= 0;//normal
        end 
        
        */
          
        //endif for writing in dataout
        
        //if for doing alignment
        
        
        
        
        if(align == 1) begin 
        
            align      <= 0;
            
            if(phase_pointer < 5) begin
                phase_pointer <= phase_pointer + 1;
                if(phase < 10) phase <= phase + 1;//normal
                else phase <= 0;//normal
            end    
            else begin
                phase_pointer <= 0;
                phase <= phase;//delays
            end    
        end//align
        
        
        else begin//noalign
        
            if(phase < 10) phase <= phase + 1;//normal
            else phase <= 0;//normal
            
 
            
            
            if(phase == 1) begin
                
               if(alignment_found == 0) begin
               
                if(count == 255) begin
                    if(match == count)
                        alignment_found <= 1'b1;//was 1
                    else begin
                        match <= 0;
                        count <= 0;
                        align <= 1'd1; //shift phase by 1 bit
                    end
                end//count 127
                else begin
                    count <= count + 1;
                    //if((extrabits[1] == 1) && (extrabits[0] == 0)) match <= match + 1;
                    if( extrabits[1] ^ extrabits[0] ) match <= match + 1;
                end
               end//found 0
               else begin 
                if(realign && extrabits[1] == extrabits[0]) begin
                    alignment_found <= 0;
                    match <= 0;
                    count <= 0;
                 end 
               end                
    
            end//phase 1
            
            
        
        end//does no align    
        
        
        
    
        //endif for doing alignment
        
        
        
        case (phase_pointer)//phase 0
        
            3'd0: datain_aligned[5:0] <= {datain_reg_del[1:0],datain_reg[5:2]};
            3'd1: datain_aligned[5:0] <= {datain_reg_del[2:0],datain_reg[5:3]};
            3'd2: datain_aligned[5:0] <= {datain_reg_del[3:0],datain_reg[5:4]};
            3'd3: datain_aligned[5:0] <= {datain_reg_del[4:0],datain_reg[5:5]};
            3'd4: datain_aligned[5:0] <= {datain_reg_del[5:0]};
            3'd5: datain_aligned[5:0] <= {datain_reg_del2[0],datain_reg_del[5:1]};
            
        endcase
        
        
        //datain_aligned[5:0] <= datain_reg_del[5:0];
        
        if(phase == 0) begin
        case (phase_pointer)//phase 0
        
            3'd0: extrabits[1:0] <= {datain_reg[1:0]};
            3'd1: extrabits[1:0] <= {datain_reg[2:1]};
            3'd2: extrabits[1:0] <= {datain_reg[3:2]};
            3'd3: extrabits[1:0] <= {datain_reg[4:3]};
            3'd4: extrabits[1:0] <= {datain_reg[5:4]};
            3'd5: extrabits[1:0] <= {datain_reg_del[0],datain_reg[5]};
            
        
        endcase        
        end
        
        poly[5:0]   <= datain_aligned[5:0];//phase 1
        poly[11:6]  <= poly[5:0];
        poly[17:12] <= poly[11:6];
        poly[23:18] <= poly[17:12];
        poly[29:24] <= poly[23:18];
        poly[35:30] <= poly[29:24];
        poly[41:36] <= poly[35:30];
        poly[47:42] <= poly[41:36];
        poly[53:48] <= poly[47:42];
        poly[59:54] <= poly[53:48];
        poly[65:60] <= poly[59:54];
        
        
    
        
        

    
    end//no reset
end//always









endmodule
