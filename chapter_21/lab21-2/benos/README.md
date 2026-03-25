# BenOS - ARM64 进程创建实验

BenOS 是一个用于学习ARM64操作系统开发的迷你操作系统内核。本实验对应《ARM64体系结构：编程与实践》第21章的进程创建实验。

## 实验目的

1. 了解进程控制块（PCB）的设计与实现
2. 了解进程创建/执行的过程
3. 理解ARM64异常级别和上下文切换机制

## 特性

- 纯ARM64架构设计
- UART控制台输出
- 简单页分配器
- 进程管理与调度
- 定时器中断
- C++面向对象封装

## 环境要求

### 工具链

- ARM64交叉编译工具链 (`aarch64-linux-gnu-gcc`)
- QEMU ARM64模拟器 (`qemu-system-aarch64`)

### 安装工具链

**Ubuntu/Debian:**
```bash
sudo apt-get install gcc-aarch64-linux-gnu qemu-system-arm
```

**macOS:**
```bash
brew install aarch64-elf-gcc qemu
```

## 构建与运行

### 使用Makefile

```bash
# 构建内核
make

# 在QEMU中运行
make run

# 调试模式（GDB服务器在1234端口）
make debug

# 清理构建产物
make clean

# 深度清理（包括.config）
make mrproper

# 生成默认配置
make defconfig
```

### 使用构建脚本

```bash
# 构建
./scripts/build.sh build

# 运行
./scripts/build.sh run

# 调试
./scripts/build.sh debug

# 清理
./scripts/build.sh clean
```

### 调试

```bash
# 终端1：启动QEMU调试模式
make debug

# 终端2：连接GDB
aarch64-linux-gnu-gdb benos -ex 'target remote :1234'
(gdb) break kernel_main
(gdb) continue
```

## 项目结构

```
benos/
├── init/
│   └── main.cpp              # 内核入口点
├── include/benos/            # C++头文件
│   ├── types.hpp             # 类型定义
│   ├── benos.hpp             # 主头文件
│   ├── driver/
│   │   ├── uart.hpp          # UART驱动
│   │   └── timer.hpp         # 定时器驱动
│   ├── kernel/
│   │   ├── console.hpp       # 控制台
│   │   └── process.hpp       # 进程管理
│   └── mm/
│       └── page_allocator.hpp # 页分配器
├── kernel/
│   ├── fork.c                # 进程创建实现
│   └── process_manager.cpp   # 进程管理器封装
├── mm/
│   └── page_alloc.cpp        # 页分配器实现
├── lib/
│   ├── printk.cpp            # 打印函数
│   ├── uart_wrapper.cpp      # UART封装
│   └── timer_wrapper.cpp     # 定时器封装
├── arch/arm64/
│   ├── boot/                 # 启动代码
│   ├── kernel/
│   │   ├── head.S            # 汇编入口
│   │   ├── entry.S           # 异常入口
│   │   └── traps.c           # 异常处理
│   └── mach-rpi/             # 树莓派硬件抽象
├── scripts/
│   ├── build.sh              # 构建脚本
│   └── run.sh                # 运行脚本
└── docs/
    ├── knowledge.md          # 知识点讲解
    ├── code_explanation.md   # 代码说明
    └── exercises.md          # 练习题
```

## 关键数据结构

### 进程控制块 (task_struct)

```c
struct task_struct {
    enum task_state state;      // 进程状态
    enum task_flags flags;      // 进程标志
    long count;                 // 时间片
    int priority;               // 优先级
    int pid;                    // 进程ID
    struct cpu_context cpu_context; // CPU上下文
};
```

### 进程状态

| 状态 | 说明 |
|------|------|
| TASK_RUNNING | 运行中或就绪 |
| TASK_INTERRUPTIBLE | 可中断睡眠 |
| TASK_UNINTERRUPTIBLE | 不可中断睡眠 |
| TASK_ZOMBIE | 僵尸进程 |
| TASK_STOPPED | 已停止 |

## 文档

- [知识点讲解](docs/knowledge.md) - ARM64异常级别、进程管理等核心概念
- [代码说明](docs/code_explanation.md) - 主要代码结构和函数说明
- [练习题](docs/exercises.md) - 延伸练习和思考题

## 参考资料

- 《ARM64体系结构：编程与实践》
- Linux内核源码
- ARM Architecture Reference Manual ARMv8