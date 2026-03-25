// init/main.cpp - BenOS kernel main entry point
// C++ refactored version using benos namespace classes

#include <benos/benos.hpp>
#include <benos/mm/page_allocator.hpp>
#include <benos/kernel/process.hpp>
#include <benos/kernel/console.hpp>
#include <benos/driver/uart.hpp>
#include <benos/driver/timer.hpp>

#include <asm/sysregs.h>
#include <asm/irq.h>
#include <asm/mm.h>
#include <sched.h>

extern "C" {
extern char _text_boot[], _etext_boot[];
extern char _text[], _etext[];
extern char _rodata[], _erodata[];
extern char _data[], _edata[];
extern char _bss[], _ebss[];

extern union task_union init_task_union;
}

using namespace benos;

static void print_memory_layout() {
    g_console.printf("BenOS image layout:\n");
    g_console.printf("  .text.boot: 0x%08lx - 0x%08lx (%6ld B)\n",
            (u64)_text_boot, (u64)_etext_boot,
            (u32)(_etext_boot - _text_boot));
    g_console.printf("       .text: 0x%08lx - 0x%08lx (%6ld B)\n",
            (u64)_text, (u64)_etext,
            (u32)(_etext - _text));
    g_console.printf("     .rodata: 0x%08lx - 0x%08lx (%6ld B)\n",
            (u64)_rodata, (u64)_erodata,
            (u32)(_erodata - _rodata));
    g_console.printf("       .data: 0x%08lx - 0x%08lx (%6ld B)\n",
            (u64)_data, (u64)_edata,
            (u32)(_edata - _data));
    g_console.printf("        .bss: 0x%08lx - 0x%08lx (%6ld B)\n",
            (u64)_bss, (u64)_ebss,
            (u32)(_ebss - _bss));
}

static void delay(int n) {
    while (n--) ;
}

extern "C" void kernel_thread(void) {
    while (true) {
        delay(10000000);
        g_console.printf("%s: 12345\n", __func__);
    }
}

register unsigned long current_stack_pointer asm("sp");

extern "C" void kernel_main(void) {
    int el;

    // Initialize UART
    g_uart.init();
    g_uart.send_string("=Welcome BenOS!\r\n");
    g_uart.send_string("i am benshushu!\r\n");

    // Print EL
    g_console.printf("running on EL:");
    el = read_sysreg(CurrentEL) >> 2;
    g_console.printf("%d\n", el);

    // Initialize memory
    g_page_allocator.init((u64)_ebss, TOTAL_MEMORY);
    print_memory_layout();
    g_console.printf("\r\n");

    // Print process info
    g_console.printf("0 thread's task_struct: 0x%lx\n", &init_task_union.task);
    g_console.printf("SP: 0x%lx\n", current_stack_pointer);

    // Test printk format specifiers
    g_console.printf("el=%3d\n", el);
    g_console.printf("el=%-3d\n", el);
    g_console.printf("el=%03d\n", el);
    g_console.printf("el=0x%-3x\n", el);
    g_console.printf("el=0x%03x\n", el);

    g_console.printf("0x%hx\n", 0x123456789abcdef);
    g_console.printf("0x%x\n", 0x123456789abcdef);
    g_console.printf("0x%lx\n", 0x123456789abcdef);
    g_console.printf("0x%llx\n", 0x123456789abcdef);

    g_console.printf("%d data\n", 0xfffffffe);
    g_console.printf("%u data\n", 0xfffffffe);

    g_console.printf("%u, %d, %x\n", 1024, -2, -2);

    // Initialize timer and enable interrupts
    g_timer.init();
    raw_local_irq_enable();

    // Create kernel thread
    int pid = g_process_manager.create_process(kernel_thread);
    if (pid < 0) {
        g_console.printf("create thread fail\n");
    }

    // Switch to new process
    struct task_struct* next = g_process_manager.find_by_pid(pid);
    if (next) {
        switch_to(next);
    }

    // Main loop
    while (true) {
        g_uart.send(g_uart.recv());
    }
}