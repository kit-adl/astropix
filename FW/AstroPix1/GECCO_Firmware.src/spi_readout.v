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
//              Modified by: Nicolas Striebig
//
// Create Date: 24.07.2021
// Design Name:
// Module Name: SPI Readout for AstroPix
// Project Name:
// Target Devices:
// Tool Versions:
// Description: 4-Wire SPI Controller
//              Uses async FIFOs (ftdi_top/rfg/spi_xxx_fifo) to pass Write and Read data from lowspeed SPI clock to high-speed FTDI domain.
//              The readout Software accesses the data by writing/reading from/to the FTDI Registers 0x23/0x24.
//
// Dependencies:
//
// Revision:
// Revision 0.02 - File Created
//
// Additional Comments:
//
// TODOS:
// - Drive SPI_CS as GPIO via Software?
// - Write/Read on leading/trailing edge
//////////////////////////////////////////////////////////////////////////////////

module spi_readout#(
    parameter CPOL = 0,
    parameter CPHA = 1)
(
    input  wire         clock,
    input  wire         reset,
    input  wire [7:0]   clock_divider,

    output reg          spi_csb,
    output wire         spi_clock,
    output reg          spi_mosi,
    input  wire         spi_miso,

    input  wire         readback_en,
    input  wire [31:0]  data_in_fifo_data,
    input  wire         data_in_fifo_empty,
    output wire         data_in_fifo_clock,
    output reg          data_in_fifo_rd_en,

    output reg  [63:0]  data_out_fifo_data,
    input  wire         data_out_fifo_full,
    output wire         data_out_fifo_clock,
    output reg          data_out_fifo_wr_en,

    output reg          trigger
);


reg [6:0] clock_div_counter = 0;

reg [31:0] shift_data_in = 0;

reg [2:0] state = 0;
reg [6:0] loop_counter = 0;
reg temp_clk;

reg clk_en;

localparam idle         = 3'b000;
localparam load_data    = 3'b001;
localparam read_data    = 3'b010;
localparam write_data   = 3'b011;
localparam ending       = 3'b100;

assign data_in_fifo_clock = temp_clk;
assign data_out_fifo_clock = temp_clk;

//Assign Clock with correct phase and only when writing
assign spi_clock = (clk_en) && (CPHA ? ~temp_clk : temp_clk);

always @(posedge clock) begin
    if(reset) begin
        spi_csb <= 1;
        spi_mosi <= 0;

        temp_clk <= CPOL;

        data_in_fifo_rd_en <= 0;
        data_out_fifo_wr_en <= 0;

        data_out_fifo_data <= 64'b0;

        clock_div_counter <= 0;
        loop_counter <= 0;

        clk_en <= 0;

        trigger <= 0;

        state <= 0;
    end
    else begin
        //SPI Clock divider
        if(clock_div_counter[6:0] < clock_divider[7:1])
            clock_div_counter <= clock_div_counter + 8'b1;
        else begin
            clock_div_counter <= 0;

            temp_clk  <= ~temp_clk;

            if(temp_clk == 1) begin
                case(state)
                    idle: begin

                        //Disable ReadFIFO
                        data_out_fifo_wr_en <= 0;

                        loop_counter <= 0;

                        //data_in_fifo_empty is 1 if words in in_fifo >= 4
                        if(~data_in_fifo_empty)
                            state <= load_data;
                    end

                    load_data: begin //load the data to write in 4 clock cycles
                        clk_en <= 0;
                        shift_data_in <= {shift_data_in[23:0], data_in_fifo_data[7:0]};

                        data_in_fifo_rd_en <= 1;

                        if(loop_counter >= 3) begin

                            //Assign Chip-select one cycle before SPI clock starts
                            spi_csb <= 0;

                            if(readback_en) begin
                                if(shift_data_in[16] == 1)
                                    trigger <= 1;
                                state <= read_data;
                                loop_counter <= 0;
                            end
                            else begin
                                state <= write_data;
                                loop_counter <= 32;

                            end
                        end
                        else
                            loop_counter <= loop_counter + 7'b1;
                    end

                    read_data: begin  //read the first 40 bits of the 64 bit data to read
                        clk_en <= 1;
                        data_in_fifo_rd_en <= 0;
                        loop_counter <= loop_counter + 7'b1;
                        data_out_fifo_data <= {data_out_fifo_data[62:0], spi_miso};
                        if(loop_counter >= 39) // 40-1)
                            state <= write_data;
                    end

                    write_data: begin

                        //Enable Clock
                        clk_en <= 1;

                        //Disable WriteFIFO
                        data_in_fifo_rd_en <= 0;

                        loop_counter <= loop_counter + 7'b1;

                        //Shift-in MISO
                        data_out_fifo_data <= {data_out_fifo_data[62:0], spi_miso};

                        //Shift-out MOSI
                        spi_mosi <= shift_data_in[31];
                        shift_data_in <= {shift_data_in[30:0],1'b0};

                        //Go to end state after writing/reading 32 bits
                        if(loop_counter >= 63) begin
                                state <= ending;
                        end
                    end

                    ending: begin
                        //Always write MISO stream to ReadFIFO
                        data_out_fifo_wr_en <= 1;

                        //Disable spi_clock
                        clk_en <= 0;

                        //Deassert chip-select
                        spi_csb <= 1;

                        trigger <= 0;
                        if(readback_en) begin //only write to output FIFO if data was requested
                            if(~data_out_fifo_full) begin //wait for space in the FIFO
                                data_out_fifo_wr_en <= 1;
                                state <= idle;
                            end
                        end
                        else
                            state <= idle;
                    end
                endcase
            end
        end
    end
end

endmodule