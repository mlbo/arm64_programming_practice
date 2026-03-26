#!/bin/bash
# Run script for lab23-6

set -e

if [ ! -f sve_ffr_test ]; then
    echo "Binary not found. Running build first..."
    ./scripts/build.sh
fi

echo "Running SVE FFR test..."
./sve_ffr_test