`timescale 1ns / 1ps
module DEC #(parameter DATAWIDTH = 8) (a,d);
    input [DATAWIDTH - 1 : 0] a;
    output [DATAWIDTH - 1 : 0] d;

        assign d = a - 1;

endmodule
