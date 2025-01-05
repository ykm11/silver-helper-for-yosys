module dom2 (
    input clk,
    input [1:0] is0,
    input [1:0] is1,
    input [1:0] is2,
    input [2:0] refreshing,
    output [2:0] os
);

    wire c0_0, c1_0, c2_0, c3_0, c4_0, c5_0, c6_0, c7_0, c8_0;
    wire c0_1, c1_1, c2_1, c3_1, c4_1, c5_1, c6_1, c7_1, c8_1;
/*
    is0[0] = a0;
    is1[0] = a1;
    is2[0] = a2;

    is0[1] = b0;
    is1[1] = b1;
    is2[1] = b2;
*/

    assign c0_0 = (is0[0] & is0[1]);                 // a0 & b0
    assign c1_0 = (is0[0] & is1[1]) ^ refreshing[1]; // a0 & b1 ^ r1
    assign c2_0 = (is0[0] & is2[1]) ^ refreshing[2]; // a0 & b2 ^ r2

    assign c3_0 = (is1[0] & is0[1]) ^ refreshing[0]; // a1 & b0 ^ r0
    assign c4_0 = (is1[0] & is1[1]);                 // a1 & b1 
    assign c5_0 = (is1[0] & is2[1]) ^ refreshing[2]; // a1 & b0 ^ r2

    assign c6_0 = (is2[0] & is0[1]) ^ refreshing[0]; // a2 & b0 ^ r0
    assign c7_0 = (is2[0] & is1[1]) ^ refreshing[1]; // a2 & b1 ^ r1 
    assign c8_0 = (is2[0] & is2[1]);                 // a2 & b0

    DFF reg0 (clk, c0_0, c0_1);
    DFF reg1 (clk, c1_0, c1_1);
    DFF reg2 (clk, c2_0, c2_1);
    DFF reg3 (clk, c3_0, c3_1);
    DFF reg4 (clk, c4_0, c4_1);
    DFF reg5 (clk, c5_0, c5_1);
    DFF reg6 (clk, c6_0, c6_1);
    DFF reg7 (clk, c7_0, c7_1);
    DFF reg8 (clk, c8_0, c8_1);

    assign os[0] = c0_1 ^ c1_1 ^ c2_1;
    assign os[1] = c3_1 ^ c4_1 ^ c5_1;
    assign os[2] = c6_1 ^ c7_1 ^ c8_1;

endmodule
