# Everwild Odyssey

Everwild Odyssey is an original heroic high-fantasy Unreal Engine C++ project. The first target is a single-player vertical slice that feels MMO-scale: class choice, controller-friendly combat, companions, quests, gear, a living hub, an explorable region, and a dungeon boss.

## Current Direction

- Engine: Unreal Engine 5.x.
- Language: C++ first.
- Target: PC/computer.
- Input: keyboard/mouse and plugged-in controller.
- First region: Dawnwatch Keep and Starfall Vale.
- First mode: single-player alpha foundation with MMO-style systems.

## Setup

1. Install Unreal Engine through Epic Games Launcher or a source build.
2. Run `Scripts/check-unreal.sh` from the project root.
3. Open `EverwildOdyssey.uproject` with the installed Unreal version.
4. Generate project files if Unreal prompts for them.
5. Build the editor target.

Detailed macOS setup notes live in `docs/setup-unreal-macos.md`.

## Heroic MMO Rebuild Verification

The asset-first rebuild is tracked by:

- `docs/superpowers/specs/2026-06-15-heroic-mmo-asset-first-rebuild-design.md`
- `docs/superpowers/plans/2026-06-15-heroic-mmo-asset-first-rebuild.md`
- `docs/assets/heroic-mmo-asset-manifest.md`
- `docs/heroic-mmo-visual-verification.md`

Launch a 1280x720 standalone play window with:

```bash
Scripts/launch-everwild-standalone.sh
```

Use a custom Unreal Editor executable when UE 5.7 is installed outside the common macOS locations:

```bash
UNREAL_EDITOR=/path/to/UnrealEditor Scripts/launch-everwild-standalone.sh
```

## Design Docs

- `docs/superpowers/specs/2026-06-14-everwild-odyssey-design.md`
- `docs/superpowers/plans/2026-06-14-everwild-odyssey-alpha-foundation.md`

## Asset Policy

The public repo should contain repo-safe original placeholders and source code only. Local-only premium or Marketplace assets belong in ignored paths such as `Content/LocalOnly/`, `Content/Marketplace/`, or `Plugins/Marketplace/`.
