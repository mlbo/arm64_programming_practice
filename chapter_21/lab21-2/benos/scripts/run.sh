#!/bin/bash
# BenOS Run Script - Convenience wrapper for build.sh run

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
exec "$SCRIPT_DIR/build.sh" run