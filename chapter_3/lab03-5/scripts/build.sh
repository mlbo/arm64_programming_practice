#!/bin/bash
set -e
make clean 2>/dev/null || true
make
echo "Build complete."