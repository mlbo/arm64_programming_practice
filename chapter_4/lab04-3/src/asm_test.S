// asm_test.S - Lab 04-3: CMP/SBC instructions

.global compare_and_return
compare_and_return:
    // Returns 0 when arg1 >= arg2
    // Returns 0xffffffffffffffff when arg1 < arg2
    cmp    x0, x1
    sbc    x0, xzr, xzr
    ret
