# Playable Character And Camera Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Add the first playable Unreal runtime layer: game mode, player controller, hero character, isometric camera, and controller/keyboard movement bindings.

**Architecture:** Keep gameplay runtime classes in a focused `Gameplay/` folder. Use C++ first: `AEOGameMode` owns default class selection, `AEOPlayerController` configures player-facing input/cursor behavior, and `AEOHeroCharacter` owns movement, camera, and a visible repo-safe placeholder body.

**Tech Stack:** Unreal Engine 5.7, C++, `ACharacter`, `APlayerController`, `AGameModeBase`, legacy input mappings from `DefaultInput.ini`, engine template map until project `.umap` content can be created in the GUI editor.

---

## Tasks

### Task 1: Add Failing Playable Foundation Test

**Files:**

- Create: `Source/EverwildOdyssey/Private/Tests/EOPlayableFoundationTests.cpp`

- [ ] Add an automation test that includes `Gameplay/EOHeroCharacter.h` and checks default camera constants.
- [ ] Run the Unreal editor target build and verify it fails because `Gameplay/EOHeroCharacter.h` does not exist.

### Task 2: Add Runtime Gameplay Classes

**Files:**

- Create: `Source/EverwildOdyssey/Public/Gameplay/EOHeroCharacter.h`
- Create: `Source/EverwildOdyssey/Private/Gameplay/EOHeroCharacter.cpp`
- Create: `Source/EverwildOdyssey/Public/Gameplay/EOPlayerController.h`
- Create: `Source/EverwildOdyssey/Private/Gameplay/EOPlayerController.cpp`
- Create: `Source/EverwildOdyssey/Public/Gameplay/EOGameMode.h`
- Create: `Source/EverwildOdyssey/Private/Gameplay/EOGameMode.cpp`
- Modify: `Config/DefaultEngine.ini`

- [ ] Implement `AEOHeroCharacter` with capsule movement, a fixed isometric spring arm/camera, simple static mesh silhouette, and legacy input handlers for movement, attack, dodge, block, interact, and two abilities.
- [ ] Implement `AEOPlayerController` with game-only input and visible cursor disabled.
- [ ] Implement `AEOGameMode` with `AEOHeroCharacter` and `AEOPlayerController` as defaults.
- [ ] Point startup maps at `/Engine/Maps/Templates/Template_Default.Template_Default` until the Dawnwatch map can be authored in the editor.
- [ ] Run the Unreal editor target build and verify it succeeds.

### Task 3: Finish Verification

- [ ] Run `Scripts/check-unreal.sh`.
- [ ] Run the Unreal editor target build.
- [ ] Run `git status --short`.
- [ ] Commit the playable foundation.
