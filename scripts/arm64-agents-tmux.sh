#!/bin/bash
# ARM64 编程实践 - Agent Tmux 会话管理脚本
# 用法: ./arm64-agents-tmux.sh [命令] [选项]
#支持自动启动 Claude Code 并预填充 prompt

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
NC='\033[0m'

print_help() {
    echo "ARM64 Agent Tmux 会话管理工具"
    echo ""
    echo "用法: $0 <命令> [选项]"
    echo ""
    echo "命令:"
    echo "  -start <name>           启动 Agent 会话 (仅创建 tmux)"
    echo "  -run <name>             启动会话并自动运行 claude"
    echo "  -attach <name>          附加到 Agent 会话"
    echo "  -stop <name>            停止 Agent 会话"
    echo "  -talk <from> <to> <msg> 发送消息到会话"
    echo "  -list                   列出所有会话"
    echo "  -help                   显示帮助"
    echo ""
    echo "示例:"
    echo "  $0 -start agent-a       # 创建 tmux 会话"
    echo "  $0 -run agent-a         # 创建会话并自动启动 claude"
    echo "  $0 -attach agent-a      # 附加到会话"
}

list_sessions() {
    echo -e "${BLUE}=== Tmux 会话列表 ===${NC}"
    echo ""
    tmux list-sessions 2>/dev/null || echo "没有活跃的 tmux 会话"
    echo ""
}

start_session() {
    local name=$1
    local auto_run=${2:-false}

    # 从名单获取章节目录
    local chapters=$(grep "^| $name " "$AGENTS_FILE" | awk '{print $6}')
    local prompt_file="$PROJECT_ROOT/agents/arm64-refactor/agents/$name/$name.prompt.md"

    echo -e "${GREEN}=== 启动 Agent 会话: $name ===${NC}"
    echo ""
    echo "章节目录: $chapters"
    echo ""

    # 检查会话是否已存在
    if tmux has-session -t "$name" 2>/dev/null; then
        echo -e "${YELLOW}会话已存在${NC}"
        if [[ "$auto_run" == "true" ]]; then
            echo "附加到会话..."
            tmux attach -t "$name"
        fi
        return 0
    fi

    # 创建新会话
    tmux new-session -d -s "$name" -c "$PROJECT_ROOT"

    # 发送初始化信息
    tmux send-keys -t "$name" "cd $PROJECT_ROOT" Enter
    tmux send-keys -t "$name" "echo '========================================'" Enter
    tmux send-keys -t "$name" "echo 'Agent: $name'" Enter
    tmux send-keys -t "$name" "echo '负责章节: $chapters'" Enter
    tmux send-keys -t "$name" "echo '========================================'" Enter
    tmux send-keys -t "$name" "echo ''" Enter

    if [[ "$auto_run" == "true" ]]; then
        echo -e "${YELLOW}自动启动 Claude Code...${NC}"
        echo ""
        # 启动 claude 交互式会话
        tmux send-keys -t "$name" "claude --dangerously-skip-permissions" Enter
        sleep 3
        # 发送引导消息让 agent 知道自己的任务
        tmux send-keys -t "$name" "你是 $name，负责章节: $chapters。请读取你的任务文件: $prompt_file 然后开始工作。" Enter
        echo -e "${GREEN}Claude Code 已启动${NC}"
        echo ""
        echo "附加到会话: tmux attach -t $name"
    else
        echo -e "${GREEN}会话已创建${NC}"
        echo ""
        echo "下一步:"
        echo "  1. tmux attach -t $name"
        echo "  2. 运行: claude"
        echo "  3. 输入: /read $prompt_file"
    fi
}

run_session() {
    start_session "$1" true
}

attach_session() {
    local name=$1

    if ! tmux has-session -t "$name" 2>/dev/null; then
        echo -e "${RED}错误: 会话 $name 不存在${NC}"
        echo "先运行: $0 -start $name"
        exit 1
    fi

    tmux attach -t "$name"
}

stop_session() {
    local name=$1

    if ! tmux has-session -t "$name" 2>/dev/null; then
        echo -e "${YELLOW}会话 $name 不存在${NC}"
        return 0
    fi

    tmux kill-session -t "$name"
    echo -e "${GREEN}会话 $name 已停止${NC}"
}

send_message() {
    local from=$1
    local to=$2
    local message=$3
    local timestamp=$(date '+%Y-%m-%d %H:%M:%S %z')

    # 记录日志
    echo "[$timestamp] From: $from -> To: $to" >> "$LOG_DIR/talk.log"
    echo "  Message: $message" >> "$LOG_DIR/talk.log"
    echo "" >> "$LOG_DIR/talk.log"

    # 如果目标会话存在，发送消息
    if tmux has-session -t "$to" 2>/dev/null; then
        tmux send-keys -t "$to" "" Enter
        tmux send-keys -t "$to" "echo '========================================'" Enter
        tmux send-keys -t "$to" "echo '[$timestamp] From: $from'" Enter
        tmux send-keys -t "$to" "echo '$message'" Enter
        tmux send-keys -t "$to" "echo '========================================'" Enter
    fi

    echo -e "${GREEN}消息已发送${NC}"
}

# 主逻辑
case "$1" in
    -list)
        list_sessions
        ;;
    -start)
        start_session "$2" false
        ;;
    -run)
        run_session "$2"
        ;;
    -attach)
        attach_session "$2"
        ;;
    -stop)
        stop_session "$2"
        ;;
    -talk)
        send_message "$2" "$3" "$4"
        ;;
    -help|--help|*)
        print_help
        ;;
esac