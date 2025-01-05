module dom1 (
    input clk,
    input [1:0] is0,
    input [1:0] is1,
    input refreshing,
    output [1:0] os
);

    wire c0_0, c1_0, c2_0, c3_0;
    wire c0_1, c1_1, c2_1, c3_1;
/*
    is0[0] = a0;
    is1[0] = a1;

    is0[1] = b0;
    is1[1] = b1;
*/

    assign c0_0 = is0[0] & is0[1]; // a0 & b0
    assign c1_0 = (is0[0] & is1[1]) ^ refreshing; // a0 & b1 ^ r

    assign c2_0 = (is1[0] & is0[1]) ^ refreshing; // a1 & b0 ^ r
    assign c3_0 = is1[0] & is1[1]; // a1 & b1

    DFF reg0 (clk, c0_0, c0_1);
    DFF reg1 (clk, c1_0, c1_1);
    DFF reg2 (clk, c2_0, c2_1);
    DFF reg3 (clk, c3_0, c3_1);

    assign os[0] = c0_1 ^ c1_1;
    assign os[1] = c2_1 ^ c3_1;

endmodule
