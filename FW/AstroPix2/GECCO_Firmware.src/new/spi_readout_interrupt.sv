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

module spi_readout_interrupt#(
    parameter CPOL = 0,
    parameter CPHA = 1)
(
    input  logic         clock,
    input  logic         reset,
    input  logic [7:0]   clock_divider,

    input  logic         interruptB,
    input  logic         interrupt_mode,

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
    output logic         data_out_fifo_wr_en
);


logic [6:0] clock_div_counter;
logic [6:0] loop_counter;

logic [31:0] shift_data_in; //no reset needed

logic temp_clk;
logic clk_en;

logic first_read;

logic [4:0] read_cnt;

logic [2:0] init_writebuffer;

enum logic [2:0] {
    idle         = 3'b000,
    load_data    = 3'b001,
    read_data    = 3'b010,
    write_data   = 3'b011,
    ending       = 3'b100
} state;

assign data_in_fifo_clock = temp_clk;
assign data_out_fifo_clock = temp_clk;

//Assign Clock with correct phase and only when writing
assign spi_clock = (clk_en) && (CPHA ? ~temp_clk : temp_clk);

always_ff @(negedge spi_clock) begin
    //Read 2 MISO bits
    data_out_fifo_data <= data_out_fifo_data[63:0] << 2 | spi_miso0 << 1 | spi_miso1;
end

always_ff @(posedge clock) begin
    if(reset) begin
        spi_csb <= 1;
        spi_mosi <= 0;

        temp_clk <= CPOL;

        data_in_fifo_rd_en <= 0;
        data_out_fifo_wr_en <= 0;

        //data_out_fifo_data <= 64'b0; no reset needed

        clock_div_counter <= 0;
        loop_counter <= 0;

        clk_en <= 0;

        read_cnt <= 0;
        first_read <= 0;

        state <= idle;

        init_writebuffer <= 0;
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
                    default: state <= idle;

                    idle: begin

                        //Disable ReadFIFO
                        data_out_fifo_wr_en <= 0;

                        loop_counter <= 0;

                        //~data_in_fifo_empty is 1 if words in in_fifo < 4
                        if(interrupt_mode)
                            state <= read_data;
                        else if(~data_in_fifo_empty)
                            state <= load_data;
                    end

                    load_data: begin //Load data mode
                        clk_en <= 0;

                        //don't load data in read-only mode
                        if(readback_en) begin
                            state <= read_data;
                            loop_counter <= 32;
                        end else begin
                            shift_data_in <= {shift_data_in[23:0], data_in_fifo_data[7:0]};

                            data_in_fifo_rd_en <= 1;

                            //load data to write in 4 clock cycles
                            if(loop_counter >= 3) begin

                                //Assign Chip-select one cycle before SPI clock starts
                                spi_csb <= 0;

                                state <= write_data;
                                loop_counter <= 32;

                            end
                            else
                                loop_counter <= loop_counter + 7'b1;
                        end
                    end

                    read_data: begin //RW mode

                        spi_csb <= 0;

                        if(!interruptB && !data_out_fifo_full) begin
                            //Enable Clock
                            clk_en <= 1;

                            //Disable WriteFIFO
                            data_in_fifo_rd_en <= 0;

                            //Shift-out MOSI
                            spi_mosi <= 0; //shift_data_in[31];

                            if(loop_counter == 63) begin
                                loop_counter <= 0;
                                state <= ending;
                            end
                            else
                                loop_counter <= loop_counter + 1;
                        end

                    end

                    write_data: begin //RW mode

                        //Enable Clock
                        clk_en <= 1;

                        //Disable WriteFIFO
                        //data_in_fifo_rd_en <= 0;

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

                        if ((loop_counter % 8) == 7) begin

                            if(!data_in_fifo_empty) begin
                                data_in_fifo_rd_en <= 1;
                                shift_data_in <= {shift_data_in[30:7], data_in_fifo_data[7:0]};
                                loop_counter <= loop_counter - 7'd7;
                            end
                            else begin
                                if(init_writebuffer == 3) begin
                                    state <= ending;
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

                    ending: begin
                        //Always write MISO stream to ReadFIFO
                        //data_out_fifo_wr_en <= 1;

                        //Disable spi_clock
                        clk_en <= 0;

                        spi_mosi <= 0;

                        //Deassert chip-select
                        spi_csb <= 1;

                        if(~data_out_fifo_full) begin //wait for space in the FIFO
                            data_out_fifo_wr_en <= 1;
                            state <= idle;
                        end

                        state <= idle;
                    end
                endcase
            end
        end
    end
end

endmodule