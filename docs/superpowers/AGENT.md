# Agent 协调指南

> ARM64 编程实践仓库 C++ 重构 - 多 Agent 并行工作指南

## 快速开始

**每个 agent 的详细启动 Prompt**: [`agent-prompts.md`](./agent-prompts.md)

将对应的 "启动 Prompt" 复制给 agent 即可开始工作。

## 职责分配

| Agent | 章节 | 计划文件 |
|-------|------|----------|
| Agent-A | chapter_2, 3, 4 | `plans/2026-03-25-chapters-2-4.md` |
| Agent-B | chapter_5, 6, 7, 8 | `plans/2026-03-25-chapters-5-8.md` |
| Agent-C | chapter_9, 10, 11 | `plans/2026-03-25-chapters-9-11.md` |
| Agent-D | chapter_12, 13, 14, 15 | `plans/2026-03-25-chapters-12-15.md` |
| Agent-E | chapter_16, 18, 20 | `plans/2026-03-25-chapters-16-18-20.md` |
| Agent-F | chapter_22, 23 | `plans/2026-03-25-chapters-22-23.md` |

---

## 共享规则 (所有 Agent 必须遵守)

### 1. 文件边界

- **不要修改 `shared/`** - 共享基础设施由主 agent 维护
- **只修改分配的章节目录**
- **如需共享组件变更，提 issue 给主 agent**

### 2. 代码风格

参考 `chapter_21/lab21-2/benos/` 作为模板：

- **命名空间**: `namespace benos {}` (内核) 或 `namespace arm64lab {}` (独立实验)
- **类型别名**: `using u8 = uint8_t;` 等
- **类设计**: 公有成员在前，方法在后，私有成员后缀 `_`
- **错误处理**: 返回 int，0 成功，负数错误
- **禁用特性**: 无 exceptions, RTTI, STL, threadsafe-statics

### 3. 文档要求

每个 lab 必须包含:

```
labXX-Y/
├── README.md              # 实验说明
├── docs/
│   ├── knowledge.md       # 知识点讲解
│   ├── code_explanation.md # 代码说明
│   └── exercises.md       # 练习题
├── scripts/
│   ├── build.sh           # 编译脚本
│   └── run.sh             # 运行脚本
└── ...
```

### 4. 提交规范

```
git commit -m "feat(chX): refactor [描述]

- 要点 1
- 要点 2

Co-Authored-By: Claude <noreply@anthropic.com>"
```

---

## 依赖关系

### 共享基础设施 (Phase 1.5 创建)

```
shared/baremetal/
├── types.hpp              # 所有章节共用
├── error.hpp              # 错误码定义
├── new.hpp                # placement new
└── runtime/
    ├── cxx_runtime.hpp    # C++ 运行时
    └── cxx_runtime.cpp
```

### 章节间依赖

| 章节 | 依赖 |
|------|------|
| chapter_2 | 无 |
| chapter_3 | chapter_2 的 UART 驱动模式 |
| chapter_4 | chapter_3 |
| chapter_5-8 | chapter_3 基础 |
| chapter_9-11 | chapter_8 混合编程 |
| chapter_12-15 | chapter_11 异常处理 |
| chapter_16, 18, 20 | chapter_6 原子操作基础 |
| chapter_21 | 参考模板 (已完成) |
| chapter_22-23 | 独立 (SIMD) |

---

## 验证清单

每个章节完成后:

- [ ] `./scripts/build.sh` 编译成功
- [ ] `./scripts/run.sh` 运行成功 (裸机章节用 QEMU)
- [ ] README.md 存在
- [ ] docs/knowledge.md 存在
- [ ] docs/code_explanation.md 存在
- [ ] docs/exercises.md 存在

---

## 冲突处理

如果多个 agent 需要修改同一文件:

1. **优先级**: 主 agent > 先提交的 agent
2. **合并策略**: 在 PR 中协调
3. **共享依赖变更**: 通过 issue 讨论

---

## 执行顺序

```
Phase 1 (已完成): chapter_21 参考模板
Phase 1.5:        创建 shared/baremetal/
Phase 2 (并行):   Agent-A ~ Agent-F 同时工作
Phase 3:          按顺序合并 PR
```

---

## 联系方式

- 主 agent 负责协调和共享基础设施
- 各 agent 在独立分支工作
- 通过 issue 和 PR 进行沟通