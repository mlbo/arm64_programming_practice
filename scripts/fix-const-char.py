#!/usr/bin/env python3
# fix-const-char.py - Fix const char* issues in C/C++ files

import os
import glob
import re

def fix_file(filepath):
    with open(filepath, 'r') as f:
        content = f.read()

    original = content

    # Fix function declarations: void func(char* str) -> void func(const char* str)
    content = re.sub(r'void\s+uart_send_string\s*\(\s*char\s*\*\s*str\s*\)',
                     'void uart_send_string(const char* str)', content)

    # Fix function definitions
    content = re.sub(r'void\s+uart_send_string\s*\(\s*char\s*\s*str\s*\)',
                     'void uart_send_string(const char* str)', content)

    # Also fix any char *str patterns in function parameters
    content = re.sub(r'\(char\s*\*\s*str\)', '(const char* str)', content)

    if content != original:
        with open(filepath, 'w') as f:
            f.write(content)
        print(f"Fixed: {filepath}")
        return True
    return False

# Fix all uart.h and pl_uart.c files
fixed = 0
for pattern in ['**/uart.h', '**/pl_uart.c', '**/pl_uart.cpp', '**/kernel.cpp', '**/kernel.c']:
    for filepath in glob.glob(pattern, recursive=True):
        if 'chapter_' in filepath and fix_file(filepath):
            fixed += 1

print(f"\nTotal fixed: {fixed} files")