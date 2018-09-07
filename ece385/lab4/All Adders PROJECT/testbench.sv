module testbench();

timeunit 10ns;

timeprecision 1ns;

	logic[15:0]     A;
   logic[15:0]     B;
   logic[15:0]     Sum;
   logic           cout;
	logic Pg;
	logic Gg;
	
carry_lookahead_adder adder0(.*);

initial begin: TEST_VECTORS
	A = 16'h0003;
	B = 16'h0001;

#2 A = 16'hF3DA;
	B = 16'h13DA;

end

endmodule

	
