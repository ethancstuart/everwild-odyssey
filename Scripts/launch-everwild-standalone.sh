#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
PROJECT_FILE="$ROOT_DIR/EverwildOdyssey.uproject"
DEFAULT_UNREAL_EDITOR="/Users/Shared/Epic Games/UE_5.7/Engine/Binaries/Mac/UnrealEditor.app/Contents/MacOS/UnrealEditor"
UNREAL_EDITOR="${UNREAL_EDITOR:-}"

if [[ -z "$UNREAL_EDITOR" ]]; then
  for CANDIDATE in \
    "$DEFAULT_UNREAL_EDITOR" \
    "/Applications/Unreal Engine/UE_5.7/Engine/Binaries/Mac/UnrealEditor.app/Contents/MacOS/UnrealEditor" \
    "${HOME:-}/Applications/Unreal Engine/UE_5.7/Engine/Binaries/Mac/UnrealEditor.app/Contents/MacOS/UnrealEditor"; do
    if [[ -x "$CANDIDATE" ]]; then
      UNREAL_EDITOR="$CANDIDATE"
      break
    fi
  done
fi

if [[ ! -x "$UNREAL_EDITOR" ]]; then
  echo "Missing UnrealEditor executable: $UNREAL_EDITOR"
  echo "Set UNREAL_EDITOR=/path/to/UnrealEditor to use a custom Unreal Engine 5.7 install."
  exit 1
fi

"$UNREAL_EDITOR" "$PROJECT_FILE" -game -ResX=1280 -ResY=720 -windowed -log
