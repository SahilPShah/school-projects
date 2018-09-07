//-------------------------------------------------------------------------
//    Color_Mapper.sv                                                    --
//    Stephen Kempf                                                      --
//    3-1-06                                                             --
//                                                                       --
//    Modified by David Kesler  07-16-2008                               --
//    Translated by Joe Meng    07-07-2013                               --
//    Modified by Po-Han Huang  10-06-2017                               --
//                                                                       --
//    Fall 2017 Distribution                                             --
//                                                                       --
//    For use with ECE 385 Lab 8                                         --
//    University of Illinois ECE Department                              --
//-------------------------------------------------------------------------

// color_mapper: Decide which color to be output to VGA for each pixel.
module  color_mapper ( input logic [3:0] sprite_data,//   or background (computed in ball.sv)
                       input        [9:0] DrawX, DrawY,       // Current pixel coordinates
                       output logic [7:0] VGA_R, VGA_G, VGA_B // VGA RGB output
                     );
    
    logic [7:0] Red, Green, Blue;    
    // Output colors to VGA
    assign VGA_R = Red;
    assign VGA_G = Green;
    assign VGA_B = Blue;
    
    // Assign color based on is_ball signal
  always_comb
  begin
	 // Background with nice color gradient
    Red = 8'h40; 
    Green = 8'hE0;
    Blue = 8'hD0;
				if(sprite_data == 4'd0)
					begin
						// Hot Pink													
						Red = 8'h40;                    //draw the background
						Green = 8'hE0;
						Blue = 8'hD0;
					end
				else if(sprite_data == 4'd1)
					begin
						// Black
						Red = 8'h16;
						Green = 8'h16;
						Blue = 8'h16;
					end
				else if(sprite_data == 4'd2)
					begin
						// White
						Red = 8'hF8;
						Green = 8'hF8;
						Blue = 8'hF8;
					end
				else if(sprite_data == 3'd3)
					begin
						// Tan
						Red = 8'hDD;
						Green = 8'hC0;
						Blue = 8'h6B;
					end
				else if(sprite_data == 4'd4)
					begin
						// Light Blue
						Red = 8'h00;
						Green = 8'hCF;
						Blue = 8'hDF;
					end
				else if(sprite_data == 4'd5)
					begin
						// Dark Blue
						Red = 8'h00;
						Green = 8'h69;
						Blue = 8'h9F;
					end
				else if(sprite_data == 4'd6)
					begin
						// Blue
						Red = 8'h01;
						Green = 8'h60;
						Blue = 8'hC5;
					end
				else if(sprite_data == 4'd7)
					begin
						// Red
						Red = 8'hA6;
						Green = 8'h18;
						Blue = 8'h14;
					end
				else if(sprite_data == 4'd8)
					begin
						// Turquoise
						Red = 8'h2B;
						Green = 8'hD9;
						Blue = 8'hCC;
					end
				else if(sprite_data == 4'd9)
					begin
						// Grey
						Red = 8'h94;
						Green = 8'h94;
						Blue = 8'h94;
					end
				else if(sprite_data == 4'd10)
					begin
						// Light Green
						Red = 8'h84;
						Green = 8'hF8;
						Blue = 8'hB8;
					end
  end 
    
endmodule
