`timescale 1ns / 1ps
module MUX2x1 #(parameter DATAWIDTH = 8) (a, b, sel, d);
    input [DATAWIDTH - 1 : 0] a, b;
    input sel;
    output [DATAWIDTH - 1 : 0] d;
    
    assign d = sel ? a : b;
endmodule
