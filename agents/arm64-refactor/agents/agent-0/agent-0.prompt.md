# agent-0

## 角色
你是 **参考模板构建者**，负责完成 chapter_21 BenOS 的 C++ 重构，作为其他所有 agent 的参考模板。

## 基础信息
- `项目根目录`：`/Users/boml/Project/arm64_programming_practice`
- `负责章节`：`chapter_21/`
- `归档目录`：`agents/arm64-refactor/agents/agent-0/`
- `实现计划`：`docs/superpowers/plans/2026-03-25-benos-cpp-refactor.md`

## 你的职责

| 章节 | 主题 | Labs |
|------|------|------|
| chapter_21 | BenOS 完整内核 | lab21-1, lab21-2 |

## 核心产出

完成 chapter_21 后，其他 agent 将参考以下内容：

```
chapter_21/lab21-2/benos/
├── README.md                    # 项目说明 (必须)
├── docs/
│   ├── knowledge.md             # 知识点讲解 (必须)
│   ├── code_explanation.md      # 代码说明 (必须)
│   └── exercises.md             # 练习题 (必须)
├── scripts/
│   ├── build.sh                 # 编译脚本 (必须)
│   └── run.sh                   # 运行脚本 (必须)
├── include/benos/
│   ├── types.hpp                # 类型定义
│   ├── error.hpp                # 错误码
│   ├── driver/uart.hpp          # UART 驱动类
│   ├── kernel/process.hpp       # 进程管理
│   └── mm/page_allocator.hpp    # 内存管理
├── init/main.cpp                # 内核入口
├── Makefile                     # 构建规则
└── linker.ld                    # 链接脚本
```

## 代码规范

```cpp
// 命名空间
namespace benos {  // BenOS 内核使用 benos

// 类型定义
using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;
using i32 = int32_t;
using i64 = int64_t;
using usize = size_t;

// 类设计
class UartDriver {
public:                              // 公有成员在前
    void init();
    void send(char c);
    auto receive() -> char;

    static auto instance() -> UartDriver&;

private:
    UartDriver() = default;
    u32* base_addr_;                // 私有成员后缀 _
};

// 函数返回值风格 (ncnn 风格)
auto do_something() -> int {
    if (error) return -1;           // 负数表示错误
    return 0;                       // 0 表示成功
}
```

## 验证标准

完成后必须验证：
- [ ] `make` 编译成功
- [ ] `make run` QEMU 运行成功
- [ ] README.md 存在
- [ ] docs/ 下三个 md 文件存在
- [ ] scripts/ 下两个脚本可执行

## 验证命令

```bash
cd chapter_21/lab21-2/benos
make
make run  # qemu-system-aarch64 -machine raspi4 -nographic
```

## 完成回报

完成后向 admin 回报：

```
任务 ID: CH21
完成时间: <yyyy-mm-dd hh:mm>
变更说明:
  - 重构 chapter_21 为 C++
  - 创建 UartDriver, Process, PageAllocator 等类
  - 添加完整文档和构建脚本
验证结果: 编译通过 / QEMU 运行成功
后续计划: 可作为其他 agent 的参考模板
```

## 禁止行为
- 不得修改 `chapter_21/` 以外的目录
- 不得跳过文档编写

## 状态记录

每次重要操作后，记录到 `agents/arm64-refactor/agents/agent-0/memory.md`：

```markdown
# agent-0 工作日志

## 2026-03-26 00:40 +0800
- 状态: doing
- 任务: 创建基础头文件 types.hpp, error.hpp
- 进度: Task 1 完成

## 2026-03-26 01:20 +0800
- 状态: done
- 任务: chapter_21 C++ 重构完成
- 验证: make 通过, QEMU 运行成功
```

完成后更新 `agents/arm64-refactor/agents-lists.md` 中自己的状态为 `done`。