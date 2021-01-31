#
# ATLASPix3_SoftAndFirmware
# Copyright (C) 2019  Rudolf Schimassek (rudolf.schimassek@kit.edu)
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

## Clock Signal
#set_property -dict {PACKAGE_PIN R4 IOSTANDARD LVCMOS33} [get_ports sysclk]
#set_property -dict {PACKAGE_PIN R4 IOSTANDARD LVCMOS33} [get_ports {sysclk}]
set_property -dict { PACKAGE_PIN R4    IOSTANDARD LVCMOS33 } [get_ports { sysclk }]; #IO_L13P_T2_MRCC_34 Sch=sysclk
create_clock -period 16.660 -name prog_clko [get_ports prog_clko]
create_clock -period 10.000 -name clk [get_ports sysclk]



#create_generated_clock -name fast_clk_320 -source fast_clk_640 -divide_by 2 fast_clk_320
#create_generated_clock -name fast_clk_320p90 -source fast_clk_640p90 -divide_by 2 fast_clk_320p90
#create_generated_clock -name fast_clk_160 -source fast_clk_640 -divide_by 4 fast_clk_160
#create_generated_clock -name fast_clk_160p90 -source fast_clk_640p90 -divide_by 4 fast_clk_160p90
#create_generated_clock -name cmddec/intclock -source fast_clk_640 -divide_by 16 cmddec/intclock
#create_generated_clock -name clock_0deg -source fast_clk_640 clock_0deg
#create_generated_clock -name clock_90deg -source fast_clk_640p90 clock_90deg
#create_clock -name clk -period 5.000  [get_port { clk }]

#set wr_clock [get_ports wr_clk]
#set rd_clock [get_ports rd_clk]

## FMC Transceiver clocks (Must be set to value provided by Mezzanine card, currently set to 156.25 MHz)
## Note: This clock is attached to a MGTREFCLK pin
#set_property -dict { PACKAGE_PIN E6 } [get_ports { GTP_CLK_N }];
#set_property -dict { PACKAGE_PIN F6 } [get_ports { GTP_CLK_P }];
#create_clock -add -name gtpclk0_pin -period 6.400 -waveform {0 3.200} [get_ports {GTP_CLK_P}];
#set_property -dict { PACKAGE_PIN E10 } [get_ports { FMC_MGT_CLK_N }];
#set_property -dict { PACKAGE_PIN F10 } [get_ports { FMC_MGT_CLK_P }];
#create_clock -add -name mgtclk1_pin -period 6.400 -waveform {0 3.200} [get_ports {FMC_MGT_CLK_P}];

#set_property -dict { PACKAGE_PIN D7 IOSTANDARD LVDS_25} [get_ports { FMC_MGT_C2M_P }]
#set_property -dict { PACKAGE_PIN C7 IOSTANDARD LVDS_25} [get_ports { FMC_MGT_C2M_N }]
#set_property -dict { PACKAGE_PIN C9 IOSTANDARD  DIFF_HSTL_I_12 } [get_ports {data_n}]; #{ FMC_MGT_M2C_N }]
#set_property -dict { PACKAGE_PIN D9 IOSTANDARD  DIFF_HSTL_I_12 } [get_ports {data_p}]; #{ FMC_MGT_M2C_P }]

## LEDs
set_property -dict { PACKAGE_PIN T14   IOSTANDARD LVCMOS25 } [get_ports { led[0] }]; #IO_L15P_T2_DQS_13 Sch=led[0]
#set_property PACKAGE_PIN T14 [get_ports {led[0]}]
#set_property IOSTANDARD LVCMOS25 [get_ports {led[0]}]
set_property -dict { PACKAGE_PIN T15   IOSTANDARD LVCMOS25 } [get_ports { led[1] }]; #IO_L15N_T2_DQS_13 Sch=led[1]
#set_property PACKAGE_PIN T15 [get_ports {led[1]}]
#set_property IOSTANDARD LVCMOS25 [get_ports {led[1]}]
set_property -dict { PACKAGE_PIN T16   IOSTANDARD LVCMOS25 } [get_ports { led[2] }]; #IO_L17P_T2_13 Sch=led[2]
#set_property PACKAGE_PIN T16 [get_ports {led[2]}]
#set_property IOSTANDARD LVCMOS25 [get_ports {led[2]}]
set_property -dict { PACKAGE_PIN U16   IOSTANDARD LVCMOS25 } [get_ports { led[3] }]; #IO_L17N_T2_13 Sch=led[3]
#set_property PACKAGE_PIN U16 [get_ports {led[3]}]
#set_property IOSTANDARD LVCMOS25 [get_ports {led[3]}]
set_property -dict { PACKAGE_PIN V15   IOSTANDARD LVCMOS25 } [get_ports { led[4] }]; #IO_L14N_T2_SRCC_13 Sch=led[4]
#set_property PACKAGE_PIN V15 [get_ports {led[4]}]
#set_property IOSTANDARD LVCMOS25 [get_ports {led[4]}]
set_property -dict { PACKAGE_PIN W16   IOSTANDARD LVCMOS25 } [get_ports { led[5] }]; #IO_L16N_T2_13 Sch=led[5]
#set_property PACKAGE_PIN W16 [get_ports {led[5]}]
#set_property IOSTANDARD LVCMOS25 [get_ports {led[5]}]
set_property -dict { PACKAGE_PIN W15   IOSTANDARD LVCMOS25 } [get_ports { led[6] }]; #IO_L16P_T2_13 Sch=led[6]
#set_property PACKAGE_PIN W15 [get_ports {led[6]}]
#set_property IOSTANDARD LVCMOS25 [get_ports {led[6]}]
set_property -dict { PACKAGE_PIN Y13   IOSTANDARD LVCMOS25 } [get_ports { led[7] }]; #IO_L5P_T0_13 Sch=led[7]
#set_property PACKAGE_PIN Y13 [get_ports {led[7]}]
#set_property IOSTANDARD LVCMOS25 [get_ports {led[7]}]


## Buttons
set_property -dict { PACKAGE_PIN B22 IOSTANDARD LVCMOS25 } [get_ports { btnc }]; #IO_L20N_T3_16 Sch=btnc
set_property -dict { PACKAGE_PIN D22 IOSTANDARD LVCMOS25 } [get_ports { btnd }]; #IO_L22N_T3_16 Sch=btnd
set_property -dict { PACKAGE_PIN C22 IOSTANDARD LVCMOS25 } [get_ports { btnl }]; #IO_L20P_T3_16 Sch=btnl
set_property -dict { PACKAGE_PIN D14 IOSTANDARD LVCMOS25 } [get_ports { btnr }]; #IO_L6P_T0_16 Sch=btnr
#set_property -dict { PACKAGE_PIN F15 IOSTANDARD LVCMOS12 } [get_ports { btnu }]; #IO_0_16 Sch=btnu
#set_property -dict {PACKAGE_PIN G4 IOSTANDARD LVCMOS25} [get_ports cpu_resetn]
#set_property -dict {PACKAGE_PIN G4 IOSTANDARD LVCMOS25} [get_ports {cpu_resetn}]
set_property -dict { PACKAGE_PIN G4  IOSTANDARD LVCMOS25 } [get_ports { cpu_resetn }]; #IO_L12N_T1_MRCC_35 Sch=cpu_resetn


