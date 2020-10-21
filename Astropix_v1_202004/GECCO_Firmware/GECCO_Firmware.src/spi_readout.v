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
// Create Date: 08.08.2019
// Design Name: 
// Module Name: SPI Readout for ATLASPix3
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

module spi_readout(
    input  wire         clock,
    input  wire         reset,
    input  wire [7:0]   clock_divider,
    
    output reg          spi_csb,
    output reg          spi_clock,
    output reg          spi_mosi,
    input  wire         spi_miso,
    
    input  wire         readback_en,
    input  wire [7:0]   data_in_fifo_data,
    input  wire         data_in_fifo_empty,
    output wire         data_in_fifo_clock,
    output reg          data_in_fifo_rd_en,
    
    output reg  [63:0]  data_out_fifo_data,
    input  wire         data_out_fifo_full,
    output wire         data_out_fifo_clock,
    output reg          data_out_fifo_wr_en,
    
    output reg          trigger
);

assign data_in_fifo_clock = spi_clock;
assign data_out_fifo_clock = spi_clock;
reg [6:0] clock_div_counter = 0;

reg [23:0] shift_data_in = 0;

reg [2:0] state = 0;
reg [6:0] loop_counter = 0;

parameter idle = 3'b000;
parameter load_data = 3'b001;
parameter read_data = 3'b010;
parameter write_data = 3'b011;
parameter ending     = 3'b100;

always @(posedge clock) begin
    if(reset) begin
        spi_csb <= 1;
        spi_clock <= 0;
        spi_mosi <= 0;
        
        data_in_fifo_rd_en <= 0;
        
        data_out_fifo_data <= 64'b0;
        data_out_fifo_wr_en <= 0;
        
        clock_div_counter <= 0;
        loop_counter <= 0;
        
        trigger <= 0;
    end
    else begin
        if(clock_div_counter[6:0] < clock_divider[7:1])
            clock_div_counter <= clock_div_counter + 8'b1;
        else begin
            clock_div_counter <= 0;
            
            spi_clock <= ~spi_clock;
            
            if(spi_clock == 1) begin
                case(state)
                    idle: begin
                        data_out_fifo_wr_en <= 0;
                        loop_counter <= 0;
                        if(~data_in_fifo_empty)
                            state <= load_data;
                    end
                    load_data: begin //load the data to write in 3 clock cycles
                        shift_data_in <= {shift_data_in[15:0], data_in_fifo_data[7:0]};
                        data_in_fifo_rd_en <= 1;
                        if(loop_counter >= 2) begin
                            spi_csb <= 0;
                            if(readback_en) begin
                                if(shift_data_in[16] == 1)
                                    trigger <= 1;
                                state <= read_data;
                                loop_counter <= 0;
                             end
                            else begin
                                state <= write_data;
                                loop_counter <= 40;
                            end
                        end
                        else
                            loop_counter <= loop_counter + 7'b1;
                    end
                    read_data: begin  //read the first 40 bits of the 64 bit data to read
                        data_in_fifo_rd_en <= 0;
                        loop_counter <= loop_counter + 7'b1;
                        data_out_fifo_data <= {data_out_fifo_data[62:0], spi_miso};
                        if(loop_counter >= 39) // 40-1)
                            state <= write_data;
                    end
                    write_data: begin
                        data_in_fifo_rd_en <= 0;
                        loop_counter <= loop_counter + 7'b1;
                        data_out_fifo_data <= {data_out_fifo_data[62:0], spi_miso};
                        spi_mosi <= shift_data_in[23];
                        shift_data_in <= {shift_data_in[22:0],1'b0};
                        if(loop_counter >= 63) begin
                            state <= ending;
                        end
                    end
                    ending: begin
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