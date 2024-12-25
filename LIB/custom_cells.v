
module BUF(A, Y);
input A;
output Y;
assign Y = A;
endmodule

module NOT(A, Y);
input A;
output Y;
assign Y = ~A;
endmodule

module AND(A, B, Y);
input A, B;
output Y;
assign Y = (A & B);
endmodule

module XOR(A, B, Y);
input A, B;
output Y;
assign Y = (A ^ B);
endmodule

module DFF(C, D, Q);
input C, D;
output reg Q;
always @(posedge C)
	Q <= D;
endmodule

module RDFF(C, R, D, Q);
input C, R, D;
output reg Q;
always @(posedge C, posedge R)
    if (R == 1) begin
	    Q <= 0;
    end else begin
	    Q <= D;
    end
endmodule


