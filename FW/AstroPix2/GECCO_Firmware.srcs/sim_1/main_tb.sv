module main_tb;

    // Clocks and reset
    //---------------
    logic resetn;
    wire  clk_100M;
    //wire  clk_60M;
    
    FM_ClkGen_MHz #(.MHZ(100)) clk_100M_gen (
        .clk(clk_100M)
    );
    
    // TB
    //--------
    initial 
    begin
    
        resetn = 0 ;
        
        #500 resetn = 1;
        
        
        #5000 $stop();
    end
    
    
    
    
    // DUT
    //----------
    wire lp_data_p;
    wire lp_data_n;
    main_top dut (
       .sysclk(clk_100M),
        
       .loopback_data_p(lp_data_n),
       .loopback_data_n(lp_data_n)

    );


endmodule