`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 09/03/2016 01:09:44 PM
// Design Name: 
// Module Name: DIV
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


module DIV(a, b, quot);
    parameter DATAWIDTH = 0;
    input [DATAWIDTH-1:0] a, b;
    output reg [DATAWIDTH-1:0] quot;
     
    always @ (a, b) begin
        quot <= a/b;
    end
endmodule
