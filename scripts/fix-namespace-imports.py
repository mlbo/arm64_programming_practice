#!/usr/bin/env python3
# fix-namespace-imports.py - Fix namespace imports for chapter_22 and chapter_23

import glob
import re

def fix_main_cpp(filepath):
    with open(filepath, 'r') as f:
        content = f.read()

    # Check if already has using declarations
    if 'using arm64lab::neon::u8' in content or 'using arm64lab::sve::u8' in content:
        return False

    # Find namespace (neon or sve)
    if 'arm64lab::neon' in content:
        ns = 'neon'
    elif 'arm64lab::sve' in content:
        ns = 'sve'
    else:
        return False

    # Add using declarations after includes
    pattern = r'(#include.*\n)+'
    replacement = r'\1\n// Import types from namespace\nusing arm64lab::' + ns + r'::u8;\nusing arm64lab::' + ns + r'::usize;\nusing arm64lab::' + ns + r'::u32;\n'

    new_content = re.sub(pattern, replacement, content, count=1)

    if new_content != content:
        with open(filepath, 'w') as f:
            f.write(new_content)
        print(f"Fixed: {filepath}")
        return True
    return False

# Fix all main.cpp files in chapter_22 and chapter_23
fixed = 0
for filepath in glob.glob('chapter_22/**/main.cpp', recursive=True) + glob.glob('chapter_23/**/main.cpp', recursive=True):
    if fix_main_cpp(filepath):
        fixed += 1

print(f"\nTotal fixed: {fixed} files")