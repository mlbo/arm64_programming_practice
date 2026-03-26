# Agents 名单

| 姓名 | 状态 | 会话 | 启动类型 | 章节目录 | 章节 | 提示词 | 归档文件 |
|-----|------|------|---------|---------|------|-------|---------|
| admin | free | admin | claude | - | 调度 | ./agents/admin/admin.prompt.md | ./agents/admin/ |
| agent-0 | done | agent-0 | claude | chapter_21 | ch21参考模板 | ./agents/agent-0/agent-0.prompt.md | ./agents/agent-0/ |
| agent-a | done | agent-a | claude | chapter_2,chapter_3,chapter_4 | ch2-4 | ./agents/agent-a/agent-a.prompt.md | ./agents/agent-a/ |
| agent-b | done | agent-b | claude | chapter_5,chapter_6,chapter_7,chapter_8 | ch5-8 | ./agents/agent-b/agent-b.prompt.md | ./agents/agent-b/ |
| agent-c | done | agent-c | claude | chapter_9,chapter_10,chapter_11 | ch9-11 | ./agents/agent-c/agent-c.prompt.md | ./agents/agent-c/ |
| agent-d | done | agent-d | claude | chapter_12,chapter_13,chapter_14,chapter_15 | ch12-15 | ./agents/agent-d/agent-d.prompt.md | ./agents/agent-d/ |
| agent-e | done | agent-e | claude | chapter_16,chapter_18,chapter_20 | ch16,18,20 | ./agents/agent-e/agent-e.prompt.md | ./agents/agent-e/ |
| agent-f | done | agent-f | claude | chapter_22,chapter_23 | ch22-23 | ./agents/agent-f/agent-f.prompt.md | ./agents/agent-f/ |

## 执行顺序

```
Phase 0: agent-0 完成 chapter_21 (参考模板)
         ↓
Phase 1: 并行启动 agent-a ~ agent-f
         (每个 agent 只修改自己的章节目录)
         ↓
Phase 2: 验证 + 整合
```