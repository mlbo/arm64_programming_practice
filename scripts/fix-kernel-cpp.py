#!/usr/bin/env python3
# fix-kernel-cpp.py - Fix kernel.cpp issues

import os
import glob
import re

def fix_memset_decl(content):
    """Add memset declaration if missing"""
    if 'memset(' in content and 'extern "C" void* memset' not in content:
        # Find the first #include and add declaration after it
        lines = content.split('\n')
        new_lines = []
        added = False
        for line in lines:
            new_lines.append(line)
            if not added and line.startswith('#include'):
                new_lines.append('')
                new_lines.append('// 外部函数声明')
                new_lines.append('extern "C" void* memset(void* s, int c, unsigned long count);')
                added = True
        return '\n'.join(new_lines)
    return content

def fix_atomic_set(content):
    """Fix atomic_set declaration mismatch"""
    # Change atomic_set(unsigned long a, unsigned long b) to atomic_set(volatile unsigned long *addr, unsigned long newval)
    content = re.sub(
        r'void\s+atomic_set\s*\(\s*unsigned\s+long\s+a\s*,\s*unsigned\s+long\s+b\s*\)',
        'void atomic_set(volatile unsigned long* addr, unsigned long newval)',
        content
    )
    return content

def fix_kernel_cpp(filepath):
    with open(filepath, 'r') as f:
        content = f.read()

    original = content
    content = fix_memset_decl(content)
    content = fix_atomic_set(content)

    if content != original:
        with open(filepath, 'w') as f:
            f.write(content)
        print(f"Fixed: {filepath}")
        return True
    return False

# Fix all kernel.cpp files
fixed = 0
for filepath in glob.glob('**/kernel.cpp', recursive=True):
    if 'chapter_' in filepath and fix_kernel_cpp(filepath):
        fixed += 1

print(f"\nTotal fixed: {fixed} files")