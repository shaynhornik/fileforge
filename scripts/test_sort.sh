#!/usr/bin/env bash
set -euo pipefail
SCRIPT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
REPO_ROOT=$(cd "$SCRIPT_DIR/.." && pwd)

cmake -S "$REPO_ROOT" -B "$REPO_ROOT/build" >/dev/null
cmake --build "$REPO_ROOT/build" >/dev/null
"$REPO_ROOT/build/fileforge_integration_test"
