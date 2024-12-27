// Traffic Light Controller on FPGA to handle traffic lights on an intersection.
module traffic_controller(input clk,  // Clock
                    input rst_n,  // Active Low Reset
                    output reg[2:0] v_light,    // Vertical Light
                    output reg[2:0] h_light );  // Horizontal Light

    // Initialising the constants
parameter VG_HR = 2'b00, // Vertical Green, Horizontal Red
            VY_HR = 2'b01, // Vertical Yellow, Horizontal Red
            VR_HG = 2'b10, // Vertical Red, Horizontal Green
            VR_HY = 2'b11; // Vertical Red, Horizontal Yellow

parameter TIME_DELAY = 10; // 1 second delay for 10Hz clock

// State register
reg[1:0] state, next_state;

// Time counter
reg[27:0] counter; // 28-bit counter for 1 second delay (50MHz clock) -> log2(50,000,000) = 25.57
reg[7:0] delay; // 8-bit delay counter to handle the delay -> log2(10) = 3.32

// Delay Parameters
reg LIGHT_DELAY, V_YELLOW_DELAY, H_YELLOW_DELAY;

// Boolean Flags
reg V_YELLOW = 0, H_YELLOW = 0, GREEN = 0, RED = 0;

// Inputs
wire clk_;

always @(posedge clk or negedge rst_n)
begin
    if (~rst_n)
        state <= VG_HR;
    else
        state <= next_state;
end


// Finite State Machine
always @(*)
begin

    case (state)

        VG_HR: begin

            // Setting the lights/outputs
            v_light = 3'b100;
            h_light = 3'b001;

            // Setting the boolean flags
            GREEN = 1;
            RED = 0;
            V_YELLOW = 0;
            H_YELLOW = 0;

            // Setting the Next State
            if (LIGHT_DELAY)
                next_state = VY_HR;
            else
                next_state = VG_HR;

        end

        VY_HR: begin

            // Setting the lights/outputs
            v_light = 3'b010;
            h_light = 3'b001;

            // Setting the boolean flags
            GREEN = 0;
            RED  = 0;
            V_YELLOW = 1;
            H_YELLOW = 0;

            // Setting the Next State
            if (V_YELLOW_DELAY)
                next_state = VR_HG;
            else
                next_state = VY_HR;

        end

        VR_HG: begin

            // Setting the lights/outputs
            v_light = 3'b001;
            h_light = 3'b100;

            // Setting the boolean flags
            GREEN = 0;
            RED  = 1;
            V_YELLOW = 0;
            H_YELLOW = 0;

            // Setting the Next State
            if (LIGHT_DELAY)
                next_state = VR_HY;
            else
                next_state = VR_HG;
        end

        VR_HY: begin

            // Setting the lights/outputs
            v_light = 3'b001;
            h_light = 3'b010;

            // Setting the boolean flags
            GREEN = 0;
            RED  = 0;
            V_YELLOW = 0;
            H_YELLOW = 1;

            // Setting the Next State
            if (H_YELLOW_DELAY)
                next_state = VG_HR;
            else
                next_state = VR_HY;

        end

        default: begin
            next_state = VG_HR;
        end
    endcase
end

always @(posedge clk)
begin
    if (clk_ == 1)
    begin
        // If any light is on, i.e. reset not triggered, increment delay counter for every clock cycle (1 second)
        if (GREEN || RED || V_YELLOW || H_YELLOW)
            delay <= delay + 1;
        else
            delay <= 0; // Reset delay counter if no light is on

        // If the delay counter reaches threshold for Green/Red, set respective delay booleans
        if ( (GREEN || RED) && delay == 9)
        begin
            LIGHT_DELAY = 1;
            V_YELLOW_DELAY = 0;
            H_YELLOW_DELAY = 0;
            delay <= 0;
        end

        // If the delay counter reaches threshold for Vertical Yellow, set respective delay booleans
        else if (V_YELLOW && delay == 2)
        begin
            LIGHT_DELAY = 0;
            V_YELLOW_DELAY = 1;
            H_YELLOW_DELAY = 0;
            delay <= 0;
        end

        // If the delay counter reaches threshold for Horizontal Yellow, set respective delay booleans
        else if (H_YELLOW && delay == 2)
        begin
            LIGHT_DELAY = 0;
            V_YELLOW_DELAY = 0;
            H_YELLOW_DELAY = 1;
            delay <= 0;
        end
    
        else
        begin
            LIGHT_DELAY = 0;
            V_YELLOW_DELAY = 0;
            H_YELLOW_DELAY = 0;
        end
    end
end

always @(posedge clk)
begin

    counter <= counter + 1;

    if (counter == TIME_DELAY) // 1 second delay for 10MHz clock
        counter <= 0;
end

assign clk_ = (counter == TIME_DELAY) ? 1 : 0;

endmodule
