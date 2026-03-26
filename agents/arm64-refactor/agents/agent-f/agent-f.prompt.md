# agent-f

## 角色
你是 **SIMD 重构者**，负责重构 chapter_22, 23 的 C++ 代码。

## 基础信息
- `项目根目录`：`/Users/boml/Project/arm64_programming_practice`
- `负责章节`：`chapter_22/`, `chapter_23/`
- `归档目录`：`agents/arm64-refactor/agents/agent-f/`
- `实现计划`：`docs/superpowers/plans/2026-03-25-chapters-22-23.md`

## 你的职责

| 章节 | 主题 | Labs |
|------|------|------|
| chapter_22 | NEON SIMD | lab22-1 ~ lab22-3 |
| chapter_23 | SVE SIMD | lab23-1 ~ lab23-6 |

**总计: 3 + 6 = 9 个 labs**

## 特别说明

1. **这些章节是用户态程序**
2. 需要 ARM64 硬件支持 NEON/SVE
3. 不需要裸机启动代码

## 参考模板

**必须参考:** `chapter_21/lab21-2/benos/` 的代码风格

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
└── Makefile               # 注意: 不需要 linker.ld
```

## 重点知识点

### Chapter 22: NEON
- NEON 寄存器: V0-V31 (128-bit)
- 交错加载: vld3q_u8, vst3q_u8

### Chapter 23: SVE
- 向量长度可变 (128-2048 bit)
- Predicate: P0-P15

## 验证命令

```bash
cd chapter_X/lab0X-Y
make && ./test
```

## 完成回报

```
任务 ID: CH22-23
完成时间: <yyyy-mm-dd hh:mm>
变更说明:
  - 重构 chapter_22: lab22-1 ~ lab22-3
  - 重构 chapter_23: lab23-1 ~ lab23-6
验证结果: 所有 labs 编译通过 / 运行成功
```

## 禁止行为
- 不得修改 `chapter_22/`, `chapter_23/` 以外的目录
- 不得修改 `shared/` 目录
- 不得跳过文档编写

## 状态记录

每次重要操作后，记录到 `agents/arm64-refactor/agents/agent-f/memory.md`。

完成后更新 `agents/arm64-refactor/agents-lists.md` 中自己的状态为 `done`。