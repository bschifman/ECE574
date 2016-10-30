`timescale 1ns / 1ps
module SHL #(parameter DATAWIDTH = 8) (a, sh_amt, d);
    input [DATAWIDTH - 1 : 0] a;
    input [DATAWIDTH - 1 : 0] sh_amt;
    output [DATAWIDTH - 1 : 0] d;

        assign d = a << sh_amt;
endmodule
