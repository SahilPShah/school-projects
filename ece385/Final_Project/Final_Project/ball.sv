//-------------------------------------------------------------------------
//    Ball.sv                                                            --
//    Viral Mehta                                                        --
//    Spring 2005                                                        --
//                                                                       --
//    Modified by Stephen Kempf 03-01-2006                               --
//                              03-12-2007                               --
//    Translated by Joe Meng    07-07-2013                               --
//    Modified by Po-Han Huang  12-08-2017                               --
//    Spring 2018 Distribution                                           --
//                                                                       --
//    For use with ECE 385 Lab 8                                         --
//    UIUC ECE Department                                                --
//-------------------------------------------------------------------------


module  ball ( input         Clk,                // 50 MHz clock
                             Reset,              // Active-high reset signal
					input [9:0]   DrawX, DrawY,       // Current pixel coordinates
					input [9:0]   Megaman_position_x,
					input logic [4:0] sprite_select,
					output logic [10:0] addr_x, addr_y,
					output logic print_background
					);
      
	 //right
	 logic [9:0] Megaman_facing_right_y =  10'd336;
	 logic [9:0] Megaman_running_right_y = 10'd336;

	 logic [9:0] Megaman_running_right_inter_y = 10'd336;
	 //left
	 logic [9:0] Megaman_facing_left_y = 10'd336;
	 logic [9:0] Megaman_running_left_y = 10'd336;
	 
	 logic [11:0] shape_size_x = 9'd60;
	 logic [11:0] shape_size_y  = 9'd60;
	 
	 logic [11:0] Background_x = 12'd640;
	 logic [11:0] Background_y = 12'd480;
	 
    always_comb 
		begin
		addr_x = DrawX;
		addr_y = DrawY;
		print_background = 1'b0;
		
		   if ((DrawX >= Megaman_position_x) && (DrawX < Megaman_position_x + shape_size_x) &&		//running facing right
						(DrawY >= Megaman_facing_right_y) && (DrawY < Megaman_facing_right_y + shape_size_y) && (sprite_select == 4'd0))
				begin
					addr_x = (DrawX - Megaman_position_x);
					addr_y = (DrawY - Megaman_facing_right_y);
				end
				
			else if ((DrawX >= Megaman_position_x) && (DrawX < Megaman_position_x + shape_size_x) &&		//facing left
						(DrawY >= Megaman_facing_left_y) && (DrawY < Megaman_facing_left_y + shape_size_y) && (sprite_select == 4'd1))
				begin
					//sprite_select = 4'd1;
					addr_x = (DrawX - Megaman_position_x);
					addr_y = (DrawY - Megaman_facing_left_y);
				end
				
			else if ((DrawX >= Megaman_position_x) && (DrawX < Megaman_position_x + shape_size_x) &&		//running right
						(DrawY >= Megaman_running_right_y) && (DrawY < Megaman_running_right_y + shape_size_y) && (sprite_select == 4'd2))
				begin
					//sprite_select = 4'd2;
					addr_x = (DrawX - Megaman_position_x);
					addr_y = (DrawY - Megaman_running_right_y);
				end
			else if ((DrawX >= Megaman_position_x) && (DrawX < Megaman_position_x + shape_size_x) &&		//running right intermediate
						(DrawY >= Megaman_running_right_inter_y) && (DrawY < Megaman_running_right_inter_y + shape_size_y) && (sprite_select == 4'd3))
				begin
					//sprite_select = 4'd3;
					addr_x = (DrawX - Megaman_position_x);
					addr_y = (DrawY - Megaman_running_right_inter_y);
				end
			else if ((DrawX >= Megaman_position_x) && (DrawX < Megaman_position_x + shape_size_x) &&		//running left 
						(DrawY >= Megaman_running_left_y) && (DrawY < Megaman_running_left_y + shape_size_y) && (sprite_select == 4'd4))
				begin
					//sprite_select = 4'd4;
					addr_x = (DrawX - Megaman_position_x);
					addr_y = (DrawY - Megaman_running_left_y);
				end
				
			else if ((DrawX >= Megaman_position_x) && (DrawX < Megaman_position_x + shape_size_x) &&		//running left intermediate
						(DrawY >= Megaman_running_left_y) && (DrawY < Megaman_running_left_y + shape_size_y) && (sprite_select == 4'd6))
				begin
					//sprite_select = 4'd6;
					addr_x = (DrawX - Megaman_position_x);
					addr_y = (DrawY - Megaman_running_left_y);
				end
			
			else
				print_background = 1'b1;

		end
    
endmodule
