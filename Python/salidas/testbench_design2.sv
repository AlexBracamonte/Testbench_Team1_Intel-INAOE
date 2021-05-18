`timescale 1ns/1ns

module Basic_Gates_TB;
	reg a,b;
	wire NOT,OR,AND,NAND,XOR,XNOR;

	Basic_Gates UUT (a,b, NOT,OR,AND,NAND,XOR,XNOR);

initial begin
	$dumpfile("Basic_Gates.vcd");
	$dumpvars(1, Basic_Gates_TB);
	a= 0; b= 0;  #1
a= 0; b= 1;  #1
a= 1; b= 0;  #1
a= 1; b= 1;  #1


	$finish;
end
endmodule