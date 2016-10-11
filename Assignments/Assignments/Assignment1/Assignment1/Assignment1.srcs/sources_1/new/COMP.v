`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 09/03/2016 01:09:44 PM
// Design Name: 
// Module Name: COMP2
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


module COMP(a, b, gt, lt, eq);
    parameter DATAWIDTH = 2;
    input [DATAWIDTH-1:0] a, b;
    output reg gt, lt, eq;
    
    always @ (a, b) begin
        gt <= 0; lt <= 0; eq <= 0;
        if(a > b)
            gt <= 1;
        else if(a < b) 
            lt <= 1;
        else 
            eq <= 1;
    end
endmodule
