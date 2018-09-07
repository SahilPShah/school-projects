module CSA_4bit
(
input logic  [3:0] a,
input logic  [3:0] b, 
output logic [3:0] Sum0,
output logic [3:0] Sum1,
output logic       c_out0,
output logic       c_out1
);

logic c1,c2,c3,c4,c5,c6;

			full_adder FA0(.x(a[0]), .y(b[0]), .c_in(1'b0), .s(Sum0[0]), .c_out(c1));
			full_adder FA1(.x(a[1]), .y(b[1]), .c_in(c1), .s(Sum0[1]), .c_out(c2));
			full_adder FA2(.x(a[2]), .y(b[2]), .c_in(c2), .s(Sum0[2]), .c_out(c3));
			full_adder FA3(.x(a[3]), .y(b[3]), .c_in(c3), .s(Sum0[3]), .c_out(c_out0));
		
			full_adder FA4(.x(a[0]), .y(b[0]), .c_in(1'b1), .s(Sum1[0]), .c_out(c4));
			full_adder FA5(.x(a[1]), .y(b[1]), .c_in(c4), .s(Sum1[1]), .c_out(c5));
			full_adder FA6(.x(a[2]), .y(b[2]), .c_in(c5), .s(Sum1[2]), .c_out(c6));
			full_adder FA7(.x(a[3]), .y(b[3]), .c_in(c6), .s(Sum1[3]), .c_out(c_out1));

endmodule


