## Switches
set_property -dict { PACKAGE_PIN E22  IOSTANDARD LVCMOS25 } [get_ports { sw[0] }]; #IO_L22P_T3_16 Sch=sw[0]
#set_property PACKAGE_PIN E22 [get_ports {sw[0]}]
#set_property IOSTANDARD LVCMOS25 [get_ports {sw[0]}]
set_property -dict { PACKAGE_PIN F21  IOSTANDARD LVCMOS25 } [get_ports { sw[1] }]; #IO_25_16 Sch=sw[1]
#set_property PACKAGE_PIN F21 [get_ports {sw[1]}]
#set_property IOSTANDARD LVCMOS25 [get_ports {sw[1]}]
set_property -dict { PACKAGE_PIN G21  IOSTANDARD LVCMOS25 } [get_ports { sw[2] }]; #IO_L24P_T3_16 Sch=sw[2]
#set_property PACKAGE_PIN G21 [get_ports {sw[2]}]
#set_property IOSTANDARD LVCMOS25 [get_ports {sw[2]}]
set_property -dict { PACKAGE_PIN G22  IOSTANDARD LVCMOS25 } [get_ports { sw[3] }]; #IO_L24N_T3_16 Sch=sw[3]
#set_property PACKAGE_PIN G22 [get_ports {sw[3]}]
#set_property IOSTANDARD LVCMOS25 [get_ports {sw[3]}]
set_property -dict { PACKAGE_PIN H17  IOSTANDARD LVCMOS25 } [get_ports { sw[4] }]; #IO_L6P_T0_15 Sch=sw[4]
#set_property IOSTANDARD LVCMOS25 [get_ports {sw[4]}]
#set_property PACKAGE_PIN H17 [get_ports {sw[4]}]
set_property -dict { PACKAGE_PIN J16  IOSTANDARD LVCMOS25 } [get_ports { sw[5] }]; #IO_0_15 Sch=sw[5]
#set_property PACKAGE_PIN J16 [get_ports {sw[5]}]
#set_property IOSTANDARD LVCMOS25 [get_ports {sw[5]}]
set_property -dict { PACKAGE_PIN K13  IOSTANDARD LVCMOS25 } [get_ports { sw[6] }]; #IO_L19P_T3_A22_15 Sch=sw[6]
#set_property PACKAGE_PIN K13 [get_ports {sw[6]}]
#set_property IOSTANDARD LVCMOS25 [get_ports {sw[6]}]
set_property -dict { PACKAGE_PIN M17  IOSTANDARD LVCMOS25 } [get_ports { sw[7] }]; #IO_25_15 Sch=sw[7]
#set_property IOSTANDARD LVCMOS25 [get_ports {sw[7]}]
#set_property PACKAGE_PIN M17 [get_ports {sw[7]}]


## OLED Display
#set_property -dict { PACKAGE_PIN W22   IOSTANDARD LVCMOS33 } [get_ports { oled_dc }]; #IO_L7N_T1_D10_14 Sch=oled_dc
#set_property -dict { PACKAGE_PIN U21   IOSTANDARD LVCMOS33 } [get_ports { oled_res }]; #IO_L4N_T0_D05_14 Sch=oled_res
#set_property -dict { PACKAGE_PIN W21   IOSTANDARD LVCMOS33 } [get_ports { oled_sclk }]; #IO_L7P_T1_D09_14 Sch=oled_sclk
#set_property -dict { PACKAGE_PIN Y22   IOSTANDARD LVCMOS33 } [get_ports { oled_sdin }]; #IO_L9N_T1_DQS_D13_14 Sch=oled_sdin
#set_property -dict { PACKAGE_PIN P20   IOSTANDARD LVCMOS33 } [get_ports { oled_vbat }]; #IO_0_14 Sch=oled_vbat
#set_property -dict { PACKAGE_PIN V22   IOSTANDARD LVCMOS33 } [get_ports { oled_vdd }]; #IO_L3N_T0_DQS_EMCCLK_14 Sch=oled_vdd


## HDMI in
#set_property -dict { PACKAGE_PIN AA5   IOSTANDARD LVCMOS33 } [get_ports { hdmi_rx_cec }]; #IO_L10P_T1_34 Sch=hdmi_rx_cec
#set_property -dict { PACKAGE_PIN W4    IOSTANDARD TMDS_33  } [get_ports { hdmi_rx_clk_n }]; #IO_L12N_T1_MRCC_34 Sch=hdmi_rx_clk_n
#set_property -dict { PACKAGE_PIN V4    IOSTANDARD TMDS_33  } [get_ports { hdmi_rx_clk_p }]; #IO_L12P_T1_MRCC_34 Sch=hdmi_rx_clk_p
#set_property -dict { PACKAGE_PIN AB12  IOSTANDARD LVCMOS25 } [get_ports { hdmi_rx_hpa }]; #IO_L7N_T1_13 Sch=hdmi_rx_hpa
#set_property -dict { PACKAGE_PIN Y4    IOSTANDARD LVCMOS33 } [get_ports { hdmi_rx_scl }]; #IO_L11P_T1_SRCC_34 Sch=hdmi_rx_scl
#set_property -dict { PACKAGE_PIN AB5   IOSTANDARD LVCMOS33 } [get_ports { hdmi_rx_sda }]; #IO_L10N_T1_34 Sch=hdmi_rx_sda
#set_property -dict { PACKAGE_PIN R3    IOSTANDARD LVCMOS33 } [get_ports { hdmi_rx_txen }]; #IO_L3P_T0_DQS_34 Sch=hdmi_rx_txen
#set_property -dict { PACKAGE_PIN AA3   IOSTANDARD TMDS_33  } [get_ports { hdmi_rx_n[0] }]; #IO_L9N_T1_DQS_34 Sch=hdmi_rx_n[0]
#set_property -dict { PACKAGE_PIN Y3    IOSTANDARD TMDS_33  } [get_ports { hdmi_rx_p[0] }]; #IO_L9P_T1_DQS_34 Sch=hdmi_rx_p[0]
#set_property -dict { PACKAGE_PIN Y2    IOSTANDARD TMDS_33  } [get_ports { hdmi_rx_n[1] }]; #IO_L4N_T0_34 Sch=hdmi_rx_n[1]
#set_property -dict { PACKAGE_PIN W2    IOSTANDARD TMDS_33  } [get_ports { hdmi_rx_p[1] }]; #IO_L4P_T0_34 Sch=hdmi_rx_p[1]
#set_property -dict { PACKAGE_PIN V2    IOSTANDARD TMDS_33  } [get_ports { hdmi_rx_n[2] }]; #IO_L2N_T0_34 Sch=hdmi_rx_n[2]
#set_property -dict { PACKAGE_PIN U2    IOSTANDARD TMDS_33  } [get_ports { hdmi_rx_p[2] }]; #IO_L2P_T0_34 Sch=hdmi_rx_p[2]


