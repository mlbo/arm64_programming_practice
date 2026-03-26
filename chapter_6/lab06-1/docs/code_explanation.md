# 代码说明

## adrp_test 函数

```asm
.global adrp_test
adrp_test:
    // 方法 1: ADR 指令
    adr x0, my_test_data

    // 方法 2: ADRP + ADD
    adrp x1, my_test_data
    add x1, x1, #:lo12:my_test_data
    ldr x3, [x1]

    // 方法 3: LDR 伪指令
    ldr x4, =my_test_data
    ldr x5, my_test_data

    // 加载远距离地址
    adrp x2, init_pg_dir    // 4MB 地址
    ldr x6, =init_pg_dir
    ret
```

## my_test_data 数据

```asm
.align 3
.globl my_test_data
my_test_data:
    .dword 0x12345678abcdabcd
```

## 执行结果

| 寄存器 | 值 |
|--------|---|
| x0 | my_test_data 地址 |
| x1 | my_test_data 地址 |
| x3 | 0x12345678abcdabcd |
| x4 | my_test_data 地址 |
| x5 | 0x12345678abcdabcd |