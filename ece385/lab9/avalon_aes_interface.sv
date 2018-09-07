/************************************************************************
Avalon-MM Interface for AES Decryption IP Core

Dong Kai Wang, Fall 2017

For use with ECE 385 Experiment 9
University of Illinois ECE Department

Register Map:

 0-3 : 4x 32bit AES Key
 4-7 : 4x 32bit AES Encrypted Message
 8-11: 4x 32bit AES Decrypted Message
   12: Not Used
	13: Not Used
   14: 32bit Start Register
   15: 32bit Done Register

************************************************************************/

module avalon_aes_interface (
	// Avalon Clock Input
	input logic CLK,
	
	// Avalon Reset Input
	input logic RESET,
	
	// Avalon-MM Slave Signals
	input  logic AVL_READ,					// Avalon-MM Read
	input  logic AVL_WRITE,					// Avalon-MM Write
	input  logic AVL_CS,						// Avalon-MM Chip Select
	input  logic [3:0] AVL_BYTE_EN,		// Avalon-MM Byte Enable
	input  logic [3:0] AVL_ADDR,			// Avalon-MM Address
	input  logic [31:0] AVL_WRITEDATA,	// Avalon-MM Write Data
	output logic [31:0] AVL_READDATA,	// Avalon-MM Read Data
	
	// Exported Conduit
	output logic [31:0] EXPORT_DATA		// Exported Conduit Signal to LEDs
);

logic [31:0] Register [16];
logic [127:0] AES_KEY, AES_MSG_ENC, AES_MSG_DEC;
logic DONE_FLAG, START_FLAG;

always_ff @ (posedge CLK)
	begin	
		if(RESET)
			begin	
				for(integer i = 0; i < 16; i = i + 1)
					Register[i] <= 32'b0;
			end
		else if(AVL_WRITE && AVL_CS) //Write operation is to be performed
			begin
				if(AVL_BYTE_EN == 4'b1111)										//Write all 32 bits
					Register[AVL_ADDR] <= AVL_WRITEDATA;
				else if(AVL_BYTE_EN == 4'b1100)								//Write the two upper bytes
					Register[AVL_ADDR][31:16] <= AVL_WRITEDATA[31:16];
				else if(AVL_BYTE_EN == 4'b0011)								//Write the two lower bytes
					Register[AVL_ADDR][15:0] <= AVL_WRITEDATA[15:0];
				else if(AVL_BYTE_EN == 4'b1000)								//Write byte 3 only
					Register[AVL_ADDR][31:24] <= AVL_WRITEDATA[31:24];
				else if(AVL_BYTE_EN == 4'b0100)								//Write byte 2 only
					Register[AVL_ADDR][23:16] <= AVL_WRITEDATA[23:16];
				else if(AVL_BYTE_EN == 4'b0010)								//Write byte 1 only
					Register[AVL_ADDR][15:8] <= AVL_WRITEDATA[15:8];
				else if(AVL_BYTE_EN == 4'b0001)								//Write byte 0 only
					Register[AVL_ADDR][7:0] <= AVL_WRITEDATA[7:0];
			end
		else if(DONE_FLAG)
			begin
				Register[8]  <= AES_MSG_DEC[127:96];
				Register[9]  <= AES_MSG_DEC[95:64];
				Register[10] <= AES_MSG_DEC[63:32];
				Register[11] <= AES_MSG_DEC[31:0];	
				Register[15][0] <= DONE_FLAG;
			end
	end
	
//always_comb
//begin

assign	AES_KEY = {Register[0], Register[1], Register[2], Register[3]};
assign	AES_MSG_ENC = {Register[4], Register[5], Register[6], Register[7]};
assign   START_FLAG = Register[14][0];


assign	AVL_READDATA = (AVL_CS && AVL_READ) ? Register[AVL_ADDR]: 16'b0;

assign	EXPORT_DATA[31:24] = Register[0][31:24];
assign	EXPORT_DATA[23:16] = Register[0][23:16];						//Assign upper two bytes of AES_Key to EXPORT DATA
assign	EXPORT_DATA[15:8]  = Register[3][15:8];
assign	EXPORT_DATA[7:0]   = Register[3][7:0];							//Assign lower two bytes of AES_Key to EXPORT DATA
//end

AES AES0 (.CLK(CLK), .RESET(RESET), .AES_START(START_FLAG), .AES_KEY(AES_KEY), .AES_MSG_ENC(AES_MSG_ENC),
				.AES_DONE(DONE_FLAG), .AES_MSG_DEC(AES_MSG_DEC));
	
endmodule
