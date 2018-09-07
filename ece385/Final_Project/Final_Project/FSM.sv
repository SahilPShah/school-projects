module FSM (
				input logic [7:0] keycode,
				input logic frame_clk,
				input logic Clk, RESET,
				output logic [3:0] sprite_select,
				output logic [9:0] Megaman_x_position
			   );

	     // Detect rising edge of frame_clk
logic frame_clk_delayed, frame_clk_rising_edge;				
  always_ff @ (posedge Clk) begin
      frame_clk_delayed <= frame_clk;
      frame_clk_rising_edge <= (frame_clk == 1'b1) && (frame_clk_delayed == 1'b0);
  end

  logic [9:0] Megaman_motion_in, Megaman_motion, Megaman_x_position_in;
  logic [3:0] sprite_select_in, i, j, i_next, j_next, sprite_select_stop, sprite_select_stop_in;
  
  always_ff @ (posedge Clk) 
	begin
		if(RESET)
			begin
				Megaman_x_position <= 10'b0;
				Megaman_motion <= 10'b0;
				i = 1'b0;
				j = 1'b0;
			end
		else
			begin
				Megaman_motion <= Megaman_motion_in;
				Megaman_x_position <= Megaman_x_position_in;
				sprite_select <= sprite_select_in;
				sprite_select_stop <= sprite_select_stop_in;
				i <= i_next;
				j <= j_next;
			end
	end
		
		//NOTE the ground starts at around 392 pixels down form the top

  always_comb
  begin
  
  	Megaman_x_position_in = Megaman_x_position;
	Megaman_motion_in = Megaman_motion;
	sprite_select_in = sprite_select;											//default sprite selecting to background
	sprite_select_stop_in = sprite_select_stop;					
	i_next = i;
	j_next = j;
       
      // Update position and motion only at rising edge of frame clock
      if (frame_clk_rising_edge)
      begin			
			case(keycode[7:0])
//				8'h1A :															//Keypress of up arrow
//					begin
//	
//					end
//					
				8'h04 : 															//Keypress of left arrow
					begin
						Megaman_motion_in = (~(11'b01) + 1'b1); 		//Moving left
						sprite_select_stop_in = 4'd1;
						
						if (i[3] == 0)
							sprite_select_in = 4'd4; 						//Running left
						else 
							sprite_select_in = 4'd6; 						//Running left intermediate
						i_next = i + 1;
					end
					
				8'h07 :															//Keypress of right arrow
					begin
						Megaman_motion_in = 11'd01; 						//Moving right
						sprite_select_stop_in = 4'b0;
						
						if (j[3] == 0)
							sprite_select_in = 4'd2; 						//Running right
						else 
							sprite_select_in = 4'd3; 						//Running right intermediate
						j_next = j + 1;
					end
				
				default : 														//No key press, change sprite to face the direction of last movement and stop Megaman from moving
					begin
						sprite_select_in = sprite_select_stop;
						Megaman_motion_in = 10'b0;
					end
				

			endcase	
			
			if( (Megaman_x_position + 9'd60 >= 10'd639) && (keycode[7:0] == 8'h07) )  			// Megaman is at the Right edge, STOP
					begin
                 Megaman_motion_in = 10'd0;
					  sprite_select_in = sprite_select_stop;
					end
			else if( Megaman_x_position + 9'd10 <= 10'd1 && (keycode[7:0] == 8'h04)) 					// Megaman is at the Left edge, STOP //THIS DOES NOT WORK PROPERLY
					begin																							//**************************************************************
                 Megaman_motion_in = 10'd0;
					  sprite_select_in = sprite_select_stop;
					end
					
		
			Megaman_x_position_in = Megaman_x_position + Megaman_motion;  //changed "Megaman_x_position" from "Megaman_x_position_in"
		end
	end
  
endmodule
