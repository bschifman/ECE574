`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 09/03/2016 01:19:35 PM
// Design Name: 
// Module Name: MUL2
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

module MUL2(a, b, prod);
    parameter DATAWIDTH = 8;
    input [DATAWIDTH-1:0] a, b;
    output reg [2*DATAWIDTH-1:0] prod;
    
    always @ (a, b) begin
       prod <= a * b;
    end
endmodule
