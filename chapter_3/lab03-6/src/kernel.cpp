// kernel.cpp - Lab 03-6: memset implementation
#include "uart.hpp"

// 外部函数声明
extern "C" void* memset(void* s, int c, unsigned long count);
#include "memset.hpp"

extern "C" void my_memcpy_test();

extern "C" void kernel_main() {
    arm64lab::g_uart.init();
    arm64lab::g_uart.send_string("Lab 03-6: memset\r\n");

    my_memcpy_test();

    // Test memset
    arm64lab::memset(reinterpret_cast<void*>(0x200004), 0x55, 102);

    while (true) {
        arm64lab::g_uart.send(arm64lab::g_uart.recv());
    }
}