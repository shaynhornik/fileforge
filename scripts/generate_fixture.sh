#!/usr/bin/env bash
set -euo pipefail
ROOT=${1:-./tmp-fixture}
SEED=${2:-1337}
TOP=${3:-12}
DIRS=${4:-4}
FILES=${5:-5}

SCRIPT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
REPO_ROOT=$(cd "$SCRIPT_DIR/.." && pwd)

cmake -S "$REPO_ROOT" -B "$REPO_ROOT/build" >/dev/null
cmake --build "$REPO_ROOT/build" >/dev/null
"$REPO_ROOT/build/fileforge" fixture "$ROOT" --seed "$SEED" --top-level-files "$TOP" --nested-directories "$DIRS" --nested-files-per-directory "$FILES"
