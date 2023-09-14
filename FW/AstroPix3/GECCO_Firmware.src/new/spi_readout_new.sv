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

    input logic          interruptB
);

localparam logic [7:0] IDLE_BYTE = 8'hbc;

logic [7:0] clock_div_counter;
logic [6:0] loop_counter;
logic [31:0] shift_data_in;
logic temp_clk;
logic clk_en;
logic first_read;
logic [4:0] read_cnt;
logic [2:0] init_writebuffer;
logic notfirst_round;
logic idle;

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

// Check if last 5 bytes were idle bytes
assign idle = data_out_fifo_data[39:0] == {5{IDLE_BYTE[1:0],IDLE_BYTE[7:2]}};

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
    if(reset) State <= IDLE;
    else begin
        //SPI Clock divider
        if(clock_div_counter == clock_divider) begin

            if(temp_clk) begin
                case(State)
                    default: State <= IDLE;

                    IDLE: begin
                        if(~data_in_fifo_empty) State <= LOAD_DATA; //~data_in_fifo_empty is 1 if words in in_fifo < 4
                        else if(!interruptB && !data_out_fifo_full) State <= READ_DATA;
                    end

                    LOAD_DATA: if(loop_counter >= 3) State <= WRITE_DATA;

                    READ_DATA: begin //Readonly mode
                        if(loop_counter == 31)
                            if((interruptB && idle) || data_out_fifo_full)
                                State <= ENDING;
                    end

                    WRITE_DATA: if ((loop_counter % 8) == 7 && data_in_fifo_empty && init_writebuffer == 3) State <= ENDING;

                    ENDING: if(~data_out_fifo_full) State <= IDLE;
                endcase
            end
        end
    end
end

always_ff @(posedge clock) begin
    if(reset) begin
        spi_csb <= '1;
        spi_mosi <= '0;

        data_in_fifo_rd_en <= '0;
        data_out_fifo_wr_en <= '0;
        notfirst_round <= '0;

        loop_counter <= '0;

        clk_en <= '0;

        read_cnt <= '0;
        first_read <= '0;

        init_writebuffer <= '0;
    end
    else begin
        //SPI Clock divider
        if(clock_div_counter == clock_divider) begin

            if(temp_clk) begin
                case(State)
                    IDLE: begin
                        data_out_fifo_wr_en <= '0; //Disable ReadFIFO
                        loop_counter <= '0;
                        notfirst_round <= '0;

                        if(!interruptB && !data_out_fifo_full) spi_csb <= '0;
                    end

                    LOAD_DATA: begin //Load data mode
                        clk_en <= '0;

                        shift_data_in <= {shift_data_in[23:0], data_in_fifo_data[7:0]};

                        data_in_fifo_rd_en <= '1;

                        //load data to write in 4 clock cycles
                        if(loop_counter >= 3) begin
                            spi_csb <= '0; //Assign Chip-select one cycle before SPI clock starts
                            loop_counter <= 'd32;
                        end
                        else loop_counter <= loop_counter + 7'b1;
                    end

                    READ_DATA: begin //Readonly mode
                        clk_en <= '1; // toggle clock
                        data_in_fifo_rd_en <= '0; // disable input fifo

                        if(loop_counter == 31) begin
                            loop_counter <= '0;
                            notfirst_round <= '1;
                        end
                        else
                            loop_counter <= loop_counter + 'b1;

                        if(loop_counter == '0 && notfirst_round) data_out_fifo_wr_en <= '1;
                        else data_out_fifo_wr_en <= '0;

                        spi_mosi <= '0;
                    end

                    WRITE_DATA: begin //RW mode

                        //Enable Clock
                        clk_en <= 1;

                        if(~data_out_fifo_full && read_cnt == '0 && first_read) begin
                            data_out_fifo_wr_en <= '1;
                            read_cnt <= 'd1;
                        end
                        else begin
                            data_out_fifo_wr_en <= '0;
                            read_cnt <= read_cnt + 'd1;
                        end

                        first_read <= '1;

                        //Shift-out MOSI
                        spi_mosi <= shift_data_in[31];

                        if ((loop_counter % 8) == 'd7) begin

                            if(!data_in_fifo_empty) begin
                                data_in_fifo_rd_en <= '1;
                                shift_data_in <= {shift_data_in[30:7], data_in_fifo_data[7:0]};
                                loop_counter <= loop_counter - 7'd7;
                            end
                            else begin
                                init_writebuffer <= init_writebuffer + 'd1;
                                shift_data_in <= {shift_data_in[30:0], 1'b0};
                                loop_counter <= loop_counter + 7'b1;
                                data_in_fifo_rd_en <= '0;
                            end
                        end
                        else begin
                            shift_data_in <= {shift_data_in[30:0],1'b0};
                            data_in_fifo_rd_en <= '0;
                            loop_counter <= loop_counter + 7'b1;
                        end

                    end

                    ENDING: begin

                        clk_en <= '0; //Disable spi_clock
                        spi_mosi <= '0;
                        spi_csb <= '1; //Deassert chip-select

                        if(~data_out_fifo_full) data_out_fifo_wr_en <= '1; //wait for space in the FIFO
                    end
                endcase
            end
        end
    end
end

endmodule