# Lab 08-4: 完整裸机混合编程项目

## 实验目的

1. 综合运用 C 和汇编混合编程
2. 理解裸机环境下的调用约定
3. 掌握完整的裸机项目结构

## 项目结构

```
lab08-4/
├── README.md
├── docs/
│   ├── knowledge.md
│   ├── code_explanation.md
│   └── exercises.md
├── scripts/
│   ├── build.sh
│   └── run.sh
├── include/
├── src/
│   ├── boot.S           # 启动汇编
│   ├── asm_test.S       # 测试汇编函数
│   ├── kernel.c         # 主程序
│   └── ...
├── Makefile
└── linker.ld
```

## 实验内容

1. **汇编器伪操作**: 使用 .quad, .asciz 创建查找表
2. **宏定义**: 使用 .macro 定义可复用代码
3. **C 和汇编混合**: C 调用汇编，汇编调用 C

## 构建与运行

```bash
make && make run
```