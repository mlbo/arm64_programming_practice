// lab05-2: CSEL 条件选择指令
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
}

namespace arm64lab {
namespace lab05_2 {

void test_ldr_str() {
    ldr_test();
    my_memcpy_test();
    memset(reinterpret_cast<void*>(0x200004), 0x55, 102);
    access_label_test();
}

void test_data_process() {
    unsigned long ret;
    unsigned long val;

    add_inst_test();
    ret = compare_and_return(10, 9);
    val = compare_and_return(9, 10);
    ands_test();
    bitfield_test();
    cmp_cmn_test();
    val = csel_test(1, 3);
}

} // namespace lab05_2
} // namespace arm64lab

extern "C" void kernel_main() {
    uart_init();
    uart_send_string("Welcome BenOS - Lab 05-2: CSEL Conditional Select\r\n");

    arm64lab::lab05_2::test_ldr_str();
    arm64lab::lab05_2::test_data_process();

    while (true) {
        uart_send(uart_recv());
    }
}