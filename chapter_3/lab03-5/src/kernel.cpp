// kernel.cpp - Lab 03-5: memcpy implementation
#include "uart.hpp"

extern "C" void my_memcpy_test();

extern "C" void kernel_main() {
    arm64lab::g_uart.init();
    arm64lab::g_uart.send_string("Lab 03-5: memcpy\r\n");

    my_memcpy_test();

    while (true) {
        arm64lab::g_uart.send(arm64lab::g_uart.recv());
    }
}