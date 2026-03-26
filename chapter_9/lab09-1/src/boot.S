/*
 * Boot code for Raspberry Pi
 * Entry point for bare metal
 */

.section ".text.boot"
.global _start

_start:
    // Check processor ID, only core 0 continues
    mrs     x0, mpidr_el1
    and     x0, x0, #3
    cbnz    x0, halt

    // Set stack pointer
    ldr     x0, =_stack_top
    mov     sp, x0

    // Clear BSS section
    ldr     x0, =_bss
    ldr     x1, =_ebss
    mov     x2, #0
clear_bss:
    cmp     x0, x1
    b.ge    2f
    str     x2, [x0], #8
    b       clear_bss
2:

    // Initialize C++ global constructors
    // (for freestanding, we may need to call constructors manually)
    ldr     x0, =_init_array_start
    ldr     x1, =_init_array_end
ctor_loop:
    cmp     x0, x1
    b.ge    3f
    ldr     x2, [x0], #8
    blr     x2
    b       ctor_loop
3:

    // Jump to C++ kernel main
    bl      kernel_main

halt:
    wfi
    b       halt

/*
 * Utility functions
 */
.global memset
memset:
    cbz     x2, 2f
    mov     x3, #0
1:
    strb    w1, [x0, x3]
    add     x3, x3, #1
    cmp     x2, x3
    b.ne    1b
2:
    ret

.global memcpy
memcpy:
    cbz     x2, 2f
    mov     x3, #0
1:
    ldrb    w4, [x1, x3]
    strb    w4, [x0, x3]
    add     x3, x3, #1
    cmp     x2, x3
    b.ne    1b
2:
    ret