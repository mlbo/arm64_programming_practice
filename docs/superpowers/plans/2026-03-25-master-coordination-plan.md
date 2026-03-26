# ARM64 编程实践仓库 C++ 重构 - 总体协调计划

## 执行策略

### Phase 1: 参考模板 (chapter_21)
- 先完成 chapter_21 BenOS 作为参考模板
- 创建标准化的代码风格、目录结构、文档模板
- 验证构建和运行

### Phase 2: 并行迁移 (多 agent 协调)

**Agent 分组:**

| Agent | 章节 | 依赖关系 |
|-------|------|----------|
| Agent-A | chapter_2, chapter_3, chapter_4 | 共享: 启动代码、UART 驱动 |
| Agent-B | chapter_5, chapter_6, chapter_7, chapter_8 | 共享: 进程切换、内存操作 |
| Agent-C | chapter_9, chapter_10, chapter_11 | 共享: printk、异常处理 |
| Agent-D | chapter_12, chapter_13, chapter_14, chapter_15 | 共享: 中断、GIC、Timer |
| Agent-E | chapter_16, chapter_18, chapter_20 | 共享: 原子操作、内存模型 |
| Agent-F | chapter_22, chapter_23 | SIMD (独立) |

**协调机制:**

1. **共享基础设施先行** - 在 Phase 1.5 创建共享头文件
2. **文件锁机制** - 避免多个 agent 同时修改同一文件
3. **依赖声明** - 每个章节计划声明依赖的共享组件
4. **合并策略** - 按顺序合并，处理冲突

---

## 共享基础设施

```
shared/
├── baremetal/
│   ├── types.hpp           # 类型定义 (所有章节共用)
│   ├── error.hpp           # 错误码
│   ├── new.hpp             # placement new
│   ├── platform.h          # 平台检测宏
│   └── runtime/
│       ├── cxx_runtime.hpp # C++ 运行时支持
│       └── cxx_runtime.cpp
└── make/
    ├── baremetal.mk        # 裸机构建规则
    └── simd.mk             # SIMD 构建规则
```

**所有 agent 必须使用 shared/baremetal/types.hpp**

---

## ncnn 代码规范要点

参考 ncnn 项目风格，本项目采用以下规范：

### 类型定义 (与 ncnn 一致)

```cpp
namespace benos {

// 基础类型
using u8   = uint8_t;
using u16  = uint16_t;
using u32  = uint32_t;
using u64  = uint64_t;
using usize = size_t;
using uptr = uintptr_t;

using i8   = int8_t;
using i16  = int16_t;
using i32  = int32_t;
using i64  = int64_t;
using isize = ptrdiff_t;

} // namespace benos
```

### 错误处理 (ncnn 风格)

```cpp
// 返回 int，0 表示成功，负数表示错误
auto do_something() -> int {
    if (error_condition) {
        return -1;  // 错误码
    }
    return 0;  // 成功
}

// 判断宏
#define BENOS_OK(result)    ((result) >= 0)
#define BENOS_FAILED(result) ((result) < 0)
```

### C/C++ API 双层设计 (参考 ncnn C API)

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

// C API (兼容性层)
extern "C" {
    void benos_uart_init();
    void benos_uart_send(char c);
    char benos_uart_recv();
}
```

### 裸机环境编译选项 (参考 ncnn 嵌入式配置)

```cmake
# 禁用标准库依赖
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions -fno-rtti")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -nostdlib -ffreestanding")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-threadsafe-statics")

# 可选: 禁用 RTTI 和异常 (类似 ncnn NCNN_DISABLE_RTTI)
# option(BENOS_DISABLE_RTTI "Disable RTTI" ON)
# option(BENOS_DISABLE_EXCEPTION "Disable exceptions" ON)
```

### 类设计规范

```cpp
class Example {
public:                         // 公有成员在前
    int public_member = 0;

