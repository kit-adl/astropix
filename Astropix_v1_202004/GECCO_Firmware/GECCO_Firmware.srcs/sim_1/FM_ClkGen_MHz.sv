
module FM_ClkGen_MHz #(parameter MHZ = 100,parameter OFFSET=0)(
        output logic clk
    );


// Half the period in ns
    parameter delay   = ((1.0 / (MHZ*1000000)) * 1000000000 )/ 2.0;
    logic     started = 0;
    //parameter offset_delay = delay * OFFSET_RATIO;
    initial
    begin
        clk             = 1'b0;
        #OFFSET started = 1'b1;
    end

    always
    begin
        wait(started==1);
        #delay clk <= !clk;
    end

endmodule

