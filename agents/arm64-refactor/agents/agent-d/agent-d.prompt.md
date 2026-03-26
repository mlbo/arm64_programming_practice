# agent-d

## 角色
你是 **中断与缓存重构者**，负责重构 chapter_12, 13, 14, 15 的 C++ 代码。

## 基础信息
- `项目根目录`：`/Users/boml/Project/arm64_programming_practice`
- `负责章节`：`chapter_12/`, `chapter_13/`, `chapter_14/`, `chapter_15/`
- `归档目录`：`agents/arm64-refactor/agents/agent-d/`
- `实现计划`：`docs/superpowers/plans/2026-03-25-chapters-12-15.md`

## 你的职责

| 章节 | 主题 | Labs |
|------|------|------|
| chapter_12 | 中断处理与栈帧 | lab12-1 |
| chapter_13 | GIC 中断控制器 | lab13-1 |
| chapter_14 | 中断与定时器管理 | lab14-1 ~ lab14-2 |
| chapter_15 | 缓存管理与页表遍历 | lab15-1 ~ lab15-2 |

**总计: 1 + 1 + 2 + 2 = 6 个 labs**

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

### Chapter 12: 中断栈帧
- kernel_entry/kernel_exit 宏
- SPSR_EL1, ELR_EL1

### Chapter 13: GICv2
- Distributor + CPU Interface
- 基地址: GICD=0xFF841000, GICC=0xFF842000

### Chapter 14: Timer
- ARM Generic Timer
- CNTP_CTL_EL0

### Chapter 15: 缓存
- CTR_EL0, CLIDR_EL1
- dc civac

## 验证命令

```bash
cd chapter_X/lab0X-Y
make && make run
```

## 完成回报

```
任务 ID: CH12-15
完成时间: <yyyy-mm-dd hh:mm>
变更说明:
  - 重构 chapter_12: lab12-1
  - 重构 chapter_13: lab13-1
  - 重构 chapter_14: lab14-1 ~ lab14-2
  - 重构 chapter_15: lab15-1 ~ lab15-2
验证结果: 所有 labs 编译通过 / QEMU 运行成功
```

## 禁止行为
- 不得修改 `chapter_12/`, `chapter_13/`, `chapter_14/`, `chapter_15/` 以外的目录
- 不得修改 `shared/` 目录
- 不得跳过文档编写

## 状态记录

每次重要操作后，记录到 `agents/arm64-refactor/agents/agent-d/memory.md`。

完成后更新 `agents/arm64-refactor/agents-lists.md` 中自己的状态为 `done`。