## HDMI out
#set_property -dict { PACKAGE_PIN AA4   IOSTANDARD LVCMOS33 } [get_ports { hdmi_tx_cec }]; #IO_L11N_T1_SRCC_34 Sch=hdmi_tx_cec
#set_property -dict { PACKAGE_PIN U1    IOSTANDARD TMDS_33  } [get_ports { hdmi_tx_clk_n }]; #IO_L1N_T0_34 Sch=hdmi_tx_clk_n
#set_property -dict { PACKAGE_PIN T1    IOSTANDARD TMDS_33  } [get_ports { hdmi_tx_clk_p }]; #IO_L1P_T0_34 Sch=hdmi_tx_clk_p
#set_property -dict { PACKAGE_PIN AB13  IOSTANDARD LVCMOS25 } [get_ports { hdmi_tx_hpd }]; #IO_L3N_T0_DQS_13 Sch=hdmi_tx_hpd
#set_property -dict { PACKAGE_PIN U3    IOSTANDARD LVCMOS33 } [get_ports { hdmi_tx_rscl }]; #IO_L6P_T0_34 Sch=hdmi_tx_rscl
#set_property -dict { PACKAGE_PIN V3    IOSTANDARD LVCMOS33 } [get_ports { hdmi_tx_rsda }]; #IO_L6N_T0_VREF_34 Sch=hdmi_tx_rsda
#set_property -dict { PACKAGE_PIN Y1    IOSTANDARD TMDS_33  } [get_ports { hdmi_tx_n[0] }]; #IO_L5N_T0_34 Sch=hdmi_tx_n[0]
#set_property -dict { PACKAGE_PIN W1    IOSTANDARD TMDS_33  } [get_ports { hdmi_tx_p[0] }]; #IO_L5P_T0_34 Sch=hdmi_tx_p[0]
#set_property -dict { PACKAGE_PIN AB1   IOSTANDARD TMDS_33  } [get_ports { hdmi_tx_n[1] }]; #IO_L7N_T1_34 Sch=hdmi_tx_n[1]
#set_property -dict { PACKAGE_PIN AA1   IOSTANDARD TMDS_33  } [get_ports { hdmi_tx_p[1] }]; #IO_L7P_T1_34 Sch=hdmi_tx_p[1]
#set_property -dict { PACKAGE_PIN AB2   IOSTANDARD TMDS_33  } [get_ports { hdmi_tx_n[2] }]; #IO_L8N_T1_34 Sch=hdmi_tx_n[2]
#set_property -dict { PACKAGE_PIN AB3   IOSTANDARD TMDS_33  } [get_ports { hdmi_tx_p[2] }]; #IO_L8P_T1_34 Sch=hdmi_tx_p[2]


## Display Port
#set_property -dict { PACKAGE_PIN AB10  IOSTANDARD TMDS_33  } [get_ports { dp_tx_aux_n }]; #IO_L8N_T1_13 Sch=dp_tx_aux_n
#set_property -dict { PACKAGE_PIN AA11  IOSTANDARD TMDS_33  } [get_ports { dp_tx_aux_n }]; #IO_L9N_T1_DQS_13 Sch=dp_tx_aux_n
#set_property -dict { PACKAGE_PIN AA9   IOSTANDARD TMDS_33  } [get_ports { dp_tx_aux_p }]; #IO_L8P_T1_13 Sch=dp_tx_aux_p
#set_property -dict { PACKAGE_PIN AA10  IOSTANDARD TMDS_33  } [get_ports { dp_tx_aux_p }]; #IO_L9P_T1_DQS_13 Sch=dp_tx_aux_p
#set_property -dict { PACKAGE_PIN N15   IOSTANDARD LVCMOS33 } [get_ports { dp_tx_hpd }]; #IO_25_14 Sch=dp_tx_hpd


## Audio Codec
#set_property -dict { PACKAGE_PIN T4    IOSTANDARD LVCMOS33 } [get_ports { ac_adc_sdata }]; #IO_L13N_T2_MRCC_34 Sch=ac_adc_sdata
#set_property -dict { PACKAGE_PIN T5    IOSTANDARD LVCMOS33 } [get_ports { ac_bclk }]; #IO_L14P_T2_SRCC_34 Sch=ac_bclk
#set_property -dict { PACKAGE_PIN W6    IOSTANDARD LVCMOS33 } [get_ports { ac_dac_sdata }]; #IO_L15P_T2_DQS_34 Sch=ac_dac_sdata
#set_property -dict { PACKAGE_PIN U5    IOSTANDARD LVCMOS33 } [get_ports { ac_lrclk }]; #IO_L14N_T2_SRCC_34 Sch=ac_lrclk
#set_property -dict { PACKAGE_PIN U6    IOSTANDARD LVCMOS33 } [get_ports { ac_mclk }]; #IO_L16P_T2_34 Sch=ac_mclk


## Pmod header JA
#set_property -dict { PACKAGE_PIN AB22  IOSTANDARD LVDS } [get_ports { ja[0] }]; #IO_L10N_T1_D15_14 Sch=ja[1]
#set_property -dict { PACKAGE_PIN AB21  IOSTANDARD LVDS } [get_ports { ja[1] }]; #IO_L10P_T1_D14_14 Sch=ja[2]
#set_property -dict {PACKAGE_PIN AB21 IOSTANDARD LVDS_25} [get_ports ext_trigger_p]
#set_property -dict {PACKAGE_PIN AB22 IOSTANDARD LVDS_25} [get_ports ext_trigger_n]
#set_property -dict { PACKAGE_PIN AB20  IOSTANDARD LVDS } [get_ports { ja[2] }]; #IO_L15N_T2_DQS_DOUT_CSO_B_14 Sch=ja[3]
#set_property PACKAGE_PIN AB20 [get_ports {ja2}]
#set_property IOSTANDARD LVCMOS33 [get_ports {ja2}]
#set_property -dict { PACKAGE_PIN AB18  IOSTANDARD LVDS } [get_ports { ja[3] }]; #IO_L17N_T2_A13_D29_14 Sch=ja[4]
#set_property PACKAGE_PIN AB18 [get_ports {ja3}]
#set_property IOSTANDARD LVCMOS33 [get_ports {ja3}]

#set_property -dict { PACKAGE_PIN AA21  IOSTANDARD LVDS } [get_ports { ja[5] }]; #IO_L8N_T1_D12_14 Sch=ja[8]
#set_property -dict { PACKAGE_PIN AA20  IOSTANDARD LVDS } [get_ports { ja[6] }]; #IO_L8P_T1_D11_14 Sch=ja[9]
#set_property -dict { PACKAGE_PIN AA18  IOSTANDARD LVDS } [get_ports { ja[7] }]; #IO_L17P_T2_A14_D30_14 Sch=ja[10]


## Pmod header JB
set_property PACKAGE_PIN V9 [get_ports {config_ck1_test}]
set_property IOSTANDARD LVCMOS33 [get_ports {config_ck1_test}]
set_property PACKAGE_PIN V8 [get_ports {config_ck2_test}]
set_property IOSTANDARD LVCMOS33 [get_ports {config_ck2_test}]
set_property PACKAGE_PIN V7 [get_ports {config_sin_test}]
set_property IOSTANDARD LVCMOS33 [get_ports {config_sin_test}]
set_property PACKAGE_PIN W7 [get_ports {config_ld_test}]
set_property IOSTANDARD LVCMOS33 [get_ports {config_ld_test}]
set_property PACKAGE_PIN W9 [get_ports {config_res_n_test}]
set_property IOSTANDARD LVCMOS33 [get_ports {config_res_n_test}]
#set_property PACKAGE_PIN Y9 [get_ports {jb[5]}]
#set_property IOSTANDARD LVCMOS33 [get_ports {jb[5]}]
#set_property PACKAGE_PIN Y8 [get_ports {jb[6]}]
#set_property IOSTANDARD LVCMOS33 [get_ports {jb[6]}]
#set_property PACKAGE_PIN Y7 [get_ports {jb[7]}]
#set_property IOSTANDARD LVCMOS33 [get_ports {jb[7]}]


