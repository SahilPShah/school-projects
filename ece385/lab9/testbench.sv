module testbench();

timeunit 10ns;

timeprecision 1ns;

logic        CLOCK_50
logic [1:0]  KEY,
logic [7:0]  LEDG,
logic [17:0] LEDR,
logic [6:0]  HEX0,
logic [6:0]  HEX1,
logic [6:0]  HEX2,
logic [6:0]  HEX3
logic [6:0]  HEX4,
logic [6:0]  HEX5,
logic [6:0]  HEX6,
logic [6:0]  HEX7, 
logic [12:0] DRAM_ADDR,
logic [1:0]  DRAM_BA,
logic        DRAM_CAS_N,
logic        DRAM_CKE,
logic        DRAM_CS_N,
logic [31:0] DRAM_DQ,
logic [3:0]  DRAM_DQM,
logic        DRAM_RAS_N,
logic        DRAM_WE_N,
logic        DRAM_CLK

lab9_top lab9_top_level(.*);

// Toggle the clock
// #1 means wait for a delay of 1 timeunit
always begin : CLOCK_GENERATION
#1 Clk = ~Clk;
end

initial begin: CLOCK_INITIALIZATION
    Clk = 0;
end 

// Testing begins here
// The initial block is not synthesizable
// Everything happens sequentially inside an initial block
// as in a software program
initial begin: TEST_VECTORS
//******************************

//need to pass the 128 bit encrypted message
logic [127:0] msg_encrypt = "daec3055df058e1c39e814ea76f6747e";
logic [127:0] key = "000102030405060708090a0b0c0d0e0f";
