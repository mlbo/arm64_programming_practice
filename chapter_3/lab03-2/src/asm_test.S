
// asm_test.S - Lab 03-2: Pre/Post indexing modes
.global ldr_test
ldr_test:
    // Test LDR offset modes
    mov    x1, 0x80000
    mov    x3, 16

    ldr    x0, [x1]
    ldr    x2, [x1, #8]
    ldr    x4, [x1, x3]
    ldr    x5, [x1, x3, lsl #3]

    // Pre-indexed mode: update address then load
    ldr    x6, [x1, #8]!

    // Post-indexed mode: load then update address
    ldr    x7, [x1], #8

    ret
