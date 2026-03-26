#!/usr/bin/env python3
# fix-makefile.py - Fix Makefile to exclude C files when C++ equivalent exists

import os
import glob

def fix_makefile(filepath):
    with open(filepath, 'r') as f:
        content = f.read()

    # Check if already fixed
    if 'filter-out $(CPP_FILES:.cpp=.c)' in content:
        print(f"Already fixed: {filepath}")
        return

    # Find and replace the C_FILES line
    old_pattern = 'C_FILES = $(wildcard $(SRC_DIR)/*.c)'
    new_pattern = 'C_FILES = $(filter-out $(CPP_FILES:.cpp=.c),$(wildcard $(SRC_DIR)/*.c))'

    if old_pattern not in content:
        print(f"No pattern match: {filepath}")
        return

    content = content.replace(old_pattern, new_pattern)

    with open(filepath, 'w') as f:
        f.write(content)

    print(f"Fixed: {filepath}")

# Find all Makefiles
for chapter in ['chapter_2', 'chapter_3', 'chapter_4', 'chapter_5', 'chapter_6',
                'chapter_7', 'chapter_8', 'chapter_9', 'chapter_10', 'chapter_11',
                'chapter_12', 'chapter_13', 'chapter_14', 'chapter_15', 'chapter_16']:
    pattern = f"./{chapter}/**/Makefile"
    for filepath in glob.glob(pattern, recursive=True):
        if 'lab' in filepath:  # Only fix lab Makefiles
            fix_makefile(filepath)

print("Done!")