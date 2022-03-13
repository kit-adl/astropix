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
// Create Date: 21.03.2016 12:04:51
// Design Name:
// Module Name: main_top
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
// TODO:
// - sampleclock diff/se
// - Timestampclock divide/2
//////////////////////////////////////////////////////////////////////////////////

/////////////Uncomment if LVDS Receivers are bypassed from IN_P to OUT////////////
//`define se_clock //Uncomment if single-ended sampleclock output should be used
//`define se_clock_singleended //Uncomment if LVDS Receiver is not asembled on carrier pcb, remember to connect IN+ with Out

//`define config_singleended //Uncomment if GECCO Board has no lvds receivers for SR config


module main_top(

    //Nexys

    input        cpu_resetn,
    input        sysclk,
    input [7:0]  sw,
    input        btnc,
    input        btnr,
    input        btnl,
    input        btnd,
    input        btnu,
    output [7:0] led,

    //voltage adjustment
    output [1:0] set_vadj, //Set FMC Voltage to 1.8V
    output       vadj_en,

    //FTDI
    inout  [7:0] prog_d,
    input        prog_rxen,
    input        prog_txen,
    output       prog_rdn,
    output       prog_wrn,
    output       prog_siwun,
    output       prog_oen,
    input        prog_clko,

    //Astropix

    //Asic config SR
    input        config_sout,
    output       config_sin_p, // ConfigCard 3-> C3
    output       config_sin_n, // ConfigCard 3-> C3
    output       config_ck1_p, // ConfigCard 3-> C4
    output       config_ck1_n, // ConfigCard 3-> C4
    output       config_ck2_p, // ConfigCard 3-> C2
    output       config_ck2_n, // ConfigCard 3-> C2
    output       config_ld_p, // ConfigCard 3-> C1
    output       config_ld_n, // ConfigCard 3-> C1
    output       config_rb,

    //SPI left:
    output       spi_left_clk,
    output       spi_left_csn,
    input        spi_left_miso0,
    input        spi_left_miso1,
    output       spi_left_mosi,

    //SPI right:
    input        spi_right_clk,
    input        spi_right_csn,
    output       spi_right_miso0,
    output       spi_right_miso1,
    input        spi_right_mosi,

    //Astropix Digital Pins
    input interrupt,
    output reg res_n,
    output hold,

    //Chip Config debug output
    output debug_spi_csn,
    output debug_spi_sck,
    output debug_spi_mosi,
    output debug_spi_miso0,
    output debug_spi_miso1,

    //output config_res_n_test,

    //Astropix Sample Clk
    output sample_clk_n,
    output sample_clk_p,

    output sample_clk_se_n,
    output sample_clk_se_p,

    output timestamp_clk,

    //GECCO

    //VB Config debug output:
    output vb_clock_test,
    output vb_data_test,
    output vb_load_test,

    //Injection:
    output       gecco_inj_chopper_p,
    output       gecco_inj_chopper_n,
    output       chip_inj_chopper,

    //Voltage Boards:
    output       vb_clock_p,
    output       vb_clock_n,
    output       vb_data_p,
    output       vb_data_n,
    output       vb_load_p,
    output       vb_load_n

);

wire clk;
IBUFG clk_ibuf_inst(
    .I(sysclk),
    .O(clk)
);

//voltage selection on FMC:
assign vadj_en      = 1;
assign set_vadj[1]  = 0;
assign set_vadj[0]  = 1;

assign prog_siwun = 1;   //important for reading from FPGA


//Astropix2
assign hold = 1'b0;

// FTDI Communication / Order Sorter:
// TODO: remove?
wire        ordersorter_header0;
wire        ordersorter_read;
wire [7:0]  ordersorter_address;
wire [7:0]  ordersorter_data;

// Pattern Generator:
wire        patgen_Reset;
wire        patgen_Suspend;
wire        patgen_writeStrobe;
wire [7:0]  patgen_address;
wire [7:0]  patgen_data;
wire        injection_gecco;
wire        injection_chip;
wire        patgen_synced;
wire        patgen_tsoverflow_sync;
wire [7:0]  patgen_skipsignals;

// configuration:
wire config_sin;
wire config_ck1;
wire config_ck2;
wire config_ld;
wire config_res_n;

wire cmd;
wire vb_clock;
wire vb_data;
wire vb_load;

// SPI:
wire        spi_config_reset;
wire [7:0]  spi_clock_divider;
wire [31:0] spi_write_fifo_dout;
wire        spi_write_fifo_rd_clk;
wire        spi_write_fifo_rd_en;
wire        spi_write_fifo_empty;

