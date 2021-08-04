
`ifndef FIFO_INTERFACES
    `define FIFO_INTERFACES

    interface FIFO_SIMPLE #(parameter WIDTH=8);

        logic [WIDTH-1:0] data;
        logic [WIDTH-1:0] data_next;

        logic shift_in;
        logic shift_out;

        logic empty;
        logic almost_empty;
        logic full;
        logic almost_full;




        // Producer/Receiver scheme uses shift_in to create data or get some
        modport receiver(input shift_in,input data,output empty,output full);
        modport producer(output shift_in,output data,input empty,input full);

        // Source/Sink scheme is from the view of the fifo. Can I accept data or provide some
        // Source Actors have data and can provide them
        //--------------

        /**
         * The Write Port modport to be used by modules writting to a FIFO
         */
        modport write(output data, output shift_in,input full,input empty, input almost_full, input almost_empty, import set_no_shift_in , import set_shift_in , import shift_in_data,import set_data,import zero_data);
        modport write_only_full(output data, output shift_in,input full, import set_no_shift_in , import set_shift_in , import shift_in_data,import set_data,import zero_data);
        
        modport reading( input data,output shift_out,input full,input empty,input almost_full,input almost_empty, import set_no_shift_out,import set_shift_out);
        modport reading_with_next( input data, input data_next, output shift_out,input full,input empty,input almost_full,input almost_empty, import set_no_shift_out,import set_shift_out);

        /**
         * Write Port exposed by the receiver of data
         */
        modport written(input data, input shift_in,output full,output empty, output almost_full,output almost_empty, import set_full,import set_empty,import set_almost_empty);
        modport written_only_full(input data, input shift_in,output full, import set_full, import set_not_full);
        modport written_notasks(input data, input shift_in,output full,output empty, output almost_full,output almost_empty);
        modport read( output data,input shift_out,output full,output empty,output almost_full,output almost_empty);
        modport read_with_next( output data, output data_next,input shift_out,output full,output empty,output almost_full,output almost_empty);


        task zero_data();
            data <= {{WIDTH}{1'b0}};
        endtask
        task set_data(input logic [WIDTH-1:0] idata);
            data <= idata;
        endtask
        task shift_in_data(input logic [WIDTH-1:0] idata);
            data     <= idata;
            shift_in <= 1'b1;
        endtask
        task set_no_shift_in();
            shift_in <= 1'b0;
        endtask
        task set_shift_in();
            shift_in <= 1'b1;
        endtask

        task set_shift_out();
            shift_out <= 1'b1;
        endtask
        task set_no_shift_out();
            shift_out <= 1'b0;
        endtask

        task set_full();
            full         <= 1'b1;
            empty        <= 1'b0;
            almost_full  <= 1'b1;
            almost_empty <= 1'b0;
        endtask
        task set_not_full();
            full         <= 1'b0;
        endtask
        task set_almost_full();
            almost_full <= 1'b1;
            full <= 1'b0;
        endtask
         task unset_almost_full();
            almost_full <= 1'b0;
        endtask
        task set_empty();
            full         <= 1'b0;
            almost_full  <= 1'b0;
            empty        <= 1'b1;
            almost_empty <= 1'b1;
        endtask
        task set_not_empty();
            empty        <= 1'b0;
        endtask
        task set_almost_empty();
            almost_empty <= 1'b1;
            empty <= 1'b0;
        endtask
       task unset_almost_empty();
            almost_empty <= 1'b0;
        endtask

      

    endinterface

`endif