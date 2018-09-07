 module AddRoundKey(input logic [127:0] state_in, 
						 input logic [1407:0] curr_key,
						 input logic [4:0] Round,
						 output logic [127:0] state_out
						 );

	logic [1407:0] bit_start;

	always_comb
	begin
		unique case (Round)
			4'b0000 :
				state_out = curr_key[127:0] ^ state_in;
			4'b0001 :
				state_out = curr_key[255:128] ^ state_in;
			4'b0010 :
				state_out = curr_key[383:256] ^ state_in;
			4'b0011 :
				state_out = curr_key[511:384] ^ state_in;
			4'b0100 :
				state_out = curr_key[639:512] ^ state_in;
			4'b0101 :
				state_out = curr_key[767:640] ^ state_in;
			4'b0110 :
				state_out = curr_key[895:768] ^ state_in;
			4'b0111 :
				state_out = curr_key[1023:896] ^ state_in;
			4'b1000 :
				state_out = curr_key[1151:1024] ^ state_in;
			4'b1001 :
				state_out = curr_key[1279:1152] ^ state_in;
			4'b1010 :
				state_out = curr_key[1407:1280] ^ state_in;
		endcase
	end


endmodule
