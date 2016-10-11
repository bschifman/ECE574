`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 09/13/2016 02:27:12 PM
// Design Name: 
// Module Name: circuit4
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


module circuit4(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, fin, clk, rst);

    input [7:0] a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p;
    input clk, rst;
    
    output [31:0] fin;
    
    wire [31:0] t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14;
    
    ADD    #(.DATAWIDTH(8))   ADD_0(a, b, t1);
    ADD    #(.DATAWIDTH(32))   ADD_1(t1, c, t2);
    ADD    #(.DATAWIDTH(32))   ADD_2(t2, d, t3);
    ADD    #(.DATAWIDTH(32))   ADD_3(t3, e, t4);
    ADD    #(.DATAWIDTH(32))   ADD_4(t4, f, t5);
    ADD    #(.DATAWIDTH(32))   ADD_5(t5, g, t6);
    ADD    #(.DATAWIDTH(32))   ADD_6(t6, h, t7);
    ADD    #(.DATAWIDTH(32))   ADD_7(t7, i, t8);
    ADD    #(.DATAWIDTH(32))   ADD_8(t8, j, t9);
    ADD    #(.DATAWIDTH(32))   ADD_9(t9, l, t10);
    ADD    #(.DATAWIDTH(32))   ADD_10(t10, m, t11);
    ADD    #(.DATAWIDTH(32))   ADD_11(t11, n, t12);
    ADD    #(.DATAWIDTH(32))   ADD_12(t12, o, t13);
    ADD    #(.DATAWIDTH(32))   ADD_13(t13, p, t14);
    REG    #(.DATAWIDTH(32))   REG_1(t14, clk, rst, fin);

endmodule
