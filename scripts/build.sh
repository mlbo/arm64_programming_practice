#!/bin/bash
#
# build.sh - Build ARM64 programming practice labs
#
# Usage:
#   ./scripts/build.sh                    # Build all labs
#   ./scripts/build.sh 3                  # Build all labs in chapter 3
#   ./scripts/build.sh 3 lab03-1          # Build specific lab
#   ./scripts/build.sh 21 lab21-2         # Build BenOS
#   ./scripts/build.sh --clean            # Clean all labs
#   ./scripts/build.sh 3 --clean          # Clean chapter 3 labs
#

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Default settings
BOARD="${BOARD:-rpi4}"

# Lab categories
BARE_METAL_CHAPTERS="2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 20"
BENOS_CHAPTERS="21"
USERMODE_CHAPTERS="22 23"
SPECIAL_CHAPTERS="18"  # Litmus tests need herdtools7

print_usage() {
    echo "Usage: $0 [OPTIONS] [CHAPTER] [LAB]"
    echo ""
    echo "Options:"
    echo "  -b, --board BOARD   Target board (rpi3 or rpi4, default: rpi4)"
    echo "  -h, --help          Show this help"
    echo ""
    echo "Examples:"
    echo "  $0                  # Build all labs (always cleans first)"
    echo "  $0 3                # Build all labs in chapter 3"
    echo "  $0 3 lab03-1        # Build specific lab"
    echo "  $0 21 lab21-2       # Build BenOS kernel"
    echo "  $0 -b rpi3 3        # Build chapter 3 for Raspberry Pi 3"
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

# Build a bare-metal lab
build_bare_metal() {
    local lab_path="$1"
    local lab_name=$(basename "$lab_path")

    log_info "Building bare-metal lab: $lab_name"

    cd "$lab_path"

    # Always clean to ensure fresh build
    make clean 2>/dev/null || true

    if make board=$BOARD 2>&1; then
        log_success "Built $lab_name"
    else
        log_error "Failed to build $lab_name"
        return 1
    fi
}

# Build BenOS kernel (chapter 21)
build_benos() {
    local lab_path="$1"
    local lab_name=$(basename "$lab_path")

    log_info "Building BenOS: $lab_name"

    cd "$lab_path/benos"

    # Always clean to ensure fresh build
    make clean 2>/dev/null || true

    # Check if .config exists
    if [ ! -f ".config" ]; then
        log_info "Running defconfig for BenOS..."
        # Non-interactive defconfig
        {
            echo "1"  # ARCH_BCM2835
            echo "2"  # BOARD_PI4B
            echo "2"  # PI_PL_UART
            echo "2"  # HZ_250
        } | make defconfig 2>&1 || {
            log_warn "defconfig may need manual intervention"
        }
    fi

    if make 2>&1; then
        log_success "Built BenOS ($lab_name)"
    else
        log_error "Failed to build BenOS ($lab_name)"
        return 1
    fi
}

# Build user-mode program (NEON/SVE)
build_usermode() {
    local lab_path="$1"
    local lab_name=$(basename "$lab_path")

    log_info "Building user-mode lab: $lab_name"

    cd "$lab_path"

    # Always clean to ensure fresh build
    make clean 2>/dev/null || true

    # Set ARMGNU for cross-compilation
    export ARMGNU=aarch64-linux-gnu

    if make 2>&1; then
        log_success "Built $lab_name"
    else
        log_error "Failed to build $lab_name"
        return 1
    fi
}

# Build example
build_example() {
    local example_path="$1"
    local example_name=$(basename "$example_path")

    log_info "Building example: $example_name"

    cd "$example_path"

    # Always clean to ensure fresh build
    make clean 2>/dev/null || true

    export ARMGNU=aarch64-linux-gnu

    if make 2>&1; then
        log_success "Built example $example_name"
    else
        log_error "Failed to build example $example_name"
        return 1
    fi
}

# Find and build all labs in a chapter
build_chapter() {
    local chapter="$1"
    local chapter_path="$PROJECT_ROOT/chapter_$chapter"
    local failed=0

    if [ ! -d "$chapter_path" ]; then
        log_error "Chapter $chapter not found"
        return 1
    fi

    log_info "Building chapter $chapter..."

    # Check if this is a BenOS chapter
    if [[ " $BENOS_CHAPTERS " =~ " $chapter " ]]; then
        for lab_path in "$chapter_path"/lab*; do
            if [ -d "$lab_path" ]; then
                build_benos "$lab_path" || ((failed++))
            fi
        done
    # Check if this is a user-mode chapter
    elif [[ " $USERMODE_CHAPTERS " =~ " $chapter " ]]; then
        for lab_path in "$chapter_path"/lab*; do
            if [ -d "$lab_path" ]; then
                build_usermode "$lab_path" || ((failed++))
            fi
        done
        # Also build examples
        for example_path in "$chapter_path"/example*; do
            if [ -d "$example_path" ]; then
                build_example "$example_path" || ((failed++))
            fi
        done
    else
        # Bare-metal labs
        for lab_path in "$chapter_path"/lab*; do
            if [ -d "$lab_path" ]; then
                # Check for BenOS subdirectory
                if [ -d "$lab_path/BenOS" ]; then
                    build_bare_metal "$lab_path/BenOS" || ((failed++))
                elif [ -f "$lab_path/Makefile" ]; then
                    build_bare_metal "$lab_path" || ((failed++))
                fi
            fi
        done
    fi

    return $failed
}

# Build a specific lab
build_specific_lab() {
    local chapter="$1"
    local lab="$2"
    local chapter_path="$PROJECT_ROOT/chapter_$chapter"
    local lab_path="$chapter_path/$lab"

    if [ ! -d "$lab_path" ]; then
        log_error "Lab $lab not found in chapter $chapter"
        return 1
    fi

    # Determine lab type and build
    if [[ " $BENOS_CHAPTERS " =~ " $chapter " ]]; then
        build_benos "$lab_path"
    elif [[ " $USERMODE_CHAPTERS " =~ " $chapter " ]]; then
        build_usermode "$lab_path"
    elif [ -d "$lab_path/BenOS" ]; then
        build_bare_metal "$lab_path/BenOS"
    else
        build_bare_metal "$lab_path"
    fi
}

# Build all labs
build_all() {
    local total=0
    local failed=0

    log_info "Building all labs for board: $BOARD"
    echo ""

    # Build bare-metal chapters
    for chapter in $BARE_METAL_CHAPTERS; do
        build_chapter "$chapter" || true
    done

    # Build BenOS chapters
    for chapter in $BENOS_CHAPTERS; do
        build_chapter "$chapter" || true
    done

    # Build user-mode chapters
    for chapter in $USERMODE_CHAPTERS; do
        build_chapter "$chapter" || true
    done

    echo ""
    log_info "Build complete"
}

# Parse arguments
POSITIONAL_ARGS=()

while [[ $# -gt 0 ]]; do
    case $1 in
        -b|--board)
            BOARD="$2"
            shift 2
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

cd "$PROJECT_ROOT"

# Execute based on remaining arguments
case $# in
    0)
        build_all
        ;;
    1)
        if [[ "$1" =~ ^[0-9]+$ ]]; then
            build_chapter "$1"
        else
            log_error "Invalid chapter number: $1"
            exit 1
        fi
        ;;
    2)
        build_specific_lab "$1" "$2"
        ;;
    *)
        log_error "Too many arguments"
        print_usage
        exit 1
        ;;
esac