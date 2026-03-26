#!/bin/bash
# fix-assembly-guards.sh - Fix types.hpp for assembly compatibility

set -e

# Template for types.hpp with assembly guards
create_types_hpp() {
    local file="$1"
    local namespace="${2:-arm64lab}"

    cat > "$file" << 'EOF'
// types.hpp - ARM64 bare-metal type definitions
// 使用编译器内置类型，不依赖标准库
#pragma once

// Constants needed by both C++ and assembly
#define PAGE_SHIFT    12
#define TABLE_SHIFT   9
#define SECTION_SHIFT (PAGE_SHIFT + TABLE_SHIFT)

#define PAGE_SIZE     (1ULL << PAGE_SHIFT)
#define SECTION_SIZE  (1ULL << SECTION_SHIFT)
#define LOW_MEMORY    (2 * SECTION_SIZE)

#ifndef __ASSEMBLY__

EOF

    echo "namespace $namespace {" >> "$file"

    cat >> "$file" << 'EOF'

// 使用 GCC/Clang 内置类型定义 (freestanding 兼容)
using u8   = __UINT8_TYPE__;
using u16  = __UINT16_TYPE__;
using u32  = __UINT32_TYPE__;
using u64  = __UINT64_TYPE__;
using usize = __SIZE_TYPE__;
using uptr = __UINTPTR_TYPE__;

using i8   = __INT8_TYPE__;
using i16  = __INT16_TYPE__;
using i32  = __INT32_TYPE__;
using i64  = __INT64_TYPE__;
using isize = __PTRDIFF_TYPE__;
using iptr = __INTPTR_TYPE__;

constexpr uptr PAGE_MASK = ~(PAGE_SIZE - 1);

constexpr auto page_align_up(uptr addr) -> uptr {
    return (addr + PAGE_SIZE - 1) & PAGE_MASK;
}

EOF

    echo "} // namespace $namespace" >> "$file"

    cat >> "$file" << 'EOF'

#endif // __ASSEMBLY__
EOF

    echo "Updated: $file"
}

# Fix mm.hpp with assembly guards
fix_mm_hpp() {
    local file="$1"
    local namespace="${2:-arm64lab}"

    cat > "$file" << EOF
// mm.hpp - Memory management constants and functions
#pragma once

#include "types.hpp"

#ifndef __ASSEMBLY__

namespace $namespace {

// Assembly function declaration
extern "C" void memzero(uptr src, usize n);

} // namespace $namespace

#endif // __ASSEMBLY__
EOF

    echo "Updated: $file"
}

# Add __ASSEMBLY__ to Makefile ASMOPS
fix_makefile() {
    local file="$1"

    if grep -q "ASMOPS.*-D__ASSEMBLY__" "$file"; then
        echo "Already has __ASSEMBLY__: $file"
        return
    fi

    # Try to add -D__ASSEMBLY__ to ASMOPS
    if grep -q "^ASMOPS" "$file"; then
        sed -i.bak 's/^ASMOPS = \(.*\)$/ASMOPS = \1 -D__ASSEMBLY__/' "$file"
        echo "Fixed ASMOPS in: $file"
    elif grep -q "ASMOPS=" "$file"; then
        sed -i.bak 's/ASMOPS=\(.*\)$/ASMOPS=\1 -D__ASSEMBLY__/' "$file"
        echo "Fixed ASMOPS in: $file"
    else
        echo "No ASMOPS found in: $file"
    fi
}

# Find and fix all types.hpp files
echo "=== Fixing types.hpp files ==="
find . -path "./chapter_*" -name "types.hpp" | while read -r file; do
    create_types_hpp "$file" "arm64lab"
done

# Find and fix all mm.hpp files
echo ""
echo "=== Fixing mm.hpp files ==="
find . -path "./chapter_*" -name "mm.hpp" | while read -r file; do
    fix_mm_hpp "$file" "arm64lab"
done

# Find and fix all Makefiles
echo ""
echo "=== Fixing Makefiles ==="
find . -path "./chapter_*/lab*" -name "Makefile" | while read -r file; do
    fix_makefile "$file"
done

echo ""
echo "Done!"