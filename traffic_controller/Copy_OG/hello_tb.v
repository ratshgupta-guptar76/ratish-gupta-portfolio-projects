`timescale 1ns / 1ns
// 2. Preprocessor Directives
`define DELAY 0.5
// 3. Include Statements
`include "hello.v"
module hello_tb;
// 4. Parameter definitions
parameter ENDTIME  = 40;
// 5. DUT Input regs
//integer count, count1, a;
reg clk;
reg rst_n;
reg sensor;
wire [2:0] light_farm;
// 6. DUT Output wires
wire [2:0] light_highway;

// fpga4student.com FPGA projects, VHDL projects, Verilog projects
// 7. DUT Instantiation
hello uut (
  .light_highway(light_highway),
  .light_farm(light_farm),
  .C(sensor),
  .clk(clk),
  .rst_n(rst_n)
);

// WaveForm Dump
initial
 begin
 $dumpfile("hello_tb.vcd");
 $dumpvars(0, hello_tb);
 end

// 8. Initial Conditions
initial
 begin
 clk = 1'b0;
 rst_n = 1'b0;
 sensor = 1'b0;
 // count = 0;
//// count1=0;
// a=0;
 end
// 9. Generating Test Vectors
initial
 begin
 main;
 end
task main;
 fork
 clock_gen;
 reset_gen;
 operation_flow;
 debug_output;
 endsimulation;
 join
endtask
task clock_gen;
 begin
 forever #`DELAY clk = !clk;
 end
endtask

task reset_gen;
 begin
//  rst_n = 0;
//  # 2
 rst_n = 1;
 end
endtask

// fpga4student.com FPGA projects, VHDL projects, Verilog projects
task operation_flow;
 begin
 sensor = 0;
//  # 60
//  sensor = 1;
//  # 120
//  sensor = 0;
//  # 120
//  sensor = 1;
 end
endtask
// 10. Debug output
task debug_output;
 begin
 $display("----------------------------------------------");
$display("------------------     -----------------------");
 $display("----------- SIMULATION RESULT ----------------");
 $display("--------------             -------------------");
 $display("----------------         ---------------------");
 $display("----------------------------------------------");
 $monitor("TIME = %d, reset = %b, sensor = %b, light of highway = %h, light of farm road = %h",$time,rst_n ,sensor,light_highway,light_farm );
 end
endtask

// fpga4student.com FPGA projects, VHDL projects, Verilog projects
//12. Determines the simulation limit
task endsimulation;
 begin
 #ENDTIME
 $display("-------------- THE SIMUALTION END ------------");
 $finish;
 end
endtask
    
endmodule
