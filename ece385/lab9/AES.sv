/************************************************************************
AES Decryption Core Logic

Dong Kai Wang, Fall 2017

For use with ECE 385 Experiment 9
University of Illinois ECE Department
************************************************************************/

module AES (
	input	 logic CLK,
	input  logic RESET,
	input  logic AES_START,
	output logic AES_DONE,
	input  logic [127:0] AES_KEY,
	input  logic [127:0] AES_MSG_ENC,
	output logic [127:0] AES_MSG_DEC
);


//Key Expansion needs more than one clock cycle to finish																												
	//How to get a flag to tell when Key Expansion is done, other than hard coding it, maybe just see how long to wait for the entire thing to be done	
	
logic [1407:0] KeySchedule;
logic [127:0] State_temp0, State_temp1, State_temp2, State_temp3;
logic [31:0] Curr_Column, Out_Column;
logic [127:0] AES_MSG_DEC_next;
logic [127:0] RoundKey_in, RoundKey_out, InvShiftRows_in, InvShiftRows_out, InvSubBytes_in, InvSubBytes_out, Intermediate_State, Intermediate_State_next;
logic [4:0] Round, Round_next, i_next, i;
logic [4:0] j, j_next;
logic AES_DONE_next;
enum logic [6:0] {WAIT,
						KeyExpansion_wait, 
						//////////////////////
						AddRoundKey0, 
						////////////////////// Rounds 1 - 9:
						InvShiftRows0, 
						InvSubBytes0_1,
						InvSubBytes0_2, 
						AddRoundKey1, 
						InvMixColumns0_0, 
						InvMixColumns0_1, 
						InvMixColumns0_2, 
						InvMixColumns0_3,
						////////////////////// Final Round:
						InvShiftRows9, 
						InvSubBytes9_1,
						InvSubBytes9_2, 
						AddRoundKey10,
						HOLD
						} state, next_state; 
				 
KeyExpansion KeyExpansion1 (.clk(CLK), .Cipherkey(AES_KEY), .KeySchedule(KeySchedule));	

AddRoundKey AddRoundKey_func (.state_in(RoundKey_in), .Round(Round), .curr_key(KeySchedule), .state_out(RoundKey_out));														
InvShiftRows InvShiftRows_func (.data_in(InvShiftRows_in), .data_out(InvShiftRows_out));																						
InvSubBytes_Abstract InvSubBytes_Abstract_func (.CLK(CLK), .State_in(InvSubBytes_in), .State_out(InvSubBytes_out));																										
InvMixColumns InvMixColumns_func (.in(Curr_Column), .out(Out_Column));																																						


always_ff @ (posedge CLK)
begin
	if(RESET)
		begin
			state <= WAIT;
			AES_DONE <= 0;
			i = 4'b0001;
			j = 4'b0000;
		end
	else
		begin
			state <= next_state;
			Intermediate_State <= Intermediate_State_next;
			AES_DONE <= AES_DONE_next;
			AES_MSG_DEC <= AES_MSG_DEC_next;
			i = i_next;
			j = j_next;
			Round = Round_next;
		end
end
																																																			
always_comb
begin
next_state = state;
Intermediate_State_next = Intermediate_State;
AES_DONE_next = AES_DONE;
RoundKey_in = 128'b0;
InvSubBytes_in = 128'b0;
InvShiftRows_in = 128'b0;
Curr_Column = 32'b0;
AES_MSG_DEC_next = AES_MSG_DEC;
i_next = i;
Round_next = Round;
j_next = j;
	
	unique case (state)
		WAIT : 
			if(AES_START && !AES_DONE)
				begin
					i_next = 4'b0001;
					Intermediate_State_next = AES_MSG_ENC;			//Setting the Intermediate_State equal to the encrypted message after you start the decryption process
					next_state = KeyExpansion_wait;
				end
		
		KeyExpansion_wait : 
			if(j < 20)
				begin
					next_state = KeyExpansion_wait;
					j_next = j + 1;
				end
			else
				next_state = AddRoundKey0;
///////////////////////////////////////////////////////////////			
		AddRoundKey0 : 
			next_state = InvShiftRows0;
/////////////////////////////////////////////////////////////// Rounds 1 - 9:
		InvShiftRows0 : 
			next_state = InvSubBytes0_1;
		
		InvSubBytes0_1 : 	
			next_state = InvSubBytes0_2;
			
		InvSubBytes0_2 : 
			next_state = AddRoundKey1;
		
		AddRoundKey1 :
			next_state = InvMixColumns0_0;
		
		InvMixColumns0_0 :
			next_state = InvMixColumns0_1;
			
		InvMixColumns0_1 : 
			next_state = InvMixColumns0_2;
			
		InvMixColumns0_2 : 
			next_state = InvMixColumns0_3;
			
		InvMixColumns0_3 : 
			begin
				if(i < 9)
					begin
						next_state = InvShiftRows0;
						i_next = i + 1'b1;
					end
				else
					next_state = InvShiftRows9;
				
			end
