`timescale 1ns / 1ps
module MOD #(parameter DATAWIDTH = 8) (a, b, rem);//a is dividend, b is divisor
    input [DATAWIDTH - 1 : 0] a, b;
    output [DATAWIDTH - 1 : 0] rem;  

        assign rem = a%b;

 endmodule