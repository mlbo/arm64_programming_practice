# Lab 04-3: CMP/SBC 指令

## 实验目标

学习 CMP 和 SBC 指令的综合应用。

## 编译与运行

```bash
make
make run
```

## 核心内容

### CMP 比较指令

```asm
cmp x0, x1    // 计算 x0 - x1 并设置标志位
```

### SBC 带借位减法

```asm
sbc x0, xzr, xzr    // 等于 xzr - xzr - !C
```

### 实用技巧

```asm
compare_and_return:
    cmp x0, x1
    sbc x0, xzr, xzr
    ret
```

- 当 arg1 >= arg2 时，返回 0
- 当 arg1 < arg2 时，返回 0xffffffffffffffff