module full_adder
(
input logic x,
input logic y,
input logic c_in,
output logic s,
output logic c_out
);

	assign s=x^y^c_in;
	assign c_out=(x&y)|(y&c_in)|(x&c_in);
	
endmodule
