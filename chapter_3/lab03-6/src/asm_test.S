// asm_test.S - Lab 03-6: memset implementation

// __memset_16bytes is defined in memset_asm.S

.global my_memcpy_test
my_memcpy_test:
    mov    x1, 0x80000
    mov    x2, 0x200000
    add    x3, x1, 32

copy_loop:
    ldr    x4, [x1], #8
    str    x4, [x2], #8
    cmp    x1, x3
    b.cc   copy_loop
    ret

