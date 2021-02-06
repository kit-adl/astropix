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
//////////////////////////////////////////////////////////////////////////////////


module main_top(
    input        cpu_resetn,
    input        sysclk,
    input [7:0]  sw,
    input        btnc,
    input        btnr,
    input        btnl,
    input        btnd,
    //input btnu,
    output [7:0] led,
    
    //voltage adjustment
    output [1:0] set_vadj,
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
    
    //Ethernet: 1000BASE-T RGMII
    input        phy_rx_clk,
    input  [3:0] phy_rxd,
    input        phy_rx_ctl,
    output       phy_tx_clk,
    output [3:0] phy_txd,
    output       phy_tx_ctl,
    output       phy_reset_n,
    input        phy_int_n,
    input        phy_pme_n,
    
    //config: LA30-32 single-ended LA33 diff
    output       config_sin_p, // In1 -> ConfigCard -> C1
    //output       config_sin_n, // In1 -> ConfigCard -> C1
    output       config_ck1_p, // In2 -> ConfigCard -> C2
    //output       config_ck1_n, // In2 -> ConfigCard -> C2
    output       config_ck2_p, // In3 -> ConfigCard -> C3
    //output       config_ck2_n, // In3 -> ConfigCard -> C3
    output       config_ld_p, // In4 -> ConfigCard -> C4
    //output       config_ld_n, // In4 -> ConfigCard -> C4
    
    output       out_ld_dac,
    output       out_ld_config,
    output       out_ld_vdac,
    output       out_ld_tdac,
    output       out_ld_row,
    output       out_ld_column,
    output       out_wr_ram0,
    output       out_wr_ram1,
    output       out_wr_ram2,
    output       out_wr_ram3,

    //SPI:
    output       config_spi_csb,
    output       config_spi_sck,
    output       config_spi_mosi,
    output       config_spi_miso,
    
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
    output       vb_load_n,

    //Trigger Inputs:
    //input        trigger_ext_p,
    //input        trigger_ext_n,
    //input        trigger_ext_ttl,
    //busy output for e.g. TestBeam:
    //output       busy_flag,
    
    //configuration signals:
    //   power-on reset:
    input        por_test_reset,
    
    output       reset_analog_b,
    //input        regulator_reset_out,
    
    //output       reset_digital_b,
    //input        por,
    
    //chip configuration configuration:
    //output       use_spi,
    output       bypass_cmd,
    output       encdr,
    output       en_pll,
    output       sel_ext,
    //output       cmd_clock_invert,
    output       interface_speed,
    output       take_fast,

    // readout configuration:
    input        hitbus,
    output       always_enable_b,

    //output       untriggered_ro_en,
    output       trigro_reset_n,
    
    //digital readout / command decoder:
    //output       cmd_p,
    //output       cmd_n,    
    
    //output       ckref_p,
    //output       ckref_n,
    //output       ck_ext_p,
    //output       ck_ext_n,
    //output       sync_rst_p,
    //output       sync_rst_n,
    //output       ext_trigger_chip_p,
    //output       ext_trigger_chip_n,
    input        data_p,
    input        data_n,
    //input        data_fmc_p,
    //input        data_fmc_n,
    
    input        ckref_chipout_p,
    input        ckref_chipout_n,
	
    //Astropix test
    input interrupt,
    input spi_miso,
    
    output spi_clk,
    output spi_mosi,
    output spi_csn,
    output reg res_n,
    
    //Chip Config debug output
    output config_ck1_test,
    output config_ck2_test,
    output config_sin_test,
    output config_ld_test,
    output config_res_n_test,
    
    //VB COnfig debug output
    output vb_clock_test,
    output vb_data_test,
    output vb_load_test,

    
    //Astropix Sample Clk
    output sample_clk_n,
    output sample_clk_p
    
    // Loopback data out test
   // output      loopback_data_p,
   // output       loopback_dadta_n
    
    /*
    //memory signals for DDR3 usage:
    output   [14:0] ddr3_addr,
    output   [2:0] ddr3_ba,
    output   ddr3_ras_n,
    output   ddr3_cas_n,
    output   ddr3_reset_n,
    output   ddr3_we_n,
    output   ddr3_ck_p,
    output   ddr3_ck_n,
    output   ddr3_cke,
    output   [1:0] ddr3_dm,
    output   ddr3_odt,
    inout    [15:0] ddr3_dq,
    inout    [1:0] ddr3_dqs_p,
    inout    [1:0] ddr3_dqs_n
    */

// from here down: to check
    //output FMC_MGT_M2C_N,
//output FMC_MGT_M2C_P,
//output MI_out_n, 
//output MI_out_p,
//input MI_input_n,
//input MI_input_p,
//input FMC_MGT_C2M_P,
//input FMC_MGT_C2M_N,  
);
    

