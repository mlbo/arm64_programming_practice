#!/bin/bash
# Run script for lab22-2

set -e

if [ ! -f neon_matrix_test ]; then
    echo "Binary not found. Running build first..."
    ./scripts/build.sh
fi

echo "Running NEON matrix multiplication test..."
./neon_matrix_test