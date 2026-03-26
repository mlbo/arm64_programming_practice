#!/bin/bash
#
# run.sh - Run ARM64 programming practice labs in QEMU
#
# Usage:
#   ./scripts/run.sh                    # Run all labs (with timeout)
#   ./scripts/run.sh 3                  # Run all labs in chapter 3
#   ./scripts/run.sh 3 lab03-1          # Run specific lab
#   ./scripts/run.sh 21 lab21-2         # Run BenOS
#   ./scripts/run.sh 23 lab23-1         # Run SVE lab (user-mode QEMU)
#   ./scripts/run.sh 3 lab03-1 --debug  # Debug mode with GDB server
#

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Default settings
BOARD="${BOARD:-rpi4}"
TIMEOUT="${TIMEOUT:-5}"
DEBUG=false
VERBOSE=false

# Lab categories
BARE_METAL_CHAPTERS="2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 20"
BENOS_CHAPTERS="21"
USERMODE_CHAPTERS="22 23"

# QEMU paths
QEMU_SYSTEM="qemu-system-aarch64"
QEMU_USER="qemu-aarch64"
QEMU_USER_LIB="/usr/aarch64-linux-gnu"

print_usage() {
    echo "Usage: $0 [OPTIONS] [CHAPTER] [LAB]"
    echo ""
    echo "Options:"
    echo "  -b, --board BOARD   Target board (rpi3 or rpi4, default: rpi4)"
    echo "  -t, --timeout SEC   Timeout in seconds (default: 5, 0 for no timeout)"
    echo "  -d, --debug         Run with GDB server on port 1234"
    echo "  -v, --verbose       Show QEMU command"
    echo "  -h, --help          Show this help"
    echo ""
    echo "Examples:"
    echo "  $0 3 lab03-1        # Run lab03-1 in QEMU"
    echo "  $0 21 lab21-2       # Run BenOS kernel"
    echo "  $0 23 lab23-1       # Run SVE lab (user-mode)"
    echo "  $0 3 lab03-1 -d     # Run with GDB server"
    echo "  $0 3 lab03-1 -t 10  # Run with 10 second timeout"
}

log_info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

log_success() {
    echo -e "${GREEN}[OK]${NC} $1"
}

log_warn() {
    echo -e "${YELLOW}[WARN]${NC} $1"
}

log_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

log_cmd() {
    echo -e "${CYAN}[CMD]${NC} $1"
}

# Run bare-metal lab in QEMU system mode
run_bare_metal() {
    local lab_path="$1"
    local lab_name=$(basename "$lab_path")

    # Find the binary
    local bin_file=""
    local elf_file=""

    if [ -f "$lab_path/benos.bin" ]; then
        bin_file="$lab_path/benos.bin"
        elf_file="$lab_path/build/benos.elf"
    elif [ -f "$lab_path/build/benos.bin" ]; then
        bin_file="$lab_path/build/benos.bin"
        elf_file="$lab_path/build/benos.elf"
    else
        # Look for .bin file
        bin_file=$(find "$lab_path" -maxdepth 2 -name "*.bin" -type f 2>/dev/null | head -1)
        elf_file="${bin_file%.bin}.elf"
    fi

    # Check for user-mode executable (like atomic_test)
    if [ -z "$bin_file" ] || [ ! -f "$bin_file" ]; then
        local user_exec=$(find "$lab_path" -maxdepth 1 -type f -executable ! -name "*.o" ! -name "*.bin" 2>/dev/null | head -1)
        if [ -n "$user_exec" ]; then
            # This is a user-mode program
            run_usermode "$lab_path"
            return $?
        fi
        log_error "No binary found for $lab_name. Run build.sh first."
        return 1
    fi

    local machine=""
    case "$BOARD" in
        rpi3) machine="raspi3b" ;;
        rpi4) machine="raspi4b" ;;
    esac
    local qemu_cmd="$QEMU_SYSTEM -machine $machine -nographic"

    if [ "$DEBUG" = true ]; then
        qemu_cmd="$qemu_cmd -S -s"
        log_info "Starting QEMU with GDB server on port 1234"
        log_info "Connect with: aarch64-linux-gnu-gdb $elf_file -ex 'target remote :1234'"
    fi

    qemu_cmd="$qemu_cmd -kernel $bin_file"

    if [ "$VERBOSE" = true ]; then
        log_cmd "$qemu_cmd"
    fi

    log_info "Running $lab_name (board: $BOARD, timeout: ${TIMEOUT}s)"

    if [ "$TIMEOUT" -gt 0 ]; then
        timeout "$TIMEOUT" $qemu_cmd 2>&1 || true
    else
        $qemu_cmd
    fi

    echo ""
    log_success "Finished $lab_name"
}