wire clk;
IBUFG clk_ibufg_inst(
    .I(sysclk),
    .O(clk)
);

//voltage selection on FMC:
assign vadj_en      = 1;
assign set_vadj[1]  = 0;
assign set_vadj[0]  = 1;
    
assign prog_siwun = 1;   //important for reading from FPGA

//Astropix test
//assign res_n      = 1;
assign spi_clk    = 1;
assign spi_mosi    = 1;
assign spi_csn    = 1;

// Ethernet configuration:    
wire [47:0] ethernet_mac;
wire [31:0] ethernet_ip;
wire [31:0] ethernet_subnetmask;
wire [31:0] ethernet_gateway;
wire [15:0] ethernet_port;
wire [7:0]  ethernet_config;
wire [15:0] ethernet_delay;
wire [23:0] ethernet_timeout;
// Ethernet monitor ...
wire        ethernet_module_ready;

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

// Fast Readout:
wire [7:0]  fastreadout_data;
//wire        fastreadout_rd_en;
wire        fastreadout_control_enable;
wire        fastreadout_control_fifoclear;
wire        fastreadout_control_rst;
wire        fastreadout_control_trigger;
wire        fastreadout_control_debug;
wire        fastreadout_control_datamux;
wire        fastreadout_control_realign;
wire        fastreadout_status_empty;
wire        fastreadout_status_full;
wire        fastreadout_status_Datasetstart;
//wire        fastreadout_status_Datasetstart_hw;
//wire        fastreadout_status_full_hw;
//wire        fastreadout_status_empty_hw;
wire [63:0] fastreadout_fifo_din;
wire        fastreadout_fifo_wr_en;
wire        fastreadout_fifo_wr_clk;
wire        fastreadout_fifo_full;
wire        fastreadout_fifo_empty;
wire        fastreadout_progfull;
wire [7:0]  fastreadout_trig_delay;
wire [7:0]  fastreadout_trig_window;
wire [7:0]  fastreadout_clockspeed;
wire [7:0]  fastreadout_deser_delay;
wire [7:0]  fastreadout_tsdiv;
wire [7:0]  fastreadout_ts2div;
wire [7:0]  fastreadout_tsphase;

wire [15:0] fastreadout_trigger_count;
wire [7:0]  fastreadout_trigger_length;
wire [15:0] fastreadout_trigger_distance;
wire [15:0] fastreadout_trigger_initdelay;
wire        fastreadout_trigger_synced;



// Trigger Management:
wire        trigger_edge;
wire        trigger_FTDI_trigger;
wire [2:0]  trigger_source;
wire [2:0]  trigger_noise_suppression;
wire [23:0] trigger_delay;
wire [23:0] trigger_duration;
wire        trigger_id_reset;

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

wire config_ld_dac;   
wire config_ld_config;
wire config_ld_vdac;
wire config_ld_tdac;
wire config_ld_row;
wire config_ld_column;
wire config_wr_ram0;
wire config_wr_ram1;
wire config_wr_ram2;
wire config_wr_ram3;
wire config_no_sr;

//automatic or manual reset redirection:
wire reset_analog_b_rf;
wire reset_analog_b_buf;
wire reset_digital_b_rf;
wire reset_digital_b_buf;
wire reset_trigro_reset_n;
wire reset_trigro_reset_n_buf;
wire autoreset_digital;
wire autoreset_analog;
wire autoreset_combine;

