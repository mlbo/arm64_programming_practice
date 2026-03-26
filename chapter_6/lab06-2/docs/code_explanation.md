# 代码说明

## add_inst_test 函数

```asm
add_inst_test:
    mov x0, #0
    ldr x1, =0xffffffffffffffff
    mov x2, #3
    adds x0, x1, x1    // 0xFFFFFFFFFFFFFFFF + 0xFFFFFFFFFFFFFFFF
    adc x3, xzr, xzr   // 获取进位值 (1)
    cmp x1, x2
    adc x4, xzr, xzr   // 比较结果
    ret
```

## bitfield_test 函数

```asm
bitfield_test:
    mov x1, 0x345
    mov x0, 0
    bfi x0, x1, 8, 4   // x0 = 0x0450

    ldr x2, =0x5678abcd
    ubfx x3, x2, #4, #8  // x3 = 0xBC
    sbfx x4, x2, #4, #8  // x4 = 0xFFFFFFFFFFFFFFBC

    // 读取系统寄存器位域
    mrs x1, ID_AA64ISAR0_EL1
    ubfx x0, x1, #20, #4  // Atomic 域
    ubfx x2, x1, #4, #4   // AES 域
    ret
```