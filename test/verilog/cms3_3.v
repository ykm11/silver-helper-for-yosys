module cms3 (
    input clk,
    input [3:0] a, b,
    input [15:0] refreshing,
    output [3:0] c
);
    wire c0_0, c1_0, c2_0,  c3_0,  c4_0,  c5_0,  c6_0,  c7_0;
    wire c8_0, c9_0, c10_0, c11_0, c12_0, c13_0, c14_0, c15_0;
    wire c0_1, c1_1, c2_1,  c3_1,  c4_1,  c5_1,  c6_1,  c7_1;
    wire c8_1, c9_1, c10_1, c11_1, c12_1, c13_1, c14_1, c15_1;

    assign c0_0  = (a[0] & b[0]) ^ refreshing[0]  ^ refreshing[1];
    assign c1_0  = (a[0] & b[1]) ^ refreshing[1]  ^ refreshing[2];
    assign c2_0  = (a[0] & b[2]) ^ refreshing[2]  ^ refreshing[3];
    assign c3_0  = (a[0] & b[3]) ^ refreshing[3]  ^ refreshing[4];
    assign c4_0  = (a[1] & b[0]) ^ refreshing[4]  ^ refreshing[5];
    assign c5_0  = (a[1] & b[1]) ^ refreshing[5]  ^ refreshing[6];
    assign c6_0  = (a[1] & b[2]) ^ refreshing[6]  ^ refreshing[7];
    assign c7_0  = (a[1] & b[3]) ^ refreshing[7]  ^ refreshing[8];
    assign c8_0  = (a[2] & b[0]) ^ refreshing[8]  ^ refreshing[9];
    assign c9_0  = (a[2] & b[1]) ^ refreshing[9]  ^ refreshing[10];
    assign c10_0 = (a[2] & b[2]) ^ refreshing[10] ^ refreshing[11];
    assign c11_0 = (a[2] & b[3]) ^ refreshing[11] ^ refreshing[12];
    assign c12_0 = (a[3] & b[0]) ^ refreshing[12] ^ refreshing[13];
    assign c13_0 = (a[3] & b[1]) ^ refreshing[13] ^ refreshing[14];
    assign c14_0 = (a[3] & b[2]) ^ refreshing[14] ^ refreshing[15];
    assign c15_0 = (a[3] & b[3]) ^ refreshing[15] ^ refreshing[0];

    DFF reg0(clk, c0_0, c0_1);
    DFF reg1(clk, c1_0, c1_1);
    DFF reg2(clk, c2_0, c2_1);
    DFF reg3(clk, c3_0, c3_1);
    DFF reg4(clk, c4_0, c4_1);
    DFF reg5(clk, c5_0, c5_1);
    DFF reg6(clk, c6_0, c6_1);
    DFF reg7(clk, c7_0, c7_1);
    DFF reg8(clk, c8_0, c8_1);
    DFF reg9(clk, c9_0, c9_1);
    DFF reg10(clk, c10_0, c10_1);
    DFF reg11(clk, c11_0, c11_1);
    DFF reg12(clk, c12_0, c12_1);
    DFF reg13(clk, c13_0, c13_1);
    DFF reg14(clk, c14_0, c14_1);
    DFF reg15(clk, c15_0, c15_1);

    assign c[0] = c0_1 ^ c4_1 ^ c8_1 ^ c12_1;
    assign c[1] = c1_1 ^ c5_1 ^ c9_1 ^ c13_1;
    assign c[2] = c2_1 ^ c6_1 ^ c10_1 ^ c14_1;
    assign c[3] = c3_1 ^ c7_1 ^ c11_1 ^ c15_1;

endmodule