//sample_clk
wire sample_clk;

assign reset_analog_b_buf = reset_analog_b_rf && (~autoreset_analog || autoreset_combine || regulator_reset_out) 
                                            && (~autoreset_combine || ~autoreset_analog || por);
OBUFT #(
    .IOSTANDARD("LVCMOS25")
) OBUFT_I (
    .I(reset_analog_b_buf),
    .T(sw[7]),
    .O(reset_analog_b)
);

assign reset_digital_b_buf = reset_digital_b_rf && (~autoreset_digital || por);
OBUFT #(
    .IOSTANDARD("LVCMOS25")
) OBUFT_I2 (
    .I(reset_digital_b_buf),
    .T(sw[7]),
    .O(reset_digital_b)
);

assign reset_trigro_reset_n_buf = reset_trigro_reset_n && (~autoreset_combine || por);
OBUFT #(
    .IOSTANDARD("LVCMOS25")
) OBUFT_I3 (
    .I(reset_trigro_reset_n_buf),
    .T(sw[7]),
    .O(trigro_reset_n)
);


// SPI:
wire        spi_config_reset;
wire [7:0]  spi_clock_divider;
wire [7:0]  spi_write_fifo_dout;
wire        spi_write_fifo_rd_clk;
wire        spi_write_fifo_rd_en;
wire        spi_write_fifo_empty;

wire [63:0] spi_read_fifo_din;
wire        spi_read_fifo_wr_clk;
wire        spi_read_fifo_wr_en;
wire        spi_read_fifo_full;
wire        spi_config_readback_en;

wire        cmd_enable;   
wire [7:0]  cmd_data;
wire        cmd_rd_clk;
wire        cmd_rd_en;
wire        cmd_fifo_empty;
wire        cmd_fifo_6entries;
wire        cmd_reset;

