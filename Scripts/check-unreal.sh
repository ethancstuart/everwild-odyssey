#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
PROJECT_FILE="$ROOT_DIR/EverwildOdyssey.uproject"

echo "Everwild Odyssey setup check"
echo "Project: $PROJECT_FILE"

if [[ ! -f "$PROJECT_FILE" ]]; then
  echo "Missing EverwildOdyssey.uproject"
  exit 1
fi

echo
echo "Toolchain:"
if command -v xcodebuild >/dev/null 2>&1; then
  xcodebuild -version
else
  echo "xcodebuild not found"
fi

if command -v clang >/dev/null 2>&1; then
  clang --version | head -n 1
else
  echo "clang not found"
fi

echo
echo "Unreal search:"
FOUND_UNREAL=0
for CANDIDATE in \
  "/Applications/Epic Games Launcher.app" \
  "/Applications/Unreal Engine.app" \
  "/Users/Shared/Epic Games" \
  "$HOME/Applications/Epic Games Launcher.app"; do
  if [[ -e "$CANDIDATE" ]]; then
    echo "Found: $CANDIDATE"
    FOUND_UNREAL=1
  fi
done

if [[ "$FOUND_UNREAL" -eq 0 ]]; then
  echo "No local Unreal/Epic install found in common macOS locations."
  echo "Install Unreal Engine with Epic Games Launcher, then open EverwildOdyssey.uproject."
fi

echo
echo "Git LFS:"
if command -v git-lfs >/dev/null 2>&1; then
  git-lfs --version
else
  echo "git-lfs not found; install it before committing binary Unreal assets."
fi
