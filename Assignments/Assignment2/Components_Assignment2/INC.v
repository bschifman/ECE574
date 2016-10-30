`timescale 1ns / 1ps
module INC #(parameter DATAWIDTH = 32) (a, d);
    input [DATAWIDTH - 1 : 0] a;
    output [DATAWIDTH - 1 : 0] d;

        assign d = a + 1;

endmodule
