// asm_test.S - Lab 04-2: ADDS/CMP/ADC instructions

.global add_inst_test
add_inst_test:
    mov    x0, #0
    ldr    x1, =0xffffffffffffffff
    mov    x2, #3

    // ADDS sets condition flags when overflow occurs
    adds   x0, x1, x1        // Overflow, C=1
    adc    x3, xzr, xzr      // Capture carry

    // CMP sets condition flags
    // When x1 > x2: C=1
    // When x1 < x2: C=0
    cmp    x1, x2
    adc    x4, xzr, xzr      // Capture carry

    ret

.global compare_and_return
compare_and_return:
    // Returns 0 when arg1 >= arg2
    // Returns 0xffffffffffffffff when arg1 < arg2
    cmp    x0, x1
    sbc    x0, xzr, xzr
    ret
