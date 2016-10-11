`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 09/11/2016 04:08:44 PM
// Design Name: 
// Module Name: circuit1
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


module circuit1(a, b, c, clk, rst, z, x);
    input clk, rst;
    input [7:0] a, b, c;

    output [7:0] z;
    output [15:0] x;

    wire [7:0] d, e;
    wire [15:0] f, xwire;
    wire g;
    
    ADD     #(.DATAWIDTH(8))    ADD_0(a, b, d);
    ADD     #(.DATAWIDTH(8))    ADD_1(a, c, e);
    COMP    #(.DATAWIDTH(8))    COMP_0(.a(d), .b(e), .gt(g));
    MUX2x1  #(.DATAWIDTH(8))    MUX2x1_0(e, d, g, z);
    MUL     #(.DATAWIDTH (8))   MUL_0(a, c, f);
    SUB     #(.DATAWIDTH(16))   SUB_0(f, d, xwire);
    REG     #(.DATAWIDTH(16))   REG_0(xwire, clk, rst, x);
endmodule
