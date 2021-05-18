module T_Buffer(ctrl,in,out);
  input ctrl, in;
  output out;
  
  assign out = ctrl ? in:1'bz;
  
endmodule