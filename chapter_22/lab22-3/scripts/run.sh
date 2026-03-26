#!/bin/bash
# Run script for lab22-3

set -e

if [ ! -f auto_vectorization_test ]; then
    echo "Binary not found. Running build first..."
    ./scripts/build.sh
fi

echo "Running auto-vectorization test..."
./auto_vectorization_test