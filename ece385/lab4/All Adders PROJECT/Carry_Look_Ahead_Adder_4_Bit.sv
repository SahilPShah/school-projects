module CLA_4bit
(
input logic  [3:0] a,
input logic  [3:0] b, 
input logic        cin, 
output logic [3:0] S,
output logic       Pg, 
output logic       Gg
);

logic p0,p1,p2,p3;
logic g0,g1,g2,g3;
logic s0,s1,s2,s3;
logic c0,c1,c2,c3;

	always_comb
	begin

		p0=a[0]^b[0];	
		p1=a[1]^b[1];
		p2=a[2]^b[2];
		p3=a[3]^b[3];

		Pg=p0&p1&p2&p3;

		g0=a[0]&b[0];
		g1=a[1]&b[1];
		g2=a[2]&b[2];
		g3=a[3]&b[3];
	
		Gg= g3|(g2&p3)|(g1&p3&p2)|(g0&p3&p2&p1);

		c0=cin;
		c1=(cin&p0)|g0;
		c2=(cin&p0&p1)|(g0&p1)|g1;
		c3=(cin&p0&p1&p2)|(g0&p1&p2)|(g1&p2)|g2; 
 
		S[0] = p0^c0;
		S[1] = p1^c1;
		S[2] = p2^c2;
		S[3] = p3^c3;
	end

endmodule
