`timescale 1ns / 1ps

module cmd_decoder_tb();

reg clock = 0;
reg reset = 0;
reg enable = 0;
reg wr_en = 0;

wire empty;
wire empty6;

reg [7:0] cnt = 0;
reg [7:0] din = 0;

wire out;

wire [7:0] data;

cmd_fifo cmd_fifo_i(
    .rst(reset),
    .din(din),
    .wr_en(wr_en),
    .wr_clk(clock),
    .full(cmd_fifo_full),
    .dout(data),
    .rd_clk(rd_clk),
    .rd_en(rd_en),
    .empty(empty),
    .prog_empty(empty6)
);

cmd_decoder dec(
    .reset(reset),
    .enable(enable),
    .clock(clock),
    
    .data(data),
    .rd_clk(rd_clk),
    .rd_en(rd_en),
    .fifo_empty(empty),
    .fifo_6entries(empty6),
    
    .cmd_line(out)
);

always #10 clock = ~clock;


always @(negedge clock) begin
    if(~reset)
        din <= din + 8'd1;
    if(din >= 8'd21 && din <= 8'd26) begin
        cnt <= cnt + 8'd1;
        wr_en <= 1;
    end
    else if(din >= 8'd51 && din <= 8'd56) begin
        wr_en <= 1;
        cnt <= cnt + 8'd1;
    end
    else /*if(cnt != 0)*/ begin
        cnt <= cnt - 8'd1;
        wr_en <= 0;
    end
end

reg [3:0] phase;
reg [4:0] out_phase;
reg [15:0] shift;
reg [15:0] out_put;
reg phasefound;

always @(posedge clock) begin
    if(reset) begin
        phase     <= 4'd0;
        out_phase <= 5'b10000;
        shift     <= 16'd0;
        out_put   <= 16'd0;
        phasefound <= 0;
    end
    else begin
        shift <= {out, shift[15:1]};
        phase <= phase + 4'd1;
        if(shift == {16'b1000_0001_0111_1110}) begin
            phasefound <= 1;
            out_phase <= {1'b0, phase};
            out_put <= shift;
        end
        else if(phase == out_phase[3:0] && ~out_phase[4])
            out_put <= shift;
        else
            phasefound <= 0;
    end
end

initial begin
    reset = 1;
    din = 8'b0000_0010;
    cnt = 8'd0;
    wr_en = 0;
    #100 reset = 0;
    #100 enable = 1;
end

endmodule
