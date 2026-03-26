#!/bin/bash
# fix-makefile.sh - Fix Makefile to exclude C files when C++ equivalent exists

set -e

fix_makefile() {
    local file="$1"

    # Check if already fixed
    if grep -q "filter-out.*CPP_FILES" "$file"; then
        echo "Already fixed: $file"
        return
    fi

    # Check if file has the pattern we need to fix
    if ! grep -q "^C_FILES = \$(wildcard" "$file"; then
        echo "No C_FILES pattern: $file"
        return
    fi

    # Create backup
    cp "$file" "$file.bak2"

    # Use sed to replace the C_FILES line
    sed -i '' 's/^C_FILES = \$(wildcard \$\(SRC_DIR\)\/\*\.c)$/C_FILES = $(filter-out $(CPP_FILES:.cpp=.c),$(wildcard $(SRC_DIR)/*.c))/' "$file" 2>/dev/null || \
    sed -i 's/^C_FILES = \$(wildcard \$\(SRC_DIR\)\/\*\.c)$/C_FILES = $(filter-out $(CPP_FILES:.cpp=.c),$(wildcard $(SRC_DIR)/*.c))/' "$file"

    echo "Fixed: $file"
}

# Find and fix all Makefiles
find . -path "./chapter_*" -name "Makefile" | while read -r file; do
    fix_makefile "$file"
done

echo "Done!"