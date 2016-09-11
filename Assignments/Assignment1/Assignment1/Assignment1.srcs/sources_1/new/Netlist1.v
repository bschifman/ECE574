`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 09/11/2016 04:08:44 PM
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


module Netlist1(a, b, c, clk, rst, z, x);
    input clk, rst;
    input [7:0] a, b, c;

    output [7:0] z;
    output [15:0] x;

    wire [7:0] d, e;
    wire [15:0] f, g;
    wire [15:0] xwire;
    wire garbage;
    
    ADD2 ADD2_0(a, b, d);
    ADD2 ADD2_1(a, c, e);
    COMP2 COMP2_0(d, e, garbage, g, garbage);
    MUX2x1 MUX2x1_0(d, e, g, z);
    MUL2 MUL2_0(a, c, f);
    SUB2 SUB2_0(f, d, xwire);
    REG REG_0(xwire, clk, rst, x);
endmodule
