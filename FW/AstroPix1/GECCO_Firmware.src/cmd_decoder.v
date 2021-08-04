//////////////////////////////////////////////////////////////////////////////////
// Company:     KIT-ADL
// Engineer:    Rudolf Schimassek
// 
// Create Date: 14.11.2019
// Design Name: 
// Module Name: CMD Decoder Data Sender
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
//
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////

module cmd_decoder(
    input  wire       reset,
    input  wire       enable,
    input  wire       clock,
    
    output wire       alignment_found,
    
    input  wire [7:0] data,
    output reg        rd_clk,
    output reg        rd_en,
    input  wire       fifo_empty,
    input  wire       fifo_6entries, //one command consists of 6 bytes of data (this is an prog_empty flag)
    
    output reg        cmd_line,
    output wire       cmd_clock,
    
    output reg TS_clk_out
);





//actual data sending:
reg [2:0] clockdivider = 0; //for the FIFO (f(intclock) ~ 160 MHz > f_max(FIFO) ~ 100 MHz)
                            //  and used for 8bit data loading
//these two signals form a "handshake":
reg       async_rd_en = 0;
reg       processed   = 0;
//the data sender toggles `async_rd_en` to mark the necessity of loading the next dataset
//  and the data reader toggles `processed` and checks the XOR of the signals for setting
//  the rd_en for the FIFO

//reg [2:0] bitnumber = 3'b0; //basically the same as clockdivider -> merged as clockdivider
wire [15:0] syncword;
assign syncword[15:0] = 16'b1000_0001_0111_1110;

wire [15:0] pllword;
assign pllword[15:0] = 16'b1010_1010_1010_1010;

wire [15:0] pllword2;
assign pllword2[15:0] = 16'b1100_1100_1100_1100;

reg syncpart = 0;
reg [7:0] senddata = 8'b0;
reg alreadywriting = 1'b0;


    //static const int syncword = 0b10000001 01111110;
    //static const int rwreg    = 0b01100110 01100110;



reg [7:0] sendmanysunc;

reg detectedpart1;




always @(posedge clock) begin
    if(reset) begin
        cmd_line <= 0;//ivan
        
        //cmd_line <= ~cmd_line;//ivan
        //bitnumber <= 0;
        senddata <= syncword[7:0];
        syncpart <= 0;
        alreadywriting <= 1'b0;
        
        rd_en <= 0;
        rd_clk <= 0;
        clockdivider <= 0;
        sendmanysunc <= 0;
        detectedpart1 <= 0;
    end
    else begin
        //clock divider for FIFO and 8bit clock loading:
        clockdivider <= clockdivider + 3'b1;
        //TS_clk_out <= clockdivider[1]; //ivan old   
                

        case(clockdivider)
            3: rd_clk <= 1;
            5: rd_clk <= 0;
            //rd_en synchronisation:
            1: begin
                if(async_rd_en ^ processed) begin
                    rd_en <= 1;
                    processed <= ~processed;
                end
                else if(rd_en)
                    rd_en <= 0;
            end
        endcase
        
    
        if(enable) begin
            //load new data after sending 8 bits:
            if(clockdivider == 7) begin
                syncpart <= ~syncpart;
                //prepare a byte for sending, i.e. retrieve from FIFO during sending of 1st byte of 2 (syncpart==1):
                // or continue reading from FIFO until it is empty
                

                if(sendmanysunc > 0) begin
                    senddata <= (syncpart)? syncword[15:8] : syncword[7:0];
                    if(syncpart) sendmanysunc <= sendmanysunc -1;
                end
                else begin//begin not send many
                if((~fifo_6entries || (~fifo_empty && alreadywriting)) && (syncpart || alreadywriting)) begin
                    async_rd_en <= ~async_rd_en; //request load signal
                    alreadywriting <= 1;
                end
                else
                    alreadywriting <= 0;

                if(alreadywriting) begin
                    senddata <= data;
                    if(data == 8'b0111_1110) detectedpart1<= 1;
                    if(detectedpart1) if(data == 8'b1000_0001) begin
                     sendmanysunc <= 64;
                     detectedpart1 <= 0;
                    end
                    
                    
                end
                //always send sync word if no data is available
                else begin
                    senddata <= (syncpart)? syncword[15:8] : syncword[7:0];//ivan
                    //senddata <= (syncpart)? pllword[15:8] : pllword[7:0];
                end
                
                end//dont send many
                
            end//on cnt 7
            else
                senddata <= {1'b0, senddata[7:1]};
            cmd_line <= senddata[0];//ivan
            //cmd_line <= ~cmd_line;//ivan
        end
        else
            cmd_line <= 0;
    end
end


//code for 40mhz clk ivan

   reg [3:0]		bit_count;   
   reg [15:0]    	shift_reg;
   
   reg [3:0] SR_clk_40MHZ;	
   //reg clk_40MHZ;//delayed clock
    
   
   always @(posedge clock) begin
       if(reset) begin
            bit_count <= 0;
            shift_reg <= 0;
            SR_clk_40MHZ <= 0;
            //clk_40MHZ <= 0;
       end
       else begin
       
               SR_clk_40MHZ[3:0] <= {SR_clk_40MHZ[2:0], bit_count[1]};//
               TS_clk_out <= SR_clk_40MHZ[0];//for bx phase 0!
               
               
               shift_reg[15:0] <= {cmd_line, shift_reg[15:1]}; // receiver
                           
               if (shift_reg[15:0] == syncword) begin 
                   bit_count <= 0;
               end
               else begin
                   bit_count <= bit_count + 1;
               end
       
       
       
       end
   end
   
   


endmodule