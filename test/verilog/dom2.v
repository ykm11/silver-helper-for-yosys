module dom2 (
    input clk,
    input [2:0] a, b,
    input [2:0] refreshing,
    output [2:0] c
);
    wire c0_0, c1_0, c2_0, c3_0, c4_0, c5_0, c6_0, c7_0, c8_0;
    wire c0_1, c1_1, c2_1, c3_1, c4_1, c5_1, c6_1, c7_1, c8_1;

    assign c0_0 = (a[0] & b[0]);
    assign c1_0 = (a[0] & b[1]) ^ refreshing[1];
    assign c2_0 = (a[0] & b[2]) ^ refreshing[2];

    assign c3_0 = (a[1] & b[0]) ^ refreshing[0];
    assign c4_0 = (a[1] & b[1]);
    assign c5_0 = (a[1] & b[2]) ^ refreshing[2];

    assign c6_0 = (a[2] & b[0]) ^ refreshing[0];
    assign c7_0 = (a[2] & b[1]) ^ refreshing[1];
    assign c8_0 = (a[2] & b[2]);

    DFF reg0(clk, c0_0, c0_1);
    DFF reg1(clk, c1_0, c1_1);
    DFF reg2(clk, c2_0, c2_1);
    DFF reg3(clk, c3_0, c3_1);
    DFF reg4(clk, c4_0, c4_1);
    DFF reg5(clk, c5_0, c5_1);
    DFF reg6(clk, c6_0, c6_1);
    DFF reg7(clk, c7_0, c7_1);
    DFF reg8(clk, c8_0, c8_1);

    assign c[0] = c0_1 ^ c1_1 ^ c2_1;
    assign c[1] = c3_1 ^ c4_1 ^ c5_1;
    assign c[2] = c6_1 ^ c7_1 ^ c8_1;

endmodule
