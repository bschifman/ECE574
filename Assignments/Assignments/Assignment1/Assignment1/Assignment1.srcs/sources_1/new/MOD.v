`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 09/03/2016 01:09:44 PM
// Design Name: 
// Module Name: MOD
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


module MOD(a, b, rem);
    parameter DATAWIDTH = 0;
    input [DATAWIDTH-1:0] a, b;
    output reg [DATAWIDTH-1:0] rem;
    
    always @ (a, b) begin
       rem <= a % b;
    end
endmodule
