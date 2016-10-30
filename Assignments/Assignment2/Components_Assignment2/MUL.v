`timescale 1ns / 1ps
module MUL #(parameter DATAWIDTH = 8) (a, b, prod);
    input [DATAWIDTH - 1 : 0] a, b;
    output [DATAWIDTH - 1 : 0] prod;

        assign prod = a*b;

endmodule
