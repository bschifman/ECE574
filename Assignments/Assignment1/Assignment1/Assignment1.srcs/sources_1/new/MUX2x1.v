`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 09/03/2016 01:09:44 PM
// Design Name: 
// Module Name: MUX2x1
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


module MUX2x1(a, b, sel, d);
    parameter DATAWIDTH = 8;
    input [DATAWIDTH-1:0] a, b;
    input sel;
    output reg [DATAWIDTH-1:0] d;
    
    always @ (sel) begin
        if(~sel)
            d <= a;
        else
            d <= b;
    end
endmodule
