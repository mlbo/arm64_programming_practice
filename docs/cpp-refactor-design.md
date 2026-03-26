# ARM64 编程实践仓库全面 C++ 重构设计

## 1. 项目概述

### 1.1 目标

将 ARM64 编程实践仓库从现有代码（混乱的 C/C++ 混合）全面重构为规范的 C++ 项目：
- 参考 ncnn 项目代码风格
- 所有代码使用命名空间 `benos` 或 `arm64lab`
- 每个章节包含完整的 MD 文档（知识点、代码说明、编译运行、练习题）
- 每个章节提供 `build.sh` 和 `run.sh` 脚本

### 1.2 章节列表（20 个章节）

| 章节 | 主题 | Labs 数量 |
|------|------|-----------|
| ch2 | Hello BenOS | 1 |
| ch3 | ARM64 汇编基础 - LDR/STR | 6 |
| ch4 | ARM64 数据处理指令 | 5 |
| ch5 | ARM64 比较与跳转指令 | 3 |
| ch6 | ARM64 基础指令综合 | 4 |
| ch7 | 栈与局部变量 | 1 |
| ch8 | C 与汇编混合编程 | 4 |
| ch9 | 格式化输出 printk | 1 |
| ch10 | 汇编语言进阶与内联汇编 | 4 + examples |
| ch11 | 异常级别与异常向量表 | 3 |
| ch12 | 中断处理与栈帧 | 1 |
| ch13 | GIC 中断控制器 | 1 |
| ch14 | 中断与定时器管理 | 2 |
| ch15 | 缓存管理与页表遍历 | 2 |
| ch16 | False Sharing 演示 | 2 |
| ch18 | 内存一致性模型验证 | litmus tests |
| ch20 | 原子操作 | 1 |
| ch21 | BenOS 完整内核 | 2 |
| ch22 | NEON SIMD | 3 |
| ch23 | SVE SIMD | 6 |

---

## 2. 代码规范（参考 ncnn）

### 2.1 命名空间

```cpp
// 裸机内核使用 benos
namespace benos {
    // ...
}

// 独立实验使用 arm64lab
namespace arm64lab {
    // ...
}
```

### 2.2 类型定义

```cpp
namespace benos {

using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;
using i8  = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using usize = size_t;
using uptr = uintptr_t;

} // namespace benos
```

### 2.3 类设计原则

```cpp
class Example {
public:                         // 公有成员在前
    int public_member = 0;

    void public_method();       // 方法在后

private:
    int private_member_ = 0;    // 私有成员后缀 _
};
```

### 2.4 错误处理

```cpp
// 返回 int，0 表示成功，负数表示错误 (ncnn 风格)
auto do_something() -> int {
    if (error_condition) {
        return -1;
    }
    return 0;  // 成功
}

// 判断宏 (参考 ncnn)
#define BENOS_OK(result)    ((result) >= 0)
#define BENOS_FAILED(result) ((result) < 0)
```

### 2.5 C/C++ API 双层设计 (参考 ncnn C API)

```cpp
// C++ API
namespace benos {
class UartDriver {
public:
    void init();
    void send(char c);
    auto recv() -> char;
};
} // namespace benos

// C API (兼容性层，类似 ncnn c_api.h)
extern "C" {
    void benos_uart_init(void);
    void benos_uart_send(char c);
    char benos_uart_recv(void);
}
```

### 2.6 禁用的 C++ 特性 (参考 ncnn 嵌入式配置)

裸机环境禁用：
- `exceptions` (`-fno-exceptions`)
- `RTTI` (`-fno-rtti`)
- `STL` (无标准库)
- `threadsafe-statics` (`-fno-threadsafe-statics`)

```cmake
# 编译选项 (参考 ncnn NCNN_DISABLE_RTTI/NCNN_DISABLE_EXCEPTION)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions -fno-rtti")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -nostdlib -ffreestanding")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-threadsafe-statics")
```

### 2.7 内存对齐 (参考 ncnn 内存优化)

```cpp
// 缓存行对齐 (避免 false sharing)
template<typename T>
struct alignas(64) CacheLineAligned {
    T value;
};

// 异常向量表对齐 (必须 2KB 对齐)
struct alignas(2048) ExceptionVector {
    u8 entries[16][128];
};
```

---

## 3. 章节内容分析

### 3.1 Track A: Bare-metal 内核基础 (ch2-16, ch18, ch20-21)

#### Chapter 2: Hello BenOS
- **教学目标**: 建立第一个 ARM64 裸机程序
- **核心内容**: 启动流程、UART 输出
- **关键文件**: boot.S, mm.S, pl_uart.c, linker.ld
- **C++ 设计**: `UartDriver` 类

#### Chapter 3: ARM64 汇编基础
- **教学目标**: LDR/STR 指令的多种寻址模式
- **核心内容**: 偏移模式、前/后变基、Label 访问
- **Labs**: lab03-1 ~ lab03-6 (memcpy, memset)
- **C++ 设计**: `MemoryOps` 命名空间

