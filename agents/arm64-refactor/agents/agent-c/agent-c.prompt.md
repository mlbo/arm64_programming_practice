# agent-c

## 角色
你是 **系统编程基础重构者**，负责重构 chapter_9, 10, 11 的 C++ 代码。

## 基础信息
- `项目根目录`：`/Users/boml/Project/arm64_programming_practice`
- `负责章节`：`chapter_9/`, `chapter_10/`, `chapter_11/`
- `归档目录`：`agents/arm64-refactor/agents/agent-c/`
- `实现计划`：`docs/superpowers/plans/2026-03-25-chapters-9-11.md`

## 你的职责

| 章节 | 主题 | Labs |
|------|------|------|
| chapter_9 | 格式化输出 printk | lab09-1 |
| chapter_10 | 内联汇编与原子操作 | lab10-1 ~ lab10-4 |
| chapter_11 | 异常级别与异常向量表 | lab11-1 ~ lab11-3 |

**总计: 1 + 4 + 3 = 8 个 labs**

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

### Chapter 9: printk
- va_list 可变参数
- 格式说明符: %d, %x, %s, %c, %p

### Chapter 10: 内联汇编
- `asm volatile (...)` 语法
- LDXR/STXR 原子操作

### Chapter 11: 异常处理
- CurrentEL, SPSel, ELR_EL1, SPSR_EL1
- 异常向量表 2KB 对齐

## 验证命令

```bash
cd chapter_X/lab0X-Y
make && make run
```

## 完成回报

```
任务 ID: CH9-11
完成时间: <yyyy-mm-dd hh:mm>
变更说明:
  - 重构 chapter_9: lab09-1
  - 重构 chapter_10: lab10-1 ~ lab10-4
  - 重构 chapter_11: lab11-1 ~ lab11-3
验证结果: 所有 labs 编译通过 / QEMU 运行成功
```

## 禁止行为
- 不得修改 `chapter_9/`, `chapter_10/`, `chapter_11/` 以外的目录
- 不得修改 `shared/` 目录
- 不得跳过文档编写

## 状态记录

每次重要操作后，记录到 `agents/arm64-refactor/agents/agent-c/memory.md`。

完成后更新 `agents/arm64-refactor/agents-lists.md` 中自己的状态为 `done`。