# Run BenOS kernel
run_benos() {
    local lab_path="$1"
    local lab_name=$(basename "$lab_path")
    local benos_path="$lab_path/benos"

    local bin_file="$benos_path/arch/arm64/boot/benos.bin"
    local elf_file="$benos_path/benos"

    if [ ! -f "$bin_file" ]; then
        log_error "BenOS binary not found. Run build.sh first."
        return 1
    fi

    local machine=""
    case "$BOARD" in
        rpi3) machine="raspi3b" ;;
        rpi4) machine="raspi4b" ;;
    esac
    local qemu_cmd="$QEMU_SYSTEM -machine $machine -nographic"

    if [ "$DEBUG" = true ]; then
        qemu_cmd="$qemu_cmd -S -s"
        log_info "Starting QEMU with GDB server on port 1234"
        log_info "Connect with: aarch64-linux-gnu-gdb $elf_file -ex 'target remote :1234'"
    fi

    qemu_cmd="$qemu_cmd -kernel $bin_file"

    if [ "$VERBOSE" = true ]; then
        log_cmd "$qemu_cmd"
    fi

    log_info "Running BenOS ($lab_name, board: $BOARD, timeout: ${TIMEOUT}s)"

    if [ "$TIMEOUT" -gt 0 ]; then
        timeout "$TIMEOUT" $qemu_cmd 2>&1 || true
    else
        $qemu_cmd
    fi

    echo ""
    log_success "Finished $lab_name"
}

# Run user-mode program (NEON/SVE) in QEMU user mode
run_usermode() {
    local lab_path="$1"
    local lab_name=$(basename "$lab_path")

    # Find executable
    local exec_file=""
    for ext in "" "_test"; do
        for name in "sve${ext}" "neon${ext}" "${lab_name}${ext}"; do
            if [ -f "$lab_path/$name" ] && [ -x "$lab_path/$name" ]; then
                exec_file="$lab_path/$name"
                break 2
            fi
        done
    done

    # Look for any executable
    if [ -z "$exec_file" ]; then
        exec_file=$(find "$lab_path" -maxdepth 1 -type f -executable ! -name "*.o" ! -name "*.bin" | head -1)
    fi

    if [ -z "$exec_file" ]; then
        log_error "No executable found for $lab_name. Run build.sh first."
        return 1
    fi

    local qemu_cmd="$QEMU_USER -L $QEMU_USER_LIB -cpu max"

    if [ "$DEBUG" = true ]; then
        qemu_cmd="$qemu_cmd -g 1234"
        log_info "Starting QEMU user-mode with GDB server on port 1234"
        log_info "Connect with: aarch64-linux-gnu-gdb $exec_file -ex 'target remote :1234'"
    fi

    qemu_cmd="$qemu_cmd $exec_file"

    if [ "$VERBOSE" = true ]; then
        log_cmd "$qemu_cmd"
    fi

    log_info "Running $lab_name (user-mode QEMU, timeout: ${TIMEOUT}s)"

    if [ "$TIMEOUT" -gt 0 ]; then
        timeout "$TIMEOUT" $qemu_cmd 2>&1 || true
    else
        $qemu_cmd
    fi

    echo ""
    log_success "Finished $lab_name"
}

# Run chapter 2 special case (BenOS subdirectory)
run_chapter2_lab() {
    local lab_path="$1"

    if [ -d "$lab_path/BenOS" ]; then
        run_bare_metal "$lab_path/BenOS"
    else
        log_warn "Chapter 2 lab without BenOS directory: $(basename $lab_path)"
    fi
}

