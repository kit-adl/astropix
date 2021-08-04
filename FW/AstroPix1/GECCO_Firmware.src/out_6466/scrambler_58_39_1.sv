module scrambler_58_39_1(
        input wire clk,
        input wire res,

        input wire [63:0] DataIn,
        input wire        Ena,

        output logic [63:0] DataOut
    );

 /*   logic [127:0] scrambled_data;
    logic [63:0] reversed_data;

    assign DataOut =  {scrambled_data[127:64]};

    genvar j;
    generate

        for (j=0; j<64; j++) begin
            always_comb
            begin
                reversed_data[j] = DataIn[63-j];
                //reversed_data[j] = DataIn[j];
            end
                
        end
    endgenerate

    genvar i;
    generate

        for (i=64; i<128; i++) begin
            always_comb
                scrambled_data[i] = reversed_data[127-i] ^ (scrambled_data[i-58] ^ scrambled_data[i-39]);
        end
    endgenerate


    always_ff @(posedge clk, posedge res) begin
        if (res)
        begin
            scrambled_data[63:0] <= '{default:1};
        end
        else
        begin
            if (Ena)
                scrambled_data[63:0] <= scrambled_data[127:64];
        end

    end
*/

 lfsr_scramble  #(.REVERSE(0)) lfsr_sc_I (
        .clk(clk),
        .rst(res),
        .data_in(DataIn),
        .data_in_valid(Ena),
        .data_out(DataOut)
    );
    

endmodule

