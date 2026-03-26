# Claude Code 多 Agent 协作设计指南

基于 [codex-multi-agents](https://github.com/user/codex-multi-agents) 项目总结。

## 目录

- [核心设计理念](#核心设计理念)
- [架构概览](#架构概览)
- [角色设计](#角色设计)
- [任务管理](#任务管理)
- [通信机制](#通信机制)
- [工作流设计](#工作流设计)
- [文件与目录结构](#文件与目录结构)
- [强制约束与规则](#强制约束与规则)
- [Claude Code 原生 Agent 工具对比](#claude-code-原生-agent-工具对比)
- [实现建议](#实现建议)

---

## 核心设计理念

### 1. 会话隔离

每个 Agent 运行在独立的 tmux 会话中，确保：
- **上下文隔离**：每个 Agent 有独立的对话历史
- **并行执行**：多个 Agent 可以同时工作
- **状态追踪**：可以随时查看每个会话的状态

### 2. 职责分离

```
┌─────────────┐     分发任务     ┌─────────────┐
│   管理员     │ ───────────────> │  Worker A   │
│  (调度者)    │                  │  (执行者)   │
└─────────────┘                  └─────────────┘
       │
       │         分发任务
       └────────────────────────> ┌─────────────┐
                                   │  Worker B   │
                                   │  (执行者)   │
                                   └─────────────┘
```

- **管理员**：负责任务分发、状态协调、进度收敛，不直接修改代码
- **Worker**：负责具体实现，在授权的工作树内工作

### 3. 可追溯性

所有操作必须记录：
- 任务分发记录
- 对话日志
- 操作归档

---

## 架构概览

```
┌────────────────────────────────────────────────────────────┐
│                    Claude Code 多 Agent 系统               │
├────────────────────────────────────────────────────────────┤
│                                                            │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐     │
│  │ 名单管理脚本  │  │ 通信脚本     │  │ 任务调度脚本  │     │
│  │ list.sh      │  │ tmux.sh      │  │ task.sh      │     │
│  └──────────────┘  └──────────────┘  └──────────────┘     │
│                                                            │
│  ┌─────────────────────────────────────────────────────┐  │
│  │                   配置与状态文件                      │  │
│  │  config.txt  │  agents-lists.md  │  TODO.md        │  │
│  └─────────────────────────────────────────────────────┘  │
│                                                            │
│  ┌─────────────────────────────────────────────────────┐  │
│  │                   Agent 会话层                       │  │
│  │  ┌─────────┐  ┌─────────┐  ┌─────────┐             │  │
│  │  │ 管理员   │  │ Agent A │  │ Agent B │  ...        │  │
│  │  │ tmux    │  │ tmux    │  │ tmux    │             │  │
│  │  └─────────┘  └─────────┘  └─────────┘             │  │
│  └─────────────────────────────────────────────────────┘  │
│                                                            │
│  ┌─────────────────────────────────────────────────────┐  │
│  │                   工作树层                           │  │
│  │  ┌─────────┐  ┌─────────┐  ┌─────────┐             │  │
│  │  │ main    │  │ worktree│  │ worktree│             │  │
│  │  │ branch  │  │ for A   │  │ for B   │             │  │
│  │  └─────────┘  └─────────┘  └─────────┘             │  │
│  └─────────────────────────────────────────────────────┘  │
│                                                            │
└────────────────────────────────────────────────────────────┘
```

---

## 角色设计

### 角色属性表

| 属性 | 说明 |
|-----|------|
| 姓名 | 唯一标识符 |
| 状态 | `free` / `doing` / `pause` |
| 会话 | tmux 会话名 |
| 启动类型 | `codex` / `claude` |
| agent session | 运行时 session ID |
| worktree | 工作目录路径 |
| 介绍 | 角色能力描述 |
| 提示词 | 角色行为定义文件路径 |
| 归档文件 | 操作日志存储目录 |

### 管理员角色 (prompt-root.md)

```markdown
# <NAME>

## 角色定位
- 你是调度管理员，负责任务分发、状态协调和进度收敛。
- 你不直接修改业务代码文件。

## 行动准则
- 严格遵守 AGENTS.md 的要求。
- 将调度行为记录到归档目录/memory.md。
- 同时处于"进行中"的角色数量不得超过 <max_agents_num>。

## 调度建议
1. 先读取名单与任务清单，确认可用角色和任务状态。
2. 按职责与负载分配任务，避免重复派发。
3. 持续跟踪状态并及时更新。
```

### Worker 角色 (prompt.md)

```markdown
# <NAME>

## 行动准则
- 严格遵守 AGENTS.md 的要求。
- 仅允许修改 <worktree> 内的文件。
- 每次操作后将过程记录到归档目录/memory.md。
- 完成后向管理员回报。

## 禁止行为
- 未经特别授权，不得访问或修改配置文件、名单文件。
- 不得修改 <worktree> 以外的文件。
```

---

## 任务管理

### 任务状态流转

```
┌──────────┐    分发     ┌──────────┐    完成     ┌──────────┐
│  待处理   │ ─────────> │  进行中   │ ─────────> │  已完成   │
│ pending  │            │ doing    │            │  done    │
└──────────┘            └──────────┘            └──────────┘
                              │
                              │ 暂停
                              ▼
                         ┌──────────┐
                         │  暂停    │
                         │  pause   │
                         └──────────┘
```

### 任务类型

| 类型 | 职责 | 产出物 |
|-----|------|-------|
| spec | 设计文档编写 | 设计文档 |
| builder | 代码实现与测试 | 代码变更 + 测试 |
| review | 审查设计与代码 | 审查报告 |
| merge | 合并分支 | 合并日志 |

### 任务工作流

```
spec 任务 ──> 实现任务（含测试）──> 审查任务
                ^                    │
                │    不通过           │
                └────────────────────┘
                          │ 通过
                          ▼
                     合并任务
```

### 任务表格模板 (TODO.md)

```markdown
## 正在执行的任务

| 任务 ID | 描述 | 指派 | 状态 | 用户指导 |
|---------|------|------|------|---------|
| EX-1 | 创建 src 文件夹 | 小明 | 进行中 | xxx |

## 任务列表

| 任务 ID | 描述 | 指派 |
|---------|------|------|
| EX-1 | 创建 src 文件夹 | 无 |
```

---

## 通信机制

### 消息格式

```bash
codex-multi-agents-tmux.sh -talk \
  -from <发送者> \
  -to <接收者> \
  -session-id <目标会话ID> \
  -message "<消息内容>" \
  -log <日志文件路径>
```

### 消息规范

```
┌─────────────────────────────────────────────────┐
│ [时间戳] From: <发送者>                         │
│ To: <接收者>                                    │
│ Session: <会话ID>                               │
│────────────────────────────────────────────────│
│ <消息内容>                                      │
└─────────────────────────────────────────────────┘
```

### 回报格式模板

```
任务 ID: <task-id>
完成时间: <yyyy-mm-dd hh:mm>
变更说明: <summary>
日志路径: <log-path>
后续计划: <next-step or none>
```

---

## 工作流设计

### 初始化流程

```
1. 检查配置文件是否存在
   └── 不存在 ──> 创建 config.txt

2. 确认管理员名称

3. 创建目录结构
   ├── agents/codex-multi-agents/config/
   ├── agents/codex-multi-agents/log/
   ├── agents/codex-multi-agents/tmp/
   └── agents/codex-multi-agents/agents/

4. 初始化管理员 Agent
   └── 创建提示词文件和归档目录
```

### 新增角色流程

```
1. 确认角色名称、类型、工作树

2. 执行新增命令
   codex-multi-agents-list.sh -add -name <name> -type <type>

3. 创建归档目录
   agents/codex-multi-agents/agents/<name>/

4. 创建提示词文件
   agents/codex-multi-agents/agents/<name>/<name>.prompt.md

5. 初始化会话环境
   codex-multi-agents-tmux.sh -init-env -name <name>

6. 向新角色下发基础信息
   codex-multi-agents-list.sh -init -name <name>
```

### 任务执行流程

```
管理员                          Worker
  │                               │
  │──── 分发任务 ────────────────>│
  │     (包含任务ID、描述、要求)    │
  │                               │
  │                               │──── 执行任务
  │                               │
  │<─── 完成回报 ─────────────────│
  │     (包含日志、结果)            │
  │                               │
  │──── 确认/后续指令 ────────────>│
```

---

## 文件与目录结构

```
project/
├── agents/
│   └── codex-multi-agents/
│       ├── config/
│       │   └── config.txt          # 配置文件
│       ├── agents-lists.md         # Agent 名单
│       ├── log/
│       │   └── talk.log            # 交互日志
│       ├── tmp/                    # 临时文件
│       └── agents/
│           ├── admin/              # 管理员归档
│           │   ├── admin.prompt.md
│           │   └── memory.md
│           └── worker-a/           # Worker 归档
│               ├── worker-a.prompt.md
│               └── memory.md
├── scripts/
│   ├── codex-multi-agents-list.sh  # 名单管理
│   ├── codex-multi-agents-tmux.sh  # 会话通信
│   └── codex-multi-agents-task.sh  # 任务调度
└── TODO.md                         # 任务清单
```

### config.txt 内容

```
ROOT_NAME=admin
TODO_FILE=./TODO.md
AGENTS_FILE=./agents/codex-multi-agents/agents-lists.md
LOG_DIR=./agents/codex-multi-agents/log
TMP_DIR=./agents/codex-multi-agents/tmp
```

---

## 强制约束与规则

### 核心约束

1. **单一任务原则**：同一角色同时只能进行一项任务
2. **管理员独占分配权**：只有管理员可以分配任务
3. **工作树隔离**：涉及代码的任务必须使用 `git worktree`
4. **脚本化状态变更**：任务状态变更必须通过脚本执行
5. **完整日志**：所有操作必须可追踪

### 禁止行为

| 角色 | 禁止行为 |
|-----|---------|
| 管理员 | 直接修改业务代码文件 |
| Worker | 修改 config、TODO.md、agents-lists.md |
| Worker | 访问未授权的目录与文件 |
| Worker | 修改 worktree 以外的文件 |

### 任务约束

- spec 任务只写 spec 文件，不做实现
- 实现任务只负责实现和测试
- 审查任务只审查给意见，不写代码
- 合并任务只做合并与清理

---

## Claude Code 原生 Agent 工具对比

Claude Code 提供了内置的 `Agent` 工具，支持多种子代理类型：

### 内置 Agent 类型

| 类型 | 用途 | 与 codex-multi-agents 对比 |
|-----|------|---------------------------|
| `general-purpose` | 通用任务处理 | 类似 Worker 角色 |
| `Explore` | 代码库探索 | 研究任务专用 |
| `Plan` | 实现计划设计 | spec 任务类似 |
| `code-reviewer` | 代码审查 | review 任务类似 |
| `code-simplifier` | 代码简化 | 重构任务 |

### 关键差异

| 特性 | Claude Code Agent | codex-multi-agents |
|-----|------------------|-------------------|
| 会话管理 | 内置，临时会话 | tmux，持久会话 |
| 状态追踪 | 自动 | 手动脚本管理 |
| 通信方式 | 自动消息传递 | 脚本化消息 |
| 工作隔离 | 自动隔离 | git worktree |
| 适用场景 | 单次任务 | 长期协作项目 |

### 组合使用建议

```python
# 短期独立任务：使用内置 Agent
Agent(subagent_type="Explore", prompt="搜索 API 端点")

# 长期协作项目：使用 codex-multi-agents 框架
# - 需要多个 Agent 持续协作
# - 需要严格的状态追踪
# - 需要完整的审计日志
```

---

## 实现建议

### 1. 使用 Claude Code 内置功能简化

```bash
# 利用内置 Agent 工具
# - general-purpose: 独立研究任务
# - Explore: 快速代码库探索
# - Plan: 方案设计

# 利用 git worktree 支持
git worktree add .worktrees/feature-a feature-branch
```

### 2. 脚本化核心流程

```bash
# 名单管理
./scripts/codex-multi-agents-list.sh -add -name worker-a -type claude

# 任务分发
./scripts/codex-multi-agents-task.sh -dispatch -task_id T1 -to worker-a

# 消息通信
./scripts/codex-multi-agents-tmux.sh -talk -from admin -to worker-a \
  -session-id worker-a -message "请处理任务 T1" -log ./log/talk.log
```

### 3. 结合 Skill 系统

将多 Agent 协作封装为 Skill：

```yaml
# agents/multi-agent.yaml
interface:
  display_name: "Multi-Agent Orchestrator"
  short_description: "基于 Claude Code Agent 工具的多 Agent 协作"
  default_prompt: "使用 Agent 工具并行处理多个独立任务"
```

### 4. 状态持久化

```markdown
<!-- agents/agents-lists.md -->
| 姓名 | 状态 | 会话 | worktree | 职责 |
|-----|------|------|---------|------|
| admin | free | admin | main | 调度管理 |
| builder | doing | builder | .worktrees/feat-a | 代码实现 |
| reviewer | free | reviewer | main | 代码审查 |
```

### 5. 利用 MCP 扩展能力

```json
// 通过 MCP 工具实现跨 Agent 通信
{
  "mcpServers": {
    "agent-communication": {
      "command": "node",
      "args": ["agent-bus-server.js"]
    }
  }
}
```

---

## 最佳实践

### 任务拆分原则

- 每个任务应该是独立可完成的
- 任务之间尽量减少依赖
- 明确输入输出边界

### Agent 数量控制

- 同时运行的 Agent 不超过 CPU 核心数
- 考虑内存限制

### 错误处理

- 任务失败时回滚状态
- 记录失败原因
- 通知管理员

### 审计日志

```
2025-03-25 14:30:00 +0800: 向 builder 分发任务 T1，日志：./log/T1.log
2025-03-25 15:00:00 +0800: builder 完成任务 T1，状态：成功
2025-03-25 15:05:00 +0800: 向 reviewer 分发审查任务 R1
```

---

## 参考资料

- [codex-multi-agents 项目](https://github.com/user/codex-multi-agents)
- [Claude Code Agent 工具文档](https://docs.anthropic.com/claude-code)
- [git worktree 文档](https://git-scm.com/docs/git-worktree)