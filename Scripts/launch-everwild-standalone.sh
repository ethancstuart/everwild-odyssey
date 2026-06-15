#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
PROJECT_FILE="$ROOT_DIR/EverwildOdyssey.uproject"
UNREAL_EDITOR="/Users/Shared/Epic Games/UE_5.7/Engine/Binaries/Mac/UnrealEditor.app/Contents/MacOS/UnrealEditor"

if [[ ! -x "$UNREAL_EDITOR" ]]; then
  echo "Missing UnrealEditor executable: $UNREAL_EDITOR"
  exit 1
fi

"$UNREAL_EDITOR" "$PROJECT_FILE" -game -ResX=1280 -ResY=720 -windowed -log
