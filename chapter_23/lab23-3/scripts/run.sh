#!/bin/bash
# Run script for lab23-3

set -e

if [ ! -f sve_memcpy_test ]; then
    echo "Binary not found. Running build first..."
    ./scripts/build.sh
fi

echo "Running SVE memcpy test..."
./sve_memcpy_test