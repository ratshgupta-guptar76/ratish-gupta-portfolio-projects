// adaptive_time_delay.v
module adaptive_time_delay #(
    parameter   CLK_FREQ          =   50_000_000,
    parameter   BASE_TIME_DELAY   =   200
)(
    input       clk,
    input       rst,

    input       ns_sensor,
    input       ew_sensor,

    output reg [31:0]   ns_green_delay,
    output reg [31:0]   ew_green_delay
);

    // Defining BASE ClOCK CYCLES for GREEN phase (200ms)
    parameter   BASE_GREEN_CYCLES   =   BASE_TIME_DELAY * CLK_FREQ / 1000;

    // Multiplication factor (1.5x) for Active Sensor input
    parameter   FACTOR_NUMER        =   3;
    parameter   FACTOR_DENOM        =   2;

    always @(posedge clk or posedge rst) begin

        if (rst) begin
            ns_green_delay  <=  BASE_GREEN_CYCLES;
            ew_green_delay  <=  BASE_GREEN_CYCLES;
        end
        else begin
            if (ns_sensor)
                ns_green_delay      =       BASE_GREEN_CYCLES * FACTOR_NUMER / FACTOR_DENOM;
            else
                ns_green_delay      =       BASE_GREEN_CYCLES;
        // ----
            if (ew_sensor)
                ew_green_delay      =       BASE_GREEN_CYCLES * FACTOR_NUMER / FACTOR_DENOM;
            else
                ew_green_delay      =       BASE_GREEN_CYCLES;
        end        

    end

endmodule
