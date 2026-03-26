#!/bin/bash
# fix-singleton.sh - Fix singleton pattern in all pl_uart.cpp files

set -e

fix_pl_uart() {
    local file="$1"

    # Check if file has the problematic pattern
    if ! grep -q "namespace {" "$file"; then
        echo "Already fixed: $file"
        return
    fi

    # Get the rest of the file after line 14 (skip the old singleton code)
    local rest
    rest=$(sed -n '16,$p' "$file")

    # Write new file
    cat > "$file" << 'EOF'
// pl_uart.cpp - PL011 UART driver implementation
#include "uart.hpp"

namespace arm64lab {

auto UartDriver::instance() -> UartDriver& {
    static UartDriver inst;
    return inst;
}

UartDriver& g_uart = UartDriver::instance();

EOF

    # Append the rest
    echo "$rest" >> "$file"

    echo "Fixed: $file"
}

# Find and fix all pl_uart.cpp files
find . -path "./chapter_*" -name "pl_uart.cpp" | while read -r file; do
    fix_pl_uart "$file"
done

echo "Done!"