/////////////////////////////////////////////////////////////// Final Round:
		InvShiftRows9 : 
			next_state = InvSubBytes9_1;
		
		InvSubBytes9_1 : 	
			next_state = InvSubBytes9_2;
			
		InvSubBytes9_2 : 
			next_state = AddRoundKey10;
		
		AddRoundKey10 :
				next_state = HOLD;
			
		HOLD : 
			begin
				AES_MSG_DEC_next = Intermediate_State;
				AES_DONE_next = 1'b1;
			
				if(AES_START == 0)
					begin
						next_state = WAIT;
						AES_DONE_next = 1'b0;
					end
				else	
					next_state = HOLD;		
			end
	endcase
	
	
	
	case (state)
		WAIT : ;
		KeyExpansion_wait :
			Round_next = 4'b0000;
/////////////////////////////////////////////////////////////// 		
		AddRoundKey0 : 
			begin
				RoundKey_in = Intermediate_State;
				Intermediate_State_next = RoundKey_out;
			end
/////////////////////////////////////////////////////////////// Rounds 1 - 9:			
		InvShiftRows0 :
			begin
				InvShiftRows_in = Intermediate_State;
				Intermediate_State_next = InvShiftRows_out;
			end
			
		InvSubBytes0_1 : 
			begin
				InvSubBytes_in = Intermediate_State;
				Intermediate_State_next = InvSubBytes_out;
			end
		
		InvSubBytes0_2 : 
				Round_next = i;
			
		AddRoundKey1 : 
			begin
				RoundKey_in = Intermediate_State;
				Intermediate_State_next = RoundKey_out;
			end
			
		InvMixColumns0_0 : 
			begin
				Curr_Column = Intermediate_State[127:96];
				Intermediate_State_next[127:96] = Out_Column;
			end
			
		InvMixColumns0_1 : 
			begin
				Curr_Column = Intermediate_State[95:64];
				Intermediate_State_next[95:64] = Out_Column;
			end
			
		InvMixColumns0_2 : 
			begin
				Curr_Column = Intermediate_State[63:32];
				Intermediate_State_next[63:32] = Out_Column;
			end
			
		InvMixColumns0_3 : 
			begin
				Curr_Column = Intermediate_State[31:0];
				Intermediate_State_next[31:0] = Out_Column;
			end

/////////////////////////////////////////////////////////////// Final Round:
		InvShiftRows9 :
			begin
				InvShiftRows_in = Intermediate_State;
				Intermediate_State_next = InvShiftRows_out;
			end
			
		InvSubBytes9_1 : 
			begin
				InvSubBytes_in = Intermediate_State;
				Intermediate_State_next = InvSubBytes_out;
			end
		
		InvSubBytes9_2 : 
				Round_next = 4'b1010;

			
		AddRoundKey10 : 
			begin
				RoundKey_in = Intermediate_State;

				Intermediate_State_next = RoundKey_out;
			end
			
		HOLD : ;
		
		default : ;
	endcase
end																																																						
																												
endmodule


module InvSubBytes_Abstract( input logic CLK,
									  input  logic [127:0] State_in,
								     output logic [127:0] State_out);
 
InvSubBytes SubBytes0  (.clk(CLK), .in(State_in[127:120]), .out(State_out[127:120]));
InvSubBytes SubBytes1  (.clk(CLK), .in(State_in[119:112]), .out(State_out[119:112]));
InvSubBytes SubBytes2  (.clk(CLK), .in(State_in[111:104]), .out(State_out[111:104]));
InvSubBytes SubBytes3  (.clk(CLK), .in(State_in[103:96]),  .out(State_out[103:96]));
InvSubBytes SubBytes4  (.clk(CLK), .in(State_in[95:88]),   .out(State_out[95:88]));
InvSubBytes SubBytes5  (.clk(CLK), .in(State_in[87:80]),   .out(State_out[87:80]));
InvSubBytes SubBytes6  (.clk(CLK), .in(State_in[79:72]),   .out(State_out[79:72]));
InvSubBytes SubBytes7  (.clk(CLK), .in(State_in[71:64]),   .out(State_out[71:64]));
InvSubBytes SubBytes8  (.clk(CLK), .in(State_in[63:56]),   .out(State_out[63:56]));
InvSubBytes SubBytes9  (.clk(CLK), .in(State_in[55:48]),   .out(State_out[55:48]));
InvSubBytes SubBytes10 (.clk(CLK), .in(State_in[47:40]),   .out(State_out[47:40]));
InvSubBytes SubBytes11 (.clk(CLK), .in(State_in[39:32]),   .out(State_out[39:32]));
InvSubBytes SubBytes12 (.clk(CLK), .in(State_in[31:24]),   .out(State_out[31:24]));
InvSubBytes SubBytes13 (.clk(CLK), .in(State_in[23:16]),   .out(State_out[23:16]));
InvSubBytes SubBytes14 (.clk(CLK), .in(State_in[15:8]),    .out(State_out[15:8]));
InvSubBytes SubBytes15 (.clk(CLK), .in(State_in[7:0]),     .out(State_out[7:0]));

endmodule






