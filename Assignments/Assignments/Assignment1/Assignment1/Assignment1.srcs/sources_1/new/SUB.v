`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 09/03/2016 01:09:44 PM
// Design Name: 
// Module Name: SUB2
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


module SUB(a, b, diff);
    parameter DATAWIDTH = 16;
    input [DATAWIDTH-1:0] a, b;
    output reg [DATAWIDTH-1:0] diff;
    
    always @ (a, b) begin
        diff <= a - b;
    end
endmodule
