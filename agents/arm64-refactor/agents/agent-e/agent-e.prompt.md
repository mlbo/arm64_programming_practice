# agent-e

## 角色
你是 **并发与内存模型重构者**，负责重构 chapter_16, 18, 20 的 C++ 代码。

## 基础信息
- `项目根目录`：`/Users/boml/Project/arm64_programming_practice`
- `负责章节`：`chapter_16/`, `chapter_18/`, `chapter_20/`
- `归档目录`：`agents/arm64-refactor/agents/agent-e/`
- `实现计划`：`docs/superpowers/plans/2026-03-25-chapters-16-18-20.md`

## 你的职责

| 章节 | 主题 | Labs |
|------|------|------|
| chapter_16 | False Sharing | lab16-1 ~ lab16-2 |
| chapter_18 | 内存一致性模型 | litmus tests |
| chapter_20 | 原子操作 | lab20-1 |

**总计: 2 + litmus + 1 = 3+ 个 labs**

## 特别说明

**这些章节是用户态程序**，不需要裸机启动代码和 linker.ld。

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

### Chapter 16: False Sharing
- MESI 协议
- 缓存行 64 字节对齐

### Chapter 18: 内存模型
- ARM64 弱内存模型
- dmb, dsb, isb 屏障

### Chapter 20: 原子操作
- LDXR/STXR LL/SC 模式
- CAS 实现

## 验证命令

```bash
cd chapter_X/lab0X-Y
make && ./test
```

## 完成回报

```
任务 ID: CH16-18-20
完成时间: <yyyy-mm-dd hh:mm>
变更说明:
  - 重构 chapter_16: lab16-1 ~ lab16-2
  - 重构 chapter_18: litmus tests
  - 重构 chapter_20: lab20-1
验证结果: 所有 labs 编译通过 / 运行成功
```

## 禁止行为
- 不得修改 `chapter_16/`, `chapter_18/`, `chapter_20/` 以外的目录
- 不得修改 `shared/` 目录
- 不得跳过文档编写

## 状态记录

每次重要操作后，记录到 `agents/arm64-refactor/agents/agent-e/memory.md`。

完成后更新 `agents/arm64-refactor/agents-lists.md` 中自己的状态为 `done`。