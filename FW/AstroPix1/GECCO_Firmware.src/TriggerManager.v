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
// Create Date: 05.08.2019 11:44:12
// Design Name: 
// Module Name: TriggerManager
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
//          0.02 - added trigger signal counting
// Additional Comments:
// 
//      The trigger manager enables the multiplexing of several inputs and
//      adjusting the delay and length of the signals passing the module.
//      In addition, a check for noise can be applied by waiting for a number
//      of clock cycles (as 2^X cycles choosing X) before making sure that the
//      selected input is still at the active level. If it is not, no signal is
//      emitted at the output. If there is a signal at the input, the signal
//      delay counter is started and after reaching the number given in `delay`
//      input, the trigger signal is generated as positive pulse for the 
//      duration of `duration` clock cycles.
//      Choosing both delay and duration as '0', the trigger input is directly
//      emitted to the output not checking for noise.
//////////////////////////////////////////////////////////////////////////////////


module TriggerManager(
    input  wire        clock,
    input  wire        reset,
    input  wire        sync_reset,
    input  wire        id_reset,
    
    input  wire        edge_select,
    input  wire [2:0]  source_select,
    input  wire [2:0]  noise_suppression,
    input  wire [23:0] delay,
    input  wire [23:0] duration,
    
    input  wire [7:0]  sources,
    
    output wire        trigger,
    output reg  [15:0] trigger_id,
    output reg  [39:0] trigger_ts
    );
    
reg [31:0] ts_long = 0;
reg [3:0]  ts_short = 0;
reg [3:0]  ts_tiny = 0;
reg [23:0] delaycounter    = 0;
reg [23:0] durationcounter = 0;
reg [7:0]  noisecounter = 0;

reg last_trigger_state = 0;
reg trigger_output = 0;

assign trigger = (delay != 0 || duration != 0) ? trigger_output : (sources[source_select] ^ ~edge_select);
reg last_out_trigger = 0;  //for actual trigger output edge detection

//divide the clock to the TS clock of the chip: 
reg [3:0] clockdiv = 0;
always @(posedge clock) begin
    if(reset) begin
        clockdiv <= 0;
    end
    else begin
        clockdiv <= clockdiv + 4'd1;
    end
end
wire slowclock;  //clock as fast as the TS clock inside ATLASPix3
BUFG slowclk(
    .I(clockdiv[2]),
    .O(slowclock)
    );

always @(posedge slowclock or posedge reset) begin //clock) begin
    if(reset) begin
        last_trigger_state  <= 0;
        trigger_output      <= 0;
        delaycounter        <= 0;
        durationcounter     <= 0;
        trigger_id          <= 0;
        trigger_ts          <= 0;
        ts_long             <= 0;
        ts_short            <= 0;
        ts_tiny             <= 0;
        last_out_trigger    <= 0;
    end
    else begin
        //time stamp generation:
        if(sync_reset) begin
            ts_long  <= 0;
            ts_short <= 0;
            ts_tiny  <= 0;
        end
        else begin
            ts_tiny <= ts_tiny + 4'b1;
            if(ts_tiny == 4'hF) begin
                ts_short <= ts_short + 4'b1;
                if(ts_short == 4'hF)
                    ts_long <= ts_long + 32'b1; 
            end
        end
        //trigger ID and trigger TS update on output posedge:
        last_out_trigger <= trigger;
        if(last_out_trigger == 0 && trigger == 1) begin
            if(!id_reset)
                trigger_id <= trigger_id + 16'b1;
            else
                trigger_id <= 16'd0;
            trigger_ts <= {ts_long, ts_short, ts_tiny};
        end
        else if(id_reset)
            trigger_id <= 16'b0;
    
        last_trigger_state <= sources[source_select];
        
        if(last_trigger_state != sources[source_select] && sources[source_select] == edge_select) begin
            durationcounter <= 0;
            delaycounter <= 0;
            noisecounter <= 1;
            trigger_output <= 0;
        end
        else begin
            if(noisecounter[7:0] >= (8'd1 << noise_suppression[2:0])) begin
                noisecounter <= 0;
                // make sure the trigger signal was not just noise:
                if(sources[source_select] == edge_select || noise_suppression[2:0] == 0)
                    delaycounter <= 1;
            end
            else if(noisecounter > 0)
                noisecounter <= noisecounter + 8'd1;
            else if(delaycounter[23:0] >= delay[23:0]) begin
                delaycounter <= 0;
                durationcounter <= 1;
                trigger_output <= 1;
            end
            else if(delaycounter > 0)
                delaycounter <= delaycounter + 24'd1;
            else if(durationcounter[23:0] >= duration[23:0]) begin
                durationcounter <= 0;
                trigger_output <= 0;
            end
            else if(durationcounter > 0)
                durationcounter <= durationcounter + 24'd1;
        end
    end
end

endmodule
