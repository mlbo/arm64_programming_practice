// asm_test.S - Lab 03-5: memcpy implementation

.global my_memcpy_test
my_memcpy_test:
    // Copy 32 bytes from 0x80000 to 0x200000
    mov    x1, 0x80000
    mov    x2, 0x200000
    add    x3, x1, 32

copy_loop:
    ldr    x4, [x1], #8    // Load and increment src
    str    x4, [x2], #8    // Store and increment dst
    cmp    x1, x3
    b.cc   copy_loop

    ret

