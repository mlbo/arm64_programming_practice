# Lab11-1: 读取 CurrentEL

## 实验目的

学习 ARM64 异常级别 (Exception Level) 的概念，掌握读取当前 EL 的方法。

## 实验内容

- 异常级别 EL0-EL3
- CurrentEL 系统寄存器
- 使用 MRS 指令读取系统寄存器

## 知识点

### 异常级别

ARMv8 定义了 4 个异常级别：

| EL | 名称 | 典型用途 |
|----|------|----------|
| EL0 | User | 用户态应用程序 |
| EL1 | Kernel | 操作系统内核 |
| EL2 | Hypervisor | 虚拟化 |
| EL3 | Secure Monitor | 安全世界切换 |

### CurrentEL 寄存器

```
Bits [3:2]: Current Exception Level (0-3)
Bits [1:0]: Reserved
```

## 运行方法

```bash
make && make run
```