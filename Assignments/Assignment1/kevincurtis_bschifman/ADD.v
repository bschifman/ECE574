`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 09/03/2016 01:09:44 PM
// Design Name: 
// Module Name: ADD
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


module ADD(a, b, sum);
    parameter DATAWIDTH = 8;
    input [DATAWIDTH-1:0] a, b;
    output reg [DATAWIDTH:0] sum;     
     
    always @ (a, b) begin
       sum <= a + b;
    end
endmodule
