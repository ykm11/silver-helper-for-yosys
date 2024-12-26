module dom1 (
    input clk,
    input [1:0] a, b,
    input refreshing,
    output [1:0] c
);
    wire c0_0, c1_0, c2_0, c3_0;
    wire c0_1, c1_1, c2_1, c3_1;

    assign c0_0 = (a[0] & b[0]);
    assign c1_0 = (a[0] & b[1]) ^ refreshing;

    assign c2_0 = (a[1] & b[0]) ^ refreshing;
    assign c3_0 = (a[1] & b[1]);

    DFF reg0(clk, c0_0, c0_1);
    DFF reg1(clk, c1_0, c1_1);
    DFF reg2(clk, c2_0, c2_1);
    DFF reg3(clk, c3_0, c3_1);

    assign c[0] = c0_1 ^ c1_1;
    assign c[1] = c2_1 ^ c3_1;

endmodule
