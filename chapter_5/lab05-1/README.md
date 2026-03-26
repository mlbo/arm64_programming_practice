# Lab 05-1: LDR/STR 寻址模式实验

## 实验目的

1. 掌握 LDR 指令的多种寻址模式
2. 理解前变基和后变基寻址的区别
3. 学习 Label 的访问方式

## 实验内容

1. **LDR 基础寻址**: 偏移量寻址、寄存器寻址、扩展寄存器寻址
2. **前变基/后变基**: 观察地址寄存器的变化
3. **STR 寻址**: 存储指令的寻址模式
4. **Label 访问**: 加载立即数和地址

## 构建与运行

```bash
make                    # 编译
make run               # QEMU 运行
make debug             # GDB 调试
make board=rpi3 run    # Raspberry Pi 3B
```

## 目录结构

```
lab05-1/
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
├── Makefile
└── linker.ld
```

## 参考资料

- ARM Architecture Reference Manual ARMv8, Section C6.1
- 《ARM64体系结构：编程与实践》第5章