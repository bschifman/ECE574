`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 09/11/2016 04:01:04 PM
// Design Name: 
// Module Name: Netlist1
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


module Netlist1();

input Int8 a, b, c

output Int8 z
output Int16 x

wire Int8 d, e
wire Int16 f, g
wire Int16 xwire

d = a + b
e = a + c
g = d > e
z = g ? d : e
f = a * c
xwire = f - d  
x = xwire

endmodule
