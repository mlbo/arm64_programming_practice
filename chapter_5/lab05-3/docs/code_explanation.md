# 代码说明

## bl_test 函数 (有 Bug)

```asm
.global bl_test
bl_test:
    mov x0, 1
    mov x1, 3
    bl csel_test      // BL 会修改 x30
    ret               // 返回地址已丢失！
```

**执行流程**:

1. 调用 `bl_test` 时，x30 = 返回到调用者的地址
2. `bl csel_test` 将 `bl_test` 中的下一条指令地址写入 x30
3. `csel_test` 返回后，x30 指向 `bl_test` 中的 `ret` 指令
4. `ret` 执行后，程序跳转到 `bl_test` 中的 `ret` 指令，形成无限循环或跑飞

## csel_test 函数

```asm
.global csel_test
csel_test:
    cmp x0, 0
    sub x2, x1, 1
    add x3, x1, 2
    csel x0, x3, x2, eq
    ret              // 返回到 bl_test 的下一条指令
```

## 正确做法

参见 lab05-3-fixed，使用 `stp/ldp` 保存和恢复 x30。