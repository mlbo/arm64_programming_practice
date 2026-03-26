// kernel.cpp - Lab 04-3: CMP/SBC
#include "uart.hpp"

extern "C" auto compare_and_return(u64 a, u64 b) -> u64;

extern "C" void kernel_main() {
    arm64lab::g_uart.init();
    arm64lab::g_uart.send_string("Lab 04-3: CMP/SBC\r\n");

    u64 ret = compare_and_return(10, 9);
    u64 val = compare_and_return(9, 10);

    while (true) {
        arm64lab::g_uart.send(arm64lab::g_uart.recv());
    }
}