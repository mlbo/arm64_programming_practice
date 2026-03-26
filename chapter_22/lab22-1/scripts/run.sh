#!/bin/bash
# Run script for lab22-1

set -e

if [ ! -f neon_rgb_bgr_test ]; then
    echo "Binary not found. Running build first..."
    ./scripts/build.sh
fi

echo "Running NEON RGB/BGR conversion test..."
./neon_rgb_bgr_test