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
// Engineer:    Rudolf Schimassek
// 
// Create Date: 05.08.2019
// Design Name: 
// Module Name: PulseProlongation
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
// This is a module to prolongate signals to pass them from a module on a fast
// clock to a module on a slow clock.
// A positive signal on the signal input will produce a signal of `length` clock
// cycles more on the output than the signal on the input is long.
// For a length of 0 or in case of reset, the signal is directly routed to 
// the output.
//////////////////////////////////////////////////////////////////////////////////

module PulseProlongation(
    input  wire       clk,
    input  wire       reset,
    input  wire       signal,
    input  wire [7:0] length,
    output wire       longsignal,
    
    input  wire [7:0] skipsignals
);

reg [7:0] skipcounter = 0;
reg [7:0] counter = 0;
reg       longpulse = 0;
assign longsignal = (length == 0 || reset) ? signal : longpulse;

reg lastsignal = 0;
always @(posedge clk) begin
    if(reset) begin
        longpulse <= 0;
        counter   <= 0;
        skipcounter <= 0;
        lastsignal <= 0;
    end
    else begin
        lastsignal <= signal;
        if(lastsignal && !signal) begin
            if(skipcounter < skipsignals)
                skipcounter <= skipcounter + 8'b1;
            else
                skipcounter <= 8'b0;
        end
            
        if(signal && counter == 8'b0 && skipcounter == skipsignals) begin
            longpulse <= 1;
            counter   <= 1;
        end
        else if(counter >= length && ~signal) begin  //in case the signal is longer than the requested duration
                                                     // stop with the end of `signal`
            counter   <= 0;
            longpulse <= 0;
        end
        else if(counter != 0)
            counter <= counter + 8'd1;
    end
end

endmodule