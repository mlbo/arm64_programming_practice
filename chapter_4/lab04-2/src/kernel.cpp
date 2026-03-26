// kernel.cpp - Lab 04-2: ADDS/CMP/ADC
#include "uart.hpp"

extern "C" void add_inst_test();
extern "C" auto compare_and_return(u64 a, u64 b) -> u64;

extern "C" void kernel_main() {
    arm64lab::g_uart.init();
    arm64lab::g_uart.send_string("Lab 04-2: ADDS/CMP/ADC\r\n");

    add_inst_test();

    u64 ret = compare_and_return(10, 9);
    u64 val = compare_and_return(9, 10);

    while (true) {
        arm64lab::g_uart.send(arm64lab::g_uart.recv());
    }
}