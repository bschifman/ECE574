`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 09/13/2016 02:20:39 PM
// Design Name: 
// Module Name: circuit3
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


module circuit3(a, b, c, d, e, f, g, h, sa, clk, rst, avg);

input [15:0] a, b, c, d, e, f, g, h;
input [7:0] sa;
input clk, rst;

output [15:0] avg;

wire [31:0] l00, l01, l02, l03, 
            l10, l11, l2, l2div2, 
            l2div4, l2div8;
            
ADD     #(.DATAWIDTH(16))   ADD_0(a, b, l00);
ADD     #(.DATAWIDTH(16))   ADD_1(c, d, l01);
ADD     #(.DATAWIDTH(16))   ADD_2(e, f, l02);
ADD     #(.DATAWIDTH(16))   ADD_3(g, h, l03);
ADD     #(.DATAWIDTH(32))   ADD_4(l00, l01, l10);
ADD     #(.DATAWIDTH(32))   ADD_5(l02, l03, l11);
ADD     #(.DATAWIDTH(32))   ADD_6(l10, l11, l2);
SHR     #(.DATAWIDTH(32))   SHR_0(l2, sa, l2div2);
SHR     #(.DATAWIDTH(32))   SHR_1(l2div2, sa, l2div4);
SHR     #(.DATAWIDTH(32))   SHR_2(l2div4, sa, l2div8);
REG     #(.DATAWIDTH(32))   REG_0(l2div8, clk, rst, avg);

//l00 = a + b
//l01 = c + d
//l02 = e + f
//l03 = g + h
//l10 = l00 + l01
//l11 = l02 + l03
//l2 = l10 + l11
//l2div2 = l2 >> sa
//l2div4 = l2div2 >> sa 
//l2div8 = l2div4 >> sa
//avg = l2div8

endmodule
