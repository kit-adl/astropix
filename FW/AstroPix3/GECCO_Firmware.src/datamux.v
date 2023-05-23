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
// Create Date: 20.12.2018 11:35:54
// Design Name: 
// Module Name: datamux
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


module datamux(
    input wire clk,
    input wire rst,
    input wire enable,
    input wire [7:0]data_source_selection,
    
    //send Data to FTDI FIFO    
    output reg [63:0] dataout, 
    output wire ftdi_wr_clk, 
    output reg ftdi_wr_en,
    input wire ftdi_full, 
    input wire ftdi_fifo_empty,
    input wire ftdi_almostfull,    
    
    //receive Data from Fastreadout
    input wire [63:0] Afr_datain, 
    input wire Afr_wr_clk, 
    input wire Afr_wr_en,
    input wire Afr_rst,
    
    input wire [63:0] Bfr_datain, 
    input wire Bfr_wr_clk, 
    input wire Bfr_wr_en,
    input wire Bfr_rst,
    
    input wire [63:0] Cfr_datain, 
    input wire Cfr_wr_clk, 
    input wire Cfr_wr_en,
    input wire Cfr_rst,
    
    
    input wire [63:0] Dfr_datain, 
    input wire Dfr_wr_clk, 
    input wire Dfr_wr_en,
    input wire Dfr_rst,
    
    input wire [63:0] Efr_datain, 
    input wire Efr_wr_clk, 
    input wire Efr_wr_en,
    input wire Efr_rst
);

wire [63:0] debug_datain;
reg temp = 0;
assign debug_datain = {39'b0, temp ,data_source_selection, 10'b0, token}; // put measurements here

//IDs
parameter [3:0] DEBUG_SOURCE_ID = 0;
parameter [3:0] ATLASS1_SOURCE_ID = 1;
parameter [3:0] ATLASS2_SOURCE_ID = 2;
parameter [3:0] ATLASS3_SOURCE_ID = 3;
parameter [3:0] ATLASS4_SOURCE_ID = 4;
parameter [3:0] ATLASS5_SOURCE_ID = 5;

//token ring parameter
reg [5:0] token = 6'b100000;
wire activate_debug;
wire activate_Afr;
wire activate_Bfr;
wire activate_Cfr;
wire activate_Dfr;
wire activate_Efr;

//connection dataconcentrator datamux
wire debug_FIFO_empty;
wire Afr_FIFO_empty;
wire Bfr_FIFO_empty;
wire Cfr_FIFO_empty;
wire Dfr_FIFO_empty;
wire Efr_FIFO_empty;

reg debug_FIFO_rd_en;
reg Afr_FIFO_rd_en;
reg Bfr_FIFO_rd_en;
reg Cfr_FIFO_rd_en;
reg Dfr_FIFO_rd_en;
reg Efr_FIFO_rd_en;

wire [63:0] debug_FIFO_data;
wire [63:0] Afr_FIFO_data;
wire [63:0] Bfr_FIFO_data;
wire [63:0] Cfr_FIFO_data;
wire [63:0] Dfr_FIFO_data;
wire [63:0] Efr_FIFO_data;

//FTDI stuff
assign ftdi_wr_clk = clk;
wire FIFO_rd_en;
assign FIFO_rd_en = debug_FIFO_rd_en | Afr_FIFO_rd_en | Bfr_FIFO_rd_en | Cfr_FIFO_rd_en | Dfr_FIFO_rd_en | Efr_FIFO_rd_en;

reg prev_debug;
reg debug_wr_en;

