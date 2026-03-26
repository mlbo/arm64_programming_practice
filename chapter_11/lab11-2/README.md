# Lab11-2: 异常向量表

## 实验目的

学习 ARM64 异常向量表的建立和使用，理解异常处理流程。

## 实验内容

- 异常向量表结构
- VBAR_EL1 寄存器
- 异常处理函数

## 知识点

### 异常向量表结构

异常向量表必须 2KB (2048 字节) 对齐，包含 16 个表项：

```
Offset  Current EL    Type
------  -----------    ----
0x000   EL1, SP_EL0    Synchronous
0x080   EL1, SP_EL0    IRQ
0x100   EL1, SP_EL0    FIQ
0x180   EL1, SP_EL0    SError
0x200   EL1, SP_ELx    Synchronous
0x280   EL1, SP_ELx    IRQ
0x300   EL1, SP_ELx    FIQ
0x380   EL1, SP_ELx    SError
...
```

### 设置异常向量表

```asm
ldr x0, =vectors
msr vbar_el1, x0
```

## 运行方法

```bash
make && make run
```