wire [63:0] spi_read_fifo_din;
wire        spi_read_fifo_wr_clk;
wire        spi_read_fifo_wr_en;
wire        spi_read_fifo_full;
wire        spi_config_readback_en;

// Clocks
//assign sample_clk_se = 1'b0;
//assign sample_clk_se_n = 1'b0;

// assign fast_clk_sampleclk = 1'b0;

assign config_rb = 1'b0;

`ifdef se_clock
    assign sample_clk_se = fast_clk;
    assign fast_clk_sampleclk = 1'b0;
`else
    assign fast_clk_sampleclk = fast_clk;
    assign sample_clk_se = 1'b0;
    `ifdef se_clock_singleended
        assign sample_clk_se_n = 1'b0;
    `endif
`endif

// FTDI Configuration:
ftdi_top ftdi_top_I(
    .clk(clk),
    .res_n(cpu_resetn),
    .prog_clko(prog_clko),
    .FTDI_TXE_N(prog_txen),
    .FTDI_RXF_N(prog_rxen),
    .FTDI_RD_N(prog_rdn),
    .FTDI_OE_N(prog_oen),
    .FTDI_WR_N(prog_wrn),
    .FTDI_DATA(prog_d),

    .ChipConfig_Clock1(config_ck1),
    .ChipConfig_Clock2(config_ck2),
    .ChipConfig_Data(config_sin),
    .ChipConfig_Load(config_ld),
    .ChipConfig_Res_n(config_res_n),
    .ChipConfig_Readback(),//(config_rb),

    .ChipConfig_LdDAC(),
    .ChipConfig_LdConfig(),
    .ChipConfig_LdVDAC(),
    .ChipConfig_LdTDAC(),
    .ChipConfig_LdRow(),
    .ChipConfig_LdColumn(),
    .ChipConfig_WrRAM(),
    .ChipConfig_no_sr(),

    .reset_autoreset_analog(),
    .reset_regulator_reset_out(),
    .reset_reset_analog_b(),
    .reset_autoreset_digital(),
    .reset_por(),
    .reset_reset_digital_b(),
    .reset_por_test_reset(),
    .reset_autoreset_combine(),

    .config_mode_use_spi(),
    .config_mode_bypass_cmd(),
    .config_mode_encdr(),
    .config_mode_en_pll(),
    .config_mode_cmd_clock_invert(),
    .config_mode_interface_speed(),
    .config_mode_take_fast(),

    .work_mode_sel_ext(),
    .work_mode_always_enable_b(),
    .work_mode_untriggered_ro_en(),
    .work_mode_trig_ro_reset_n(),

    .patgen_Reset(patgen_Reset),
    .patgen_Suspend(patgen_Suspend),
    .patgen_writeStrobe(patgen_writeStrobe),
    .patgen_address(patgen_address[7:0]),
    .patgen_data(patgen_data[7:0]),
    .patgen_synced(patgen_synced),
    .injection_gecco(injection_gecco),
    .injection_chip(injection_chip),
    .patgen_tsoverflow_sync(patgen_tsoverflow_sync),
    .patgen_skipsignals(patgen_skipsignals),


    .fastreadout_data(),
    .fastreadout_control_enable(),
    .fastreadout_control_fifoclear(),
    .fastreadout_control_rst(),
    .fastreadout_control_trigger(),
    .fastreadout_control_debug(),
    .fastreadout_control_datamux(),
    .fastreadout_control_realign(),
    .fastreadout_status_empty(),
    .fastreadout_status_full(),
    .fastreadout_status_Datasetstart(),
    .fastreadout_status_hw_write(),
    .fastreadout_fifo_reset(),
    .fastreadout_fifo_din(),
    .fastreadout_fifo_wr_en(),
    .fastreadout_fifo_wr_clk(),
    .fastreadout_fifo_dout(),   // debug output
    .fastreadout_fifo_full(),
    .fastreadout_fifo_empty(),
    .fastreadout_fifo_progfull(),
    .fastreadout_trig_delay(),
    .fastreadout_trig_window(),
    .fastreadout_clockspeed(),
    .fastreadout_deser_delay(),
    .fastreadout_tsdiv(),
    .fastreadout_ts2div(),
    .fastreadout_tsphase(),

    .fastreadout_trigger_numsignals(),
    .fastreadout_trigger_length(),
    .fastreadout_trigger_distance(),
    .fastreadout_trigger_initdelay(),
    .fastreadout_trigger_synced(),

    .VoltageBoard_Clock(vb_clock),  // vbclk_wire
    .VoltageBoard_Data(vb_data),    // vboard_sin
    .VoltageBoard_Load(vb_load),    // vbld_wire


    .ethernet_mac(),
    .ethernet_ip(),
    .ethernet_subnetmask(),
    .ethernet_gateway(),
    .ethernet_port(),
    .ethernet_config(),
    .ethernet_delay(),
    .ethernet_timeout(),

    .trigger_edge(),
    .trigger_FTDI_trigger(),
    .trigger_source(),
    .trigger_noise_suppression(),
    .trigger_delay(),
    .trigger_duration(),
    .triggerid_reset(),

    .spi_config_reset(spi_config_reset),
    .spi_clock_divider(spi_clock_divider),
    .spi_write_fifo_dout(spi_write_fifo_dout),
    .spi_write_fifo_rd_clk(spi_write_fifo_rd_clk),
    .spi_write_fifo_rd_en(spi_write_fifo_rd_en),
    .spi_write_fifo_empty(spi_write_fifo_empty),

    .spi_read_fifo_din(spi_read_fifo_din),
    .spi_read_fifo_wr_clk(spi_read_fifo_wr_clk),
    .spi_read_fifo_wr_en(spi_read_fifo_wr_en),
    .spi_read_fifo_full(spi_read_fifo_full),
    .spi_config_readback_en(spi_config_readback_en),

    .sr_readback_config_reset(sr_readback_config_reset),
    .sr_readback_fifo_din(sr_readback_fifo_din),
    .sr_readback_fifo_wr_clk(sr_readback_fifo_wr_clk),
    .sr_readback_fifo_wr_en(sr_readback_fifo_wr_en),
    .sr_readback_fifo_full(sr_readback_fifo_full),

    .cmd_enable(),
    .cmd_data(),
    .cmd_rd_clk(),
    .cmd_rd_en(),
    .cmd_fifo_empty(),
    .cmd_fifo_6entries(),
    .cmd_reset(),

    .ordersorter_data(ordersorter_data[7:0])
);

//wire fast_clk_sampleclk;
//reg fast_clk_sampleclk = 0;
//wire timestamp_clk;
wire clockwiz_locked;

clk_wiz_0 I_clk_wiz_0(
    .clk_in1(clk),
    .reset(1'b0),
    .locked(clockwiz_locked),
    .clk_out_sampleclk(fast_clk),
    .clk_out_timestamp(timestamp_int_clk)
);

reg timestamp_clk_div2;

assign timestamp_clk = timestamp_clk_div2;

always @(posedge timestamp_int_clk, negedge cpu_resetn) begin
    if(!cpu_resetn)
        timestamp_clk_div2 <= 0;
    else
        timestamp_clk_div2 <= ~timestamp_clk_div2;
end


// Pattern Generator
wire gecco_inj_chopper;
wire inj_chopper_pat;
assign gecco_inj_chopper = injection_gecco & inj_chopper_pat;
assign chip_inj_chopper = injection_chip & inj_chopper_pat;


sync_async_patgen patgen(
    .clk(clk),
    .rst(patgen_Reset),
    .suspend(patgen_Suspend),
    .write(patgen_writeStrobe),
    .addr(patgen_address[3:0]),
    .din(patgen_data),
    .synced(patgen_synced),
    .syncrst(injtrigger),
    .out(inj_chopper_pat),
    .running(),
    .done()
);

assign spi_right_miso0 = 1'b0;
assign spi_right_miso1 = 1'b0;

spi_readout2 spi_readout_i(
    .clock(clk),
    .reset(~cpu_resetn | spi_config_reset),
    .clock_divider(spi_clock_divider),

    .spi_csb(spi_left_csn),
    .spi_clock(spi_left_clk),
    .spi_mosi(spi_left_mosi),
    .spi_miso0(spi_left_miso0),
    .spi_miso1(spi_left_miso1),

    .readback_en(spi_config_readback_en),
    .data_in_fifo_data(spi_write_fifo_dout),
    .data_in_fifo_clock(spi_write_fifo_rd_clk),
    .data_in_fifo_rd_en(spi_write_fifo_rd_en),
    .data_in_fifo_empty(spi_write_fifo_empty),

    .data_out_fifo_data(spi_read_fifo_din),
    .data_out_fifo_clock(spi_read_fifo_wr_clk),
    .data_out_fifo_wr_en(spi_read_fifo_wr_en),
    .data_out_fifo_full(spi_read_fifo_full),

    .trigger(spi_trigger)
);

sr_readback u_sr_readback (
    .clock                  (clk),
    .reset                  (!cpu_resetn || sr_readback_config_reset),
    .sr_ck1                 (config_ck1),
    .sr_ck2                 (config_ck2),
    .sr_ld                  (config_ld),
    .sr_sout                (config_sout),
    .data_out_fifo_data     (sr_readback_fifo_din),
    .data_out_fifo_full     (sr_readback_fifo_full),
    .data_out_fifo_clock    (sr_readback_fifo_wr_clk),
    .data_out_fifo_wr_en    (sr_readback_fifo_wr_en)
);

// Buffers:
wire [4:0] obuf_p;
wire [4:0] obuf_n;
wire [4:0] obuf_i;
assign obuf_i = {gecco_inj_chopper, ~vb_clock, vb_data, ~vb_load, fast_clk_sampleclk};
            //vb_clock and vb_load are connected inverted to the receivers on GECCO board
assign obuf_p = {gecco_inj_chopper_p, vb_clock_p, vb_data_p, vb_load_p, sample_clk_p};
assign obuf_n = {gecco_inj_chopper_n, vb_clock_n, vb_data_n, vb_load_n, sample_clk_n};

genvar i;
generate
    for (i = 0; i < 5; i = i + 1) begin
        OBUFDS #(
            .IOSTANDARD("LVDS_25")
        ) OBUFDS_I (
            .I(obuf_i[i]),
            .O(obuf_p[i]),
            .OB(obuf_n[i])
        );
    end
endgenerate

`ifdef se_clock_singleended
    OBUF #(
        .IOSTANDARD("LVCMOS25")
    ) OBUF_clock_se (
        .I(sample_clk_se),
        .O(sample_clk_se_p)
    );

