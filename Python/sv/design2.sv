mmodule Basic_Gates(a,b,NOT,OR,AND,NAND,XOR,XNOR);
  input a,b;
  output NOT,OR,AND,NAND,XOR,XNOR;
  
  assign NOT= ~a;
  assign OR= a|b;
  assign AND= a&b;
  assign NAND= ~(a|b);
  assign XOR= ~(a^b);
  
endmodule