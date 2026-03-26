# Lab 06-1: ADRL/ADRP 地址加载

## 实验目的

1. 掌握 ADRP 和 ADR 指令的使用
2. 理解 PC 相对寻址
3. 学习不同方式加载地址

## 实验内容

1. **ADR 指令**: 加载小范围地址 (±1MB)
2. **ADRP 指令**: 加载页对齐地址 (4KB 页)
3. **LDR 伪指令**: 加载任意地址

## 核心代码

```asm
// ADR: 计算地址
adr x0, my_test_data       // x0 = my_test_data 地址

// ADRP + ADD: 计算 4KB 对齐地址
adrp x1, my_test_data      // x1 = 页基址
add x1, x1, #:lo12:my_test_data  // 加上低 12 位偏移

// LDR 伪指令: 加载地址到寄存器
ldr x4, =my_test_data      // x4 = my_test_data 地址
```

## 构建与运行

```bash
make && make run
```