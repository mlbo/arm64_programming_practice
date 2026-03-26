// lab07-1: 栈与局部变量
// kernel.cpp - 主程序入口

extern "C" {
#include "uart.h"
}

// 外部函数声明
extern "C" void* memset(void* s, int c, unsigned long count);

extern "C" {
    void ldr_test(void);
    void my_memcpy_test(void);
    void access_label_test(void);
    void add_inst_test(void);
    void ands_test(void);
    void bitfield_test(void);
    void cmp_cmn_test(void);
    unsigned long compare_and_return(unsigned long a, unsigned long b);
    unsigned long csel_test(unsigned long r, unsigned long b);
    void bl_test(void);
    void adrp_test(void);
    unsigned long my_atomic_write(long data);
    void atomic_set(volatile unsigned long* addr, unsigned long newval);
}

namespace arm64lab {
namespace lab07_1 {

void test_ldr_str() {
    ldr_test();
    my_memcpy_test();
    memset(reinterpret_cast<void*>(0x200004), 0x55, 102);
    access_label_test();
}

void test_data_process() {
    unsigned long ret;
    unsigned long val;
    unsigned long p1 = 0;

    add_inst_test();
    ret = compare_and_return(10, 9);
    val = compare_and_return(9, 10);
    ands_test();
    bitfield_test();
    cmp_cmn_test();
    val = csel_test(1, 3);
    bl_test();
    adrp_test();

    val = my_atomic_write(0x345);
    atomic_set(0x11, &p1);
}

} // namespace lab07_1
} // namespace arm64lab

extern "C" void kernel_main() {
    uart_init();
    uart_send_string("Welcome BenOS - Lab 07-1: Stack and Local Variables\r\n");

    arm64lab::lab07_1::test_ldr_str();
    arm64lab::lab07_1::test_data_process();

    while (true) {
        uart_send(uart_recv());
    }
}