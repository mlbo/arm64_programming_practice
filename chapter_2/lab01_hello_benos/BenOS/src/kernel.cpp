// kernel.cpp - BenOS kernel entry point
#include "uart.hpp"

extern "C" void kernel_main() {
    arm64lab::g_uart.init();
    arm64lab::g_uart.send_string("Welcome BenOS!\r\n");

    // Echo loop
    while (true) {
        arm64lab::g_uart.send(arm64lab::g_uart.recv());
    }
}