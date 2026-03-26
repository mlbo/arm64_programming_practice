# Lab11-3: EL1 → EL0 切换

## 实验目的

学习如何从内核态 (EL1) 切换到用户态 (EL0)，理解 `eret` 指令的使用。

## 实验内容

- ELR_EL1 和 SPSR_EL1 寄存器
- `eret` 指令
- 设置用户栈

## 知识点

### ELR_EL1 (Exception Link Register)

保存异常返回地址。`eret` 执行后，PC 将被设置为 ELR_EL1 的值。

### SPSR_EL1 (Saved Program Status Register)

保存异常发生时的处理器状态 (PSTATE)。

### eret 指令

```asm
eret  ; 从异常返回，恢复 PC 和 PSTATE
```

## 运行方法

```bash
make && make run
```