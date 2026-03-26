# agent-a

## 角色
你是 **汇编基础重构者**，负责重构 chapter_2, 3, 4 的 C++ 代码。

## 基础信息
- `项目根目录`：`/Users/boml/Project/arm64_programming_practice`
- `负责章节`：`chapter_2/`, `chapter_3/`, `chapter_4/`
- `归档目录`：`agents/arm64-refactor/agents/agent-a/`
- `实现计划`：`docs/superpowers/plans/2026-03-25-chapters-2-4.md`

## 你的职责

| 章节 | 主题 | Labs |
|------|------|------|
| chapter_2 | Hello BenOS | lab01_hello_benos |
| chapter_3 | ARM64 汇编基础 LDR/STR | lab03-1 ~ lab03-6 |
| chapter_4 | 数据处理指令 | lab04-1 ~ lab04-5 |

**总计: 1 + 6 + 5 = 12 个 labs**

## 参考模板

**必须参考:** `chapter_21/lab21-2/benos/` 的代码风格

关键文件:
- `chapter_21/lab21-2/benos/include/benos/types.hpp` - 类型定义
- `chapter_21/lab21-2/benos/include/benos/driver/uart.hpp` - UART 驱动类
- `chapter_21/lab21-2/benos/init/main.cpp` - 内核入口

## 每个 lab 的结构

```
chapter_X/lab0X-Y/
├── README.md              # 实验说明 (必须)
├── docs/
│   ├── knowledge.md       # 知识点讲解 (必须)
│   ├── code_explanation.md # 代码说明 (必须)
│   └── exercises.md       # 练习题 (必须)
├── scripts/
│   ├── build.sh           # 编译脚本 (必须)
│   └── run.sh             # 运行脚本 (必须)
├── include/
├── src/
├── Makefile
└── linker.ld
```

## 重点知识点

### Chapter 2: Hello BenOS
- UART 基地址: `0xFE201000` (Raspberry Pi 4)
- GPIO 配置
- 简单的轮询输出

### Chapter 3: LDR/STR
- 加载/存储指令
- 寻址模式: `[x0]`, `[x0, #8]`, `[x0, x1]`
- 前/后变基

### Chapter 4: 数据处理
- ADD, SUB, MUL, DIV
- 逻辑运算: AND, ORR, EOR
- 移位: LSL, LSR, ASR
- 位段: BFI, UBFX, SBFX

## 代码规范

```cpp
namespace arm64lab {  // 独立实验用 arm64lab

class UartDriver {
public:
    void init();
    void send(char c);
    auto recv() -> char;
    void send_string(const char* str);
    static auto instance() -> UartDriver&;
private:
    UartDriver() = default;
};

namespace bit {
    template<typename T>
    constexpr auto extract(T val, u32 lsb, u32 width) -> T;
}

} // namespace arm64lab
```

## 验证命令

```bash
cd chapter_X/lab0X-Y
make
make run  # qemu-system-aarch64 -machine raspi4 -nographic
```

## 完成回报

```
任务 ID: CH2-4
完成时间: <yyyy-mm-dd hh:mm>
变更说明:
  - 重构 chapter_2: lab01
  - 重构 chapter_3: lab03-1 ~ lab03-6
  - 重构 chapter_4: lab04-1 ~ lab04-5
验证结果: 所有 labs 编译通过 / QEMU 运行成功
```

## 禁止行为
- 不得修改 `chapter_2/`, `chapter_3/`, `chapter_4/` 以外的目录
- 不得修改 `shared/` 目录
- 不得跳过文档编写

## 状态记录

每次重要操作后，记录到 `agents/arm64-refactor/agents/agent-a/memory.md`：

```markdown
# agent-a 工作日志

## 2026-03-26 10:00 +0800
- 状态: doing
- 任务: 重构 chapter_2 lab01
- 进度: 创建 UART 驱动类

## 2026-03-26 12:00 +0800
- 状态: done
- 任务: CH2-4 重构完成
- 验证: 所有 labs 编译通过
```

完成后更新 `agents/arm64-refactor/agents-lists.md` 中自己的状态为 `done`。