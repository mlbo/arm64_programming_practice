// asm_test.S - Lab 03-4: LDR with Label
#define MY_LABEL 0x20

.global ldr_test
ldr_test:
    // LDR offset modes
    mov    x1, 0x80000
    mov    x3, 16

    ldr    x0, [x1]
    ldr    x2, [x1, #8]
    ldr    x4, [x1, x3]
    ldr    x5, [x1, x3, lsl #3]

    // Pre-indexed LDR
    ldr    x6, [x1, #8]!

    // Post-indexed LDR
    ldr    x7, [x1], #8

    // STR pre/post indexed
    mov    x2, 0x400000
    ldr    x6, =0x1234abce
    str    x6, [x2, #8]!

    mov    x2, 0x500000
    str    x6, [x2], #8

    // LDR with label - load literal value
    ldr    x6, MY_LABEL

    // LDR with = - load address of label
    ldr    x7, =MY_LABEL

    ret
