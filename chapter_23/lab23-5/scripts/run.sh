#!/bin/bash
# Run script for lab23-5

set -e

if [ ! -f sve_matrix_test ]; then
    echo "Binary not found. Running build first..."
    ./scripts/build.sh
fi

echo "Running SVE matrix test..."
./sve_matrix_test