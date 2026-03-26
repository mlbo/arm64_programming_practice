// kernel.cpp - Lab 04-4: ANDS and Z flag
#include "uart.hpp"

extern "C" void ands_test();

extern "C" void kernel_main() {
    arm64lab::g_uart.init();
    arm64lab::g_uart.send_string("Lab 04-4: ANDS and Z Flag\r\n");

    ands_test();

    while (true) {
        arm64lab::g_uart.send(arm64lab::g_uart.recv());
    }
}