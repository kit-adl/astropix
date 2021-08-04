`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 08.05.2020 15:18:49
// Design Name: 
// Module Name: triggeredRO_align_tb
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


module triggeredRO_align_tb(

    );
  
reg clock = 0;
reg reset = 1;
    
reg [65:0] data = 66'h1;
reg [6:0] cnt = 0;
reg [7:0] index = 0;

always #10 clock = ~clock;

initial begin
    clock = 0;
    reset = 1;
    
    #100 reset = 0;
end

    
always @(posedge clock) begin
    if(cnt < 65) begin
        data[65:0] <= {data[64:0], data[65]};
        cnt <= cnt + 7'd1;
    end
    else begin
        data <= {48'd0, index, 8'd0, 2'b10};
        index <= index + 8'd1;
        cnt <= 0;
    end
end
    
triggered_data_aligner align(
    .clock(clock),
    .reset(reset),

    .block_update(),
    .datain(data[0]),
    .dataout(),
    .alignment_found()
);
endmodule