always @ (posedge clk) begin
	if (rst) begin
	   token <= 6'b100000;
	   debug_FIFO_rd_en <= 0;
       Afr_FIFO_rd_en <= 0;
       Bfr_FIFO_rd_en <= 0;
       Cfr_FIFO_rd_en <= 0;
       Dfr_FIFO_rd_en <= 0;
       Efr_FIFO_rd_en <= 0;
       dataout <= 0;
       temp <= 0;
	end else begin
        prev_debug <= data_source_selection[6];
        if (debug_wr_en) begin
            debug_wr_en <= 0;
        end else if ((!prev_debug & data_source_selection[6]) || (data_source_selection[0])) begin
            debug_wr_en <= 1;
        end
        
	    if (!ftdi_wr_en) begin
            if (activate_debug) begin    //debug
                dataout <= debug_FIFO_data;
                debug_FIFO_rd_en <= 1;
                token <= 6'b000001;
            end 
                   
            if (activate_Afr) begin //ATLAS S1
                dataout <= Afr_FIFO_data;
                Afr_FIFO_rd_en <= 1;
                token <= 6'b000010;
            end
            
            if (activate_Bfr) begin //ATLAS S2
                dataout <= Bfr_FIFO_data;
                Bfr_FIFO_rd_en <= 1;
                token <= 6'b000100;
            end
            
            if (activate_Cfr) begin //ATLAS S3
                dataout <= Cfr_FIFO_data;
                Cfr_FIFO_rd_en <= 1;
                token <= 6'b001000;
            end
            
            if (activate_Dfr) begin //ATLAS S4
                dataout <= Dfr_FIFO_data;
                Dfr_FIFO_rd_en <= 1;
                token <= 6'b010000;
            end
            
            if (activate_Efr) begin //ATLAS S5
                dataout <= Efr_FIFO_data;
                Efr_FIFO_rd_en <= 1;
                token <= 6'b100000;
            end
        end else begin
            if (debug_FIFO_rd_en)
                debug_FIFO_rd_en<=0;
            if (Afr_FIFO_rd_en)
                Afr_FIFO_rd_en<=0;
            if (Bfr_FIFO_rd_en)
                Bfr_FIFO_rd_en<=0;
            if (Cfr_FIFO_rd_en)
                Cfr_FIFO_rd_en<=0;
            if (Dfr_FIFO_rd_en)
                Dfr_FIFO_rd_en<=0;   
            if (Efr_FIFO_rd_en)
                Efr_FIFO_rd_en<=0;  
        end
        if (FIFO_rd_en)
            ftdi_wr_en <= 1;
        if (ftdi_wr_en)
            ftdi_wr_en <= 0;
    end
end




assign activate_debug = !debug_FIFO_empty & ( 
                            token[5] |
                            token[4] & Efr_FIFO_empty |
                            token[3] & Efr_FIFO_empty & Dfr_FIFO_empty |
                            token[2] & Efr_FIFO_empty & Dfr_FIFO_empty & Cfr_FIFO_empty |
                            token[1] & Efr_FIFO_empty & Dfr_FIFO_empty & Cfr_FIFO_empty & Bfr_FIFO_empty |
                            token[0] & Efr_FIFO_empty & Dfr_FIFO_empty & Cfr_FIFO_empty & Bfr_FIFO_empty & Afr_FIFO_empty
                            );
assign activate_Afr = !Afr_FIFO_empty & ( 
                            token[0] |
                            token[5] & debug_FIFO_empty |
                            token[4] & debug_FIFO_empty & Efr_FIFO_empty |
                            token[3] & debug_FIFO_empty & Efr_FIFO_empty & Dfr_FIFO_empty |
                            token[2] & debug_FIFO_empty & Efr_FIFO_empty & Dfr_FIFO_empty & Cfr_FIFO_empty |
                            token[1] & debug_FIFO_empty & Efr_FIFO_empty & Dfr_FIFO_empty & Cfr_FIFO_empty & Bfr_FIFO_empty
                            );
assign activate_Bfr = !Bfr_FIFO_empty & ( 
                            token[1] |
                            token[0] & Afr_FIFO_empty |
                            token[5] & Afr_FIFO_empty & debug_FIFO_empty |
                            token[4] & Afr_FIFO_empty & debug_FIFO_empty & Efr_FIFO_empty |
                            token[3] & Afr_FIFO_empty & debug_FIFO_empty & Efr_FIFO_empty & Dfr_FIFO_empty |
                            token[2] & Afr_FIFO_empty & debug_FIFO_empty & Efr_FIFO_empty & Dfr_FIFO_empty & Cfr_FIFO_empty
                            );
assign activate_Cfr = !Cfr_FIFO_empty & ( 
                            token[2] |
                            token[1] & Bfr_FIFO_empty |
                            token[0] & Bfr_FIFO_empty & Afr_FIFO_empty |
                            token[5] & Bfr_FIFO_empty & Afr_FIFO_empty & debug_FIFO_empty |
                            token[4] & Bfr_FIFO_empty & Afr_FIFO_empty & debug_FIFO_empty & Efr_FIFO_empty |
                            token[3] & Bfr_FIFO_empty & Afr_FIFO_empty & debug_FIFO_empty & Efr_FIFO_empty & Dfr_FIFO_empty
                            );
assign activate_Dfr = !Dfr_FIFO_empty & ( 
                            token[3] |
                            token[2] & Cfr_FIFO_empty |
                            token[1] & Cfr_FIFO_empty & Bfr_FIFO_empty |
                            token[0] & Cfr_FIFO_empty & Bfr_FIFO_empty & Afr_FIFO_empty |
                            token[5] & Cfr_FIFO_empty & Bfr_FIFO_empty & Afr_FIFO_empty & debug_FIFO_empty |
                            token[4] & Cfr_FIFO_empty & Bfr_FIFO_empty & Afr_FIFO_empty & debug_FIFO_empty & Efr_FIFO_empty
                            );
