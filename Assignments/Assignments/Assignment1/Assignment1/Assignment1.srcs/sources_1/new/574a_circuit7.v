`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 09/13/2016 03:27:54 PM
// Design Name: 
// Module Name: circuit7
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


module circuit7(a,b, c, d, zero, rst, clk, z);

input [63:0] a, b, c, d, zero;
input rst, clk;

output [63:0] z;

wire [63:0] e, f, g, zwire;
wire gEQz;

DIV     #(.DATAWIDTH(64))   DIV_0(a, b, e);
DIV     #(.DATAWIDTH(64))   DIV_1(c, d, f);
MOD     #(.DATAWIDTH(64))   MOD_0(a, b, g);
COMP    #(.DATAWIDTH(64))   COMP_0(.a(g), .b(zero), .eq(gEQz));
MUX2x1  #(.DATAWIDTH(64))   MUX2x1_0(e, f, gEQz, zwire);
REG     #(.DATAWIDTH(64))   REG_0(zwire, rst, clk, z);

//e = a / b
//f = c / d
//g = a % b  
//gEQz = g == zero
//zwire = gEQz ? e : f 
//z = zwire
endmodule
