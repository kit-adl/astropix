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
// Module Name: Data Receiver for triggered Readout on ATLASPix3
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

module DataReceiver(
    input  wire clock,
    input  wire reset,
    input  wire enable,
    input  wire edgeselect,
    
    //input  wire datain_p,
    //input  wire datain_n,
    input  wire datain,
    
    output wire [1:0] data 
);

//wire datain;

//IBUFDS #(
//    .DIFF_TERM("FALSE"),
//    .IBUF_DELAY_VALUE("0"),
//    .IOSTANDARD("LVDS_25") //"DIFF_HSTL_I_12")
//) IBUFDS_data (
//    .O(datain),
//    .I(datain_p),
//    .IB(datain_n)
//);

reg [1:0] pdata;
always @(posedge clock) begin
    if(reset) begin
        pdata <= 2'b0;
    end
    else if(enable) begin
        pdata <= {pdata[0:0], datain};
    end
end

reg [1:0] ndata;
always @(negedge clock) begin
    if(reset) begin
        ndata <= 2'b0;
    end
    else if(enable) begin
        ndata <= {ndata[0:0], datain};
    end
end

assign data = (edgeselect)?pdata:ndata;

endmodule