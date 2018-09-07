module register_unit#(N=8)(input [N-1:0] Din, input clk, A, B, Q Shift_EN, 
							LOADA,LOADB,output logic [N-1:0] DoutA,DoutB);
logic loadA = 0;
logic shift_ENA = 0;
logic loadB = 0;
logic shift_ENB = 0;

always_ff @(posedge clk)
begin
	//combinational logic for Shift_EN, Q, LoadA, LoadB is produced in the processor module
	//load 
	if(LOADA==1 && Q==0)
		loadA <= 1;
		shift_ENA <= 0;
		
	if(LOADB==1 && Q==0)
		loadB <= 1;
		shift_ENB <= 0;
	
	if(Q && Shift_EN)
		loadA <= 0;
		loadB <= 0;
		shift_ENA <= 1;
		shift_ENB <= 1;
		
	shiftReg#(N=8) regA(.clk(clk), .Shift_EN(shift_ENA), .Shift_IN(A), .LOAD(loadA), .Din(Din));
	shiftReg#(N=8) regB(.clk(clk), .Shift_EN(shift_ENB), .Shift_IN(B), .LOAD(loadB), .Din(Din)); 
end
endmodule