# Run all labs in a chapter
run_chapter() {
    local chapter="$1"
    local chapter_path="$PROJECT_ROOT/chapter_$chapter"

    if [ ! -d "$chapter_path" ]; then
        log_error "Chapter $chapter not found"
        return 1
    fi

    log_info "Running chapter $chapter labs..."
    echo ""

    for lab_path in "$chapter_path"/lab*; do
        if [ -d "$lab_path" ]; then
            local lab_name=$(basename "$lab_path")

            if [[ " $BENOS_CHAPTERS " =~ " $chapter " ]]; then
                run_benos "$lab_path" || true
            elif [[ " $USERMODE_CHAPTERS " =~ " $chapter " ]]; then
                run_usermode "$lab_path" || true
            elif [ "$chapter" = "2" ]; then
                run_chapter2_lab "$lab_path" || true
            else
                run_bare_metal "$lab_path" || true
            fi
            echo ""
        fi
    done
}

# Run a specific lab
run_specific_lab() {
    local chapter="$1"
    local lab="$2"
    local chapter_path="$PROJECT_ROOT/chapter_$chapter"
    local lab_path="$chapter_path/$lab"

    if [ ! -d "$lab_path" ]; then
        log_error "Lab $lab not found in chapter $chapter"
        return 1
    fi

    if [[ " $BENOS_CHAPTERS " =~ " $chapter " ]]; then
        run_benos "$lab_path"
    elif [[ " $USERMODE_CHAPTERS " =~ " $chapter " ]]; then
        run_usermode "$lab_path"
    elif [ "$chapter" = "2" ]; then
        run_chapter2_lab "$lab_path"
    else
        run_bare_metal "$lab_path"
    fi
}

# Run all labs
run_all() {
    log_info "Running all labs (timeout: ${TIMEOUT}s per lab)"
    log_warn "This will run all labs sequentially with timeout"
    echo ""

    # Build bare-metal chapters
    for chapter in $BARE_METAL_CHAPTERS; do
        run_chapter "$chapter" || true
    done

    # Build BenOS chapters
    for chapter in $BENOS_CHAPTERS; do
        run_chapter "$chapter" || true
    done

    # Build user-mode chapters
    for chapter in $USERMODE_CHAPTERS; do
        run_chapter "$chapter" || true
    done

    log_info "All labs completed"
}

# Parse arguments
POSITIONAL_ARGS=()

while [[ $# -gt 0 ]]; do
    case $1 in
        -b|--board)
            BOARD="$2"
            shift 2
            ;;
        -t|--timeout)
            TIMEOUT="$2"
            shift 2
            ;;
        -d|--debug)
            DEBUG=true
            shift
            ;;
        -v|--verbose)
            VERBOSE=true
            shift
            ;;
        -h|--help)
            print_usage
            exit 0
            ;;
        -*)
            log_error "Unknown option: $1"
            print_usage
            exit 1
            ;;
        *)
            POSITIONAL_ARGS+=("$1")
            shift
            ;;
    esac
done

# Restore positional args
set -- "${POSITIONAL_ARGS[@]}"

# Validate board
if [[ "$BOARD" != "rpi3" && "$BOARD" != "rpi4" ]]; then
    log_error "Invalid board: $BOARD. Must be 'rpi3' or 'rpi4'"
    exit 1
fi

# Check QEMU availability
if ! command -v $QEMU_SYSTEM &> /dev/null; then
    log_error "QEMU not found: $QEMU_SYSTEM"
    exit 1
fi

cd "$PROJECT_ROOT"

# Execute based on remaining arguments
case $# in
    0)
        run_all
        ;;
    1)
        if [[ "$1" =~ ^[0-9]+$ ]]; then
            run_chapter "$1"
        else
            log_error "Invalid chapter number: $1"
            exit 1
        fi
        ;;
    2)
        run_specific_lab "$1" "$2"
        ;;
    *)
        log_error "Too many arguments"
        print_usage
        exit 1
        ;;
esac