module intersection_controller (
  input         clk,
  input         rst,
  // Traffic sensor inputs for adaptive timing
  input         ns_sensor,
  input         sensor_EW,
  // Pedestrian request buttons
  input         pd_button_ew,
  input         pd_button_ns,

  // Traffic light outputs for vehicles
  output        NS_red,
  output        NS_yellow,
  output        NS_green,
  output        EW_red,
  output        EW_yellow,
  output        EW_green,
  // Pedestrian crossing signals
  output        pd_FREE_NS,
  output        pd_Caution_NS,
  output        pd_FREE_EW,
  output        pd_Caution_EW,
  output        pd_button_NS,

  // Pedestrian Timer Display
  output [31:0] LCD_input
);

    parameter CLOCK_FREQUENCY = 50_000_000; // 50 MHz
    parameter BASE_GREEN_TIME = 30; // 30 seconds
    parameter FREE_WALK_PERCENT = 50; // 50% of the green time
    parameter YELLOW_DELAY_TIME = 5; // 5 seconds

    wire [31:0]  NS_GREEN_DELAY,
                 EW_GREEN_DELAY;

    wire [31:0]  PEDESTRIAN_TOTAL_CYCLES_NS,
                 PEDESTRIAN_FREE_CYCLE_NS,
                 NS_COUNTER_VALUE,
                 PEDESTRIAN_TOTAL_CYCLES_EW,
                 PEDESTRIAN_FREE_CYCLE_EW,
                 EW_COUNTER_VALUE;

// DYNAMIC GREEN TIME DELAY CALCULATOR
    adaptive_time_delay #(
        .CLK_FREQ(CLOCK_FREQUENCY),
        .BASE_TIME_DELAY(BASE_GREEN_TIME)
    ) DYNAMIC_TIMER (
        .clk(clk),
        .rst(rst),
        .ns_sensor(ns_sensor),
        .ew_sensor(sensor_EW),
        .ns_green_delay(NS_GREEN_DELAY),
        .ew_green_delay(EW_GREEN_DELAY)
    );

// PEDESTRIAN CROSSING CONTROLLER
    pedestrian_light_controller #(
        .CLK_FREQ(CLOCK_FREQUENCY),
        .FREE_WALK_PERCENT(FREE_WALK_PERCENT)
    ) PEDESTRIAN_CROSSING_LIGHT_CONTROLLER (
        .clk(clk),
        .rst(rst),
        .pd_button_ns(pd_button_ns),
        .pd_button_ew(pd_button_ew),
        .ns_green_delay(NS_GREEN_DELAY),
        .ew_green_delay(EW_GREEN_DELAY),
        .NS_RED(NS_red),
        .EW_RED(EW_red),
        .pd_FREE_NS(pd_FREE_NS),
        .pd_CAUTION_NS(pd_Caution_NS),
        .pd_FREE_EW(pd_FREE_EW),
        .pd_CAUTION_EW(pd_Caution_EW),
        .pd_total_cycles_ns(PEDESTRIAN_TOTAL_CYCLES_NS),
        .pd_free_cycles_ns(PEDESTRIAN_FREE_CYCLE_NS),
        .pd_current_counter_ns(NS_COUNTER_VALUE),
        .pd_total_cycles_ew(PEDESTRIAN_TOTAL_CYCLES_EW),
        .pd_free_cycles_ew(PEDESTRIAN_FREE_CYCLE_EW),
        .pd_current_counter_ew(EW_COUNTER_VALUE)
    );

// PEDESTRIAN CLOCK DISPLAY INPUT: NORTH-SOUTH
    pedestrian_timer_display #(
        .CLK_FREQ(CLOCK_FREQUENCY)
    ) PEDESTRIAN_TIMER_DISPLAY_NS (
        .clk(clk),
        .rst(rst),
        .pd_caution(pd_Caution_NS),
        .pd_counter(NS_COUNTER_VALUE),
        .pd_total_cycles(PEDESTRIAN_TOTAL_CYCLES_NS),
        .pd_free_cycles(PEDESTRIAN_FREE_CYCLE_NS),
        .time_left_ms(LCD_input)
    );

// BASE TRAFFIC LIGHT FSM CONTROLLER
    base_fsm #(
        .CLK_FREQ(CLOCK_FREQUENCY),
        .YELLOW_DELAY_TIME(YELLOW_DELAY_TIME)
    ) TRAFFIC_LIGHT_CONTROLLER (
        .clk(clk),
        .rst(rst),
        .ns_green_delay(NS_GREEN_DELAY),
        .ew_green_delay(EW_GREEN_DELAY),
        .NS_GREEN(NS_green),
        .NS_YELLOW(NS_yellow),
        .NS_RED(NS_red),
        .EW_GREEN(EW_green),
        .EW_YELLOW(EW_yellow),
        .EW_RED(EW_red)
    );

endmodule