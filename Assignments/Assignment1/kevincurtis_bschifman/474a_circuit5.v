`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 09/13/2016 02:34:18 PM
// Design Name: 
// Module Name: circuit5
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


module circuit5(a, b, c, clk, rst, z, x);

input [63:0] a, b, c;
input clk, rst;

output [31:0] z, x;

wire [63:0] d, e, f, g, h; 
wire dLTe, dEQe; 
wire [63:0] xrin, zrin;
wire [63:0] greg, hreg;


ADD     #(.DATAWIDTH(64))    ADD_0(a, b, d);
ADD     #(.DATAWIDTH(64))    ADD_1(a, c, e);
SUB     #(.DATAWIDTH(64))    SUB_0(a, b, f);
COMP    #(.DATAWIDTH(64))    COMP_0(.a(d), .b(e), .eq(dEQe));
COMP    #(.DATAWIDTH(64))    COMP_1(.a(d), .b(e), .lt(dLTe));
MUX2x1  #(.DATAWIDTH(64))    MUX2x1_0(d, e, dLTe, g);
MUX2x1  #(.DATAWIDTH(64))    MUX2x1_1(g, f, dEQe, h);
REG     #(.DATAWIDTH(64))    REG_0(g, rst, clk, greg);
REG     #(.DATAWIDTH(64))    REG_1(h, rst, clk, hreg);
SHL     #(.DATAWIDTH(64))    SHL_0(hreg, dLTe, xrin);
SHR     #(.DATAWIDTH(64))    SHR_0(greg, dEQe, zrin);
REG     #(.DATAWIDTH(64))    REG_2(xrin, rst, clk, x);
REG     #(.DATAWIDTH(64))    REG_3(zrin, rst, clk, z);
//d = a + b
//e = a + c
//f = a - b  
//dEQe = d == e
//dLTe = d < e
//g = dLTe ? d : e 
//h = dEQe ? g : f 
//greg = g
//hreg = h
//xrin = hreg << dLTe
//zrin = greg >> dEQe
//x = xrin
//z = zrin
endmodule
