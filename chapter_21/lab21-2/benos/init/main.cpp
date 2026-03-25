// init/main.cpp - BenOS kernel main entry point
// C++ refactored version using benos namespace classes

#include <benos/benos.hpp>
#include <benos/mm/page_allocator.hpp>
#include <benos/kernel/process.hpp>
#include <benos/kernel/console.hpp>
#include <benos/driver/uart.hpp>
#include <benos/driver/timer.hpp>

#include <asm/sysregs.h>
#include <irq.h>
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

// Note: Do not use "using namespace benos;" to avoid u64 type conflict with type.h

static void print_memory_layout() {
    benos::g_console.printf("BenOS image layout:\n");
    benos::g_console.printf("  .text.boot: 0x%08lx - 0x%08lx (%6ld B)\n",
            (unsigned long)_text_boot, (unsigned long)_etext_boot,
            (long)(_etext_boot - _text_boot));
    benos::g_console.printf("       .text: 0x%08lx - 0x%08lx (%6ld B)\n",
            (unsigned long)_text, (unsigned long)_etext,
            (long)(_etext - _text));
    benos::g_console.printf("     .rodata: 0x%08lx - 0x%08lx (%6ld B)\n",
            (unsigned long)_rodata, (unsigned long)_erodata,
            (long)(_erodata - _rodata));
    benos::g_console.printf("       .data: 0x%08lx - 0x%08lx (%6ld B)\n",
            (unsigned long)_data, (unsigned long)_edata,
            (long)(_edata - _data));
    benos::g_console.printf("        .bss: 0x%08lx - 0x%08lx (%6ld B)\n",
            (unsigned long)_bss, (unsigned long)_ebss,
            (long)(_ebss - _bss));
}

static void delay(int n) {
    while (n--) ;
}

extern "C" void kernel_thread(void) {
    while (true) {
        delay(10000000);
        benos::g_console.printf("%s: 12345\n", __func__);
    }
}

register unsigned long current_stack_pointer asm("sp");

extern "C" void kernel_main(void) {
    int el;

    // Initialize UART
    benos::g_uart.init();
    benos::g_uart.send_string("=Welcome BenOS!\r\n");
    benos::g_uart.send_string("i am benshushu!\r\n");

    // Print EL
    benos::g_console.printf("running on EL:");
    el = read_sysreg(CurrentEL) >> 2;
    benos::g_console.printf("%d\n", el);

    // Initialize memory
    benos::g_page_allocator.init((unsigned long)_ebss, TOTAL_MEMORY);
    print_memory_layout();
    benos::g_console.printf("\r\n");

    // Print process info
    benos::g_console.printf("0 thread's task_struct: 0x%lx\n", &init_task_union.task);
    benos::g_console.printf("SP: 0x%lx\n", current_stack_pointer);

    // Test printk format specifiers
    benos::g_console.printf("el=%3d\n", el);
    benos::g_console.printf("el=%-3d\n", el);
    benos::g_console.printf("el=%03d\n", el);
    benos::g_console.printf("el=0x%-3x\n", el);
    benos::g_console.printf("el=0x%03x\n", el);

    benos::g_console.printf("0x%hx\n", 0x123456789abcdef);
    benos::g_console.printf("0x%x\n", 0x123456789abcdef);
    benos::g_console.printf("0x%lx\n", 0x123456789abcdef);
    benos::g_console.printf("0x%llx\n", 0x123456789abcdef);

    benos::g_console.printf("%d data\n", 0xfffffffe);
    benos::g_console.printf("%u data\n", 0xfffffffe);

    benos::g_console.printf("%u, %d, %x\n", 1024, -2, -2);

    // Initialize timer and enable interrupts
    benos::g_timer.init();
    raw_local_irq_enable();

    // Create kernel thread
    int pid = benos::g_process_manager.create_process(kernel_thread);
    if (pid < 0) {
        benos::g_console.printf("create thread fail\n");
    }

    // Switch to new process
    struct task_struct* next = benos::g_process_manager.find_by_pid(pid);
    if (next) {
        switch_to(next);
    }

    // Main loop
    while (true) {
        benos::g_uart.send(benos::g_uart.recv());
    }
}
