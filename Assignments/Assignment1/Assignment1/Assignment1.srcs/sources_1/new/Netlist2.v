`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 09/13/2016 01:55:37 PM
// Design Name: 
// Module Name: Netlist2
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


module Netlist2(a, b, c, z, x, clk, rst);


    input [31:0] a, b, c;
    input clk, rst;
    
    output [31:0] z, x;
    
    wire [31:0] d, e, f, g, h;
    wire dLTe, dEQe  ;
    wire [31:0] zwire, xwire;
    
    ADD     #(.DATAWIDTH(32))   ADD_0(a, b, d);
    ADD     #(.DATAWIDTH(32))   ADD_1(a, c, e);
    SUB     #(.DATAWIDTH(32))   SUB_0(a, c, e);
    COMP    #(.DATAWIDTH(32))   COMP_0(.a(d), .b(e), .eq(dEQe), .lt(dLTe));
    MUX2x1  #(.DATAWIDTH(32))   MUX2x1_0(e, d, dLTe, g);
    MUX2x1  #(.DATAWIDTH(32))   MUX2x1_1(f, g, dEQe, h);
    SHL     #(.DATAWIDTH(32))   SHL_0(g, dLTe, xwire);
    SHR     #(.DATAWIDTH(32))   SHR_0(h, dEQe, zwire);
    REG     #(.DATAWIDTH(32))   REG_0(xwire, clk, rst, x);
    REG     #(.DATAWIDTH(32))   REG_1(zwire, clk, rst, z);


endmodule
