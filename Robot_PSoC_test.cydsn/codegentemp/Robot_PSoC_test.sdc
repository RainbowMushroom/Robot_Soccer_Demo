# THIS FILE IS AUTOMATICALLY GENERATED
# Project: C:\Users\bscj\Documents\Curtis\Robotics Project\Robot_PSoC_test.cydsn\Robot_PSoC_test.cydsn\Robot_PSoC_test.cyprj
# Date: Fri, 26 Feb 2021 08:42:13 GMT
#set_units -time ns
create_clock -name {CyILO} -period 10000 -waveform {0 5000} [list [get_pins {ClockBlock/ilo}] [get_pins {ClockBlock/clk_100k}] [get_pins {ClockBlock/clk_1k}] [get_pins {ClockBlock/clk_32k}]]
create_clock -name {CyIMO} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/imo}]]
create_clock -name {CyPLL_OUT} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/pllout}]]
create_clock -name {CyMASTER_CLK} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/clk_sync}]]
create_generated_clock -name {CyBUS_CLK} -source [get_pins {ClockBlock/clk_sync}] -edges {1 2 3} [list [get_pins {ClockBlock/clk_bus_glb}]]
create_generated_clock -name {UART_IntClock} -source [get_pins {ClockBlock/clk_sync}] -edges {1 27 53} -nominal_period 1083.3333333333333 [list [get_pins {ClockBlock/dclk_glb_1}]]
create_generated_clock -name {Clock_2} -source [get_pins {ClockBlock/clk_sync}] -edges {1 7 13} [list [get_pins {ClockBlock/dclk_glb_0}]]
create_generated_clock -name {Clock_1} -source [get_pins {ClockBlock/clk_sync}] -edges {1 941 1881} -nominal_period 39208.333333333328 [list [get_pins {ClockBlock/dclk_glb_3}]]
create_generated_clock -name {Clock_3} -source [get_pins {ClockBlock/clk_sync}] -edges {1 193 385} [list [get_pins {ClockBlock/dclk_glb_2}]]


# Component constraints for C:\Users\bscj\Documents\Curtis\Robotics Project\Robot_PSoC_test.cydsn\Robot_PSoC_test.cydsn\TopDesign\TopDesign.cysch
# Project: C:\Users\bscj\Documents\Curtis\Robotics Project\Robot_PSoC_test.cydsn\Robot_PSoC_test.cydsn\Robot_PSoC_test.cyprj
# Date: Fri, 26 Feb 2021 08:42:06 GMT