### Pmod header JC
set_property PACKAGE_PIN Y6 [get_ports {busy_flag}];
set_property IOSTANDARD LVCMOS33 [get_ports {busy_flag}]; 
# busy_flag had a standard of LVCMOS25
set_property PACKAGE_PIN AA6 [get_ports {trigger_ext_ttl}]; 
set_property IOSTANDARD LVCMOS33 [get_ports {trigger_ext_ttl}];
set_property PACKAGE_PIN AA8 [get_ports {trigger_ext_p}]; #{jc[2]}]
set_property IOSTANDARD LVDS_25 [get_ports {trigger_ext_p}]; #{jc[2]}]
set_property PACKAGE_PIN AB8 [get_ports {trigger_ext_n}]; #{jc[3]}]
set_property IOSTANDARD LVDS_25 [get_ports {trigger_ext_n}]; #{jc[3]}]
#set_property PACKAGE_PIN R6 [get_ports {trigger_ttl}]
#set_property IOSTANDARD LVCMOS33 [get_ports {trigger_ttl}]  # LVCMOS25
#jc[4]}]
#set_property PACKAGE_PIN T6 [get_ports {jc[5]}]
#set_property IOSTANDARD LVCMOS33 [get_ports {jc[5]}]
#set_property PACKAGE_PIN AB7 [get_ports {jc[6]}]
#set_property IOSTANDARD LVCMOS33 [get_ports {jc[6]}]
#set_property PACKAGE_PIN AB6 [get_ports {jc[7]}]
#set_property IOSTANDARD LVCMOS33 [get_ports {jc[7]}]


## XADC Header
#set_property -dict { PACKAGE_PIN J14   IOSTANDARD LVCMOS33 } [get_ports { xa_p[0] }]; #IO_L3P_T0_DQS_AD1P_15 Sch=xa_p[1]
#set_property -dict { PACKAGE_PIN H14   IOSTANDARD LVCMOS33 } [get_ports { xa_n[0] }]; #IO_L3N_T0_DQS_AD1N_15 Sch=xa_n[1]
#set_property -dict { PACKAGE_PIN H13   IOSTANDARD LVCMOS33 } [get_ports { xa_p[1] }]; #IO_L1P_T0_AD0P_15 Sch=xa_p[2]
#set_property -dict { PACKAGE_PIN G13   IOSTANDARD LVCMOS33 } [get_ports { xa_n[1] }]; #IO_L1N_T0_AD0N_15 Sch=xa_n[2]
#set_property -dict { PACKAGE_PIN G15   IOSTANDARD LVCMOS33 } [get_ports { xa_p[2] }]; #IO_L2P_T0_AD8P_15 Sch=xa_p[3]
#set_property -dict { PACKAGE_PIN G16   IOSTANDARD LVCMOS33 } [get_ports { xa_n[2] }]; #IO_L2N_T0_AD8N_15 Sch=xa_n[3]
#set_property -dict { PACKAGE_PIN J15   IOSTANDARD LVCMOS33 } [get_ports { xa_p[3] }]; #IO_L5P_T0_AD9P_15 Sch=xa_p[4]
#set_property -dict { PACKAGE_PIN H15   IOSTANDARD LVCMOS33 } [get_ports { xa_n[3] }]; #IO_L5N_T0_AD9N_15 Sch=xa_n[4]


## UART
#set_property -dict { PACKAGE_PIN AA19  IOSTANDARD LVCMOS33 } [get_ports { uart_rx_out }]; #IO_L15P_T2_DQS_RDWR_B_14 Sch=uart_rx_out
#set_property -dict { PACKAGE_PIN V18   IOSTANDARD LVCMOS33 } [get_ports { uart_tx_in }]; #IO_L14P_T2_SRCC_14 Sch=uart_tx_in


## Ethernet
set_property -dict {LOC V13 IOSTANDARD LVCMOS25} [get_ports phy_rx_clk]
set_property -dict {LOC AB16 IOSTANDARD LVCMOS25} [get_ports {phy_rxd[0]}]
set_property -dict {LOC AA15 IOSTANDARD LVCMOS25} [get_ports {phy_rxd[1]}]
set_property -dict {LOC AB15 IOSTANDARD LVCMOS25} [get_ports {phy_rxd[2]}]
set_property -dict {LOC AB11 IOSTANDARD LVCMOS25} [get_ports {phy_rxd[3]}]
set_property -dict {LOC W10 IOSTANDARD LVCMOS25} [get_ports phy_rx_ctl]
set_property -dict {LOC AA14 IOSTANDARD LVCMOS25 SLEW FAST DRIVE 16} [get_ports phy_tx_clk]
set_property -dict {LOC Y12 IOSTANDARD LVCMOS25 SLEW FAST DRIVE 16} [get_ports {phy_txd[0]}]
set_property -dict {LOC W12 IOSTANDARD LVCMOS25 SLEW FAST DRIVE 16} [get_ports {phy_txd[1]}]
set_property -dict {LOC W11 IOSTANDARD LVCMOS25 SLEW FAST DRIVE 16} [get_ports {phy_txd[2]}]
set_property -dict {LOC Y11 IOSTANDARD LVCMOS25 SLEW FAST DRIVE 16} [get_ports {phy_txd[3]}]
set_property -dict {LOC V10 IOSTANDARD LVCMOS25 SLEW FAST DRIVE 16} [get_ports phy_tx_ctl]
set_property -dict {LOC U7 IOSTANDARD LVCMOS33 SLEW SLOW DRIVE 12} [get_ports phy_reset_n]
set_property -dict {LOC Y14 IOSTANDARD LVCMOS25} [get_ports phy_int_n]
set_property -dict {LOC W14 IOSTANDARD LVCMOS25} [get_ports phy_pme_n]
#set_property -dict {LOC Y16  IOSTANDARD LVCMOS25 SLEW SLOW DRIVE 12} [get_ports phy_mdio]
#set_property -dict {LOC AA16 IOSTANDARD LVCMOS25 SLEW SLOW DRIVE 12} [get_ports phy_mdc]

## Fan PWM
#set_property -dict { PACKAGE_PIN U15   IOSTANDARD LVCMOS25 } [get_ports { fan_pwm }]; #IO_L14P_T2_SRCC_13 Sch=fan_pwm


