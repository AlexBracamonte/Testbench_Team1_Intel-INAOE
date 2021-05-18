`timescale 1ns/1ns

module Circuito1_TB;
	reg a, b, c;
	wire out_1;

	Circuito1 UUT (a, b, c, out_1);

initial begin
	$dumpfile("Circuito1.vcd");
	$dumpvars(1, Circuito1_TB);
	a= 0;  b= 0;  c= 0;  #1
a= 0;  b= 0;  c= 1;  #1
a= 0;  b= 1;  c= 0;  #1
a= 0;  b= 1;  c= 1;  #1
a= 1;  b= 0;  c= 0;  #1
a= 1;  b= 0;  c= 1;  #1
a= 1;  b= 1;  c= 0;  #1
a= 1;  b= 1;  c= 1;  #1


	$finish;
end
endmodule