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
// Create Date: 28.12.2021
// Design Name:
// Module Name: SPI Readout for AstroPix 2
// Project Name:
// Target Devices:
// Tool Versions:
// Description: 5-Wire SPI Controller
//              Uses async FIFOs (ftdi_top/rfg/spi_xxx_fifo) to pass Write and Read data from lowspeed SPI clock to high-speed FTDI domain.
//              The readout Software accesses the data by writing/reading from/to the FTDI Registers 0x23/0x24.
//              Modes:
//                   WR: Write until Writebuffer is empty and for every bit, read two bits from MISO
//                   R-only: Read two bits from MISO for 32 SPI clock cycles
// Dependencies:
//
// Revision:
// Revision 0.1: 03.01.2022: Write on spi_clk posedge, read on negedge
//
// Additional Comments:
//
// TODOS:
// - Drive SPI_CS as GPIO via Software?
//
//////////////////////////////////////////////////////////////////////////////////

module spi_readout2#(
    parameter CPOL = 0,
    parameter CPHA = 1)
(
    input  logic         clock,
    input  logic         reset,
    input  logic [7:0]   clock_divider,

    output logic         spi_csb,
    output logic         spi_clock,
    output logic         spi_mosi,
    input  logic         spi_miso0,
    input  logic         spi_miso1,

    input  logic         readback_en,
    input  logic [31:0]  data_in_fifo_data,
    input  logic         data_in_fifo_empty,
    output logic         data_in_fifo_clock,
    output logic         data_in_fifo_rd_en,

    output logic [63:0]  data_out_fifo_data,
    input  logic         data_out_fifo_full,
    output logic         data_out_fifo_clock,
    output logic         data_out_fifo_wr_en,

    output logic         trigger,

    input logic          interruptB
);


logic [7:0] clock_div_counter;
logic [6:0] loop_counter;

logic [31:0] shift_data_in; //no reset needed

logic temp_clk;
logic clk_en;

logic first_read;

logic [4:0] read_cnt;

logic [2:0] init_writebuffer;

logic notfirst_round;

enum int {
    IDLE,
    LOAD_DATA,
    READ_DATA,
    WRITE_DATA,
    ENDING
} State;

assign data_in_fifo_clock = temp_clk;
assign data_out_fifo_clock = temp_clk;

//Assign Clock with correct phase and only when writing
assign spi_clock = (clk_en) && (CPHA ? ~temp_clk : temp_clk);

always_ff @(negedge spi_clock) begin
    //Read 2 MISO bits
    data_out_fifo_data <= {data_out_fifo_data[61:0], spi_miso0, spi_miso1};
end


always_ff @(posedge clock) begin : spi_clkdiv
    if(reset) begin
        clock_div_counter <= 0;
        temp_clk <= CPOL;
    end
    else begin
        if(clock_div_counter < clock_divider) clock_div_counter <= clock_div_counter + 8'b1;
        else begin
            clock_div_counter <= '0;
            temp_clk <= ~temp_clk;
        end
    end
end

always_ff @(posedge clock) begin
    if(reset) begin
        spi_csb <= 1;
        spi_mosi <= 0;

        data_in_fifo_rd_en <= 0;
        data_out_fifo_wr_en <= 0;
        notfirst_round <= 0;

        //data_out_fifo_data <= 64'b0; no reset needed

        loop_counter <= 0;

        clk_en <= 0;

        trigger <= 0;
        
        read_cnt <= 0;
        first_read <= 0;

        State <= IDLE;
        
        init_writebuffer <= 0;
    end
    else begin
        //SPI Clock divider
        if(clock_div_counter == clock_divider) begin

            if(temp_clk) begin
                case(State)
                    default: State <= IDLE;

                    IDLE: begin
                        data_out_fifo_wr_en <= '0; //Disable ReadFIFO
                        loop_counter <= '0;                    
                        
                        if(~data_in_fifo_empty) State <= LOAD_DATA; //~data_in_fifo_empty is 1 if words in in_fifo < 4
                        else if(!interruptB) begin
                            State <= READ_DATA;
                            spi_csb <= '0;
                        end
                    end

                    LOAD_DATA: begin //Load data mode
                        clk_en <= '0;

                        shift_data_in <= {shift_data_in[23:0], data_in_fifo_data[7:0]};

                        data_in_fifo_rd_en <= '1;

                        //load data to write in 4 clock cycles
                        if(loop_counter >= 3) begin
                            spi_csb <= '0; //Assign Chip-select one cycle before SPI clock starts

                            State <= WRITE_DATA;
                            loop_counter <= 'd32;
                        end
                        else loop_counter <= loop_counter + 7'b1;
                    end

                    READ_DATA: begin //Readonly mode
                        clk_en <= '1; // toggle clock
                        data_in_fifo_rd_en <= '0; // disable input fifo

                        if(loop_counter == 31) begin
                            //data_out_fifo_wr_en <= 1;
                            notfirst_round <= '1;
                            loop_counter <= '0;
                            if(interruptB && notfirst_round) begin// back to idle if hits are readout, at least 2*64 cycles
                                State <= ENDING;
                                //clk_en <= '0;
                                notfirst_round <= '0;
                            end
                        end
                        else begin
                            loop_counter <= loop_counter + 'b1;
                            data_out_fifo_wr_en <= '0;
                        end
                        
                        if(loop_counter == '0 && notfirst_round)
                            data_out_fifo_wr_en <= '1;
                            

                        spi_mosi <= '0;
                    end

                    WRITE_DATA: begin //RW mode

                        //Enable Clock
                        clk_en <= 1;

                        if(~data_out_fifo_full && read_cnt == 0 && first_read) begin
                            data_out_fifo_wr_en <= 1;
                            read_cnt <= 1;
                        end
                        else begin
                            data_out_fifo_wr_en <= 0;
                            read_cnt <= read_cnt + 1;
                        end
                        
                        first_read <= 1;

                        //Shift-out MOSI
                        spi_mosi <= shift_data_in[31];
                        //shift_data_in <= {shift_data_in[30:0],1'b0};
                        
                        if ((loop_counter % 8) == 7) begin

                            if(!data_in_fifo_empty) begin
                                data_in_fifo_rd_en <= 1;
                                shift_data_in <= {shift_data_in[30:7], data_in_fifo_data[7:0]};
                                loop_counter <= loop_counter - 7'd7;
                            end
                            else begin
                                if(init_writebuffer == 3) begin
                                    State <= ENDING;
                                end
                                
                                init_writebuffer <= init_writebuffer +1;
                                shift_data_in <= {shift_data_in[30:0], 1'b0};
                                loop_counter <= loop_counter + 7'b1;
                                data_in_fifo_rd_en <= 0;
                            end
                        end
                        else begin
                            shift_data_in <= {shift_data_in[30:0],1'b0};
                            data_in_fifo_rd_en <= 0;    
                            loop_counter <= loop_counter + 7'b1;
                        end
                            
                    end

                    ENDING: begin
                        //Disable spi_clock
                        clk_en <= 0;
                        
                        spi_mosi <= 0;

                        //Deassert chip-select
                        spi_csb <= 1;

                        trigger <= 0;

                        if(~data_out_fifo_full) begin //wait for space in the FIFO
                            data_out_fifo_wr_en <= 1;
                            State <= IDLE;
                        end
                    end
                endcase
            end
        end
    end
end

endmodule