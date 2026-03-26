// kernel.cpp - Lab 04-5: BFI/UBFX/SBFX
#include "uart.hpp"

extern "C" void bitfield_test();

extern "C" void kernel_main() {
    arm64lab::g_uart.init();
    arm64lab::g_uart.send_string("Lab 04-5: BFI/UBFX/SBFX\r\n");

    bitfield_test();

    while (true) {
        arm64lab::g_uart.send(arm64lab::g_uart.recv());
    }
}