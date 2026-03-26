// kernel.cpp - Lab 04-1: Label access
#include "uart.hpp"

extern "C" void access_label_test();

extern "C" void kernel_main() {
    arm64lab::g_uart.init();
    arm64lab::g_uart.send_string("Lab 04-1: Label Access\r\n");

    access_label_test();

    while (true) {
        arm64lab::g_uart.send(arm64lab::g_uart.recv());
    }
}