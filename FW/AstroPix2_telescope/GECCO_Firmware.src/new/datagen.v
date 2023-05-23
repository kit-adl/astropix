`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 07/25/2020 12:53:52 PM
// Design Name: 
// Module Name: datagen
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


module datagen(
    input wire clk_400p,
    input wire reset,
    input wire injection,
    output reg bit_d_out_gen_del,
    output reg sync
    );
    
    
    
    reg [63:0] datagen;
    reg [63:0] datagenpoly;
    wire xorgen;
    wire bit_d_out_gen;
    reg [7:0] shiftcntScrGen;
    //reg bit_d_out_gen_del;//this is the output
    
    //scramble
    assign xorgen = datagen[63] ^ datagenpoly[57] ^ datagenpoly[38];
    //assign xorgen =  datagen[63] ^ datagenpoly[0];
    
    //noscramble
    //assign xorgen = datagen[63];
    
    assign bit_d_out_gen = ((shiftcntScrGen==0) || (shiftcntScrGen==1)) ? ~shiftcntScrGen[0] : xorgen;
    
    
    wire [31:0] HitData, HitData2, BoEData, EoEData, NoData;
    wire [7:0] ColAddFromDet;
    wire [6:0] TS2FromDet;
    wire [8:0] RowAddFromDet;
    wire [9:0] TSFromDet;
    wire [6:0] TriggerTag;
    wire  fifo_full;
    wire [9:0] L1TSToDet;
    wire [15:0] BinCounter;
    
    
    assign ColAddFromDet = 0;
    assign TS2FromDet = 0;
    assign RowAddFromDet = 0;
    assign TSFromDet = 0;
    assign TriggerTag = 0;
    assign  fifo_full = 0;
    assign L1TSToDet = 0;
    assign BinCounter = 0;
    
        
    assign HitData = {8'hDA, ColAddFromDet[7:0], TS2FromDet[6:0], RowAddFromDet[8:0]};
    assign HitData2 = {4'hC,1'h0,RowAddFromDet[8:0],ColAddFromDet[7:0],TSFromDet[9:0]};
    assign EoEData = {8'hEE,1'h0,TriggerTag[6:0],5'h0,fifo_full,L1TSToDet[9:0]};
    assign BoEData = {16'hABCD, BinCounter[15:0]};
    assign NoData  = {8'hBC, 8'hBC, 8'hBC, 8'hBC};//actually not used, only after reset
    
    
    reg senddata1, senddata2, senddata3;
    
    
        always @(posedge clk_400p) 
        begin
                if (reset) begin            
                    datagenpoly <= 64'h11;
                    shiftcntScrGen <= 18;
                    senddata1 <= 0;
                    senddata2 <= 0;
                    senddata3 <= 0;
                    
                end    
                else begin
                        bit_d_out_gen_del <= bit_d_out_gen;
                        
                        sync <= shiftcntScrGen == 8'd65;
                        
                        if(shiftcntScrGen < 8'd65) shiftcntScrGen <= shiftcntScrGen + 1;
                        else begin
                        
                            shiftcntScrGen <= 0;
                        
                        end
                        if (shiftcntScrGen == 0) begin
                        
                            //senddata1 <= injection & ~senddata1 & ~senddata2 & ~senddata3;
                            
                            senddata1 <= injection;
                            senddata2 <= senddata1;
                            senddata3 <= senddata2;
                            if (senddata1 == 1 && senddata2 == 0) datagen <= {HitData, BoEData};
                            else if (senddata2 == 1 && senddata3 == 0) datagen <= {EoEData, HitData};                    
                            //else datagen <= 64'h78_10_00_00_00_00_0A_DA;//data to send  8'h78,8'h10,48'hADA
                            //else datagen <= 64'hFF_FF_FF_FF_FF_FF_FF_FF;//data to send  8'h78,8'h10,48'hADA
                            else datagen <= 64'hFF_FF_FF_00_FF_FF_FF_FF;//data to send  8'h78,8'h10,48'hADA
                        
                        end
                        
                        if (shiftcntScrGen >= 2) datagen <= {datagen[62:0], 1'b0};
                        if (shiftcntScrGen >= 2) datagenpoly <= {datagenpoly[62:0], xorgen};            
                end      
        end // always
    
    
    
endmodule
