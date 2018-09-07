module routing_unit(input A_,B_,func, input [1:0] R, output A,B);
always_comb
begin
	case(R)
		2'b00 : A=A_;
		2'b01 : A=A_;
		2'b10 : A=func;
		default : A=B_;
					
	endcase
	case(R)
		2'b00 : B=B_;
		2'b01 : B=func;
		2'b10 : B=B_;
		default : B=A_;
	endcase
end 
endmodule
					 