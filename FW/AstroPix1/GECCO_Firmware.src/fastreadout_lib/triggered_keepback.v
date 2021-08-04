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
// Company: 
// Engineer: 
// 
// Create Date: 29.05.2020 10:01:47
// Design Name: 
// Module Name: triggered_keepback
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: This module is supposed to send the correct Beginning-of-Data 
//              (BoD) word with the filled events keeping it back until another 
//              data word is received so that the event is not empty
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module triggered_keepback(
    input  wire        clock,
    input  wire        reset,
    input  wire        enable,
    
    input  wire [63:0] data_in,
    input  wire        wr_en_in,
    
    output wire [63:0] data_out,
    output reg         wr_en_out //,
    //output wire        wr_clk_out
    );
    
reg [63:0] fifo1 = 64'd0;
reg [63:0] fifo2 = 64'd0;

//assign wr_clk_out = clock;
assign data_out[63:0] = fifo2[63:0];

always @(posedge clock) begin
    if(reset) begin
        fifo1     <= 64'd0;
        fifo2     <= 64'd0;
    end
    else if(enable) begin
        if(wr_en_in)
            fifo1 <= data_in;
        else if(wr_en_out)
            fifo1 <= 64'd0;
            
        if(fifo2 != 64'd0 || (wr_en_in && !(data_in[63:60] == 4'd2 && fifo1[63:60] == 4'd2)))
            fifo2 <= fifo1;
    end
end

always @(negedge clock) begin
    if(reset)
        wr_en_out <= 1'd0;
    else
        wr_en_out <= (fifo2 != 64'd0);
end

endmodule
