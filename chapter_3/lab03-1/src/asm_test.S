// asm_test.S - LDR offset mode test
.global ldr_test
ldr_test:
    // Test LDR address offset modes
    mov    x1, 0x80000
    mov    x3, 16

    // Load from [x1] - address 0x80000
    ldr    x0, [x1]

    // Load from [x1, #8] - address 0x80008
    ldr    x2, [x1, #8]

    // Load from [x1, x3] - address 0x80010
    ldr    x4, [x1, x3]

    // Load from [x1, x3, lsl #3] - address 0x80080
    ldr    x5, [x1, x3, lsl #3]

    ret