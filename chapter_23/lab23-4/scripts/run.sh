#!/bin/bash
# Run script for lab23-4

set -e

if [ ! -f sve_strcmp_test ]; then
    echo "Binary not found. Running build first..."
    ./scripts/build.sh
fi

echo "Running SVE strcmp test..."
./sve_strcmp_test