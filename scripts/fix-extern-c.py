#!/usr/bin/env python3
# fix-extern-c.py - Fix extern "C" includes in kernel.cpp

import glob
import re

def fix_kernel_cpp(filepath):
    with open(filepath, 'r') as f:
        content = f.read()

    original = content

    # Pattern 1: #include "uart.h" without extern "C"
    if re.search(r'#include\s*"uart\.h"', content) and 'extern "C" {\n#include "uart.h"' not in content:
        # Replace simple include with extern "C" wrapped
        content = re.sub(
            r'#include\s*"uart\.h"',
            'extern "C" {\n#include "uart.h"\n}',
            content
        )

    # Pattern 2: already has extern "C" but needs uart.h inside
    if 'extern "C" {' in content and '#include "uart.h"' in content:
        # Make sure uart.h is inside extern "C"
        pass

    if content != original:
        with open(filepath, 'w') as f:
            f.write(content)
        print(f"Fixed: {filepath}")
        return True
    return False

# Fix all kernel.cpp files
fixed = 0
for filepath in glob.glob('**/kernel.cpp', recursive=True):
    if 'chapter_' in filepath:
        if fix_kernel_cpp(filepath):
            fixed += 1

print(f"\nTotal fixed: {fixed} files")