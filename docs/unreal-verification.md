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

## Heroic MMO Rebuild Visual Review

Date: 2026-06-15

- Build result: `Scripts/check-unreal.sh` passed; final `EverwildOdysseyEditor Mac Development` build succeeded with UE 5.7.
- Command-line automation result: `UnrealEditor-Cmd` with both positional project path and `-Project=` syntax exited without running the project automation suite and reported `Failed to find game directory: /Users/Shared/Epic Games/UE_5.7/EverwildOdyssey/Binaries`. Treat editor-target build plus standalone launch as the verified path until commandlet startup is corrected.
- Standalone launch result: initial launch connected to an idle Unreal home window; after quitting that idle window, `Scripts/launch-everwild-standalone.sh` opened `Everwild Odyssey Everwild Odyssey Alpha` in a 1280x720 standalone window.
- First-frame read: pass for current proxy milestone. The frame shows a dense authored proxy scene with foreground/midground/background read, heavy foliage/rocks, Dawnwatch-blue structural dressing, road markers, target frame, player/party HUD, quest tracker, event feed, minimap, and controller glyph prompts.
- Combat-frame read: partial pass. The launched frame shows the Relic Sentinel target frame, relic surge warning state, action bar, and event feed; live combat interaction still needs a hands-on play pass.
- Controller verification: mapped, not physically tested.
- Required missing asset roles: no launch-log review found a required-role fatal error during this pass, but the asset manifest still records that no committed fantasy character, enemy, environment, animation, or UI asset pack is present.
- Optional proxy roles: extensive. Current visual rescue uses Engine-safe primitive and volumetric proxy assemblies, not final production art.
- Biggest remaining visual gap: the scene is now more composed and readable, but it still lacks real stylized fantasy character/environment assets, authored animations, bespoke VFX, and production UI art.
