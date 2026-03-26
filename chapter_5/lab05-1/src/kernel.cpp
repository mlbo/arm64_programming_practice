// lab05-1: LDR/STR 寻址模式实验
// kernel.cpp - 主程序入口

// C 函数声明 (extern "C" 用于链接 C 代码)
extern "C" {
#include "uart.h"
void* memset(void* s, int c, unsigned long count);
}

// 汇编函数声明 (extern "C" 用于链接汇编代码)
extern "C" {
    void ldr_test(void);
    void my_memcpy_test(void);
    void access_label_test(void);
    void add_inst_test(void);
    void ands_test(void);
    void bitfield_test(void);
    void cmp_cmn_test(void);
    unsigned long compare_and_return(unsigned long a, unsigned long b);
}

namespace arm64lab {
namespace lab05_1 {

// LDR/STR 测试
void test_ldr_str() {
    ldr_test();
    my_memcpy_test();

    // memset 测试
    memset(reinterpret_cast<void*>(0x200004), 0x55, 102);

    access_label_test();
}

// 数据处理指令测试
void test_data_process() {
    unsigned long ret;
    unsigned long val;

    add_inst_test();

    ret = compare_and_return(10, 9);
    val = compare_and_return(9, 10);

    ands_test();
    bitfield_test();
    cmp_cmn_test();
}

} // namespace lab05_1
} // namespace arm64lab

// C 风格入口点 (被 boot.S 调用)
extern "C" void kernel_main() {
    uart_init();
    uart_send_string("Welcome BenOS - Lab 05-1: LDR/STR Addressing Modes\r\n");

    arm64lab::lab05_1::test_ldr_str();
    arm64lab::lab05_1::test_data_process();

    while (true) {
        uart_send(uart_recv());
    }
}