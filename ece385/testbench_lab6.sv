module testbench();

timeunit 10ns;	// Half clock cycle at 50 MHz
			// This is the amount of time represented by #1 
timeprecision 1ns;

// These signals are internal because the processor will be 
// instantiated as a submodule in testbench.
logic [15:0] S;
logic Clk, Reset_unsync, Run_unsync, Continue_unsync;
logic [11:0] LED;
logic [6:0] HEX0, HEX1, HEX2, HEX3, HEX4, HEX5, HEX6, HEX7; // idk 
logic CE, UB, LB, OE, WE;
logic [19:0] ADDR;
wire [15:0] Data;

// To store expected results
				
		
// Instantiating the DUT
// Make sure the module and signal names match with those in your design
lab6_toplevel lab6_top_level_0(.*);	

// Toggle the clock
// #1 means wait for a delay of 1 timeunit
always begin : CLOCK_GENERATION
#1 Clk = ~Clk;
end

initial begin: CLOCK_INITIALIZATION
    Clk = 0;
end 

// Testing begins here
// The initial block is not synthesizable
// Everything happens sequentially inside an initial block
// as in a software program
initial begin: TEST_VECTORS

//**********************

#2 Reset_unsync = 0;
	Continue_unsync = 1;
	S = 16'h0006;

#2 Reset_unsync = 1 ;

#2 Run_unsync = 1;

#2 Run_unsync = 0;

#20 Continue_unsync = 0;

#20 Continue_unsync = 1;

#20 Continue_unsync = 0;

#20 Continue_unsync = 1;

#20 Continue_unsync = 0;

#20 Continue_unsync = 1;

#20 Continue_unsync = 0;

#20 Continue_unsync = 1;

#20 Continue_unsync = 0;

#20 Continue_unsync = 1;

#20 Continue_unsync = 0;

#20 Continue_unsync = 1;

#20 Continue_unsync = 0;

#20 Continue_unsync = 1;

#20 Continue_unsync = 0;

#20 Continue_unsync = 1;





end
endmodule
