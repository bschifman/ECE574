`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 09/06/2016 02:52:46 PM
// Design Name: 
// Module Name: Assignment1_tb
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////

module Assignment1_tb();

    parameter [6:0] DATAWIDTH = 2;
    reg [DATAWIDTH-1:0] a_tb, b_tb;
    wire [DATAWIDTH-1:0] d_tb;
    
    ADD2 ADD2_0(a_tb, b_tb, d_tb);
    initial begin
       #10 a_tb <= 2; b_tb <= 0;
       #10 b_tb <= 2;
    end    
endmodule
