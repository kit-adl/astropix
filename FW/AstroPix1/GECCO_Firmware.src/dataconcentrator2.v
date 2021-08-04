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
 * This module was developed by Felix Ehrler (felix.ehrler@kit.edu)
 */
`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 	KIT-ADL
// Engineer: 	Felix Ehrler
// 
// Create Date: 20.12.2018 11:17:08
// Design Name: 
// Module Name: dataconcentrator2
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module dataconcentrator2(
    input wire rst,
    input wire enable,
      
    //receive Data from Fastreadout
    input wire [3:0] source,
    input wire [63:0] datain, 
    input wire wr_clk, 
    input wire wr_en,
    
    //send data to datamux
    output wire [63:0] dataout, 
    input wire rd_clk,
    input wire rd_en,
    output wire FIFO_empty
    );
    
    //AtlasSimple 1
    wire [63:0] buffer; 
    reg [3:0] counter = 0;
    reg [7:0] column = 0;
    reg [5:0] ToT = 0;
    reg [9:0] TS = 0;
    reg [7:0] row = 0;
    reg [7:0] TriggerIndex = 0;
    reg [15:0] TriggerTimestamp = 0;
    assign buffer = {source, counter, column, ToT, TS, row, TriggerIndex, TriggerTimestamp};
    
    reg FIFO_wr_en = 0;

    
always @ (posedge wr_clk or posedge rst) begin
    if (rst) begin
        counter             <= 0;//could be used to send several connected data sets
        column              <= 0;
        ToT                 <= 0;
        TS                  <= 0;
        row                 <= 0;
        TriggerIndex        <= 0;
        TriggerTimestamp    <= 0;
        FIFO_wr_en          <= 0;
    end else begin
        if (wr_en) begin   
            case (datain[63:56])
                4'd4:     column[7:0] <= datain[55:48];
                4'd5:     {ToT[5:0], TS[9:8]} <= datain[55:48];
                4'd6:     TS[7:0] <= datain[55:48];
                4'd7:     begin row <= datain[55:48];
                        FIFO_wr_en <= 1'b1;
                end                            
            endcase
        end else begin
            if(FIFO_wr_en)
                FIFO_wr_en <= 0;
        end
    end 
end

data_concentrator_fifo_buffer dc_FIFO (
	.rst(rst),
	.wr_clk(wr_clk),
	.rd_clk(rd_clk),
    .din(buffer[63:0]),
    .wr_en(FIFO_wr_en),
    .rd_en(rd_en | rst),
    .dout(dataout[63:0]),
    .full(),
    .almost_full(),
    .empty(FIFO_empty),
    .almost_empty(),
    .prog_full()
);
    
endmodule
