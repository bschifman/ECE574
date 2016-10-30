`timescale 1ns / 1ps

module ADD #(parameter DATAWIDTH = 8) (a, b, sum);
    input [DATAWIDTH - 1 : 0] a, b;
    output [DATAWIDTH - 1 : 0] sum;

        assign sum = a + b;

endmodule
