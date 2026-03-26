// Kernel main for lab09-1
// Demonstrates printk functionality

#include "console.hpp"
#include "uart.hpp"

// Linker symbols for memory layout
extern "C" {
    extern char _text_boot[];
    extern char _etext_boot[];
    extern char _text[];
    extern char _etext[];
    extern char _rodata[];
    extern char _erodata[];
    extern char _data[];
    extern char _edata[];
    extern char _bss[];
    extern char _ebss[];
}

namespace {

void print_memory_layout() {
    benos::printk("BenOS image layout:\n");
    benos::printk("  .text.boot: 0x%08lx - 0x%08lx (%6ld B)\n",
        reinterpret_cast<benos::uptr>(_text_boot),
        reinterpret_cast<benos::uptr>(_etext_boot),
        _etext_boot - _text_boot);
    benos::printk("       .text: 0x%08lx - 0x%08lx (%6ld B)\n",
        reinterpret_cast<benos::uptr>(_text),
        reinterpret_cast<benos::uptr>(_etext),
        _etext - _text);
    benos::printk("     .rodata: 0x%08lx - 0x%08lx (%6ld B)\n",
        reinterpret_cast<benos::uptr>(_rodata),
        reinterpret_cast<benos::uptr>(_erodata),
        _erodata - _rodata);
    benos::printk("       .data: 0x%08lx - 0x%08lx (%6ld B)\n",
        reinterpret_cast<benos::uptr>(_data),
        reinterpret_cast<benos::uptr>(_edata),
        _edata - _data);
    benos::printk("        .bss: 0x%08lx - 0x%08lx (%6ld B)\n",
        reinterpret_cast<benos::uptr>(_bss),
        reinterpret_cast<benos::uptr>(_ebss),
        _ebss - _bss);
}

void test_printk() {
    benos::printk("\n=== printk Test ===\n");

    // Test various format specifiers
    benos::printk("Decimal: %d, %d, %d\n", 123, -456, 0);
    benos::printk("Hex: 0x%x, 0x%X\n", 0xdeadbeef, 0xcafebabe);
    benos::printk("Octal: %o\n", 0755);
    benos::printk("String: %s\n", "Hello, BenOS!");
    benos::printk("Char: %c %c %c\n", 'A', 'B', 'C');
    benos::printk("Pointer: %p\n", reinterpret_cast<void*>(0x80000));

    // Test width and padding
    benos::printk("Width: [%10d] [%-10d] [%010d]\n", 123, 123, 123);

    // Test long long
    benos::printk("Long long: %lld, 0x%llx\n", 0x123456789ABCDEFLL, 0x123456789ABCDEFLL);

    benos::printk("=== Test Complete ===\n\n");
}

}  // anonymous namespace

extern "C" void kernel_main() {
    // Initialize UART and console
    benos::g_uart.init();
    benos::g_console.init();

    // Welcome message
    benos::g_uart.send_string("Welcome BenOS!\r\n");
    benos::printk("printk init done\n");

    // Test printk
    test_printk();

    // Print memory layout
    print_memory_layout();

    // Echo loop
    while (true) {
        char c = benos::g_uart.recv();
        if (c == '\r') {
            benos::g_uart.send_string("\r\n");
        } else {
            benos::g_uart.send(c);
        }
    }
}