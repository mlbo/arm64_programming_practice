# Lab09-1: 格式化输出 printk

## 实验目的

学习 ARM64 裸机环境下的格式化输出实现，掌握可变参数处理和格式化字符串解析。

## 实验内容

- 实现 `Console` 类封装 UART 输出
- 实现类似 printf 的格式化输出函数
- 支持常用格式说明符: `%d`, `%x`, `%s`, `%c`, `%p`

## 运行方法

```bash
# 编译
make

# 运行 (QEMU)
make run

# 调试
make debug
```

## 目录结构

```
lab09-1/
├── README.md              # 本文件
├── docs/
│   ├── knowledge.md       # 知识点讲解
│   ├── code_explanation.md # 代码说明
│   └── exercises.md       # 练习题
├── scripts/
│   ├── build.sh           # 编译脚本
│   └── run.sh             # 运行脚本
├── include/               # 头文件
├── src/                   # 源代码
├── Makefile
└── linker.ld
```

## 预期输出

```
Welcome BenOS!
printk init done
BenOS image layout:
  .text.boot: 0x00080000 - 0x000800xx (xxx B)
       .text: 0x000800xx - 0x00080xxx (xxxx B)
     .rodata: ...
       .data: ...
        .bss: ...
```