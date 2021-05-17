odule Circuito1(a,b,c);
  input a, b, c;
  output out_1;

  assign out_1 = !(a & b) & !(a + b) & c;
  
endmodule