//wire [8:0]  delayed_clock;

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

    .ChipConfig_LdDAC(config_ld_dac),
    .ChipConfig_LdConfig(config_ld_config),
    .ChipConfig_LdVDAC(config_ld_vdac),
    .ChipConfig_LdTDAC(config_ld_tdac),
    .ChipConfig_LdRow(config_ld_row),
    .ChipConfig_LdColumn(config_ld_column),
    .ChipConfig_WrRAM({config_wr_ram3,config_wr_ram2, config_wr_ram1,config_wr_ram0}),
    .ChipConfig_no_sr(config_no_sr),

    .reset_autoreset_analog(autoreset_analog),      
    .reset_regulator_reset_out(regulator_reset_out),
    .reset_reset_analog_b(reset_analog_b_rf),
    .reset_autoreset_digital(autoreset_digital),
    .reset_por(por),
    .reset_reset_digital_b(reset_digital_b_rf),
    .reset_por_test_reset(por_test_reset),
    .reset_autoreset_combine(autoreset_combine),
                                 
    .config_mode_use_spi(use_spi),
    .config_mode_bypass_cmd(bypass_cmd),
    .config_mode_encdr(encdr),
    .config_mode_en_pll(en_pll),
    .config_mode_cmd_clock_invert(cmd_clock_invert),
    .config_mode_interface_speed(interface_speed),
    .config_mode_take_fast(take_fast),
    
    .work_mode_sel_ext(sel_ext),
    .work_mode_always_enable_b(always_enable_b),
    .work_mode_untriggered_ro_en(untriggered_ro_en),
    .work_mode_trig_ro_reset_n(reset_trigro_reset_n),

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
    
    .fastreadout_data(fastreadout_data[7:0]),
    //.fastreadout_data_hw_write(fastreadout_rd_en),
    .fastreadout_control_enable(fastreadout_control_enable),
    .fastreadout_control_fifoclear(fastreadout_control_fifoclear),
    .fastreadout_control_rst(fastreadout_control_rst),
    .fastreadout_control_trigger(fastreadout_control_trigger),
    .fastreadout_control_debug(fastreadout_control_debug),
    .fastreadout_control_datamux(fastreadout_control_datamux),
    .fastreadout_control_realign(fastreadout_control_realign),
    .fastreadout_status_empty(fastreadout_status_empty),
    .fastreadout_status_full(fastreadout_status_full),
    .fastreadout_status_Datasetstart(fastreadout_status_Datasetstart),
    .fastreadout_status_hw_write(1),
    //.fastreadout_status_Datasetstart_hw(fastreadout_status_Datasetstart_hw),
    //.fastreadout_status_full_hw(fastreadout_status_full_hw),
    //.fastreadout_status_empty_hw(fastreadout_status_empty_hw),
    .fastreadout_fifo_reset(fastreadout_control_fifoclear),
    .fastreadout_fifo_din(fastreadout_fifo_din[63:0]),
    .fastreadout_fifo_wr_en(fastreadout_fifo_wr_en),
    .fastreadout_fifo_wr_clk(fastreadout_fifo_wr_clk),
    .fastreadout_fifo_dout(),   // debug output
    .fastreadout_fifo_full(fastreadout_fifo_full),
    .fastreadout_fifo_empty(fastreadout_fifo_empty),
    .fastreadout_fifo_progfull(fastreadout_progfull),
    .fastreadout_trig_delay(fastreadout_trig_delay),
    .fastreadout_trig_window(fastreadout_trig_window),
    .fastreadout_clockspeed(fastreadout_clockspeed),
    .fastreadout_deser_delay(fastreadout_deser_delay),
    .fastreadout_tsdiv(fastreadout_tsdiv),
    .fastreadout_ts2div(fastreadout_ts2div),
    .fastreadout_tsphase(fastreadout_tsphase),
    
    .fastreadout_trigger_numsignals(fastreadout_trigger_count),
    .fastreadout_trigger_length(fastreadout_trigger_length),
    .fastreadout_trigger_distance(fastreadout_trigger_distance),
    .fastreadout_trigger_initdelay(fastreadout_trigger_initdelay),
    .fastreadout_trigger_synced(fastreadout_trigger_synced),
    
    .VoltageBoard_Clock(vb_clock),  // vbclk_wire
    .VoltageBoard_Data(vb_data),    // vboard_sin
    .VoltageBoard_Load(vb_load),    // vbld_wire    
    
    .ethernet_mac(ethernet_mac),
    .ethernet_ip(ethernet_ip),
    .ethernet_subnetmask(ethernet_subnetmask),
    .ethernet_gateway(ethernet_gateway),
    .ethernet_port(ethernet_port),
    .ethernet_config(ethernet_config),
    .ethernet_delay(ethernet_delay),
    .ethernet_timeout(ethernet_timeout),
    
    .trigger_edge(trigger_edge),
    .trigger_FTDI_trigger(trigger_FTDI_trigger),
    .trigger_source(trigger_source),
    .trigger_noise_suppression(trigger_noise_suppression),
    .trigger_delay(trigger_delay),
    .trigger_duration(trigger_duration),
    .triggerid_reset(trigger_id_reset),
    
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
    
    .cmd_enable(cmd_enable),
    .cmd_data(cmd_data),
    .cmd_rd_clk(cmd_rd_clk),
    .cmd_rd_en(cmd_rd_en),
    .cmd_fifo_empty(cmd_fifo_empty),
    .cmd_fifo_6entries(cmd_fifo_6entries),
    .cmd_reset(cmd_reset),
    
    .ordersorter_data(ordersorter_data[7:0])
);

// Ethernet Communication (by now only readout)

