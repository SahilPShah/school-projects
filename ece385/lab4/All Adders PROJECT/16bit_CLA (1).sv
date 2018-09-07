module CLA4x4(input [15:0] A,B, input cin, output [15:0] sum, output Pg, Gg, cout);
logic [3:0] s0,s1,s2,s3;
logic pg0,pg4,pg8,pg12,gg0,gg4,gg8,gg12,c0,c4,c8,c12; 

	assign c0 = cin;
	
	CLA_4bit CLA0(.a(A[3:0]), .b(B[3:0]), .cin(cin), .S(sum[3:0]), .Pg(pg0), .Gg(gg0));
	assign c4 = gg0|(c0&pg0);
	
	CLA_4bit CLA1(.a(A[7:4]), .b(B[7:4]), .cin(c4), .S(sum[7:4]), .Pg(pg4), .Gg(gg4));
	assign c8 = gg4|(gg0&pg4)|(c0&pg0&pg4);
	
	CLA_4bit CLA2(.a(A[11:8]), .b(B[11:8]), .cin(c8), .S(sum[11:8]), .Pg(pg8), .Gg(gg8));
	assign c12 = gg8|(gg4&pg8)|(gg0&pg8&pg4)|(c0&pg8&pg4&pg0);
	
	CLA_4bit CLA3(.a(A[15:12]), .b(B[15:12]), .cin(c12), .S(sum[15:12]), .Pg(pg12), .Gg(gg12));
	
	assign cout = gg12|(pg12&c12);
	assign Pg = pg0&pg4&pg8&pg12;
	assign Gg = gg12|(gg8&pg12)|(gg4&pg12&pg8)|(gg0&pg12&pg8&pg4);

endmodule
