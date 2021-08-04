`timescale 1ns/1ps
//Reset active low
module FM_RstnGen(
        output logic res_n,
        input  bit   ref_clk

    );

    initial
    begin
        res_n = 1'b1;
    end

    task doResetNclkCycles;
        input integer nClkCycles;
        integer count;

        begin
            @(posedge ref_clk)
                enableReset;
            for (count = 0; count < nClkCycles; count = count +1)
            begin
                @(posedge ref_clk)
                    #0.005;
            end
            disableReset;
        end
    endtask

    task enableReset;
        begin
            #0.005 res_n = 1'b0;
        end
    endtask

    task disableReset;
        begin
            #0.005 res_n = 1'b1;
        end
    endtask



endmodule
