#!/bin/bash
# BenOS Build Script
# Usage: ./scripts/build.sh [build|run|clean|debug]

set -e

# Get the absolute path of the BenOS directory
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
BENOS_DIR="$(dirname "$SCRIPT_DIR")"

# Cross compiler prefix (can be overridden by environment)
CROSS_COMPILE="${CROSS_COMPILE:-aarch64-linux-gnu-}"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

log_info() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

log_warn() {
    echo -e "${YELLOW}[WARN]${NC} $1"
}

log_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

check_toolchain() {
    if ! command -v "${CROSS_COMPILE}gcc" &> /dev/null; then
        log_error "Cross compiler '${CROSS_COMPILE}gcc' not found!"
        log_info "Please install ARM64 toolchain:"
        echo "  Ubuntu/Debian: sudo apt-get install gcc-aarch64-linux-gnu"
        echo "  macOS: brew install aarch64-elf-gcc"
        exit 1
    fi
}

check_qemu() {
    if ! command -v qemu-system-aarch64 &> /dev/null; then
        log_error "QEMU for ARM64 not found!"
        log_info "Please install QEMU:"
        echo "  Ubuntu/Debian: sudo apt-get install qemu-system-arm"
        echo "  macOS: brew install qemu"
        exit 1
    fi
}

build() {
    log_info "Building BenOS..."
    cd "$BENOS_DIR"

    # Check toolchain
    check_toolchain

    # Clean previous build
    make clean 2>/dev/null || true

    # Build
    make CROSS_COMPILE="$CROSS_COMPILE"

    if [ -f "$BENOS_DIR/benos" ]; then
        log_info "Build successful!"
        log_info "Output: $BENOS_DIR/benos"
        ls -lh "$BENOS_DIR/benos"
    else
        log_error "Build failed - benos binary not found"
        exit 1
    fi
}

run() {
    # Build if needed
    if [ ! -f "$BENOS_DIR/benos" ]; then
        build
    fi

    # Check QEMU
    check_qemu

    log_info "Running BenOS in QEMU..."
    log_info "Press Ctrl+A then X to exit QEMU"

    qemu-system-aarch64 \
        -M raspi4b \
        -kernel "$BENOS_DIR/benos" \
        -nographic \
        -serial mon:stdio
}

debug() {
    # Build if needed
    if [ ! -f "$BENOS_DIR/benos" ]; then
        build
    fi

    # Check QEMU
    check_qemu

    log_info "Starting BenOS in debug mode..."
    log_info "QEMU GDB server listening on port 1234"
    log_info "Connect with: ${CROSS_COMPILE}gdb $BENOS_DIR/benos -ex 'target remote :1234'"

    qemu-system-aarch64 \
        -M raspi4b \
        -kernel "$BENOS_DIR/benos" \
        -nographic \
        -serial mon:stdio \
        -S -s
}

clean() {
    log_info "Cleaning BenOS..."
    cd "$BENOS_DIR"
    make clean
    log_info "Clean complete"
}

mrproper() {
    log_info "Deep cleaning BenOS..."
    cd "$BENOS_DIR"
    make mrproper
    log_info "Mrproper complete"
}

defconfig() {
    log_info "Generating default config..."
    cd "$BENOS_DIR"
    make defconfig
    log_info "Default config generated"
}

usage() {
    echo "BenOS Build Script"
    echo ""
    echo "Usage: $0 [command]"
    echo ""
    echo "Commands:"
    echo "  build     Build BenOS kernel (default)"
    echo "  run       Build and run in QEMU"
    echo "  debug     Build and run with GDB server on port 1234"
    echo "  clean     Clean build artifacts"
    echo "  mrproper  Deep clean (removes .config)"
    echo "  defconfig Generate default configuration"
    echo ""
    echo "Environment Variables:"
    echo "  CROSS_COMPILE  Cross compiler prefix (default: aarch64-linux-gnu-)"
    echo ""
    echo "Examples:"
    echo "  $0 build"
    echo "  $0 run"
    echo "  CROSS_COMPILE=aarch64-elf- $0 build"
}

# Main
case "${1:-build}" in
    build)
        build
        ;;
    run)
        run
        ;;
    debug)
        debug
        ;;
    clean)
        clean
        ;;
    mrproper)
        mrproper
        ;;
    defconfig)
        defconfig
        ;;
    help|--help|-h)
        usage
        ;;
    *)
        log_error "Unknown command: $1"
        usage
        exit 1
        ;;
esac