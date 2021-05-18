`timescale 1ns/1ns

module Xor_Gate_TB;
	reg a,b;
	wire q;

	Xor_Gate UUT (a,b, q);

initial begin
	$dumpfile("Xor_Gate.vcd");
	$dumpvars(1, Xor_Gate_TB);
	a= 0; b= 0;  #1
a= 0; b= 1;  #1
a= 1; b= 0;  #1
a= 1; b= 1;  #1


	$finish;
end
endmodule