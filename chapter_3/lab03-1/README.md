# Lab 03-1: LDR 偏移模式

## 实验目标

学习 ARM64 LDR 指令的多种地址偏移模式。

## 编译与运行

```bash
make
make run
```

## 核心内容

### 1. 基础偏移 `[x1]`

```asm
ldr x0, [x1]        // 从 x1 指向的地址加载
```

### 2. 立即数偏移 `[x1, #8]`

```asm
ldr x2, [x1, #8]    // 从 x1+8 地址加载
```

### 3. 寄存器偏移 `[x1, x3]`

```asm
ldr x4, [x1, x3]    // 从 x1+x3 地址加载
```

### 4. 移位偏移 `[x1, x3, lsl #3]`

```asm
ldr x5, [x1, x3, lsl #3]  // 从 x1+(x3<<3) 地址加载
```

## 文件结构

```
lab03-1/
├── README.md
├── docs/
│   └── knowledge.md
├── scripts/
│   ├── build.sh
│   └── run.sh
├── include/
│   ├── types.hpp
│   ├── io.hpp
│   ├── uart.hpp
│   └── mm.hpp
├── src/
│   ├── boot.S
│   ├── mm.S
│   ├── asm_test.S
│   ├── pl_uart.cpp
│   └── kernel.cpp
├── Makefile
└── linker.ld
```