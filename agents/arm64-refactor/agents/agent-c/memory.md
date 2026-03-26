# agent-c 操作记录

---

## 操作历史

### 2026-03-26: Chapters 9-11 重构完成

#### 完成内容

**Chapter 9: 格式化输出 printk (lab09-1)**
- 创建新 lab09-1 目录，包含完整 C++ 实现
- Console 类 (ncnn 风格单例模式)
- 支持 %d, %x, %s, %c, %p 等格式
- 完整文档: README.md, knowledge.md, code_explanation.md, exercises.md
- 构建脚本: build.sh, run.sh

**Chapter 10: 内联汇编与原子操作 (lab10-1 ~ lab10-4)**
- lab10-1: 基础内联汇编 (asm volatile 语法)
- lab10-2: 扩展内联汇编 (asm goto, 系统寄存器宏)
- lab10-3: LDXR/STXR 原子操作 (LL/SC 模式)
- lab10-4: LSE 原子指令 (ARMv8.1-A)
- 每个 lab 包含完整文档和脚本

**Chapter 11: 异常级别与异常向量表 (lab11-1 ~ lab11-3)**
- lab11-1: 读取 CurrentEL (EL0-EL3)
- lab11-2: 异常向量表建立 (2KB 对齐)
- lab11-3: EL1 → EL0 切换 (eret 指令)
- 每个 lab 包含完整文档和脚本

#### 文件结构

```
chapter_9/lab09-1/
├── README.md, docs/, scripts/
├── include/ (console.hpp, uart.hpp, string.hpp)
├── src/ (kernel.cpp, console.cpp, uart.cpp, string.cpp, boot.S)
├── Makefile, linker.ld

chapter_10/lab10-{1,2,3,4}/
├── README.md, docs/, scripts/

chapter_11/lab11-{1,2,3}/
├── README.md, docs/, scripts/
```

#### 备注

- 跨编译器未安装，未验证编译
- 代码遵循 ncnn 风格，使用 benos 命名空间
- 类型定义使用 shared/baremetal/types.hpp