`else
    OBUFDS #(
            .IOSTANDARD("LVDS_25")
        ) OBUFDS_clock_diff (
            .I(sample_clk_se),
            .O(sample_clk_se_p),
            .OB(sample_clk_se_n)
        );
`endif

//Clock


wire [3:0] obuf2_p;
wire [3:0] obuf2_n;
wire [3:0] obuf2_i;
assign obuf2_i = {config_sin, config_ck1, config_ck2, config_ld};
assign obuf2_p = {config_sin_p, config_ck1_p, config_ck2_p, config_ld_p};
assign obuf2_n = {config_sin_n, config_ck1_n, config_ck2_n, config_ld_n};

//Asicconfig Buffers
`ifdef config_singleended
    generate
        for (i = 0; i < 4; i = i + 1) begin
            OBUF #(
                .IOSTANDARD("LVCMOS25")
            ) OBUF_config_se (
                .I(obuf2_i[i]),
                .O(obuf2_p[i])
            );
        end
    endgenerate
`else
    generate
        for (i = 0; i < 4; i = i + 1) begin
            OBUFDS #(
                .IOSTANDARD("LVDS_25")
            ) OBUFDS_config_diff (
                .I(obuf2_i[i]),
                .O(obuf2_p[i]),
                .OB(obuf2_n[i])
            );
        end
    endgenerate