    void public_method();       // 方法在后

private:
    int private_member_ = 0;    // 私有成员后缀 _
};
```

---

## 章节计划文件索引

| 章节组 | 计划文件 | Agent |
|--------|----------|-------|
| chapter_21 | `plans/2026-03-25-benos-cpp-refactor.md` | 主 agent |
| chapter_2-4 | `plans/2026-03-25-chapters-2-4.md` | Agent-A |
| chapter_5-8 | `plans/2026-03-25-chapters-5-8.md` | Agent-B |
| chapter_9-11 | `plans/2026-03-25-chapters-9-11.md` | Agent-C |
| chapter_12-15 | `plans/2026-03-25-chapters-12-15.md` | Agent-D |
| chapter_16,18,20 | `plans/2026-03-25-chapters-16-18-20.md` | Agent-E |
| chapter_22-23 | `plans/2026-03-25-chapters-22-23.md` | Agent-F |

---

## 文件修改矩阵

| 文件/目录 | Agent-A | Agent-B | Agent-C | Agent-D | Agent-E | Agent-F |
|-----------|---------|---------|---------|---------|---------|---------|
| shared/baremetal/ | - | - | - | - | - | - |
| chapter_2/ | ✅ | - | - | - | - | - |
| chapter_3/ | ✅ | - | - | - | - | - |
| chapter_4/ | ✅ | - | - | - | - | - |
| chapter_5/ | - | ✅ | - | - | - | - |
| chapter_6/ | - | ✅ | - | - | - | - |
| chapter_7/ | - | ✅ | - | - | - | - |
| chapter_8/ | - | ✅ | - | - | - | - |
| chapter_9/ | - | - | ✅ | - | - | - |
| chapter_10/ | - | - | ✅ | - | - | - |
| chapter_11/ | - | - | ✅ | - | - | - |
| chapter_12/ | - | - | - | ✅ | - | - |
| chapter_13/ | - | - | - | ✅ | - | - |
| chapter_14/ | - | - | - | ✅ | - | - |
| chapter_15/ | - | - | - | ✅ | - | - |
| chapter_16/ | - | - | - | - | ✅ | - |
| chapter_18/ | - | - | - | - | ✅ | - |
| chapter_20/ | - | - | - | - | ✅ | - |
| chapter_22/ | - | - | - | - | - | ✅ |
| chapter_23/ | - | - | - | - | - | ✅ |

---

## 执行流程

```
Phase 1 (顺序):
  └── chapter_21 完成 → 提交到 main

Phase 1.5 (顺序):
  └── 创建 shared/baremetal/ → 提交

Phase 2 (并行):
  ├── Agent-A: chapter_2-4
  ├── Agent-B: chapter_5-8
  ├── Agent-C: chapter_9-11
  ├── Agent-D: chapter_12-15
  ├── Agent-E: chapter_16,18,20
  └── Agent-F: chapter_22-23

Phase 3 (合并):
  └── 按顺序合并各 agent 的 PR
```

---

## Agent 协调规则

1. **不要修改 shared/** - 共享文件由主 agent 维护
2. **使用标准模板** - 参照 chapter_21 的代码风格
3. **保持文档一致** - 每章必须有 README.md, docs/knowledge.md 等
4. **独立提交** - 每个 agent 在自己的分支工作
5. **冲突处理** - 如果需要修改共享依赖，提 issue 给主 agent

---

## 构建系统规范

### 裸机章节 Makefile 模板

```makefile
ARMGNU ?= aarch64-linux-gnu

board ?= rpi4

ifeq ($(board), rpi4)
COPS += -DCONFIG_BOARD_PI4B
QEMU_FLAGS  += -machine raspi4b
endif

# C++ 编译选项 (参考 ncnn 嵌入式配置)
CXXOPS = -std=c++17 -g -Wall -nostdlib -nostdinc
CXXOPS += -fno-exceptions -fno-rtti -ffreestanding
CXXOPS += -fno-threadsafe-statics -mgeneral-regs-only
CXXOPS += -Iinclude -I../../shared/baremetal

# C 编译选项
COPS = -g -Wall -nostdlib -nostdinc -Iinclude -mgeneral-regs-only
ASMOPS = -g -Iinclude

BUILD_DIR = build
SRC_DIR = src

all : benos.bin

clean :
	rm -rf $(BUILD_DIR) *.bin *.elf *.map

# ... 其他规则 ...
```

### SIMD 章节 Makefile 模板

```makefile
# SIMD 章节使用标准 GCC，不需要裸机标志
CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2

# NEON 支持
CXXFLAGS_NEON = -march=armv8-a+simd

# SVE 支持
CXXFLAGS_SVE = -march=armv8-a+sve

neon_test: src/neon_test.cpp
	$(CXX) $(CXXFLAGS) $(CXXFLAGS_NEON) -o $@ $<

sve_test: src/sve_test.cpp
	$(CXX) $(CXXFLAGS) $(CXXFLAGS_SVE) -o $@ $<
```

---

## 验证标准

每个章节完成后必须验证:
- [ ] 编译成功: `./scripts/build.sh`
- [ ] 运行成功: `./scripts/run.sh` (裸机章节)
- [ ] 文档完整: README.md + docs/
- [ ] 代码风格: 使用 namespace benos/arm64lab
- [ ] 错误处理: 返回 int，0=成功，负数=错误
- [ ] API 设计: 提供 C++ 和 C 两层 API (可选)