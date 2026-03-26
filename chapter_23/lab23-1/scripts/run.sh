#!/bin/bash
# Run script for lab23-1

set -e

if [ ! -f sve_basic_test ]; then
    echo "Binary not found. Running build first..."
    ./scripts/build.sh
fi

echo "Running SVE basic test..."
./sve_basic_test