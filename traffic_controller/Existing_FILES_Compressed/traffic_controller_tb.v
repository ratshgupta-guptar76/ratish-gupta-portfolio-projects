`timescale 1ns/1ps
`include "traffic_controller.v"

`define CLOCK_DELAY 100 // Adjusted to 100ns for 5Hz clock
`define RESET_DELAY 50  // Adjusted to 50ns for reset duration

module traffic_controller_tb;
parameter ENDTIME = 1_000_000;  // Set the simulation time to 10s

// Testbench signals
reg clk;
reg rst_n;
wire [2:0] v_light;
wire [2:0] h_light;

// Instantiate the Unit Under Test (UUT)
traffic_controller uut(
    .clk(clk),
    .rst_n(rst_n),
    .v_light(v_light),
    .h_light(h_light)
);

// Frequency Generator
task CLOCK_GENERATOR;
begin
    forever
    begin
        #`CLOCK_DELAY clk = ~clk;  // Clock period 200ns for 5Hz clock
    end
end
endtask

// Reset Generator
task RESET_GENERATOR;
begin
    rst_n = 0;  // Active low reset
    #`RESET_DELAY;
    rst_n = 1;
    #`RESET_DELAY;  // Ensure reset is held for a sufficient duration
end
endtask

// Debug Output - Monitor signals
task DEBUG_OUTPUT;
begin
    $display("----------------------------------------------");
    $display("----------- SIMULATION RESULT ----------------");
    $display("----------------------------------------------");
    $monitor("Time = %d, Vertical Lights = %b, Horizontal Lights = %b, Reset = %b", 
             $time, v_light, h_light, rst_n);
end
endtask

// End Simulation
task END_SIM;
begin
    #ENDTIME;
    $display("----------- SIMULATION ENDED  ----------------");
    $finish;
end
endtask

// Main simulation flow
task main;
fork
    CLOCK_GENERATOR;
    RESET_GENERATOR;  // Ensure reset is generated
    DEBUG_OUTPUT;     // Ensure debug output is enabled
    END_SIM;
join
endtask

// GtkWave Simulation
initial begin
    $dumpfile("traffic_controller_tb.vcd");
    $dumpvars(0, traffic_controller_tb);
end

// Initial block
initial begin
    clk = 1'b0;
    rst_n = 1'b1;
end

initial begin
    main;
end

endmodule
