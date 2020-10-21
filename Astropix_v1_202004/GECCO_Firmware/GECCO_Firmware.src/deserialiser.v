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
// Company:     KIT - ADL
// Engineer:    Rudolf Schimassek
// 
// Create Date: 22.01.2018 14:08:34
// Design Name: 
// Module Name: deserialiser
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: This module uses a fast clock to synchronise signals on an input
//              to a slower clock. 
//
//              The edges of the fast clock are counted and after a selectable
//              number of edges (input `delay`) the state of data_in is stored
//              and presented at data_out for a whole slow clock period.
//
//              The module is running driven by fast_clock where `clock` is
//              oversampled. This means that the rising edge of the clock is
//              detected using the falling edge of the fast_clock. As the 
//              clocks might run in phase, the edge detection has two options:
//                  1. 2 points at consecutive fast_clock periods
//                  2. 2 points with a gap of one fast_clock period between the
//                          points
//              The mode is chosen via the input `delay2steps` where false is the
//              first option and true is the second option 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module deserialiser(
    input wire clock,
    input wire fast_clock,
    input wire reset,
    input wire [3:0] delay,
    input wire data_in,
    output reg data_out,
    input wire delay2steps
    );
    
    reg [2:0] delayvalue = 3'b0;
    reg lastclock = 0;
    reg lastclock2 = 0;
    reg data_reg_pos = 0;
    reg data_reg_neg = 0;
    
    always @ (negedge fast_clock) begin
        if(reset) begin
            lastclock   <= 0;
            lastclock2  <= 0;
            delayvalue  <= 0;
            data_reg_neg    <= 0;
            data_out <= 0;
        end
        else begin
            lastclock <= clock;
            lastclock2 <= lastclock;
            if((~delay2steps && clock && ~lastclock) || (delay2steps && lastclock && ~lastclock2)) begin
                delayvalue[2:0] <= 3'b0;
                data_out    <= data_reg_pos | data_reg_neg;
            end
            else
                delayvalue[2:0] <= delayvalue[2:0] + 3'b1;
                
            if(delay[0] == 1) begin
                if(delayvalue[2:0] == delay[3:1])
                    data_reg_neg <= data_in;
            end
            else //if(delay[0] == 0)
                data_reg_neg <= 0;
        end
    end
    
    always @ (posedge fast_clock) begin
        if(reset) begin
            data_reg_pos <= 0;
        end
        else begin
            if(delay[0] == 0) begin
                if(delayvalue[2:0] == delay[3:1])
                    data_reg_pos <= data_in;
            end
            else //if(delay[0] == 1)
                data_reg_pos <= 0;
        end
    end
    
endmodule
