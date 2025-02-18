/////////////////////////////////////////////////////////////////////////////////////////////
//
// Written by:     Ratish Gupta
// 
// Create Date:    December 24, 2025
// Design Name:    Traffic Light Intersection Controller
// Module Name:    intersection_controller
// Project Name:   Traffic Light System
// Target Devices: FPGA
// Tool Versions:  Xilinix Vivado 2020.2
// Description: 
//      This module implements a traffic light intersection controller for an intersection 
//      with two directions (North-South and East-West). Each direction is equipped 
//      with red, yellow, and green lights. The controller operates as a finite state 
//      machine (FSM) cycling through the following states:
//          1. NS_GREEN_ST:   North-South has green, East-West has red.
//          2. NS_YELLOW_ST:  North-South has yellow, East-West has red.
//          3. EW_GREEN_ST:   East-West has green, North-South has red.
//          4. EW_YELLOW_ST:  East-West has yellow, North-South has red.
// 
//      A counter is used to determine the duration of each state. The timing parameters 
//      are defined as parameters and should be adjusted to match the clock frequency 
//      and the required durations for each state.
// 
// Dependencies: None
// 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
`define ON 1'b1
`define OFF 1'b0

module intersection_controller (
    input clk,              // Global Clock Signal
    input rst,              // Synchronous Reset Signal (active HI)

    output reg NS_red,      // North-South direction RED light output register 
               NS_yellow,   // North-South direction YELLOW light output register
               NS_green,    // North-South direction GREEN light output register

    output reg EW_green,    // EAST-WEST direction GREEN light output register
               EW_yellow,   // EAST-WEST direction YELLOW light output register
               EW_red       // EAST-WEST direction RED light output register
);


    ////////////////////////////////////////////////////////////////////////////////
    // Clock Frequency and Timing Parameters (in milliseconds)
    ////////////////////////////////////////////////////////////////////////////////
    parameter CLK_FREQ      =   50_000_000;           // Example: 50MHz clock frequency

    parameter GREEN_RED_ms  =   200;                  // GREEN or RED state duration in ms (200 ms)
    parameter YELLOW_ms     =   40;                   // YELLOW state duration in ms (40 ms)

    parameter T_GREEN_RED   =     GREEN_RED_ms * CLK_FREQ / 1000;       // Duration for GREEN or RED state
    parameter T_YELLOW      =     YELLOW_ms * CLK_FREQ / 1000;          // Duration for YELLOW state

    ////////////////////////////////////////////////////////////////////////////////
    // FSM State Encoding using localparam for internal state
    ////////////////////////////////////////////////////////////////////////////////
    
    localparam NS_GREEN_ST = 2'b00,      // NORTH-SOUTH GREEN    |   |     EAST-WEST RED
              NS_YELLOW_ST = 2'b01,     // NORTH-SOUTH YELLOW   |   |     EAST-WEST RED
              EW_GREEN_ST = 2'b10,      // EAST-WEST GREEN      |   |   NORTH-SOUTH RED
              EW_YELLOW_ST = 2'b11;     // EAST-WEST YELLOW     |   |   NORTH-SOUTH RED    

    // State register to hold current state of the FSM
    reg [1:0] state;

    // Counter register for state timing duration and delay 
    reg [31:0] counter;

    /////////////////////////////////////////////////////////////////////////////////////////
    // Sequential Logic: FSM State Transition and Counter
    /////////////////////////////////////////////////////////////////////////////////////////

    always @(posedge clk) begin
        if (rst) begin
            state <= NS_GREEN_ST;           // Reset state to NORTH-SOUTH GREEN
            counter <= 0;
        end

        else begin
            counter <= counter + 1;         // Increment counter on every clock cycle

            case (state)
                NS_GREEN_ST: begin
                    // Transition when T_GREEN_RED cycles have elapsed
                    if (counter >=  T_GREEN_RED - 1) begin
                        state <= NS_YELLOW_ST;
                        counter <= 32'd0;
                    end
                end

                NS_YELLOW_ST: begin
                    // Transition when T_YELLOW cycles have elapsed
                    if (counter >= T_YELLOW - 1) begin
                        state <= EW_GREEN_ST;
                        counter <= 32'd0;
                    end
                end

                EW_GREEN_ST: begin
                    // Transition when T_GREEN_RED cycles have elapsed
                    if (counter >= T_GREEN_RED - 1) begin
                        state <= EW_YELLOW_ST;
                        counter <= 32'd0;
                    end
                end

                EW_YELLOW_ST: begin
                    // Transition when T_YELLOW cycles have elapsed
                    if (counter >= T_YELLOW - 1) begin
                        state <= NS_GREEN_ST;
                        counter <= 32'd0;
                    end
                end

                default: begin
                    // Default state to handle extra-ordinary cases
                    state <= NS_GREEN_ST;
                    counter <= 32'd0;
                end

            endcase
        end
    end

    //////////////////////////////////////////////////////////////////////////////
    // Combinational Logic: Traffic Light Output Signal Assignment
    //////////////////////////////////////////////////////////////////////////////

    always @(*) begin
        // Default all outputs to ZERO or Initial State (inactive state / OFF-State) when a change occures
        NS_red = OFF;
        NS_yellow = OFF;
        NS_green = OFF;

        EW_green = OFF;
        EW_yellow = OFF;
        EW_red = OFF;

        // Set outputs according to the current FSM state
        case (state) 
        
            NS_GREEN_ST: begin
                NS_green = ON;
                EW_red = ON;
            end

            NS_YELLOW_ST: begin
                NS_yellow = ON;
                EW_red = ON;
            end

            EW_GREEN_ST: begin
                NS_red = ON;
                EW_green = ON;
            end

            EW_YELLOW_ST: begin
                NS_red = ON;
                EW_yellow = ON;
            end

            default: begin
                // All outputs remain OFF
                NS_red = OFF;
                NS_yellow = OFF;
                NS_green = OFF;

                EW_green = OFF;
                EW_yellow = OFF;
                EW_red = OFF;
            end

        endcase
    end

endmodule


