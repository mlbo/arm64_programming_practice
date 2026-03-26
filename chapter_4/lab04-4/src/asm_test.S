// asm_test.S - Lab 04-4: ANDS and Z flag

.global ands_test
ands_test:
    mov    x1, #0x3
    mov    x2, #0

    ands   x3, x1, x2        // AND and set flags

    // Read NZCV register to check Z flag
    mrs    x0, nzcv
    ret
