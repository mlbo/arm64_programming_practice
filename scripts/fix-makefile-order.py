#!/usr/bin/env python3
# fix-makefile-order.py - Fix Makefile variable order

import glob
import re

def fix_makefile(filepath):
    with open(filepath, 'r') as f:
        content = f.read()

    # Check if there's the problematic pattern
    if 'C_FILES = $(filter-out' in content and 'CPP_FILES = $(wildcard' in content:
        # Find and reorder
        old_pattern = r'C_FILES = \$\(filter-out \$\(CPP_FILES:\.cpp=\.c\),\$\(wildcard \$\(SRC_DIR\)/\*\.c\)\)\nCPP_FILES = \$\(wildcard \$\(SRC_DIR\)/\*\.cpp\)'
        new_pattern = 'CPP_FILES = $(wildcard $(SRC_DIR)/*.cpp)\nC_FILES = $(filter-out $(CPP_FILES:.cpp=.c),$(wildcard $(SRC_DIR)/*.c))'

        new_content = re.sub(old_pattern, new_pattern, content)

        if new_content != content:
            with open(filepath, 'w') as f:
                f.write(new_content)
            print(f"Fixed: {filepath}")
            return True
    return False

# Fix all Makefiles
fixed = 0
for filepath in glob.glob('**/Makefile', recursive=True):
    if 'chapter_' in filepath and 'lab' in filepath:
        if fix_makefile(filepath):
            fixed += 1

print(f"\nTotal fixed: {fixed} files")