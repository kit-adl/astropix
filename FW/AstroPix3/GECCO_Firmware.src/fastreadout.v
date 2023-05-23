/*
 * ATLASPix3_SoftAndFirmware
 * Copyright (C) 2019  Rudolf Schimassek (rudolf.schimassek@kit.edu)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 	KIT-ADL
// Engineer: 
// 
// Create Date: 06.08.2019 09:23:54
// Design Name: 
// Module Name: fastreadout
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


module fastreadout(

    input wire fast_clk_1200,   
    input wire fast_clk_600,
    input wire fast_clk_600p90,
    input wire fast_clk_150,
    input wire fast_clk_200,
    input wire clk,
    
    input wire TS_clk,
    
    
    input  wire        reset,
    input  wire        enable,
    input  wire        debug_output,
    input  wire        data_mux,
    //input  wire        keepback_bod,

    input  wire        shift_chip_clock,
    input  wire        shift_data_rec,
    input  wire        fine_shift_data_rec,
    //input  wire [1:0]  phaseshift_chip_clock,
    //input  wire [6:0]  phaseshift_block_clock,
    input  wire        disable_hitword1,
    input  wire        disable_hitword2,
    input  wire        data_switch,
    
    input  wire        use_CkRef,
    input  wire        trigger_chip, //for fast readout configuration trigger signal
    //input  wire [7:0]  trigger_delay,
    //input  wire [7:0]  trigger_duration,
    input  wire [7:0]  tsdiv,
    input  wire [7:0]  ts2div,
    input  wire [7:0]  tsphase,
    
    //input  wire        trigger_ref,     //trigger input for data taking
    input  wire [39:0] trigger_ts,
    input  wire [15:0] trigger_id,
    
    //data output to FIFO:
    output wire [63:0] FIFO_data_out,
    output wire        FIFO_wr_clock,
    output wire        FIFO_wr_enable,
    //input  wire        FIFO_full,
    
    //differential inputs and outputs:
    input  wire        ckref_chipout_p,
    input  wire        ckref_chipout_n,
    output wire        ckref_p,
    output wire        ckref_n,
    output wire        ckext_p,
    output wire        ckext_n,
    output wire        sync_rst_p,
    output wire        sync_rst_n,
    output wire        ext_trigger_p,
    output wire        ext_trigger_n,
    input  wire        data_p,
    input  wire        data_n,
    input  wire        data2_p,//fmc
    input  wire        data2_n,//fmc
    
    //debug outputs:
    output wire        alignment_found,
    output wire        debug_empty_data,
    output wire        debug_data_start,
    output wire        debug_ckref_from_chip,
    output wire        sync_rst,
    output wire        ts_overflow,
    
    //output wire        bit_clock,
    //output wire [8:0]  delayed_clock,
    
    //input  wire        invertreceiverclock,
    
    //configuration for trigger generation:
    input  wire        trigger_in,
    input  wire [15:0] trigger_count,
    input  wire [7:0]  trigger_length,
    input  wire [15:0] trigger_distance,
    input  wire [15:0] trigger_initialdelay,
    input  wire        trigger_synced,
    
    input  wire        realign,
    input  wire        aligndebug,
    input  wire        shortsearch,
    output wire        block_update,
    output wire        phase,
    input wire injection,
    output wire injtrigger
    
    
    //,
    //input  wire [2:0]  bitselect
    );

wire ckref_out;
wire data;
wire data_sync;

wire ckref_local;
wire ckref;
wire ext_trigger;
wire ckext_local;
wire ckext;
wire chip_clock;

assign ckref_local = fast_clk_150;
//assign ext_trigger = 0; //trigger_chip & trigger_ref;  //not used for untriggered Readout
//TODO: add something for the trigger...
//assign ckext_local = fast_clk_600;

assign ckext_local = 0;

assign debug_ckref_from_chip = ckref_out;

// === data receiving ===
wire [9:0] deser_output;



BUFG clkout(
   .I(ckext_local),
   .O(ckext)
   );
   
BUFG clkrefout(
    .I(ckref_local),
    .O(ckref)
    );







wire data_input;
wire data1_input;
IBUFDS #(
    .DIFF_TERM("FALSE"),
    .IBUF_DELAY_VALUE("0"),
    .IOSTANDARD("LVDS_25")
) IBUFDS_data (
    .O(data1_input),
    .I(data_p),
    .IB(data_n)
);



wire data2_input;

//ivan
/*
IBUFDS #(
    .DIFF_TERM("FALSE"),
    .IBUF_DELAY_VALUE("0"),
    .IOSTANDARD("LVDS_25")
) IBUFDS_data2 (
    .O(data2_input),
    .I(data2_p),
    .IB(data2_n)
);

*/

//assign data_input = (data_switch)?data1_input:data2_input;//ivan

//assign data_input = data2_input;//fmc


