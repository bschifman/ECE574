`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 09/03/2016 01:09:44 PM
// Design Name: 
// Module Name: SHR
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


module SHR(a, sh_amt, d);
    parameter DATAWIDTH = 0;
    input [DATAWIDTH-1:0] a, sh_amt;
    output reg [DATAWIDTH-1:0] d;
     
    always @ (a, sh_amt) begin
        d <= (a >> sh_amt);    
    end
endmodule
