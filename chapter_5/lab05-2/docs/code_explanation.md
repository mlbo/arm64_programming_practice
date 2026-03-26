# 代码说明

## csel_test 函数

```asm
.global csel_test
csel_test:
    cmp x0, 0           // 比较 x0 和 0
    sub x2, x1, 1       // x2 = x1 - 1
    add x3, x1, 2       // x3 = x1 + 2
    csel x0, x3, x2, eq // 如果 x0 == 0，返回 x3，否则返回 x2
    ret
```

**功能**:
- 如果第一个参数为 0，返回 `x1 + 2`
- 否则返回 `x1 - 1`

## 比较和返回函数

```asm
.global compare_and_return
compare_and_return:
    cmp x0, x1
    sbc x0, xzr, xzr    // 使用 SBC 指令设置返回值
    ret
```

**SBC 指令原理**:
- SBC (Subtract with Carry) 执行 `Rd = Rn - Rm - !C`
- 当 x0 >= x1 时，C=1，结果为 0
- 当 x0 < x1 时，C=0，结果为 0xffffffffffffffff