/*

ethernet_top networking(
    .clk100mhz(clk),
    .reset(ethernet_config[0] || ~cpu_resetn),
    .reset_data_sender(ethernet_config[1]),
    .reset_send_fifo(ethernet_config[2]),
    .clk125mhz(),
    
    .phy_rx_clk(phy_rx_clk),
    .phy_rxd(phy_rxd),
    .phy_rx_ctl(phy_rx_ctl),
    .phy_tx_clk(phy_tx_clk),
    .phy_txd(phy_txd),
    .phy_tx_ctl(phy_tx_ctl),
    .phy_reset_n(phy_reset_n),
    .phy_int_n(phy_int_n),
    .phy_pme_n(phy_pme_n),
    
    .local_mac(ethernet_mac),           //48'h02_00_00_00_00_00),
    .local_ip(ethernet_ip),             //{8'd192, 8'd168, 8'd1, 8'd128}),
    .subnet_mask(ethernet_subnetmask),  //{8'd255, 8'd255, 8'd255, 8'd0}),
    .gateway_ip(ethernet_gateway),      //{8'd192, 8'd168, 8'd1, 8'd1}),
    .port(ethernet_port),               //16'd1234),
    
    .send_delay(ethernet_delay),
    .timeout(ethernet_timeout),
    
    .module_ready(ethernet_module_ready),
    .debug_mode_n(ethernet_config[3]),
    .debug_send_package(btnc),
    .debug_data_to_send(sw),
    
    .send_fifo_data_in(fastreadout_fifo_din),
    .send_fifo_wr_en(fastreadout_fifo_wr_en),
    .send_fifo_wr_clk(fastreadout_fifo_wr_clk),
    .send_fifo_full(),

    //receiving not implemented yet
    .receive_fifo_data_out(),
    .receive_fifo_rd_en(),
    .receive_fifo_rd_clk(),
    .receive_fifo_empty()
);


*/
    
// Fast Readout Clock Generation:
wire fast_clk_1200; //clock for fast readout
wire fast_clk_600; //
wire fast_clk_600p90; //
wire fast_clk_150; //
wire fast_clk_200; //




