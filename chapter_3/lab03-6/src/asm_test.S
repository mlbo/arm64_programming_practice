// asm_test.S - Lab 03-6: memset implementation

.global __memset_16bytes
__memset_16bytes:
    mov    x4, #0
1:
    stp    x1, x1, [x0], #16    // Store 16 bytes and increment
    add    x4, x4, #16
    cmp    x4, x2
    bne    1b
    ret

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