/*
wire sync;

datagen datagen_0(
.clk_400p(fast_clk_1200),
.reset(reset),
.injection(injection),
.bit_d_out_gen_del(data_input_debug),
.sync(sync)
);
*/







// === data alignment ===
wire [63:0] decoded_data;

wire [63:0] decoded_data0;
wire        block_update_0;
wire        alignment_found_0;
wire        phase_0;



/*
reg [5:0] data_input_par;
reg [5:0] data_input_ser;

reg [2:0] sercnt;

reg slowsync;

always @(posedge fast_clk_1200) begin
    
    data_input_ser <= {data_input_ser[4:0],data_input};
  
    
end

always @(posedge fast_clk_200) begin
    
    data_input_par <= data_input_ser;
    
end

*/     

wire [5:0] swapped;
wire [5:0] data_input_par;

selectio_wiz_0 serdes_0 (
   .data_in_from_pins_p(data2_p),
   .data_in_from_pins_n(data2_n),
   .data_in_to_device(swapped),
   .bitslip(0),                           
   .clk_in(fast_clk_600p90),//this works usually better
   //.clk_in(fast_clk_600),                             
   .clk_div_in(fast_clk_200),                        
   .io_reset(0)
   );
   
   
   /*
   selectio_wiz_0 serdes_0 (
      .data_in_from_pins_p(data2_p),
      .data_in_from_pins_n(data2_n),
      .data_in_to_device(swapped),
      .bitslip(0),                           
      .clk_in(~fast_clk_1200),                            
      .clk_div_in(fast_clk_200),                        
      .io_reset(0)
      );
   */
 
assign data_input_par[5:0] = {swapped[0],swapped[1],swapped[2],swapped[3],swapped[4],swapped[5]}; 

triggered_data_alignment_parallel align_0(
    .clock(fast_clk_200),//ivan
    .reset(reset || ~enable),
    .realign(realign),
    //.realign(1),
    .shortsearch(shortsearch),
    .debug(aligndebug), //debug_output),   //for sending data about alignment process    
    .block_update(block_update_0),
    .datain(data_input_par),
    .dataout(decoded_data0),
    .alignment_found(alignment_found_0),
    .phasemsb(phase_0),
    .sync(slowsync)

);


assign decoded_data[63:0] = decoded_data0[63:0];
assign block_update       = block_update_0;
assign alignment_found    = alignment_found_0;
assign phase              = phase_0;

//time stamp generation:
wire [39:0] ts;
wire [31:0] ts2;
triggered_TSGenerator tsgen(
    .clock(TS_clk),
    .reset(reset),
    .enable(enable),
    
    .tsdiv(tsdiv),
    .ts2div(ts2div),
    .tsphase(tsphase[7:0]),
    
    .tsout(ts),
    .ts2out(ts2),
    .syncReset(sync_rst),
    .overflowsync(ts_overflow),
    .trigger(ext_trigger),
    .injtrigger(injtrigger)
);


//data interpreter:
wire wr_en;
assign FIFO_wr_enable = wr_en & trigger_chip; //only write to FIFO if trigger is enabled
triggered_statemachine SM(
    .clock(clk),//fast_clk_200
    //.clock(fast_clk_200),//fast_clk_200
    .block_update(block_update),
    .reset(reset),
    .enable(enable),
    .debug(debug_output || aligndebug),
    //.keepback_bod(keepback_bod),
    .disable_hitword1(disable_hitword1),
    .disable_hitword2(disable_hitword2),
    
    .datain(decoded_data[63:0]),
    .ts_from_SM(ts),
    .ts2_from_SM(ts2),
    .trigger_ts(trigger_ts),
    .trigger_id(trigger_id[15:0]),
    
    .dataset(FIFO_data_out[63:0]),
    .FIFO_wr_clk(FIFO_wr_clock),
    .FIFO_wr_enable(wr_en),
    
    .debug_found_empty_output(debug_empty_data),
    .debug_found_data_start(debug_data_start)
);

//buffers for input and output:

IBUFDS #(
    .DIFF_TERM("FALSE"),
    .IBUF_DELAY_VALUE("0"),  
    .IOSTANDARD("LVDS_25")
) IBUFDS_ckref_out (
    .I(ckref_chipout_p),
    .IB(ckref_chipout_n),
    .O(ckref_out)
);

wire [3:0] obuf_i;
wire [3:0] obuf_p;
wire [3:0] obuf_n;
assign obuf_i = {sync_rst, ckref, ext_trigger, ckext};
assign obuf_p = {sync_rst_p, ckref_p, ext_trigger_p, ckext_p};
assign obuf_n = {sync_rst_n, ckref_n, ext_trigger_n, ckext_n};

genvar i;
generate
    for(i = 0; i < 4; i = i + 1) begin
        OBUFDS #(
            .IOSTANDARD("LVDS_25")
        ) OBUFDS_I (
            .I(obuf_i[i]),
            .O(obuf_p[i]),
            .OB(obuf_n[i])
        );
    end
endgenerate

endmodule
