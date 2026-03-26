#!/bin/bash
# ARM64 编程实践 - Agent 任务分发脚本
# 用法: ./arm64-agents-task.sh [命令] [选项]

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
CONFIG_FILE="$PROJECT_ROOT/agents/arm64-refactor/config/config.txt"

# 加载配置
source "$CONFIG_FILE"

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m'

print_help() {
    echo "ARM64 Agent 任务分发工具"
    echo ""
    echo "用法: $0 <命令> [选项]"
    echo ""
    echo "命令:"
    echo "  -dispatch <task_id> <agent>  分发任务给 Agent"
    echo "  -complete <task_id> <agent>  标记任务完成"
    echo "  -status                      显示任务状态"
    echo "  -list-agents                 列出所有 Agent"
    echo "  -help                        显示帮助"
    echo ""
    echo "任务 ID 映射:"
    echo "  CH21      → chapter_21 (agent-0)"
    echo "  CH2-4     → chapters 2-4 (agent-a)"
    echo "  CH5-8     → chapters 5-8 (agent-b)"
    echo "  CH9-11    → chapters 9-11 (agent-c)"
    echo "  CH12-15   → chapters 12-15 (agent-d)"
    echo "  CH16-18-20 → chapters 16,18,20 (agent-e)"
    echo "  CH22-23   → chapters 22-23 (agent-f)"
    echo ""
    echo "示例:"
    echo "  $0 -dispatch CH21 agent-0"
    echo "  $0 -complete CH21 agent-0"
}

get_timestamp() {
    date '+%Y-%m-%d %H:%M:%S %z'
}

update_todo_dispatch() {
    local task_id=$1
    local agent=$2
    local timestamp=$(get_timestamp)

    # 读取当前 TODO.md
    local todo_file="$PROJECT_ROOT/$TODO_FILE"

    # 创建临时文件
    local tmp_file=$(mktemp)

    # 更新 "正在执行的任务" 部分
    local in_doing=false
    local in_pending=true

    while IFS= read -r line; do
        if [[ "$line" == "## 正在执行的任务" ]]; then
            in_doing=true
            in_pending=false
            echo "$line" >> "$tmp_file"
            echo "" >> "$tmp_file"
            echo "| $task_id | 重构 $(get_task_desc $task_id) | $agent | doing | 进行中 |" >> "$tmp_file"
            continue
        fi

        if [[ "$line" == "## 待处理任务" ]]; then
            in_doing=false
            in_pending=true
            echo "$line" >> "$tmp_file"
            continue
        fi

        if [[ "$in_pending" == true && "$line" == *"|$task_id|"* ]]; then
            # 跳过待处理中的该任务
            continue
        fi

        echo "$line" >> "$tmp_file"
    done < "$todo_file"

    # 替换原文件
    mv "$tmp_file" "$todo_file"

    echo -e "${GREEN}任务 $task_id 已分发给 $agent${NC}"
}

get_task_desc() {
    local task_id=$1
    case $task_id in
        CH21) echo "chapter_21 (参考模板)" ;;
        CH2-4) echo "chapters 2-4" ;;
        CH5-8) echo "chapters 5-8" ;;
        CH9-11) echo "chapters 9-11" ;;
        CH12-15) echo "chapters 12-15" ;;
        CH16-18-20) echo "chapters 16,18,20" ;;
        CH22-23) echo "chapters 22-23" ;;
        *) echo "未知任务" ;;
    esac
}

get_agent_chapters() {
    local agent=$1
    case $agent in
        agent-0) echo "chapter_21" ;;
        agent-a) echo "chapter_2, chapter_3, chapter_4" ;;
        agent-b) echo "chapter_5, chapter_6, chapter_7, chapter_8" ;;
        agent-c) echo "chapter_9, chapter_10, chapter_11" ;;
        agent-d) echo "chapter_12, chapter_13, chapter_14, chapter_15" ;;
        agent-e) echo "chapter_16, chapter_18, chapter_20" ;;
        agent-f) echo "chapter_22, chapter_23" ;;
        *) echo "" ;;
    esac
}

