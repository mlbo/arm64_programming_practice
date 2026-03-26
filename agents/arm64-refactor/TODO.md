# ARM64 编程实践 C++ 重构任务清单

## 任务概览

| 任务 ID | 章节 | 描述 | 优先级 | 预估 Labs | 指派 |
|---------|------|------|-------|----------|------|
| CH21 | chapter_21 | BenOS 完整内核 (参考模板) | 最高 | 2 | agent-0 |
| CH2 | chapter_2 | Hello BenOS | 高 | 1 | agent-a |
| CH3 | chapter_3 | LDR/STR 汇编基础 | 高 | 6 | agent-a |
| CH4 | chapter_4 | 数据处理指令 | 高 | 5 | agent-a |
| CH5 | chapter_5 | 比较与跳转指令 | 高 | 3 | agent-b |
| CH6 | chapter_6 | 基础指令综合 | 中 | 4 | agent-b |
| CH7 | chapter_7 | 栈与局部变量 | 中 | 1 | agent-b |
| CH8 | chapter_8 | C 与汇编混合编程 | 高 | 4 | agent-b |
| CH9 | chapter_9 | 格式化输出 printk | 高 | 1 | agent-c |
| CH10 | chapter_10 | 内联汇编与原子操作 | 高 | 4 | agent-c |
| CH11 | chapter_11 | 异常级别与异常向量表 | 高 | 3 | agent-c |
| CH12 | chapter_12 | 中断处理与栈帧 | 高 | 1 | agent-d |
| CH13 | chapter_13 | GIC 中断控制器 | 高 | 1 | agent-d |
| CH14 | chapter_14 | 中断与定时器管理 | 中 | 2 | agent-d |
| CH15 | chapter_15 | 缓存管理与页表遍历 | 中 | 2 | agent-d |
| CH16 | chapter_16 | False Sharing | 低 | 2 | agent-e |
| CH18 | chapter_18 | 内存一致性模型 | 中 | litmus tests | agent-e |
| CH20 | chapter_20 | 原子操作 | 高 | 1 | agent-e |
| CH22 | chapter_22 | NEON SIMD | 中 | 3 | agent-f |
| CH23 | chapter_23 | SVE SIMD | 中 | 6 | agent-f |

## 正在执行的任务

| 任务 ID | 描述 | 指派 | 状态 | 验证结果 |
|---------|------|------|------|---------|
| CH21 | 重构 chapter_21 (参考模板) | agent-0 | doing | 进行中 |

## 待处理任务

| 任务 ID | 描述 | 指派 | 优先级 |
|---------|------|------|-------|
| CH2-4 | 重构 chapters 2-4 | agent-a | 高 |
| CH5-8 | 重构 chapters 5-8 | agent-b | 高 |
| CH9-11 | 重构 chapters 9-11 | agent-c | 高 |
| CH12-15 | 重构 chapters 12-15 | agent-d | 中 |
| CH16-18-20 | 重构 chapters 16,18,20 | agent-e | 中 |
| CH22-23 | 重构 chapters 22-23 | agent-f | 中 |

## 已完成任务

| 任务 ID | 描述 | 完成时间 | 日志 |
|---------|------|---------|------|
| (无) | | | |

## 需要用户确认的事项

| 任务 ID | 描述 | 用户确认状态 | 下一步动作 |
|---------|------|-------------|-----------|
| (无) | | | |

---

## 任务类型说明

| 类型 | 职责 | 产出物 |
|-----|------|-------|
| builder | 代码实现与测试 | 代码变更 + 测试 |
| review | 审查设计与代码 | 审查报告 |

## 任务工作流

```
Phase 0: agent-0 完成 chapter_21 (参考模板)
         ↓
Phase 1: 并行启动 agent-a ~ agent-f
         (每个 agent 只修改自己的章节目录)
         ↓
Phase 2: 验证 + 整合
```

## Agent 与章节对应

| Agent | 章节 | 目录 |
|-------|------|------|
| agent-0 | chapter_21 | chapter_21/ |
| agent-a | chapter_2, 3, 4 | chapter_2/, chapter_3/, chapter_4/ |
| agent-b | chapter_5, 6, 7, 8 | chapter_5/, chapter_6/, chapter_7/, chapter_8/ |
| agent-c | chapter_9, 10, 11 | chapter_9/, chapter_10/, chapter_11/ |
| agent-d | chapter_12, 13, 14, 15 | chapter_12/, chapter_13/, chapter_14/, chapter_15/ |
| agent-e | chapter_16, 18, 20 | chapter_16/, chapter_18/, chapter_20/ |