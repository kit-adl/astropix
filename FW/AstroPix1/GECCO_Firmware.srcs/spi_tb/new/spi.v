`timescale 1 ns/1 ps

module spi_readout_test_tb;

parameter CPOL = 0;
parameter CPHA = 1;
parameter CS_INACTIVE_CLKS = 2;

reg clk;

reg reset;
reg [7:0] clk_div;
reg [31:0] tx_data;

reg data_in_fifo_empty;

wire spi_csb;
reg spi_miso;
wire spi_mosi;
wire spi_sck;

reg readback_en;


reg spi_write_fifo_empty;
wire spi_write_fifo_rd_clk;
wire spi_write_fifo_rd_en;

wire [63:0] spi_read_fifo_din;
reg spi_read_fifo_full;
wire spi_read_fifo_wr_clk;
wire spi_read_fifo_wr_en;

spi_readout #(
    .CPOL(CPOL),
    .CPHA(CPHA))

spi_readout_i 
(
    .clock(clk),
    .reset(reset),
    .clock_divider(clk_div),
    
    .spi_csb(spi_csb),
    .spi_clock(spi_sck),
    .spi_mosi(spi_mosi),
    .spi_miso(spi_miso),
    
    .readback_en(spi_config_readback_en),
    .data_in_fifo_data(tx_data),
    .data_in_fifo_clock(spi_write_fifo_rd_clk),
    .data_in_fifo_rd_en(spi_write_fifo_rd_en),
    .data_in_fifo_empty(data_in_fifo_empty),
    
    .data_out_fifo_data(spi_read_fifo_din),
    .data_out_fifo_clock(spi_read_fifo_wr_clk),
    .data_out_fifo_wr_en(spi_read_fifo_wr_en),
    .data_out_fifo_full(spi_read_fifo_full),
    
    .trigger(spi_trigger)
);

initial begin
//Reset
clk = 0;
reset=1;
data_in_fifo_empty=1;
clk_div=3;
spi_miso=1'bZ;

readback_en=0;
#10
reset=0;
#30
data_in_fifo_empty=0;
tx_data='h01020304;
#10
//data_in_fifo_empty=1;
wait((spi_csb==0) & (spi_sck == 1))
spi_miso=1;
#8
spi_miso=0;
#8
spi_miso=1;
#8
spi_miso=0;
#8
spi_miso=1;
#8
spi_miso=0;
#8
spi_miso=1;
#100
spi_miso=1'bZ;
end

always #2 clk = ~clk;


endmodule