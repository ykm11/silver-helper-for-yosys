module uma2(
    input clk,
    input [1:0] is0,
    input [1:0] is1,
    input [1:0] is2,
    input [1:0] refreshing,
    output os0,
    output os1,
    output os2
);

    wire c0_0, c1_0, c2_0, c3_0, c4_0, c5_0, c6_0, c7_0, c8_0;
    wire r;
    
/*
    is0[0] = a0;
    is1[0] = a1;
    is2[0] = a2;

    is0[1] = b0;
    is1[1] = b1;
    is2[1] = b2;
*/

    assign c0_0 = (is0[0] & is0[1]); // a0 & b0
    assign c1_0 = (is0[0] & is1[1]); // a0 & b1
    assign c2_0 = (is0[0] & is2[1]); // a0 & b2

    assign c3_0 = (is1[0] & is0[1]); // a1 & b0
    assign c4_0 = (is1[0] & is1[1]); // a1 & b1 
    assign c5_0 = (is1[0] & is2[1]); // a1 & b2

    assign c6_0 = (is2[0] & is0[1]); // a2 & b0
    assign c7_0 = (is2[0] & is1[1]); // a2 & b1
    assign c8_0 = (is2[0] & is2[1]); // a2 & b2

    assign r = refreshing[0] ^ refreshing[1];
    wire r0_1, r1_1, r_1;

    DFF r0_ff_1 (clk, refreshing[0], r0_1);
    DFF r1_ff_1 (clk, refreshing[1], r1_1);
    DFF r_ff_1 (clk, r, r_1);

    wire s00, s01, s02;
    wire s00_1, s01_1, s02_1;

    assign s00 = r0_1 ^ c1_0;
    assign s01 = r1_1 ^ c5_0;
    assign s02 = r_1  ^ c6_0;
    DFF s00_ff_1 (clk, s00, s00_1);
    DFF s01_ff_1 (clk, s01, s01_1);
    DFF s02_ff_1 (clk, s02, s02_1);

    wire s10, s11, s12;
    assign s10 = s00_1 ^ c3_0;
    assign s11 = s01_1 ^ c7_0;
    assign s12 = s02_1 ^ c2_0;

    wire s10_1, s11_1, s12_1;
    DFF s10_ff_1 (clk, s10, s10_1);
    DFF s11_ff_1 (clk, s11, s11_1);
    DFF s12_ff_1 (clk, s12, s12_1);

    wire s20, s21, s22;
    assign s20 = s10_1 ^ c0_0;
    assign s21 = s11_1 ^ c4_0;
    assign s22 = s12_1 ^ c8_0;

    DFF s20_ff_1 (clk, s20, os0);
    DFF s21_ff_1 (clk, s21, os1);
    DFF s22_ff_1 (clk, s22, os2);

endmodule
