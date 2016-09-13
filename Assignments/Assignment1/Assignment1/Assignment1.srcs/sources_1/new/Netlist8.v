`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 09/13/2016 03:24:40 PM
// Design Name: 
// Module Name: Netlist8
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


module Netlist8(a, b, c, zero, clk, rst, z);

    input [63:0] a, b, c, zero;
    input clk, rst;
    
    output [63:0] z;
    
    wire [63:0] e, f, g, zwire;
    wire  gEQz;
    
    DEC     #(.DATAWIDTH(64))   DEC_0(a, e);
    INC     #(.DATAWIDTH(64))   INC_0(c, f);
    MOD     #(.DATAWIDTH(64))   MOD_0(a, c, g);
    COMP    #(.DATAWIDTH(64))   COMP_0(.a(g), .b(zero), .eq(gEQz));
    MUX2x1  #(.DATAWIDTH(64))   MUX2x1_0(f, e, gEQz, zwire);
    REG     #(.DATAWIDTH(64))   REG_0(zwire, clk, rst, z);


endmodule
