# Lab 04-4: ANDS 与 Z 标志位

## 实验目标

学习 ANDS 指令对 Z 标志位的影响。

## 编译与运行

```bash
make
make run
```

## 核心内容

### ANDS 指令

```asm
ands x3, x1, x2    // x3 = x1 & x2, 设置标志位
```

### Z 标志位

- 结果为 0 时 Z=1
- 结果非 0 时 Z=0

### 读取 NZCV

```asm
mrs x0, nzcv       // 读取条件标志位寄存器
```

NZCV 格式: `N:Z:C:V` (bit 31-28)