dispatch_task() {
    local task_id=$1
    local agent=$2
    local timestamp=$(get_timestamp)
    local chapters=$(get_agent_chapters $agent)
    local prompt_file="$PROJECT_ROOT/agents/arm64-refactor/agents/$agent/$agent.prompt.md"

    echo -e "${CYAN}=== 分发任务: $task_id → $agent ===${NC}"
    echo ""
    echo "时间: $timestamp"
    echo "任务: $task_id"
    echo "描述: $(get_task_desc $task_id)"
    echo "Agent: $agent"
    echo "章节: $chapters"
    echo ""

    # 检查 agent 会话是否存在
    if ! tmux has-session -t "$agent" 2>/dev/null; then
        echo -e "${YELLOW}警告: Agent 会话 $agent 不存在${NC}"
        echo "请先启动会话: scripts/arm64-agents-tmux.sh -run $agent"
        echo ""
        echo -e "${YELLOW}尝试自动启动...${NC}"
        "$SCRIPT_DIR/arm64-agents-tmux.sh" -run "$agent"
        sleep 3
    fi

    # 发送任务消息到 agent 会话
    tmux send-keys -t "$agent" "" Enter
    tmux send-keys -t "$agent" "echo '========================================'" Enter
    tmux send-keys -t "$agent" "echo '[$timestamp] ADMIN 分发任务: $task_id'" Enter
    tmux send-keys -t "$agent" "echo '任务描述: $(get_task_desc $task_id)'" Enter
    tmux send-keys -t "$agent" "echo '负责章节: $chapters'" Enter
    tmux send-keys -t "$agent" "echo '提示词文件: $prompt_file'" Enter
    tmux send-keys -t "$agent" "echo '========================================'" Enter
    tmux send-keys -t "$agent" "" Enter
    tmux send-keys -t "$agent" "请读取你的任务文件: $prompt_file 然后开始工作。" Enter

    # 更新 TODO.md
    update_todo_dispatch "$task_id" "$agent"

    # 记录日志
    local log_file="$PROJECT_ROOT/$LOG_DIR/dispatch.log"
    echo "[$timestamp] 向 $agent 分发任务 $task_id" >> "$log_file"

    echo ""
    echo -e "${GREEN}任务已分发!${NC}"
    echo "监控进度: tmux attach -t $agent"
}

complete_task() {
    local task_id=$1
    local agent=$2
    local result=${3:-"成功"}
    local timestamp=$(get_timestamp)

    echo -e "${GREEN}=== 标记任务完成: $task_id ===${NC}"
    echo ""
    echo "时间: $timestamp"
    echo "任务: $task_id"
    echo "Agent: $agent"
    echo "结果: $result"
    echo ""

    # 记录日志
    local log_file="$PROJECT_ROOT/$LOG_DIR/dispatch.log"
    echo "[$timestamp] $agent 完成任务 $task_id，验证结果: $result" >> "$log_file"

    # 更新 agents-lists.md 中 agent 状态为 free
    local agents_file="$PROJECT_ROOT/$AGENTS_FILE"
    local tmp_file=$(mktemp)

    while IFS= read -r line; do
        if [[ "$line" == "| $agent "* ]]; then
            # 更新状态为 free
            echo "$line" | sed 's/| doing |/| free |/' >> "$tmp_file"
        else
            echo "$line" >> "$tmp_file"
        fi
    done < "$agents_file"

    mv "$tmp_file" "$agents_file"

    echo -e "${GREEN}任务已标记完成${NC}"
}

show_status() {
    echo -e "${BLUE}=== 任务状态 ===${NC}"
    echo ""

    echo -e "${YELLOW}正在执行的任务:${NC}"
    grep -A 10 "## 正在执行的任务" "$PROJECT_ROOT/$TODO_FILE" | tail -n +3 | head -n 10 || echo "  (无)"
    echo ""

    echo -e "${YELLOW}待处理任务:${NC}"
    grep -A 20 "## 待处理任务" "$PROJECT_ROOT/$TODO_FILE" | tail -n +3 | head -n 10 || echo "  (无)"
    echo ""

    echo -e "${YELLOW}已完成任务:${NC}"
    grep -A 10 "## 已完成任务" "$PROJECT_ROOT/$TODO_FILE" | tail -n +3 | head -n 10 || echo "  (无)"
    echo ""

    echo -e "${YELLOW}活跃 Tmux 会话:${NC}"
    tmux list-sessions 2>/dev/null || echo "  (无)"
}

list_agents() {
    echo -e "${BLUE}=== Agent 名单 ===${NC}"
    echo ""
    cat "$PROJECT_ROOT/$AGENTS_FILE"
}

# 主逻辑
case "$1" in
    -dispatch)
        if [[ -z "$2" || -z "$3" ]]; then
            echo -e "${RED}错误: 需要指定 task_id 和 agent${NC}"
            echo "用法: $0 -dispatch <task_id> <agent>"
            exit 1
        fi
        dispatch_task "$2" "$3"
        ;;
    -complete)
        if [[ -z "$2" || -z "$3" ]]; then
            echo -e "${RED}错误: 需要指定 task_id 和 agent${NC}"
            echo "用法: $0 -complete <task_id> <agent> [result]"
            exit 1
        fi
        complete_task "$2" "$3" "${4:-成功}"
        ;;
    -status)
        show_status
        ;;
    -list-agents)
        list_agents
        ;;
    -help|--help|*)
        print_help
        ;;
esac