#### Chapter 4: 数据处理指令
- **教学目标**: ADDS/CMP/ADC, 位段指令
- **核心内容**: 条件标志位, BFI/UBFX/SBFX
- **C++ 设计**: `BitOps` 命名空间

#### Chapter 5: 比较与跳转指令
- **教学目标**: CMP/CMN, CSEL, BL
- **核心内容**: 条件选择, 函数调用
- **C++ 设计**: 内联汇编模板

#### Chapter 6: 基础指令综合
- **教学目标**: 内存访问、原子操作入门
- **核心内容**: LDXR/STXR 原子操作
- **C++ 设计**: `Atomic<T>` 封装

#### Chapter 7: 栈与局部变量
- **教学目标**: 栈帧建立、局部变量存储
- **C++ 设计**: 保持汇编示例

#### Chapter 8: C 与汇编混合编程
- **教学目标**: C 调汇编、汇编调 C
- **核心内容**: ARM64 ABI 调用约定
- **C++ 设计**: `extern "C"` 链接

#### Chapter 9: 格式化输出
- **教学目标**: printk 实现
- **核心内容**: 可变参数、格式化
- **C++ 设计**: `Console` 类

#### Chapter 10: 汇编进阶
- **教学目标**: 内联汇编、原子操作
- **核心内容**: LDXR/STXR, LSE STSET
- **C++ 设计**: `std::atomic` 或自定义封装

#### Chapter 11: 异常级别与异常向量表
- **教学目标**: EL0-EL3, VBAR_EL1
- **核心内容**: EL 切换、异常向量表布局
- **C++ 设计**: `ExceptionLevel` 类, `alignas(11)` 向量表

#### Chapter 12: 中断处理
- **教学目标**: pt_regs 栈帧、定时器中断
- **核心内容**: kernel_entry/kernel_exit 宏
- **C++ 设计**: `ExceptionFrame` 结构体 (POD)

#### Chapter 13: GIC 中断控制器
- **教学目标**: GICv2 驱动实现
- **核心内容**: Distributor, CPU Interface
- **C++ 设计**: `GicV2Controller` 类

#### Chapter 14: 中断与定时器管理
- **教学目标**: GIC + Timer + MMU
- **核心内容**: 中断处理流程、页表
- **C++ 设计**: `IrqController`, `PageTable`

#### Chapter 15: 缓存管理
- **教学目标**: 缓存层次结构、缓存维护
- **核心内容**: CTR_EL0, CLIDR_EL1, dc civac
- **C++ 设计**: `CacheInfo`, `CacheMaintainer` 类

#### Chapter 16: False Sharing
- **教学目标**: 缓存一致性、缓存行对齐
- **核心内容**: MESI 协议、性能影响
- **C++ 设计**: `alignas(64)`, `CacheLineAligned<T>`

#### Chapter 18: 内存一致性模型
- **教学目标**: Litmus 测试、内存屏障
- **核心内容**: herdtools7, Message Passing
- **C++ 设计**: `std::atomic_thread_fence`

#### Chapter 20: 原子操作
- **教学目标**: LDXR/STXR 实现
- **核心内容**: LL/SC 模式
- **C++ 设计**: `std::atomic<T>`

#### Chapter 21: BenOS 完整内核
- **教学目标**: 完整的微型操作系统
- **核心内容**:
  - 进程管理: `task_struct`, `cpu_context`, `do_fork`, `switch_to`
  - 内存管理: `get_free_page`, `mem_init`
  - 驱动: UART, Timer
  - 中断: IRQ 处理, 异常向量表
- **C++ 设计**:
  - `Process` 类 (替代 task_struct)
  - `PageAllocator` 类
  - `CpuContext` 类
  - `ExceptionFrame` 类
  - `Console` 类
  - `UartDriver` 抽象基类

### 3.2 Track B: SIMD (ch22-23)

#### Chapter 22: NEON
- **教学目标**: NEON intrinsics、自动向量化
- **核心内容**:
  - RGB/BGR 转换: `vld3q_u8`, `vst3q_u8`
  - 矩阵乘法: `vfmaq_laneq_f32`
  - 自动向量化对比
- **C++ 设计**: 保留 intrinsics 可见性，不封装

#### Chapter 23: SVE
- **教学目标**: SVE predicate、VLA 编程
- **核心内容**:
  - `whilelo`, `ptrue` predicate 操作
  - `ld3b`/`st3b` 交错加载/存储
  - FFR (First Fault Register)
  - memcpy, strcmp, 矩阵乘法
- **C++ 设计**: 保留汇编实现，展示 SVE 特性

---

## 4. 每章标准化结构

