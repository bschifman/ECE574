`timescale 1ns / 1ps
module SUB #(parameter DATAWIDTH = 8) (a, b, diff);
    input [DATAWIDTH - 1 : 0] a, b;
    output [DATAWIDTH - 1 : 0] diff;
    
        assign diff = a - b;
endmodule
