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
////////////////////////////
// This is a testing module for the Gigabit Ethernet communication using
// UDP protocol. The last byte of the received command is used as starting
// value of a decrementing counter filling the answer data package
////////////////////////////
module answerbot(
    input  wire clk,
    input  wire reset,
    
    //data input:
    input  wire [7:0] datain,
    input  wire       datain_valid,
    output reg        datain_ready,
    input  wire       datain_lastbyte,
    
    //data output:
    output reg  [7:0] dataout,
    output reg        dataout_valid,
    input  wire       dataout_ready,
    output reg        dataout_lastbyte,
    output reg        send,
    
    //debugging output:
    output wire [7:0] status,
    
    //debugging input:
    input  wire [7:0] byte,
    input  wire       debug
);

reg [1:0] state = 0;

reg [7:0] counter = 0;
reg [5:0] idcounter = 0;

assign status = {state, datain_valid, idcounter[4:0]};

parameter start = 0;
parameter store = 1;
parameter count = 2;

reg isfinished = 0;
reg startcond = 0;

reg lastdebug = 0;

always @(negedge clk) begin
    if(reset) begin
        state   <= start;
        counter <= 0;
        datain_ready <= 0;
        dataout_valid <= 0;
        dataout <= 0;
        dataout_lastbyte <= 1;
        idcounter <= 0;
        isfinished <= 0;
        startcond <= 0;
        lastdebug <= 0;
    end
    else begin
        lastdebug <= debug;
    
        case(state)
            start: begin
                dataout_valid <= 0;
                dataout_lastbyte <= 0;
                datain_ready <= 1;
                if(datain_valid) begin
                    state <= store;
                    counter <= datain; //to catch 1 byte pakets
                    idcounter <= idcounter + 6'd1;
                    startcond <= datain_lastbyte;
                end
                else if(debug != lastdebug && debug == 1'b0) begin
                    state <= count;
                    counter <= byte;
                    idcounter <= idcounter + 6'b1;
                    send <= 1;
                end
                else
                    send <= 0;
            end
            store: begin
                datain_ready <= 1;
                if((!datain_valid && startcond) || datain_lastbyte) begin
                    isfinished <= 1;
                end

                if(isfinished) begin
                    counter <= datain;
                    state <= count;
                    send <= 1;
                end
            end
            count: begin
                datain_ready <= 0;
                isfinished <= 0;
                if(dataout_ready) begin
                    counter <= counter - 8'b1;
                    dataout_valid <= 1;
                    dataout <= counter;
                    if(counter == 0) begin
                        state <= start;
                        dataout_lastbyte <= 1;
                    end
                    else begin
                        dataout_lastbyte <= 0;
                    end
                end
                else begin
                    dataout_valid <= 0;
                end
            end
        endcase
    end
end



endmodule