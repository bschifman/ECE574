`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 09/03/2016 01:09:44 PM
// Design Name: 
// Module Name: REG
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


module REG (d, clk, rst, q);
    parameter DATAWIDTH = 16;
    input [DATAWIDTH-1:0] d;
    input clk, rst;
    output reg [DATAWIDTH-1:0] q;
    
    always @(posedge clk) begin
        if(rst)
            q <= 0;
        else 
            q <= d;
    end
endmodule
