#!/bin/bash
# ARM64 编程实践 - 快速启动脚本
# 支持分阶段启动 agents，自动触发 Claude Code

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m'

echo -e "${BLUE}=====================================${NC}"
echo -e "${BLUE}  ARM64 C++ 重构 - Agent 启动工具  ${NC}"
echo -e "${BLUE}=====================================${NC}"
echo ""

print_help() {
    echo "用法: $0 <命令> [选项]"
    echo ""
    echo "命令:"
    echo "  start-all           启动 Phase 0 (admin + agent-0)"
    echo "  phase1              启动 Phase 1 (agent-a ~ agent-f，需 agent-0 完成后)"
    echo "  start <name>        启动单个 Agent 会话"
    echo "  attach <name>       附加到 Agent 会话"
    echo "  stop-all            停止所有会话"
    echo "  status              显示当前状态"
    echo "  help                显示帮助"
    echo ""
    echo "执行流程:"
    echo "  1. start-all        → Phase 0 (agent-0 完成 chapter_21)"
    echo "  2. phase1           → Phase 1 (agent-a ~ agent-f 并行重构)"
    echo ""
    echo "示例:"
    echo "  $0 start-all        # 启动 Phase 0"
    echo "  $0 phase1           # agent-0 完成后启动 Phase 1"
    echo "  $0 attach agent-0   # 监控 agent-0 进度"
}

start_agent() {
    local name=$1
    "$SCRIPT_DIR/arm64-agents-tmux.sh" -run "$name"
}

start_all() {
    echo -e "${CYAN}=== 启动 Phase 0 ===${NC}"
    echo ""
    echo "Phase 0: admin + agent-0 (完成 chapter_21 参考模板)"
    echo "Phase 1 将在 agent-0 完成后启动"
    echo ""

    # Phase 0: admin + agent-0
    echo -e "${GREEN}--- 启动 admin ---${NC}"
    start_agent "admin"
    echo ""

    echo -e "${GREEN}--- 启动 agent-0 ---${NC}"
    start_agent "agent-0"
    echo ""

    echo -e "${GREEN}=========================================${NC}"
    echo -e "${GREEN}  Phase 0 Agent 已启动!${NC}"
    echo -e "${GREEN}=========================================${NC}"
    echo ""
    echo -e "${CYAN}下一步:${NC}"
    echo ""
    echo "1. 监控 agent-0 进度:"
    echo "   tmux attach -t agent-0"
    echo ""
    echo "2. agent-0 完成 chapter_21 后，启动 Phase 1:"
    echo "   $0 phase1"
    echo ""
    echo -e "${YELLOW}注意: Phase 1 需要 chapter_21 作为参考模板${NC}"
}

start_phase1() {
    echo -e "${GREEN}=== Phase 1: 启动 agent-a ~ agent-f ===${NC}"
    echo ""
    echo -e "${YELLOW}前提: chapter_21 已完成 (参考模板)${NC}"
    echo ""

    local agents=("agent-a" "agent-b" "agent-c" "agent-d" "agent-e" "agent-f")

    for agent in "${agents[@]}"; do
        echo -e "${CYAN}启动 $agent...${NC}"
        start_agent "$agent"
        echo ""
    done

    echo -e "${GREEN}=== Phase 1 已启动 ===${NC}"
    echo ""
    echo "查看所有会话: tmux list-sessions"
    echo "附加到会话: tmux attach -t <agent-name>"
}

attach_agent() {
    local name=$1
    "$SCRIPT_DIR/arm64-agents-tmux.sh" -attach "$name"
}

stop_agent() {
    local name=$1
    "$SCRIPT_DIR/arm64-agents-tmux.sh" -stop "$name"
}

stop_all() {
    echo -e "${YELLOW}停止所有会话...${NC}"

    local sessions=("admin" "agent-0" "agent-a" "agent-b" "agent-c" "agent-d" "agent-e" "agent-f")

    for session in "${sessions[@]}"; do
        if tmux has-session -t "$session" 2>/dev/null; then
            tmux kill-session -t "$session"
            echo "  已停止: $session"
        fi
    done

    echo -e "${GREEN}所有会话已停止${NC}"
}

show_status() {
    echo -e "${BLUE}=== 项目状态 ===${NC}"
    echo ""

    echo -e "${YELLOW}Agent 状态:${NC}"
    cat "$PROJECT_ROOT/agents/arm64-refactor/agents-lists.md"
    echo ""

    echo -e "${YELLOW}活跃 Tmux 会话:${NC}"
    tmux list-sessions 2>/dev/null || echo "  (无)"
    echo ""

    echo -e "${YELLOW}执行流程:${NC}"
    echo "  1. start-all → Phase 0 (agent-0 完成 chapter_21)"
    echo "  2. phase1    → Phase 1 (agent-a ~ agent-f 并行重构)"
    echo ""

    echo -e "${YELLOW}可用命令:${NC}"
    echo "  $0 start-all     # 启动 Phase 0"
    echo "  $0 phase1        # agent-0 完成后启动 Phase 1"
    echo "  $0 attach <name> # 附加到会话"
}

# 主逻辑
case "${1:-help}" in
    start-all)
        start_all
        ;;
    phase1)
        start_phase1
        ;;
    start)
        start_agent "$2"
        ;;
    attach)
        attach_agent "$2"
        ;;
    stop)
        stop_agent "$2"
        ;;
    stop-all)
        stop_all
        ;;
    status)
        show_status
        ;;
    help|--help|-h)
        print_help
        ;;
    *)
        echo -e "${RED}未知命令: $1${NC}"
        print_help
        exit 1
        ;;
esac