module shiftReg#(N=8)(input clk,Shift_EN, Shift_IN, LOAD, input [N-1:0] Din,
							 output logic [N-1:0] Dout, output logic Shift_Out);

always_ff @ (posedge Clk)
begin
	if(Load)//if load ==1, put Din into Dout (i.e load the registers)
		Dout<=Din;
	else if(Shift_En)//if shift_en ==1 shift the bits right 
		Dout<={Shift_IN,Dout[N-1:1]}
		
end
assign Shift_Out = Dout[0];//bit that was shifted out
endmodule