wire clockwiz_locked;
clk_wiz_0 I_clk_wiz_0(
    .clk_in1(clk),
    .reset(1'b0), 
    .locked(clockwiz_locked),    
    //.clk_out1200(fast_clk_1200),    
    .clk_out600(fast_clk_600),
    .clk_out600p90(fast_clk_600p90),
    .clk_out150(fast_clk_150),
    .clk_out200(fast_clk_200)


    //.clk_out400(fast_clk_400),
    //.clk_out640p90(fast_clk_640p90)
);


// Pattern Generator
wire fastreadout_ts_overflow;
wire gecco_inj_chopper;
wire inj_chopper_pat;
assign gecco_inj_chopper = injection_gecco & inj_chopper_pat;
assign chip_inj_chopper = injection_chip & inj_chopper_pat;





//todo
wire patgen_sync_rst_in;




sync_async_patgen patgen(
    .clk(fast_clk_600), //fast_clk_400),  //fast_clk_640),
    .rst(patgen_Reset),
    .suspend(patgen_Suspend),
    .write(patgen_writeStrobe),
    .addr(patgen_address[3:0]),
    .din(patgen_data),
    .synced(patgen_synced),
    .syncrst(injtrigger),//patgen_sync_rst_in
    .out(inj_chopper_pat),
    .running(),
    .done()
);


// Triggering:
/*
wire trigger_ext;

IBUFDS #(
    .DIFF_TERM("FALSE"),
    .IBUF_DELAY_VALUE("0"),  
    .IOSTANDARD("LVDS")
) IBUFDS_Trigger (    
    .O(trigger_ext),    
    .I(trigger_ext_p),   
    .IB(trigger_ext_n)
);
*/






//TODO trigger


//FastReadout Stuff
wire debug_alignment_found;
wire debug_empty_data;
wire debug_data_start;
wire debug_ckref_from_chip;
//wire debug_deser_bit_clock;

wire debug_recdata;
wire block_update;
wire phasemsb;


wire TS_clk;

/*fastreadout fastreadout_I(

    .fast_clk_1200(fast_clk_1200),   
    .fast_clk_600(fast_clk_600),
    .fast_clk_600p90(fast_clk_600p90),
    .fast_clk_150(fast_clk_150),
    .fast_clk_200(fast_clk_200),
    .clk(clk),
    
    .TS_clk(TS_clk),
    
    
    .reset(fastreadout_control_rst || ~cpu_resetn),
    .enable(fastreadout_control_enable),
    .debug_output(fastreadout_control_debug),
    .data_mux(fastreadout_control_datamux),
    //.keepback_bod(sw[4]),
        
    .shift_chip_clock(fastreadout_deser_delay[0]),
    .shift_data_rec(fastreadout_deser_delay[1]),
    .fine_shift_data_rec(fastreadout_deser_delay[2]),
    //.phaseshift_chip_clock({1'b0,fastreadout_deser_delay[0]}),
    //.phaseshift_block_clock(fastreadout_deser_delay[7:1]),
    .disable_hitword1(fastreadout_deser_delay[3]),
    .disable_hitword2(fastreadout_deser_delay[4]),
    .data_switch(fastreadout_deser_delay[5]),
    
    .use_CkRef(~sel_ext),
    .trigger_chip(fastreadout_control_trigger),
    
    .tsdiv(fastreadout_tsdiv),
    .ts2div(fastreadout_ts2div),
    .tsphase(fastreadout_tsphase),
    
    //.trigger_ref(trigger_mux),
    .trigger_ts(trigger_ts),
    .trigger_id(trigger_id),
    
    .FIFO_data_out(fastreadout_fifo_din[63:0]),
    .FIFO_wr_clock(fastreadout_fifo_wr_clk),
    .FIFO_wr_enable(fastreadout_fifo_wr_en),
    
    .ckref_chipout_p(ckref_chipout_p),
    .ckref_chipout_n(ckref_chipout_n),
    .ckref_p(ckref_p),
    .ckref_n(ckref_n),
    .ckext_p(ck_ext_p),
    .ckext_n(ck_ext_n),
    .sync_rst_p(sync_rst_p),
    .sync_rst_n(sync_rst_n),
    .sync_rst(patgen_sync_rst_in),
    .ext_trigger_p(ext_trigger_chip_p),
    .ext_trigger_n(ext_trigger_chip_n),
    .data_p(data_p),
    .data_n(data_n),
    .data2_p(data_fmc_p),
    .data2_n(data_fmc_n),
    
    .alignment_found(debug_alignment_found),
    .debug_empty_data(debug_empty_data),
    .debug_data_start(debug_data_start),
    .debug_ckref_from_chip(debug_ckref_from_chip),
    .ts_overflow(fastreadout_ts_overflow),
    
    //.bit_clock(debug_deser_bit_clock),
    
    //.delayed_clock(delayed_clock),
    
    //.invertreceiverclock(sw[1]),
    
    .trigger_in(trigger_mux),
    .trigger_count(fastreadout_trigger_count),
    .trigger_length(fastreadout_trigger_length),
    .trigger_distance(fastreadout_trigger_distance),
    .trigger_initialdelay(fastreadout_trigger_initdelay),
    .trigger_synced(fastreadout_trigger_synced),
    .realign(fastreadout_control_realign) ,
    .aligndebug(sw[3]),
    .shortsearch(sw[2]),     //DEBUG feature, document or remove -> pick remove and fix this issue
    .block_update(block_update), //DEBUG feature, only for display on LED
    .phase(phasemsb),
    .injection(inj_chopper_pat),//ivan
    .injtrigger(injtrigger)//ivan
    
    
    //,
    //.bitselect(sw[5:3])
);*/

spi_readout spi_readout_i(
    .clock(clk),
    .reset(~cpu_resetn | spi_config_reset),
    .clock_divider(spi_clock_divider),
    
    .spi_csb(config_spi_csb),
    .spi_clock(config_spi_sck),
    .spi_mosi(config_spi_mosi),
    .spi_miso(config_spi_miso),
    
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


wire cmd_aligned;
wire cmd_clock;
cmd_decoder cmddec(
    .reset(~cpu_resetn || cmd_reset),
    //.enable(cmd_enable),
    .enable(1),//ivan
    .clock(fast_clk_150), 
    .TS_clk_out(TS_clk), 
    .alignment_found(cmd_aligned),
    
    .data(cmd_data),
    .rd_clk(cmd_rd_clk),
    .rd_en(cmd_rd_en),
    .fifo_empty(cmd_fifo_empty),
    .fifo_6entries(cmd_fifo_6entries),
    
    .cmd_line(cmd),
    .cmd_clock(cmd_clock)
);

// Loopback Data Generator
//-------------------------------

//-- Data Gen

//-- 6466 Scrambled Stage
/*serdes_6466_66stage lpout_6466_I (
    .reset(!clockwiz_locked),
    .clk(fast_clk_150)

);*/

//-- 6bit DDR Serialiser using 6 bits
//-- This runs on target clock / 3 (6bits divider but DDR so only /3)
//-- for 900 mits -> 300 Mhz
//-- Since it has a rate of 66:6 , the source clock can be 11x slower
//-- That's why we are using a clock domain crossing FIFO to put data logic in a slow clock and the output fast



// Buffers:
wire [4:0] obuf_p;
wire [4:0] obuf_n;
wire [4:0] obuf_i;
assign obuf_i = {/*config_sin, config_ck1, config_ck2, config_ld,*/ gecco_inj_chopper, ~vb_clock, vb_data, ~vb_load, /*cmd,*/ sample_clk};
            //vb_clock and vb_load are connected inverted to the receivers on GECCO board
assign obuf_p = {/*config_sin_p, config_ck1_p, config_ck2_p, config_ld_p, */
                        gecco_inj_chopper_p, vb_clock_p, vb_data_p, vb_load_p, /*cmd_p,*/ sample_clk_p};
assign obuf_n = {/*config_sin_n, config_ck1_n, config_ck2_n, config_ld_n, */
                        gecco_inj_chopper_n, vb_clock_n, vb_data_n, vb_load_n, /*cmd_n,*/ sample_clk_n};

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

//buffer outputs for configuration:
wire [3:0] config1_out = {config_ck1_p, config_ck2_p, config_ld_p, config_sin_p};
wire [3:0] config1_in = {config_ck1, config_ck2, config_ld, config_sin};
generate
    for (i = 0; i <= 3; i = i + 1) begin
        OBUF #(
            .IOSTANDARD("LVCMOS25")
        ) OBUF_I (
            .I(config1_in[i]),
            .O(config1_out[i])
        );
    end
endgenerate

//tristate outputs for configuration:
wire [9:0] config_t = {out_ld_dac, out_ld_config, out_ld_vdac, out_ld_tdac, out_ld_row, out_ld_column, 
                        out_wr_ram0, out_wr_ram1, out_wr_ram2, out_wr_ram3};
wire [9:0] config_out = {config_ld_dac, config_ld_config, config_ld_vdac, config_ld_tdac, config_ld_row, 
                            config_ld_column, config_wr_ram0, config_wr_ram1, config_wr_ram2, config_wr_ram3};
generate
    for (i = 0; i <= 9; i = i + 1) begin
        OBUFT #(
            .IOSTANDARD("LVCMOS25")
        ) OBUFT_I (
            .I(config_out[i]),
            .T(~config_no_sr),
            .O(config_t[i])
        );
    end
endgenerate

//reg [9:0] test;
//always @(posedge clk) begin
//    test <= test + {1'b0,delayed_clock};
//end

//Chip config JB Debug output
assign config_ck1_test = config_ck1;
assign config_ck2_test = config_ck2;
assign config_sin_test = config_sin;
assign config_ld_test = config_ld;
assign config_res_n_test = config_res_n ^ 1;

//Chip VB JB Debug output
assign vb_clock_test = vb_clock;
assign vb_data_test = vb_data;
assign vb_load_test = vb_load;

//DEBUG: res_n low if Center-Button is pressed
always@(posedge clk) begin
    if(btnc) begin
        res_n <= 0;
    end else begin
        res_n <= 1 ^ config_res_n;
    end
end


//LED contents:
assign led[0] = debug_alignment_found;
assign led[1] = block_update; //cmd_aligned;
assign led[2] = fastreadout_fifo_wr_clk; //debug_ckref_from_chip;
assign led[3] = debug_data_start;
assign led[4] = debug_empty_data; //~cmd_fifo_6entries; 
//assign led[5] = trigger_id[0];
assign led[6] = clockwiz_locked;
assign led[7] = fastreadout_control_enable;        
        
endmodule


