/*
 * ECE385-HelperTools/PNG-To-Txt
 * Author: Rishi Thakkar
 *
 */

module  ROM
(
		input [11:0] read_address,
		input Clk,

		output logic [3:0] data_Out
);

// mem has width of 3 bits and a total of 3600 addresses
logic [3:0] mem [0:3599];

initial
begin
	 $readmemh("E:/ECE_385/Final_Project/Final_Project/PNG_HEX_SPRITES/Megaman_facing_right.txt", mem);
end


always_ff @ (posedge Clk)
	begin
		data_Out<= mem[read_address];
	end

endmodule
