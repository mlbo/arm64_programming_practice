// lab05-3-fixed: BL 指令与返回地址 (正确实现)
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
    void bl_test(void);  // 正确实现
}

namespace arm64lab {
namespace lab05_3_fixed {

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

    // 正确实现：bl_test 保存了 x30
    bl_test();
}

} // namespace lab05_3_fixed
} // namespace arm64lab

extern "C" void kernel_main() {
    uart_init();
    uart_send_string("Welcome BenOS - Lab 05-3-Fixed: BL Instruction Fixed\r\n");

    arm64lab::lab05_3_fixed::test_ldr_str();
    arm64lab::lab05_3_fixed::test_data_process();

    while (true) {
        uart_send(uart_recv());
    }
}