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
// Engineer:    Nicolas Striebig
//
// Create Date: 12.03.2022
// Design Name:
// Module Name: SR readback for AstroPix 2
// Project Name:
// Target Devices:
// Tool Versions:
// Description: Write last 64 readback configbits into fifo
// Dependencies:
//
// Revision:
// Revision 0.1:
//
// Additional Comments:
//
//
//////////////////////////////////////////////////////////////////////////////////

module sr_readback(
    input  logic         clock,
    input  logic         reset,

    input  logic         sr_ck1,
    input  logic         sr_ck2,
    input  logic         sr_ld,
    input  logic         sr_sout,

    output logic [63:0]  data_out_fifo_data,
    input  logic         data_out_fifo_full,
    output logic         data_out_fifo_clock,
    output logic         data_out_fifo_wr_en
);

    logic sout_in;

    logic [7:0] error_cnt;
    logic [7:0]  data_cnt;
    
    logic data_ready;
    
    logic resbyfsm;

    enum logic [1:0] {
        idle            = 2'b00,
        read_sr         = 2'b01,
        write_fifo      = 2'b10
    } state;

    assign data_out_fifo_clock = clock;

    always_ff @(negedge sr_ck1) begin
        sout_in <= sr_sout;
    end

    always_ff @(negedge sr_ck2, posedge resbyfsm) begin
        if(resbyfsm) begin //TODO: sync reset
            error_cnt <= 0;
            data_cnt  <= 0;
            data_ready <= 0;
        end
        else begin
            if(sout_in != sr_sout)
                error_cnt <= error_cnt + 1;

            if(data_cnt < 8'd63) begin
                data_cnt <= data_cnt + 1;
                data_out_fifo_data <= {data_out_fifo_data[62:0], sr_sout};
                data_ready <= 0;
            end
            else
                data_ready <= 1;
        end
    end

    always_ff @(posedge clock, posedge reset) begin
        if(reset) begin
            state <= idle;
            data_out_fifo_wr_en <= 0;
            resbyfsm <= 0;
        end
        else begin
            case(state)
                idle: state <= read_sr;
                read_sr: if(data_ready) state <= write_fifo;
                write_fifo: state <= idle;
            endcase

            case(state)
                idle: begin
                    data_out_fifo_wr_en <= 0;
                    resbyfsm <= 1;
                end
                read_sr: begin
                    data_out_fifo_wr_en <= 0;
                    resbyfsm <= 0;
                end
                write_fifo: begin
                    if(!data_out_fifo_full) data_out_fifo_wr_en <= 1;
                end
            endcase
        end
    end

endmodule