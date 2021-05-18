`timescale 1ns/1ns

module T_Buffer_TB;
	reg ctrl, in;
	wire out;

	T_Buffer UUT (ctrl, in, out);

initial begin
	$dumpfile("T_Buffer.vcd");
	$dumpvars(1, T_Buffer_TB);
	ctrl= 0;  in= 0;  #1
ctrl= 0;  in= 1;  #1
ctrl= 1;  in= 0;  #1
ctrl= 1;  in= 1;  #1


	$finish;
end
endmodule