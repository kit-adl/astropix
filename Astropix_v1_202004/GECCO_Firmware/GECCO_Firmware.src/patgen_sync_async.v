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
 * This module was initially developed by Felix Ehrler as sync_patgen_simple
 */
`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 	KIT-ADL
// Engineer:    Felix Ehrler
// 
// Create Date: 12.04.2018 08:13:55
// Design Name: 
// Module Name: sync_async_patgen
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
//          0.02 - added synchronisation flag and added option to run 
//                  asynchronously
//                 added a suspend input to pause the execution
//                 (08.02.19, Rudolf Schimassek)
// Additional Comments:
//
//      Pattern Generator for use synchronised or asynchronous with respect to
//      an external signal (<syncrst>). The mode is determined by <synced>.
//      In sync mode, each pulse set has to be initiated with a new syncrst
//      signal. In async mode, the pulse sets are sent right after each other.
//      The pulse generation can be paused with <suspend> or aborted using
//      <rst>.
//      <Periode> (address 8) specifies the length of the pulse and half the
//          repetition length for pulse sets: In pulse sets, <periode> defines
//          the length of the pulses and of the time between them (50% duty
//          cycle).
//      <Numpulses> (address 7) specifies the number of pulses in a pulse set.
//          Keeping it 0 equals single pulse mode.
//      <Runlen> (addresses 10 & 11) specifies the number of pulse sets before
//          ending.
//      <Idelay> (addresses 12 & 13) is the delay before the first pulse in a 
//          pulse set. For single pulses in async mode, it also specifies the 
//          off-time between the signals.
//      <Clkfac> (addresses 14 & 15) is the value for the clock divider 
//          applied to the whole structure giving the time base for the other 
//          values. This value is smaller by 1 than the number of input clock
//          periods used for one cycle (for '0', input clock equals the driving
//          clock of the functional code part, '1' equals a clock divider of 2).
//
//      The interface and addresses are taken from the pattern generator written
//      by Martin Koniczek (Uni Mannheim). However, the synchronisation inputs
//      were added and the last toggle time stamp address (7) is used for the
//      number of pulses in a pulse set.
// 
//////////////////////////////////////////////////////////////////////////////////


module sync_async_patgen(
    input clk, 
    input rst,
    input suspend, 
    input write, 
    input [3:0] addr, 
    input [7:0] din,
    input wire synced,
    input wire syncrst,
	output reg out, 
	output reg running, 
	output reg done
    );

reg [15:0] runlen    = 0; // number of pulse sets
reg [15:0] idelay    = 0; // wait before first pulse
reg [15:0] clkfac    = 0; // clock divider
reg [7:0]  periode   = 0; // length of a signle pulse ( <=> Period / 2 for several pulses)
reg [8:0]  numpulses = 0; // number of toggle times per pulse set
                          //    => 2 toggles per pulse

reg [15:0] runcnt;      // number of pulse set
reg [15:0] idelaycnt;   // wait after reset
reg [15:0] clkfaccnt;   // clock divider
reg [15:0] periodecnt;  // length of pulse
reg [8:0]  pulsecnt;    // toggle index in a pulse set
reg infinite;           // infinite number of injections

reg prev_syncrst;       // for edge detection on syncrst
reg syncrst_reg;

always @( posedge clk ) begin

  /* register file writing */
  if (write) begin
  	 casex ( addr[3:0] )
	 	//4'b0xxx: begin
		//	regfile[addr[2:0]] <= din;
		//end
		4'b0111: numpulses    <= {din,1'b0} - ((din > 0)?9'd1:9'd0);
		4'b1000: periode      <= din;
		//4'b1001: {rstate, istate, rsondone, isondone, rsonsusp, isonsusp, dummy , rstttaddr} <= 
		//			{din[0], din[1],  din[2] ,  din[3] ,  din[4] ,  din[5] , din[6], din[7] };
		4'b1010: runlen[15:8] <= din;
		4'b1011: runlen[ 7:0] <= din;
		4'b1100: idelay[15:8] <= din;
		4'b1101: idelay[ 7:0] <= din;		
		4'b1110: clkfac[15:8] <= din;
		4'b1111: clkfac[ 7:0] <= din;
	 endcase	 
  end
  
  /*reset state values are taken on reset*/
    if (rst) begin
        runcnt      <= runlen-1'b1;
        if (runlen == 16'b0)
            infinite <= 1'b1;
        else
            infinite <= 1'b0;
        idelaycnt   <= idelay;  //TODO: add a shift by 8 bits like for periode
        periodecnt  <= {periode, 8'd0};
        pulsecnt    <= numpulses;
        running     <= 1'b0;
        done        <= 1'b0;
        out         <= 1'b0;
        clkfaccnt   <= clkfac;
        prev_syncrst <= 0;       // for edge detection on syncrst
        syncrst_reg <= 0;
    end else if(!suspend) begin     //suspend pauses the execution of the pulse sending

        //edge detection for synchronisation:
        prev_syncrst <= syncrst_reg;//ivan
        syncrst_reg <= syncrst;//ivan
        if (!done && !running && synced) begin     //start condition ivan               
            //if (!prev_syncrst && syncrst) begin
            if (!prev_syncrst && syncrst_reg) begin
                running <= 1'b1;
                clkfaccnt <= clkfac;
            end
        end     //!done && !running
        
        //pulse generation after a syncrst (sync mode) or as long as not done (async mode):
        if ((running || !synced) && !done) begin               
            if (clkfaccnt > 16'b0)                      //clock divider
                clkfaccnt <= clkfaccnt - 16'b1;            
            else begin
                clkfaccnt <= clkfac;   
                if (idelaycnt > 16'b0 && out == 1'b0)   //wait initial delay
                    idelaycnt <= idelaycnt - 16'b1;
                else if (idelaycnt == 16'b0 && out == 1'b0 && pulsecnt == numpulses)   // turn on output for first pulse
                    out <= 1'b1;                    
                else if ((out == 1'b1 || pulsecnt > 9'd0) && periodecnt > 1'b0)  // wait <periode>
                    periodecnt <= periodecnt - 1'b1;
                else if (pulsecnt > 1 && periodecnt == 16'b0) begin     //toggle output for several pulses
                    out         <= ~out;
                    periodecnt  <= {periode, 8'd0};
                    pulsecnt    <= pulsecnt - 9'd1;
                end
                else begin                              //turn off output and prepare for the next start
                    out         <= 1'b0;
                    running     <= 1'b0;
                    idelaycnt   <= idelay;
                    periodecnt  <= {periode, 8'd0};
                    pulsecnt    <= numpulses;
                    if (!infinite) begin                //pulse set management
                        if (runcnt > 1'b0)
                            runcnt  <= runcnt - 1'b1;
                        else if (runcnt == 16'b0)
                            done    <= 1'b1;
                    end // infinite
                end // (out == 1'b1 && periodecnt == 16'b0)
            end // clkfaccnt == 0
        end  //(running || !sync) && !done
    end  //!rst
end // always @ clk

endmodule