`endif


//DEBUG OUTPUTS
//SPI PMOD JB Debug output
assign debug_spi_csn = spi_right_csn;
assign debug_spi_sck = spi_left_clk;
assign debug_spi_mosi = spi_left_mosi;
assign debug_spi_miso0 = spi_left_miso0;
assign debug_spi_miso1 = spi_left_miso1;

//assign config_res_n_test = config_res_n ^ 1;

//Chip VB JB Debug output
assign vb_clock_test = vb_clock;
assign vb_data_test = vb_data;
assign vb_load_test = vb_load;

//DEBUG: res_n low if Center-Button is pressed
always@(posedge clk or posedge btnc) begin
    if(btnc) begin
        res_n <= 0;
    end else begin
        res_n <= 1 ^ config_res_n;
    end
end

//LED contents:
assign led[0] = res_n; //Reset_n
assign led[1] = spi_config_reset; //SPI Reset
assign led[2] = spi_write_fifo_empty; //SPI WR FIFO EMPTY
assign led[3] = spi_read_fifo_full; //SPI READ FIFO EMPTY
assign led[4] = spi_config_readback_en; //SPI READ-ONLY MODE;
assign led[5] = interrupt;
assign led[6] = hold;
//assign led[7] = fastreadout_control_enable;

endmodule