```
chapter_XX/
├── README.md                    # 章节概述
├── labXX-Y/
│   ├── README.md                # 实验说明
│   ├── docs/
│   │   ├── knowledge.md         # 知识点讲解
│   │   ├── code_explanation.md  # 代码说明
│   │   └── exercises.md         # 练习题
│   ├── scripts/
│   │   ├── build.sh             # 编译脚本
│   │   └── run.sh               # 运行脚本
│   ├── include/
│   │   └── ...
│   ├── src/
│   │   └── ...
│   ├── Makefile
│   └── linker.ld
└── ...
```

### 4.1 README.md 模板

```markdown
# Chapter X: 章节主题

## 概述

本章学习 ARM64 的 XXX 功能...

## 学习目标

1. 理解 XXX
2. 掌握 XXX
3. 实现 XXX

## 实验

| 实验 | 内容 | 难度 |
|------|------|------|
| labX-1 | XXX | ⭐ |
| labX-2 | XXX | ⭐⭐ |

## 编译运行

```bash
cd labX-1
./scripts/build.sh
./scripts/run.sh
```

## 参考资料

- ARM Architecture Reference Manual
- ...
```

### 4.2 knowledge.md 模板

```markdown
# 知识点讲解

## 1. 核心概念

### 1.1 XXX

XXX 是 ARM64 体系结构中的...

### 1.2 YYY

...

## 2. 相关指令

| 指令 | 功能 | 示例 |
|------|------|------|
| LDR | 加载 | `ldr x0, [x1]` |
| STR | 存储 | `str x0, [x1]` |

## 3. 代码示例

\`\`\`asm
// 示例代码
\`\`\`

## 4. 注意事项

- 注意点 1
- 注意点 2
```

### 4.3 build.sh 模板

```bash
#!/bin/bash
# build.sh - Build script for ARM64 Lab
# Usage: ./build.sh [clean|run|debug]

set -e

LAB_DIR=$(dirname "$(readlink -f "$0")")
LAB_NAME=$(basename "$LAB_DIR")

# Toolchain
CROSS_COMPILE=${CROSS_COMPILE:-aarch64-linux-gnu-}
CC="${CROSS_COMPILE}gcc"
CXX="${CROSS_COMPILE}g++"
OBJCOPY="${CROSS_COMPILE}objcopy"

# QEMU
QEMU=${QEMU:-qemu-system-aarch64}
QEMU_MACHINE=${QEMU_MACHINE:-raspi4b}

# Flags
CFLAGS="-Wall -O2 -ffreestanding -nostdlib -nostartfiles"
CXXFLAGS="-std=c++17 -Wall -O2 -ffreestanding -nostdlib -nostartfiles -fno-exceptions -fno-rtti"
LDFLAGS="-nostdlib"

# Includes
INCLUDES="-I${LAB_DIR}/include -I${LAB_DIR}/../../shared/baremetal"

# ... (build logic)
```

---

## 5. 执行计划

### Phase 1: 参考模板 (chapter_21)

1. 重构 chapter_21 BenOS 完整内核
2. 创建规范化的 C++ 类设计
3. 编写完整的 MD 文档
4. 提供 build.sh 和 run.sh 脚本
5. 验证编译和运行

### Phase 2-6: 并行迁移其他章节

按技术 track 分组并行执行：
- **Track A**: Bare-metal 内核基础 (ch2-16, ch18, ch20)
- **Track B**: SIMD (ch22-23)

每个章节参照 chapter_21 模板风格进行重构。

---

## 6. C++ 类设计总览

### 6.1 核心类（chapter_21 BenOS）

```cpp
namespace benos {

// 进程管理
class Process;
class ProcessManager;

// 内存管理
class PageAllocator;

// 架构相关
namespace arch::arm64 {
    class CpuContext;
    class ExceptionFrame;
    class IrqController;
}

// 驱动
namespace mach {
    class UartDriver;
    class TimerDriver;
}

// 工具库
namespace lib {
    class Console;
}

} // namespace benos
```

### 6.2 工具类（独立实验）

```cpp
namespace arm64lab {

// 位操作
namespace bit {
    template<typename T>
    constexpr T extract(T val, unsigned lsb, unsigned width);

    template<typename T>
    constexpr T insert(T dst, T src, unsigned lsb, unsigned width);
}

// 内存操作
namespace mem {
    void* memset(void* dst, int c, size_t n);
    void* memcpy(void* dst, const void* src, size_t n);
}

// 原子操作
template<typename T>
class Atomic;

// 缓存对齐
template<typename T>
struct CacheLineAligned {
    alignas(64) T value;
};

} // namespace arm64lab
```

---

## 7. 验证标准

### 7.1 编译验证
- 所有章节 `./scripts/build.sh` 成功
- 无编译警告

### 7.2 运行验证
- Bare-metal 章节: QEMU raspi4b 启动成功
- SIMD 章节: ARM64 主机运行成功

### 7.3 文档验证
- 每个 lab 有 README.md
- 每个章节有知识点文档
- 每个章节有练习题