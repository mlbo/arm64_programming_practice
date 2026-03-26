#!/bin/bash
# Run script for lab23-2

set -e

if [ ! -f sve_interleaved_test ]; then
    echo "Binary not found. Running build first..."
    ./scripts/build.sh
fi

echo "Running SVE interleaved test..."
./sve_interleaved_test