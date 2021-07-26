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
 *
 * This module was developed by Felix Ehrler and Richard Leys
 */
`timescale 1ns / 1ps

module ftdi_interface_control_fsm ( clk, res_n, rxf_n, txe_n, rf_almost_full, wf_almost_empty, wf_empty, prio, 
rd_n, oe_n, wr_n );                                                                                                

input wire clk, res_n, rxf_n, txe_n, rf_almost_full, wf_almost_empty, wf_empty, prio;
output wire rd_n, oe_n, wr_n;                                                            

parameter idle = 3'b111;
parameter out_en = 3'b101;
parameter read = 3'b001;  
parameter write = 3'b110; 

reg [2:0] current_state, next_state;
assign {rd_n, oe_n, wr_n} = current_state;

wire [5:0] inputvector;
assign inputvector = {rxf_n, txe_n, rf_almost_full, wf_almost_empty, wf_empty, prio};


always @(*) begin
  casex({inputvector, current_state})
    {6'bxx1x1x, idle}:   next_state = idle;
    {6'bx11xxx, idle}:   next_state = idle;
    {6'b1xxx1x, idle}:   next_state = idle;
    {6'b11xxxx, idle}:   next_state = idle;
    {6'b0x0xx0, idle}:   next_state = out_en;
    {6'b0x0x11, idle}:   next_state = out_en;
    {6'b010x01, idle}:   next_state = out_en;
    {6'bx0xx01, idle}:   next_state = write;
    {6'b10xx00, idle}:   next_state = write;
    {6'b001x00, idle}:   next_state = write;
    {6'bxxxxxx, out_en}:   next_state = read;
    {6'b0x0xx0, read}:   next_state = read;
    {6'b0x0x1x, read}:   next_state = read;
    {6'b010xxx, read}:   next_state = read;
    {6'b1xxxxx, read}:   next_state = idle;
    {6'bxx1xxx, read}:   next_state = idle;
    {6'bx0xx01, read}:   next_state = idle;
    {6'bxxx1xx, write}:   next_state = idle;
    {6'bx1xxxx, write}:   next_state = idle;
    {6'b0x0xx0, write}:   next_state = idle;
    {6'bx0x0x1, write}:   next_state = write;
    {6'bx010xx, write}:   next_state = write;
    {6'b10x0xx, write}:   next_state = write;
    default:  next_state = idle;
  endcase
end

always @(posedge clk or negedge res_n ) begin
  if (res_n  == 1'b0)
    current_state <= idle;
  else
    current_state <= next_state;
end

endmodule