## DPTI/DSPI
set_property -dict {PACKAGE_PIN Y18 IOSTANDARD LVCMOS33} [get_ports prog_clko]
set_property PACKAGE_PIN U20 [get_ports {prog_d[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {prog_d[0]}]
set_property PACKAGE_PIN P14 [get_ports {prog_d[1]}]
set_property IOSTANDARD LVCMOS33 [get_ports {prog_d[1]}]
set_property PACKAGE_PIN P15 [get_ports {prog_d[2]}]
set_property IOSTANDARD LVCMOS33 [get_ports {prog_d[2]}]
set_property PACKAGE_PIN U17 [get_ports {prog_d[3]}]
set_property IOSTANDARD LVCMOS33 [get_ports {prog_d[3]}]
set_property PACKAGE_PIN R17 [get_ports {prog_d[4]}]
set_property IOSTANDARD LVCMOS33 [get_ports {prog_d[4]}]
set_property PACKAGE_PIN P16 [get_ports {prog_d[5]}]
set_property IOSTANDARD LVCMOS33 [get_ports {prog_d[5]}]
set_property PACKAGE_PIN R18 [get_ports {prog_d[6]}]
set_property IOSTANDARD LVCMOS33 [get_ports {prog_d[6]}]
set_property PACKAGE_PIN N14 [get_ports {prog_d[7]}]
set_property IOSTANDARD LVCMOS33 [get_ports {prog_d[7]}]
set_property -dict {PACKAGE_PIN V17 IOSTANDARD LVCMOS33} [get_ports prog_oen]
set_property -dict {PACKAGE_PIN P19 IOSTANDARD LVCMOS33} [get_ports prog_rdn]
set_property -dict {PACKAGE_PIN N17 IOSTANDARD LVCMOS33} [get_ports prog_rxen]
set_property -dict {PACKAGE_PIN P17 IOSTANDARD LVCMOS33} [get_ports prog_siwun]
#set_property -dict { PACKAGE_PIN R14   IOSTANDARD LVCMOS33 } [get_ports { prog_spien }]; #IO_L19N_T3_A09_D25_VREF_14 Sch=prog_spien
set_property -dict {PACKAGE_PIN Y19 IOSTANDARD LVCMOS33} [get_ports prog_txen]
set_property -dict {PACKAGE_PIN R19 IOSTANDARD LVCMOS33} [get_ports prog_wrn]


## HID port
#set_property -dict { PACKAGE_PIN W17   IOSTANDARD LVCMOS33 } [get_ports { ps2_clk }]; #IO_L16N_T2_A15_D31_14 Sch=ps2_clk
#set_property -dict { PACKAGE_PIN N13   IOSTANDARD LVCMOS33 } [get_ports { ps2_data }]; #IO_L23P_T3_A03_D19_14 Sch=ps2_data


## QSPI
#set_property -dict { PACKAGE_PIN T19   IOSTANDARD LVCMOS33 } [get_ports { qspi_cs }]; #IO_L6P_T0_FCS_B_14 Sch=qspi_cs
#set_property -dict { PACKAGE_PIN P22   IOSTANDARD LVCMOS33 } [get_ports { qspi_dq[0] }]; #IO_L1P_T0_D00_MOSI_14 Sch=qspi_dq[0]
#set_property -dict { PACKAGE_PIN R22   IOSTANDARD LVCMOS33 } [get_ports { qspi_dq[1] }]; #IO_L1N_T0_D01_DIN_14 Sch=qspi_dq[1]
#set_property -dict { PACKAGE_PIN P21   IOSTANDARD LVCMOS33 } [get_ports { qspi_dq[2] }]; #IO_L2P_T0_D02_14 Sch=qspi_dq[2]
#set_property -dict { PACKAGE_PIN R21   IOSTANDARD LVCMOS33 } [get_ports { qspi_dq[3] }]; #IO_L2N_T0_D03_14 Sch=qspi_dq[3]


## SD card
#set_property -dict { PACKAGE_PIN W19   IOSTANDARD LVCMOS33 } [get_ports { sd_cclk }]; #IO_L12P_T1_MRCC_14 Sch=sd_cclk
#set_property -dict { PACKAGE_PIN T18   IOSTANDARD LVCMOS33 } [get_ports { sd_cd }]; #IO_L20N_T3_A07_D23_14 Sch=sd_cd
#set_property -dict { PACKAGE_PIN W20   IOSTANDARD LVCMOS33 } [get_ports { sd_cmd }]; #IO_L12N_T1_MRCC_14 Sch=sd_cmd
#set_property -dict { PACKAGE_PIN V19   IOSTANDARD LVCMOS33 } [get_ports { sd_d[0] }]; #IO_L14N_T2_SRCC_14 Sch=sd_d[0]
#set_property -dict { PACKAGE_PIN T21   IOSTANDARD LVCMOS33 } [get_ports { sd_d[1] }]; #IO_L4P_T0_D04_14 Sch=sd_d[1]
#set_property -dict { PACKAGE_PIN T20   IOSTANDARD LVCMOS33 } [get_ports { sd_d[2] }]; #IO_L6N_T0_D08_VREF_14 Sch=sd_d[2]
#set_property -dict { PACKAGE_PIN U18   IOSTANDARD LVCMOS33 } [get_ports { sd_d[3] }]; #IO_L18N_T2_A11_D27_14 Sch=sd_d[3]
#set_property -dict { PACKAGE_PIN V20   IOSTANDARD LVCMOS33 } [get_ports { sd_reset }]; #IO_L11N_T1_SRCC_14 Sch=sd_reset


## I2C
#set_property -dict { PACKAGE_PIN W5    IOSTANDARD LVCMOS33 } [get_ports { scl }]; #IO_L15N_T2_DQS_34 Sch=scl
#set_property -dict { PACKAGE_PIN V5    IOSTANDARD LVCMOS33 } [get_ports { sda }]; #IO_L16N_T2_34 Sch=sda


## Voltage Adjust
set_property -dict { PACKAGE_PIN AA13  IOSTANDARD LVCMOS25 } [get_ports { set_vadj[0] }]; #IO_L3P_T0_DQS_13 Sch=set_vadj[0]
set_property -dict { PACKAGE_PIN AB17  IOSTANDARD LVCMOS25 } [get_ports { set_vadj[1] }]; #IO_L2N_T0_13 Sch=set_vadj[1]
set_property -dict { PACKAGE_PIN V14   IOSTANDARD LVCMOS25 } [get_ports { vadj_en }]; #IO_L13N_T2_MRCC_13 Sch=vadj_en


##DDR3
#set_property -dict {PACKAGE_PIN M2} [get_ports {ddr3_addr[0]}]
#set_property -dict {PACKAGE_PIN M5} [get_ports {ddr3_addr[1]}]
#set_property -dict {PACKAGE_PIN M3} [get_ports {ddr3_addr[2]}]
#set_property -dict {PACKAGE_PIN M1} [get_ports {ddr3_addr[3]}]
#set_property -dict {PACKAGE_PIN L6} [get_ports {ddr3_addr[4]}]
#set_property -dict {PACKAGE_PIN P1} [get_ports {ddr3_addr[5]}]
#set_property -dict {PACKAGE_PIN N3} [get_ports {ddr3_addr[6]}]
#set_property -dict {PACKAGE_PIN N2} [get_ports {ddr3_addr[7]}]
#set_property -dict {PACKAGE_PIN M6} [get_ports {ddr3_addr[8]}]
#set_property -dict {PACKAGE_PIN R1} [get_ports {ddr3_addr[9]}]
#set_property -dict {PACKAGE_PIN L5} [get_ports {ddr3_addr[10]}]
#set_property -dict {PACKAGE_PIN N5} [get_ports {ddr3_addr[11]}]
#set_property -dict {PACKAGE_PIN N4} [get_ports {ddr3_addr[12]}]
#set_property -dict {PACKAGE_PIN P2} [get_ports {ddr3_addr[13]}]
#set_property -dict {PACKAGE_PIN P6} [get_ports {ddr3_addr[14]}]
#set_property -dict {PACKAGE_PIN L3} [get_ports {ddr3_ba[0]}]
#set_property -dict {PACKAGE_PIN K6} [get_ports {ddr3_ba[1]}]
#set_property -dict {PACKAGE_PIN L4} [get_ports {ddr3_ba[2]}]
#set_property -dict {PACKAGE_PIN K3} [get_ports ddr3_cas_n]
#set_property -dict {PACKAGE_PIN J6} [get_ports ddr3_cke]
#set_property -dict {PACKAGE_PIN P4} [get_ports ddr3_ck_n]
#set_property -dict {PACKAGE_PIN P5} [get_ports ddr3_ck_p]
#set_property -dict {PACKAGE_PIN G3} [get_ports {ddr3_dm[0]}]
#set_property -dict {PACKAGE_PIN F1} [get_ports {ddr3_dm[1]}]
#set_property -dict {PACKAGE_PIN G2} [get_ports {ddr3_dq[0]}]
#set_property -dict {PACKAGE_PIN H4} [get_ports {ddr3_dq[1]}]
#set_property -dict {PACKAGE_PIN H5} [get_ports {ddr3_dq[2]}]
#set_property -dict {PACKAGE_PIN J1} [get_ports {ddr3_dq[3]}]
#set_property -dict {PACKAGE_PIN K1} [get_ports {ddr3_dq[4]}]
#set_property -dict {PACKAGE_PIN H3} [get_ports {ddr3_dq[5]}]
#set_property -dict {PACKAGE_PIN H2} [get_ports {ddr3_dq[6]}]
#set_property -dict {PACKAGE_PIN J5} [get_ports {ddr3_dq[7]}]
#set_property -dict {PACKAGE_PIN E3} [get_ports {ddr3_dq[8]}]
#set_property -dict {PACKAGE_PIN B2} [get_ports {ddr3_dq[9]}]
#set_property -dict {PACKAGE_PIN F3} [get_ports {ddr3_dq[10]}]
#set_property -dict {PACKAGE_PIN D2} [get_ports {ddr3_dq[11]}]
#set_property -dict {PACKAGE_PIN C2} [get_ports {ddr3_dq[12]}]
#set_property -dict {PACKAGE_PIN A1} [get_ports {ddr3_dq[13]}]
#set_property -dict {PACKAGE_PIN E2} [get_ports {ddr3_dq[14]}]
#set_property -dict {PACKAGE_PIN B1} [get_ports {ddr3_dq[15]}]
#set_property -dict {PACKAGE_PIN J2} [get_ports {ddr3_dqs_n[0]}]
#set_property -dict {PACKAGE_PIN K2} [get_ports {ddr3_dqs_p[0]}]
#set_property -dict {PACKAGE_PIN D1} [get_ports {ddr3_dqs_n[1]}]
#set_property -dict {PACKAGE_PIN E1} [get_ports {ddr3_dqs_p[1]}]
#set_property -dict {PACKAGE_PIN K4} [get_ports ddr3_odt]
#set_property -dict {PACKAGE_PIN J4} [get_ports ddr3_ras_n]
#set_property -dict { PACKAGE_PIN G1   } [get_ports { ddr3_reset_n }]; #IO_L5P_T0_AD13P_35 Sch=ddr3_reset
#set_property -dict {PACKAGE_PIN L1} [get_ports ddr3_we_n]


## FMC
#set_property -dict { PACKAGE_PIN H19   IOSTANDARD LVCMOS12 } [get_ports { fmc_clk0_m2c_n }]; #IO_L12N_T1_MRCC_15 Sch=fmc_clk0_m2c_n
#set_property -dict { PACKAGE_PIN J19   IOSTANDARD LVCMOS12 } [get_ports { fmc_clk0_m2c_p }]; #IO_L12P_T1_MRCC_15 Sch=fmc_clk0_m2c_p
#set_property -dict { PACKAGE_PIN C19   IOSTANDARD LVCMOS12 } [get_ports { fmc_clk1_m2c_n }]; #IO_L13N_T2_MRCC_16 Sch=fmc_clk1_m2c_n
#set_property -dict { PACKAGE_PIN C18   IOSTANDARD LVCMOS12 } [get_ports { fmc_clk1_m2c_p }]; #IO_L13P_T2_MRCC_16 Sch=fmc_clk1_m2c_p
set_property -dict { PACKAGE_PIN K19   IOSTANDARD LVDS_25 } [get_ports { ckref_chipout_n }]; #IO_L13N_T2_MRCC_15 Sch=fmc_la00_cc_n
set_property -dict { PACKAGE_PIN K18   IOSTANDARD LVDS_25 } [get_ports { ckref_chipout_p }]; #IO_L13P_T2_MRCC_15 Sch=fmc_la00_cc_p
set_property -dict { PACKAGE_PIN J21   IOSTANDARD LVDS_25 } [get_ports { sync_rst_n }]; #IO_L11N_T1_SRCC_15 Sch=fmc_la01_cc_n
set_property -dict { PACKAGE_PIN J20   IOSTANDARD LVDS_25 } [get_ports { sync_rst_p}]; #IO_L11P_T1_SRCC_15 Sch=fmc_la01_cc_p
set_property -dict { PACKAGE_PIN L18   IOSTANDARD LVDS_25 } [get_ports { ckref_n }]; #IO_L16N_T2_A27_15 Sch=fmc_la_n[02]
set_property -dict { PACKAGE_PIN M18   IOSTANDARD LVDS_25 } [get_ports { ckref_p }]; #IO_L16P_T2_A28_15 Sch=fmc_la_p[02]
set_property -dict { PACKAGE_PIN N19   IOSTANDARD LVDS_25 } [get_ports { ext_trigger_chip_n }]; #IO_L17N_T2_A25_15 Sch=fmc_la_n[03]
set_property -dict { PACKAGE_PIN N18   IOSTANDARD LVDS_25 } [get_ports { ext_trigger_chip_p }]; #IO_L17P_T2_A26_15 Sch=fmc_la_p[03]
set_property -dict { PACKAGE_PIN M20   IOSTANDARD LVDS_25 } [get_ports { cmd_n }]; #IO_L18N_T2_A23_15 Sch=fmc_la_n[04]
set_property -dict { PACKAGE_PIN N20   IOSTANDARD LVDS_25 } [get_ports { cmd_p }]; #IO_L18P_T2_A24_15 Sch=fmc_la_p[04]
set_property -dict { PACKAGE_PIN L21   IOSTANDARD LVDS_25 } [get_ports { ck_ext_n }]; #IO_L10N_T1_AD11N_15 Sch=fmc_la_n[05]
set_property -dict { PACKAGE_PIN M21   IOSTANDARD LVDS_25 } [get_ports { ck_ext_p }]; #{ fmc_la_p[05] }]; #IO_L10P_T1_AD11P_15 Sch=fmc_la_p[05]
set_property -dict { PACKAGE_PIN M22   IOSTANDARD LVCMOS25 } [get_ports { por_test_reset }]; #IO_L15N_T2_DQS_ADV_B_15 Sch=fmc_la_n[06]
#set_property -dict { PACKAGE_PIN N22   IOSTANDARD LVDS_25 } [get_ports { MI_input_p }]; #IO_L15P_T2_DQS_15 Sch=fmc_la_p[06]
set_property -dict { PACKAGE_PIN L13   IOSTANDARD LVCMOS25 } [get_ports { always_enable_b }]; #IO_L20N_T3_A19_15 Sch=fmc_la_n[07]
set_property -dict { PACKAGE_PIN M13   IOSTANDARD LVCMOS25 } [get_ports { config_spi_mosi }]; #{ fmc_la_p[07] }]; #IO_L20P_T3_A20_15 Sch=fmc_la_p[07]
set_property -dict { PACKAGE_PIN M16   IOSTANDARD LVCMOS25 } [get_ports { sel_ext  }]; #IO_L24N_T3_RS0_15 Sch=fmc_la_n[08]
set_property -dict { PACKAGE_PIN M15   IOSTANDARD LVCMOS25 } [get_ports { out_wr_ram1 }]; #{ fmc_la_p[08] }]; #IO_L24P_T3_RS1_15 Sch=fmc_la_p[08]
set_property -dict { PACKAGE_PIN G20   IOSTANDARD LVCMOS25 } [get_ports { out_ld_config }]; #IO_L8N_T1_AD10N_15 Sch=fmc_la_n[09]
set_property -dict { PACKAGE_PIN H20   IOSTANDARD LVCMOS25 } [get_ports { out_wr_ram3 }]; #{ fmc_la_p[09] }]; #IO_L8P_T1_AD10P_15 Sch=fmc_la_p[09]
set_property -dict { PACKAGE_PIN K22   IOSTANDARD LVCMOS25 } [get_ports { config_spi_sck }]; #IO_L9N_T1_DQS_AD3N_15 Sch=fmc_la_n[10]
set_property -dict { PACKAGE_PIN K21   IOSTANDARD LVCMOS25 } [get_ports { out_wr_ram0 }]; #IO_L9P_T1_DQS_AD3P_15 Sch=fmc_la_p[10]
set_property -dict { PACKAGE_PIN L15   IOSTANDARD LVCMOS25 } [get_ports { out_ld_tdac }]; #{ fmc_la_n[11] }]; #IO_L22N_T3_A16_15 Sch=fmc_la_n[11]
#set_property -dict { PACKAGE_PIN L14   IOSTANDARD LVCMOS12 } [get_ports { fmc_la_p[11] }]; #IO_L22P_T3_A17_15 Sch=fmc_la_p[11]
set_property -dict { PACKAGE_PIN L20   IOSTANDARD LVCMOS25 } [get_ports { out_ld_dac }]; #IO_L14N_T2_SRCC_15 Sch=fmc_la_n[12]
set_property -dict { PACKAGE_PIN L19   IOSTANDARD LVCMOS25 } [get_ports { out_ld_row }]; #IO_L14P_T2_SRCC_15 Sch=fmc_la_p[12]
set_property -dict { PACKAGE_PIN J17   IOSTANDARD LVCMOS25 } [get_ports { encdr }]; #{ fmc_la_n[13] }]; #IO_L21N_T3_DQS_A18_15 Sch=fmc_la_n[13]
set_property -dict { PACKAGE_PIN K17   IOSTANDARD LVCMOS25 } [get_ports { config_spi_miso }]; #IO_L21P_T3_DQS_15 Sch=fmc_la_p[13]
set_property -dict { PACKAGE_PIN H22   IOSTANDARD LVCMOS25 } [get_ports { en_pll }]; #IO_L7N_T1_AD2N_15 Sch=fmc_la_n[14]
set_property -dict { PACKAGE_PIN J22   IOSTANDARD LVCMOS25 } [get_ports { config_spi_csb}]; #IO_L7P_T1_AD2P_15 Sch=fmc_la_p[14]
set_property -dict { PACKAGE_PIN K16   IOSTANDARD LVCMOS25 } [get_ports { out_wr_ram2 }]; #IO_L23N_T3_FWE_B_15 Sch=fmc_la_n[15]
set_property -dict { PACKAGE_PIN L16   IOSTANDARD LVCMOS25 } [get_ports { out_ld_vdac }]; #IO_L23P_T3_FOE_B_15 Sch=fmc_la_p[15]
set_property -dict { PACKAGE_PIN G18   IOSTANDARD LVCMOS25 } [get_ports { out_ld_column }]; #IO_L4N_T0_15 Sch=fmc_la_n[16]
set_property -dict { PACKAGE_PIN G17   IOSTANDARD LVCMOS25 } [get_ports { chip_inj_chopper }]; #IO_L4P_T0_15 Sch=fmc_la_p[16]
set_property -dict { PACKAGE_PIN B18   IOSTANDARD LVCMOS25 } [get_ports { reset_analog_b }]; #IO_L11N_T1_SRCC_16 Sch=fmc_la17_cc_n
set_property -dict { PACKAGE_PIN B17   IOSTANDARD LVCMOS25 } [get_ports { bypass_cmd }]; #IO_L11P_T1_SRCC_16 Sch=fmc_la17_cc_p
#set_property -dict { PACKAGE_PIN C17   IOSTANDARD LVCMOS25 } [get_ports { reset_digital_b }]; #IO_L12N_T1_MRCC_16 Sch=fmc_la18_cc_n
#set_property -dict { PACKAGE_PIN D17   IOSTANDARD LVCMOS25 } [get_ports { cmd_clock_invert }]; #IO_L12P_T1_MRCC_16 Sch=fmc_la18_cc_p
set_property -dict { PACKAGE_PIN A19   IOSTANDARD LVCMOS25 } [get_ports { trigro_reset_n }]; #IO_L17N_T2_16 Sch=fmc_la_n[19]
set_property -dict { PACKAGE_PIN A18   IOSTANDARD LVCMOS25 } [get_ports { interface_speed }]; #IO_L17P_T2_16 Sch=fmc_la_p[19]
#set_property -dict { PACKAGE_PIN F20   IOSTANDARD LVCMOS25 } [get_ports { untriggered_ro_en }]; #fmc_la_n[20] }]; #IO_L18N_T2_16 Sch=fmc_la_n[20]
#set_property -dict { PACKAGE_PIN F19   IOSTANDARD LVCMOS25 } [get_ports { use_spi }]; #IO_L18P_T2_16 Sch=fmc_la_p[20]
set_property -dict { PACKAGE_PIN D19   IOSTANDARD LVCMOS25 } [get_ports { hitbus }]; #fmc_la_n[21] }]; #IO_L14N_T2_SRCC_16 Sch=fmc_la_n[21]
set_property -dict { PACKAGE_PIN E19   IOSTANDARD LVCMOS25 } [get_ports { take_fast }]; #IO_L14P_T2_SRCC_16 Sch=fmc_la_p[21]
#set_property -dict { PACKAGE_PIN D21   IOSTANDARD LVCMOS25 } [get_ports { por }]; #IO_L23N_T3_16 Sch=fmc_la_n[22]
#set_property -dict { PACKAGE_PIN E21   IOSTANDARD LVCMOS25 } [get_ports { regulator_reset_out }]; #IO_L23P_T3_16 Sch=fmc_la_p[22]
#set_property -dict { PACKAGE_PIN A21   IOSTANDARD LVDS_25 } [get_ports { trigger_test_n}]; #cdo_n }]; #IO_L21N_T3_DQS_16 Sch=fmc_la_n[23]
#set_property -dict { PACKAGE_PIN B21   IOSTANDARD LVDS_25 } [get_ports { trigger_test_p}]; #cdo_p }]; #IO_L21P_T3_DQS_16 Sch=fmc_la_p[23]
#set_property -dict { PACKAGE_PIN B16   IOSTANDARD LVDS_25 } [get_ports { data_fmc_n }]; #IO_L7N_T1_16 Sch=fmc_la_n[24]
#set_property -dict { PACKAGE_PIN B15   IOSTANDARD LVDS_25 } [get_ports { data_fmc_p }]; #IO_L7P_T1_16 Sch=fmc_la_p[24]

#BEGIN Astropix FMC
#interrupt: Hit interrupt
set_property -dict { PACKAGE_PIN B15   IOSTANDARD LVCMOS25 } [get_ports { interrupt }]; #IO_L7P_T1_16 Sch=fmc_la_p[24]

#res_n: negedge async reset
set_property -dict { PACKAGE_PIN B16   IOSTANDARD LVCMOS25 } [get_ports { res_n }]; #IO_L7N_T1_16 Sch=fmc_la_n[24]

#sample_clk
set_property -dict { PACKAGE_PIN C17   IOSTANDARD LVDS_25 } [get_ports { sample_clk_n }]; #IO_L12N_T1_MRCC_16 Sch=fmc_la18_cc_n
set_property -dict { PACKAGE_PIN D17   IOSTANDARD LVDS_25 } [get_ports { sample_clk_p }]; #IO_L12P_T1_MRCC_16 Sch=fmc_la18_cc_p

#SPI
set_property -dict { PACKAGE_PIN D21   IOSTANDARD LVCMOS25 } [get_ports { spi_miso }]; #IO_L23N_T3_16 Sch=fmc_la_n[22]
set_property -dict { PACKAGE_PIN E21   IOSTANDARD LVCMOS25 } [get_ports { spi_clk }]; #IO_L23P_T3_16 Sch=fmc_la_p[22]
set_property -dict { PACKAGE_PIN F20   IOSTANDARD LVCMOS25 } [get_ports { spi_mosi }]; #fmc_la_n[20] }]; #IO_L18N_T2_16 Sch=fmc_la_n[20]
set_property -dict { PACKAGE_PIN F19   IOSTANDARD LVCMOS25 } [get_ports { spi_csn }]; #IO_L18P_T2_16 Sch=fmc_la_p[20]

#END Astropix FMC
#set_property -dict { PACKAGE_PIN B16   DIFF_TERM TRUE } [get_ports {data_fmc_n}];
#set_property -dict { PACKAGE_PIN B15   DIFF_TERM TRUE } [get_ports {data_fmc_n}];

#set_property DIFF_TERM TRUE  [get_ports {data_fmc_p}];
#set_property DIFF_TERM TRUE  [get_ports {data_fmc_n}];

#set_property DIFF_TERM TRUE  [get_ports {data_p}];
#set_property DIFF_TERM TRUE  [get_ports {data_n}];


#set_property -dict { PACKAGE_PIN E17   IOSTANDARD LVDS_25 } [get_ports { bsyncres_n }]; #IO_L2N_T0_16 Sch=fmc_la_n[25]
#set_property -dict { PACKAGE_PIN F16   IOSTANDARD LVDS_25 } [get_ports { bsyncres_p }]; #IO_L2P_T0_16 Sch=fmc_la_p[25]
set_property -dict { PACKAGE_PIN E18   IOSTANDARD LVDS_25 } [get_ports { gecco_inj_chopper_n }]; #IO_L15N_T2_DQS_16 Sch=fmc_la_n[26]
set_property -dict { PACKAGE_PIN F18   IOSTANDARD LVDS_25 } [get_ports { gecco_inj_chopper_p }]; #IO_L15P_T2_DQS_16 Sch=fmc_la_p[26]
set_property -dict { PACKAGE_PIN A20   IOSTANDARD LVDS_25 } [get_ports { vb_data_n  }]; #IO_L16N_T2_16 Sch=fmc_la_n[27]
set_property -dict { PACKAGE_PIN B20   IOSTANDARD LVDS_25 } [get_ports { vb_data_p  }]; #IO_L16P_T2_16 Sch=fmc_la_p[27]
set_property -dict { PACKAGE_PIN B13   IOSTANDARD LVDS_25 } [get_ports { vb_clock_n }]; #IO_L8N_T1_16 Sch=fmc_la_n[28]
set_property -dict { PACKAGE_PIN C13   IOSTANDARD LVDS_25 } [get_ports { vb_clock_p }]; #IO_L8P_T1_16 Sch=fmc_la_p[28]
set_property -dict { PACKAGE_PIN C15   IOSTANDARD LVDS_25 } [get_ports { vb_load_n  }]; #IO_L3N_T0_DQS_16 Sch=fmc_la_n[29]
set_property -dict { PACKAGE_PIN C14   IOSTANDARD LVDS_25 } [get_ports { vb_load_p  }]; #IO_L3P_T0_DQS_16 Sch=fmc_la_p[29]

#Astropix
#LA30-32 single-ended LA33 diff
#IN1
#set_property -dict { PACKAGE_PIN A16   IOSTANDARD LVDS_25 } [get_ports { config_ld_n }]; #IO_L9N_T1_DQS_16 Sch=fmc_la_n[32]
set_property -dict { PACKAGE_PIN A15   IOSTANDARD LVCMOS25 } [get_ports { config_ld_p }]; #IO_L9P_T1_DQS_16 Sch=fmc_la_p[32]
#IN2
#set_property -dict { PACKAGE_PIN F14   IOSTANDARD LVDS_25 } [get_ports { config_ck2_n }]; #IO_L1N_T0_16 Sch=fmc_la_n[33]
set_property -dict { PACKAGE_PIN F13   IOSTANDARD LVCMOS25 } [get_ports { config_ck2_p }]; #IO_L1P_T0_16 Sch=fmc_la_p[33]
#IN3
#set_property -dict { PACKAGE_PIN A14   IOSTANDARD LVDS_25 } [get_ports { config_sin_n }]; #IO_L10N_T1_16 Sch=fmc_la_n[30]
set_property -dict { PACKAGE_PIN A13   IOSTANDARD LVCMOS25 } [get_ports { config_sin_p }]; #IO_L10P_T1_16 Sch=fmc_la_p[30]
#IN4
#set_property -dict { PACKAGE_PIN E14   IOSTANDARD LVDS_25 } [get_ports { config_ck1_n }]; #IO_L4N_T0_16 Sch=fmc_la_n[31]
set_property -dict { PACKAGE_PIN E13   IOSTANDARD LVCMOS25 } [get_ports { config_ck1_p }]; #IO_L4P_T0_16 Sch=fmc_la_p[31]



#set_property -dict { PACKAGE_PIN K19   IOSTANDARD LVCMOS25 } [get_ports { vboard_sin }];
#set_property -dict { PACKAGE_PIN J21   IOSTANDARD LVCMOS25 } [get_ports { vboard_ld }];
#set_property -dict { PACKAGE_PIN J20   IOSTANDARD LVCMOS25 } [get_ports { vboard_ck }];
#set_property -dict {PACKAGE_PIN L18 IOSTANDARD LVDS_25} [get_ports fast_naddrl_n]
#set_property -dict {PACKAGE_PIN M18 IOSTANDARD LVDS_25} [get_ports fast_naddrl_p]
#set_property -dict {PACKAGE_PIN N19 IOSTANDARD LVDS_25} [get_ports fast_ntsl_n]
#set_property -dict {PACKAGE_PIN N18 IOSTANDARD LVDS_25} [get_ports fast_ntsl_p]
#set_property -dict {PACKAGE_PIN M20 IOSTANDARD LVDS_25} [get_ports fast_naddrr_n]
#set_property -dict {PACKAGE_PIN N20 IOSTANDARD LVDS_25} [get_ports fast_naddrr_p]
#set_property -dict {PACKAGE_PIN L13 IOSTANDARD LVDS_25} [get_ports nhitbus_n]
#set_property -dict {PACKAGE_PIN M13 IOSTANDARD LVDS_25} [get_ports nhitbus_p]
#set_property -dict {PACKAGE_PIN M16 IOSTANDARD LVDS_25} [get_ports fast_ntsr_n]
#set_property -dict {PACKAGE_PIN M15 IOSTANDARD LVDS_25} [get_ports fast_ntsr_p]
#set_property -dict { PACKAGE_PIN K17   IOSTANDARD LVCMOS25 } [get_ports { inj_chopper }]; 
#set_property -dict {PACKAGE_PIN L16 IOSTANDARD LVCMOS25} [get_ports config_ck1]
#set_property -dict {PACKAGE_PIN G17 IOSTANDARD LVCMOS25} [get_ports config_ck2]
#set_property -dict {PACKAGE_PIN A19 IOSTANDARD LVDS_25} [get_ports fast_syres_n]
#set_property -dict {PACKAGE_PIN A18 IOSTANDARD LVDS_25} [get_ports fast_syres_p]
#set_property -dict {PACKAGE_PIN F20 IOSTANDARD LVDS_25} [get_ports fast_fastck_n]
#set_property -dict {PACKAGE_PIN F19 IOSTANDARD LVDS_25} [get_ports fast_fastck_p]
#set_property -dict {PACKAGE_PIN E19 IOSTANDARD LVCMOS25} [get_ports config_nsin]
#set_property -dict {PACKAGE_PIN E21 IOSTANDARD LVCMOS25} [get_ports config_asin]
#set_property -dict {PACKAGE_PIN B15 IOSTANDARD LVCMOS25} [get_ports config_ld]
#set_property -dict {PACKAGE_PIN F16 IOSTANDARD LVCMOS25} [get_ports config__sin]
#set_property -dict {PACKAGE_PIN C15 IOSTANDARD LVCMOS25} [get_ports config_bsin]


set_false_path -from [get_clocks prog_clko] -to [get_clocks clk]
set_false_path -from [get_clocks clk] -to [get_clocks prog_clko]

