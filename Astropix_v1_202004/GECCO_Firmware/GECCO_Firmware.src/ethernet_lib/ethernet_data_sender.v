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
// Company: 	KIT-ADL
// Engineer: 	Rudolf Schimassek
// 
// Create Date: 10.07.2019 12:54:42
// Design Name: 
// Module Name: ethernet_data_sender
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: This module reads data from a FIFO and fills it into the senddata
//				FIFO of the UDP sender using the specified package size and delay
//				between packages in order not to loose packages at the receiver
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module ethernet_data_sender(
    input  wire         clk,
    input  wire         reset,

    //controlling:
    input  wire [7:0]   packagesize,
    input  wire [15:0]  restartdelay,
    input  wire         enable,
    input  wire [23:0]  timeout,

    // data input from FIFO:
    output wire         datain_clk,
    input  wire [7:0]   datain,
    input  wire         datain_valid,
    output reg          datain_rd_en,

    // data output to UDP FIFO:
    output reg  [7:0]   dataout,
    output reg          dataout_valid,
    input  wire         dataout_ready,
    output reg          dataout_lastbyte,
    output reg          dataout_package
    );

assign datain_clk = clk;
reg [15:0] packageindex = 0;

reg [10:0] bytecounter = 0;
reg [15:0] delay = 0;
reg [23:0] timeoutcounter = 0;

parameter idle         = 3'd0;
parameter startpackage = 3'd1;
parameter id1          = 3'd2;
parameter id2          = 3'd3;
parameter senddata     = 3'd4;
parameter endpackage   = 3'd5;

reg [2:0] state = idle;

reg delaysend = 0; //to delay the wr_en after the fifo was empty
reg delaydone = 0;

always @(posedge clk) begin
    if(reset) begin
        datain_rd_en     <= 0;
        dataout_valid    <= 0;
        dataout_lastbyte <= 0;
        dataout_package  <= 0;
        
        timeoutcounter  <= 0;
        
        state            <= idle;
    end
    else if(enable) begin
        if(delaydone)
            delaysend <= 0;
        else
            delaysend <= delaysend | ~datain_valid;
    
        case(state)
            idle: begin
                dataout_package  <= 0;
                dataout_lastbyte <= 0;
                datain_rd_en     <= 0;
                timeoutcounter   <= 0;
                
                if(delay == 16'd0 && datain_valid && dataout_ready) begin
                    state <= startpackage;
                    packageindex <= packageindex + 16'd1;
                end
                else if(delay > 0)
                    delay <= delay - 16'd1;
            end
            startpackage: begin
                dataout_package <= 1;
                dataout <= packageindex[15:8];
                
                if(dataout_ready) begin
                    state <= id1;
                    dataout_valid <= 1;
                end
            end
            id1: begin
                dataout <= packageindex[7:0];
                bytecounter <= {packagesize[7:0],3'd0};
                
                if(dataout_ready) begin
                    state <= id2;
                    dataout_valid <= 1;
                end
                else begin
                    state <= id1;
                    dataout_valid <= 0;
                end
            end
            id2: begin
                dataout <= datain;
                
                if(dataout_ready) begin
                    datain_rd_en <= 1;
                    dataout_valid <= 1;
                    bytecounter <= bytecounter - 11'd1;
                    
                    state <= senddata;
                end
                else begin                    
                    datain_rd_en <= 0;
                    dataout_valid <= 0;
                    
                    state <= id2;
                end
            end
            senddata: begin
                if(dataout_ready && datain_valid && bytecounter > 1) begin
                    datain_rd_en <= 1;
                    if(delaysend && ~delaydone) begin
                        dataout_valid <= 0;
                        delaydone <= 1;
                    end
                    else begin
                        dataout_valid <= 1;
                        bytecounter <= bytecounter - 11'd1;
                        delaydone <= 0;
                    end
                    dataout <= datain;
                    
                    timeoutcounter <= 0;
                    state <= senddata;
                end
                //initiate timeout on no data arriving:
                else if(datain_valid == 0 && dataout_ready == 1 && timeoutcounter < timeout) begin
                    timeoutcounter <= timeoutcounter + 24'd1;
                    datain_rd_en <= 0;
                    dataout_valid <= 0;
                    dataout <= datain;
                    
                    state <= senddata;
                end
                //write out '0' in case of timeout to fill the package:
                else if(datain_valid == 0 && dataout_ready == 1 && bytecounter > 1) begin
                    datain_rd_en <= 1;
                    dataout_valid <= 1;
                    bytecounter <= bytecounter - 11'd1;
                    dataout <= 8'd0;
                    
                    state <= senddata;
                end 
                //no timeout counting if output is also blocked:
                else if(/*datain_valid == 0 ||*/ dataout_ready == 0) begin
                    datain_rd_en <= 0;
                    dataout_valid <= 0;
                    dataout <= datain;
                    
                    state <= senddata;
                end
                else if(bytecounter <= 1) begin
                    datain_rd_en <= 1;
                    dataout_valid <= 1;
                    bytecounter <= bytecounter - 11'd1;
                    dataout_lastbyte <= 1;
                    dataout <= (datain_valid)?datain:8'd0;
                    timeoutcounter <= 0;
                    
                    state <= endpackage;
                end
            end
            endpackage: begin
                datain_rd_en <= 0;
                dataout_lastbyte <= 0;
                dataout_valid <= 0;
                
                delay <= restartdelay;
                
                state <= idle;
            end
        endcase
    end
    else begin
        dataout_valid <= 0;
    end
end

endmodule
