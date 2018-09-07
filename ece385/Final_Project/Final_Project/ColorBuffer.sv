module ColorBuffer(input [3:0] data,
						 input [4:0] sprite_select_in, 
						 
						 output [4:0] sprite_select
						 );
						 
	always_comb
		begin
		sprite_select = sprite_select_in;
		
			if(data == 4'd0)
				begin	
					sprite_select = 4'b5;
				end
		end
endmodule
						 