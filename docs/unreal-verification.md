# Unreal Verification Notes

Date: 2026-06-14

## Environment

- Unreal Engine: `/Users/Shared/Epic Games/UE_5.7`
- Epic Games Launcher: `/Applications/Epic Games Launcher.app`
- Xcode: 26.3
- Apple clang: 17.0.0
- Git LFS: 3.7.1

## Passing Checks

Setup check:

```bash
Scripts/check-unreal.sh
```

Result:

- Epic Games Launcher found.
- Unreal Engine 5.7 found.
- Xcode/clang found.
- Git LFS found.

Project file generation:

```bash
"/Users/Shared/Epic Games/UE_5.7/Engine/Build/BatchFiles/Mac/GenerateProjectFiles.sh" \
  -project="/Users/ethanstuart/Projects/everwild-odyssey/.worktrees/alpha-foundation/EverwildOdyssey.uproject" \
  -game
```

Result: succeeded.

Editor target build:

```bash
"/Users/Shared/Epic Games/UE_5.7/Engine/Build/BatchFiles/Mac/Build.sh" \
  EverwildOdysseyEditor Mac Development \
  -Project="/Users/ethanstuart/Projects/everwild-odyssey/.worktrees/alpha-foundation/EverwildOdyssey.uproject" \
  -WaitMutex
```

Result: succeeded.

## Compatibility Fix

Unreal 5.7 required target files to use `BuildSettingsVersion.V6`.

Updated:

- `Source/EverwildOdyssey.Target.cs`
- `Source/EverwildOdysseyEditor.Target.cs`

## Automation Test Status

The automation test source files compile as part of the editor target:

- `EverwildOdyssey.Core.HeroClassCatalog`
- `EverwildOdyssey.Core.ProgressionModel`

However, command-line editor automation is currently blocked because `UnrealEditor-Cmd` hangs before normal log initialization, even for:

```bash
"/Users/Shared/Epic Games/UE_5.7/Engine/Binaries/Mac/UnrealEditor-Cmd" -help
```

Observed output before hang:

```text
Failure on line 688 in function id scheduleApplicationNotification(...)
Connection Invalid error for service com.apple.hiservices-xpcservice.
```

This appears to be an environment/editor startup issue rather than a project compile issue. Re-check automation after launching Unreal Editor normally once, accepting any first-run prompts, and confirming command-line editor startup can print help or create an `-AbsLog` file.

## Heroic MMO Rebuild Verification Commands

Build:

```bash
"/Users/Shared/Epic Games/UE_5.7/Engine/Build/BatchFiles/Mac/Build.sh" \
  EverwildOdysseyEditor Mac Development \
  -Project="/Users/ethanstuart/Projects/everwild-odyssey/.worktrees/alpha-foundation/EverwildOdyssey.uproject" \
  -WaitMutex
```

Standalone launch:

```bash
Scripts/launch-everwild-standalone.sh
```

Visual pass/fail criteria live in `docs/heroic-mmo-visual-verification.md`.
