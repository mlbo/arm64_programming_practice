// lab08-4: 完整裸机混合编程项目
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
    unsigned long macro_test_1(long a, long b);
    unsigned long macro_test_2(long a, long b);
}

// 外部符号（在汇编中定义）
extern "C" unsigned long func_addr[];
extern "C" unsigned long func_num_syms;
extern "C" char func_string[];

namespace arm64lab {
namespace lab08_4 {

static int print_func_name(unsigned long addr) {
    int i;
    char *p, *string;

    for (i = 0; i < static_cast<int>(func_num_syms); i++) {
        if (addr == func_addr[i]) {
            goto found;
        }
    }
    return 0;

found:
    p = &func_string;
    if (i == 0) {
        string = p;
        goto done;
    }
    while (true) {
        p++;
        if (*p == '\0') {
            i--;
        }
        if (i == 0) {
            p++;
            string = p;
            break;
        }
    }

done:
    uart_send_string(string);
    uart_send_string("\n");
    return 0;
}

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

void test_assembler_features() {
    // 测试查找表
    print_func_name(0x800880);
    print_func_name(0x800860);
    print_func_name(0x800800);

    // 测试宏
    unsigned long val = macro_test_1(5, 5);
    val = macro_test_2(5, 5);
    (void)val;
}

} // namespace lab08_4
} // namespace arm64lab

extern "C" void kernel_main() {
    uart_init();
    uart_send_string("Welcome BenOS - Lab 08-4: Mixed C/ASM Bare Metal\r\n");

    arm64lab::lab08_4::test_ldr_str();
    arm64lab::lab08_4::test_data_process();
    arm64lab::lab08_4::test_assembler_features();

    while (true) {
        uart_send(uart_recv());
    }
}