`timescale 1ns / 1ps
module DIV #(parameter DATAWIDTH = 8) (a, b, quot);//a is dividend, b is divisor

    input [DATAWIDTH - 1 : 0] a, b;
    output [DATAWIDTH - 1 : 0] quot;  

        assign quot = a/b;

 endmodule

