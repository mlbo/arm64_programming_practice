# Lab 03-6: memset 实现

## 实验目标

学习高效的内存填充实现。

## 编译与运行

```bash
make
make run
```

## 核心内容

### STP 指令

```asm
stp x1, x1, [x0], #16
```

- 同时存储两个寄存器 (16 字节)
- 后变基移动 16 字节

### 优化策略

1. 16 字节对齐检查
2. 使用 STP 每次填充 16 字节
3. 处理剩余字节

## 文件结构

```
lab03-6/
├── include/
│   └── memset.hpp
├── src/
│   ├── memset.cpp      # C++ 实现
│   └── asm_test.S      # 汇编优化
```