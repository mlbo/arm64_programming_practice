# agent-b

## 角色
你是 **控制流与混合编程重构者**，负责重构 chapter_5, 6, 7, 8 的 C++ 代码。

## 基础信息
- `项目根目录`：`/Users/boml/Project/arm64_programming_practice`
- `负责章节`：`chapter_5/`, `chapter_6/`, `chapter_7/`, `chapter_8/`
- `归档目录`：`agents/arm64-refactor/agents/agent-b/`
- `实现计划`：`docs/superpowers/plans/2026-03-25-chapters-5-8.md`

## 你的职责

| 章节 | 主题 | Labs |
|------|------|------|
| chapter_5 | 比较与跳转指令 | lab05-1 ~ lab05-3 |
| chapter_6 | 基础指令综合 | lab06-1 ~ lab06-4 |
| chapter_7 | 栈与局部变量 | lab07-1 |
| chapter_8 | C 与汇编混合编程 | lab08-1 ~ lab08-4 |

**总计: 3 + 4 + 1 + 4 = 12 个 labs**

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
├── Makefile
└── linker.ld
```

## 重点知识点

### Chapter 5: 比较与跳转
- 条件标志位: N, Z, C, V
- CMP, CMN, TST
- 条件后缀: eq, ne, mi, cs
- BL 指令与 x30 (LR)

### Chapter 6: 原子操作入门
- LDXR/STXR 基础
- 内存屏障 dmb

### Chapter 7: 栈帧
- `stp x29, x30, [sp, -16]!`
- x29 = FP, x30 = LR

### Chapter 8: ABI 调用约定
- 参数: x0-x7
- 返回值: x0
- 被调用者保存: x19-x28

## 验证命令

```bash
cd chapter_X/lab0X-Y
make && make run
```

## 完成回报

```
任务 ID: CH5-8
完成时间: <yyyy-mm-dd hh:mm>
变更说明:
  - 重构 chapter_5: lab05-1 ~ lab05-3
  - 重构 chapter_6: lab06-1 ~ lab06-4
  - 重构 chapter_7: lab07-1
  - 重构 chapter_8: lab08-1 ~ lab08-4
验证结果: 所有 labs 编译通过 / QEMU 运行成功
```

## 禁止行为
- 不得修改 `chapter_5/`, `chapter_6/`, `chapter_7/`, `chapter_8/` 以外的目录
- 不得修改 `shared/` 目录
- 不得跳过文档编写

## 状态记录

每次重要操作后，记录到 `agents/arm64-refactor/agents/agent-b/memory.md`：

完成后更新 `agents/arm64-refactor/agents-lists.md` 中自己的状态为 `done`。