assign activate_Efr = !Efr_FIFO_empty & ( 
                            token[4] |
                            token[3] & Dfr_FIFO_empty |
                            token[2] & Dfr_FIFO_empty & Cfr_FIFO_empty |
                            token[1] & Dfr_FIFO_empty & Cfr_FIFO_empty & Bfr_FIFO_empty |
                            token[0] & Dfr_FIFO_empty & Cfr_FIFO_empty & Bfr_FIFO_empty & Afr_FIFO_empty |
                            token[5] & Dfr_FIFO_empty & Cfr_FIFO_empty & Bfr_FIFO_empty & Afr_FIFO_empty & debug_FIFO_empty
                            );

dataconcentrator2 dc_Afr(
    .rst(Afr_rst | rst),
    .enable(enable/* & data_source_selection[1]*/),
      
    //receive Data from Fastreadout
    .source(ATLASS1_SOURCE_ID),
    .datain(Afr_datain), 
    .wr_clk(Afr_wr_clk & data_source_selection[1]), 
    .wr_en(Afr_wr_en & data_source_selection[1]),
    
    //send data to datamux
    .dataout(Afr_FIFO_data), 
    .rd_clk(clk),
    .rd_en(Afr_FIFO_rd_en),
    .FIFO_empty(Afr_FIFO_empty)
);

dataconcentrator2 dc_Bfr(
    .rst(Bfr_rst | rst),
    .enable(enable/* & data_source_selection[2]*/),
      
    //receive Data from Fastreadout
    .source(ATLASS2_SOURCE_ID),
    .datain(Bfr_datain), 
    .wr_clk(Bfr_wr_clk & data_source_selection[2]), 
    .wr_en(Bfr_wr_en & data_source_selection[2]),
    
    //send data to datamux
    .dataout(Bfr_FIFO_data), 
    .rd_clk(clk),
    .rd_en(Bfr_FIFO_rd_en),
    .FIFO_empty(Bfr_FIFO_empty)
);

dataconcentrator2 dc_Cfr(
    .rst(Cfr_rst | rst),
    .enable(enable/* & data_source_selection[3]*/),
      
    //receive Data from Fastreadout
    .source(ATLASS3_SOURCE_ID),
    .datain(Cfr_datain), 
    .wr_clk(Cfr_wr_clk & data_source_selection[3]), 
    .wr_en(Cfr_wr_en & data_source_selection[3]),
    
    //send data to datamux
    .dataout(Cfr_FIFO_data), 
    .rd_clk(clk),
    .rd_en(Cfr_FIFO_rd_en),
    .FIFO_empty(Cfr_FIFO_empty)
);

dataconcentrator2 dc_Dfr(
    .rst(Dfr_rst | rst),
    .enable(enable/* & data_source_selection[4]*/),
      
    //receive Data from Fastreadout
    .source(ATLASS4_SOURCE_ID),
    .datain(Dfr_datain), 
    .wr_clk(Dfr_wr_clk & data_source_selection[4]), 
    .wr_en(Dfr_wr_en & data_source_selection[4]),
    
    //send data to datamux
    .dataout(Dfr_FIFO_data), 
    .rd_clk(clk),
    .rd_en(Dfr_FIFO_rd_en),
    .FIFO_empty(Dfr_FIFO_empty)
);

dataconcentrator2 dc_Efr(
    .rst(Efr_rst | rst),
    .enable(enable/* & data_source_selection[5]*/),
      
    //receive Data from Fastreadout
    .source(ATLASS5_SOURCE_ID),
    .datain(Efr_datain), 
    .wr_clk(Efr_wr_clk & data_source_selection[5]), 
    .wr_en(Efr_wr_en & data_source_selection[5]),
    
    //send data to datamux
    .dataout(Efr_FIFO_data), 
    .rd_clk(clk),
    .rd_en(Efr_FIFO_rd_en),
    .FIFO_empty(Efr_FIFO_empty)
);

data_concentrator_fifo_buffer dc_debug (
	.rst(rst),
	.wr_clk(clk),
	.rd_clk(clk),
    .din(debug_datain),
    .wr_en(debug_wr_en),
    .rd_en(debug_FIFO_rd_en | rst),
    .dout(debug_FIFO_data),
    .full(),
    .almost_full(),
    .empty(debug_FIFO_empty),
    .almost_empty(),
    .prog_full()
);



endmodule
