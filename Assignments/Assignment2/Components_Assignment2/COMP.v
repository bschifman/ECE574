`timescale 1ns / 1ps

module COMP #(parameter DATAWIDTH = 8)(a, b, gt, lt, eq);

    input [DATAWIDTH-1:0] a, b;
    output gt, lt, eq;
    
    assign eq = (a == b);
    assign lt = (a < b);
    assign gt = (a > b);
    
endmodule
