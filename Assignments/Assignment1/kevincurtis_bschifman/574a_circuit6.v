`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 09/13/2016 02:57:10 PM
// Design Name: 
// Module Name: circuit6
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


module circuit6(a, b, c, d, e, f, g, h, num, clk, rst, avg);

    input [15:0] a, b, c, d, e, f, g, h, num;
    input clk, rst;
    
    output [15:0] avg;
    
    wire [15:0] r1, r2, r3, r4, r5, r6, r7 ;    /*was registers, think we just want them as wires though*/
    
    wire [15:0] avgwire;
    wire [31:0] t1, t2, t3, t4, t5, t6, t7;
    
    ADD     #(.DATAWIDTH(32))   ADD_0(a, b, t1);
    REG     #(.DATAWIDTH(32))   REG_0(t1, clk, rst, r1);
    ADD     #(.DATAWIDTH(32))   ADD_1(r1, c, t2);
    REG     #(.DATAWIDTH(32))   REG_1(t2, clk, rst, r2);
    ADD     #(.DATAWIDTH(32))   ADD_2(r2, d, t3);
    REG     #(.DATAWIDTH(32))   REG_2(t3, clk, rst, r3);
    ADD     #(.DATAWIDTH(32))   ADD_3(r3, e, t4);
    REG     #(.DATAWIDTH(32))   REG_3(t4, clk, rst, r4);
    ADD     #(.DATAWIDTH(32))   ADD_4(r4, f, t5);
    REG     #(.DATAWIDTH(32))   REG_4(t5, clk, rst, r5);
    ADD     #(.DATAWIDTH(32))   ADD_5(r5, g, t6);
    REG     #(.DATAWIDTH(32))   REG_5(t6, clk, rst, r6);
    ADD     #(.DATAWIDTH(32))   ADD_6(r6, h, t7);
    REG     #(.DATAWIDTH(32))   REG_6(t7, clk, rst, r7);
    DIV     #(.DATAWIDTH(32))   DIV_0(r7, num, avgwire);
    REG     #(.DATAWIDTH(32))   REG_7(avgwire, clk, rst, avg);



endmodule
