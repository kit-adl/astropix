`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 28.04.2020 14:33:05
// Design Name: 
// Module Name: udp_sender_tb
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module udp_sender_tb(

    );

reg clock = 0;
reg reset = 1;
reg enable = 0;
    
wire       source_clk;
wire [7:0] source_data;
wire       source_invalid;
wire       source_rd_en;
    
ethernet_data_sender send(
    .clk(clock),
    .reset(reset),
    .packagesize(8'd4),
    .restartdelay(16'd20),
    .enable(enable),
    .timeout(24'd200),
    
    .datain_clk(source_clk),
    .datain(source_data),
    .datain_valid(~source_invalid),
    .datain_rd_en(source_rd_en),
    
    .dataout(),
    .dataout_valid(),
    .dataout_ready(1),
    .dataout_lastbyte(),
    .dataout_package()
);

reg [63:0]  fifo_data = 64'd0;
reg         fifo_wr_en = 0;    

ethernet_fifo_buffer ethernet_fastreadout_fifo(
    .rst(reset),
	
    .din(fifo_data),
    .wr_en(fifo_wr_en),
    .wr_clk(clock),
    .full(),
	
    .dout(source_data),
    .rd_en(source_rd_en),
    .rd_clk(source_clk),
    .empty(source_invalid)
);

always #10 clock = ~clock;

reg writedata = 0;
reg [7:0] counter = 8'd0;
reg delay = 1;

initial begin
    reset = 1;
    enable = 0;
    #100 reset = 0;
    #100 enable = 1;
    #2000 writedata = 1;
end

always @(posedge clock) begin
    if(reset) begin
        counter <= 8'd0;
    end
    else begin
        counter <= counter + 8'd1;
        if(writedata) begin
            fifo_data <= {8'd1, counter, 16'h0304, counter, 16'h0607, 8'h08};
            fifo_wr_en <= 1;
            //delay <= 1;
            writedata <= 0;
        end
        else if(counter[3:0] == 4'd7) begin
            fifo_wr_en <= 0;
            writedata <= 1;
        end
        else if(fifo_wr_en == 1 && delay == 1)
            delay <= 0;
        else
            fifo_wr_en <= 0;
    end
end

endmodule
