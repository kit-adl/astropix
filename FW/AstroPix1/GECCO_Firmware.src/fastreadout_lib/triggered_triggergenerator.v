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
// Create Date: 10.03.2020
// Design Name: 
// Module Name: Trigger Signal Generator for triggered Readout on ATLASPix3
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

module triggered_TriggerGenerator(
    input  wire        clock,
    input  wire        reset,
    
    input  wire        triggerin,
    
    input  wire [9:0]  timestamp,
    
    input  wire [15:0] numtriggers,
    input  wire [7:0]  triggerlength,
    input  wire [15:0] triggerdistance,
    input  wire [15:0] initialdelay,  //delay after triggerin for !synced, timestamp for turning on for synced
    input  wire        synced,
    
    output wire        active,
    output reg         triggerout
);

parameter idle     = 2'd0;
parameter init     = 2'd1;
parameter trig_on  = 2'd2;
parameter trig_off = 2'd3;

reg [1:0]  state;
reg [15:0] tscounter;
reg [15:0] loopcounter;
reg        lasttriggerin;

assign active = (state != 2'b0);

always @(posedge clock) begin
    if(reset) begin
        state       <=  2'd0;
        tscounter   <= 16'd0;
        loopcounter <= 16'd0;
        triggerout  <=  1'b0;
    end
    else begin
        lasttriggerin <= triggerin;
        case(state)
            idle: begin
                triggerout  <=  1'b0;
                if(triggerin && ~lasttriggerin) begin
                    state       <= init;
                    tscounter   <= 16'd0;
                end
            end
            init: begin
                if(tscounter >= initialdelay) begin
                    tscounter <= 16'd0;
                    state <= trig_on;
                    triggerout <= 1'b1;
                    if(numtriggers != 16'b0)
                        loopcounter <= 16'd1;
                    else
                        loopcounter <= 16'd0;
                end
                else
                    tscounter <= tscounter + 16'd1;
            end
            trig_on: begin
                if(tscounter[11:0] >= {triggerlength[7:0],4'b0}) begin
                    triggerout <= 1'b0;
                    tscounter <= 16'd0;
                    if(loopcounter >= numtriggers && numtriggers != 16'd0)
                        state <= idle;
                    else
                        state <= trig_off;
                end
                else
                    tscounter <= tscounter + 16'd1;
            end
            trig_off: begin
                if(tscounter[11:0] >= {triggerdistance[7:0], 4'b0}) begin
                    if(numtriggers != 16'd0)
                        loopcounter <= loopcounter + 16'd1;
                    state <= trig_on;
                    triggerout <= 1'b1;
                    tscounter <= 16'd0;
                end
                else
                    tscounter <= tscounter + 16'd1;
            end
        endcase;
    end
end

endmodule