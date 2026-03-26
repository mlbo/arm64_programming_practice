// kernel.cpp - Lab 03-3: Pre/Post indexing with STR
#include "uart.hpp"

extern "C" void ldr_test();

void my_ldr_str_test() {
    ldr_test();
}

extern "C" void kernel_main() {
    arm64lab::g_uart.init();
    arm64lab::g_uart.send_string("Lab 03-3: STR Pre/Post Indexing\r\n");

    my_ldr_str_test();

    while (true) {
        arm64lab::g_uart.send(arm64lab::g_uart.recv());
    }
}