# Combat Resource Component Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Add the first reusable combat runtime component for health, armor-mitigated damage, resource spending, and class stat initialization.

**Architecture:** Implement `UEOCombatStatsComponent` as a focused actor component that owns mutable combat state while reusing the immutable `FEOCombatStats` data model. Attach it to `AEOHeroCharacter` so future abilities, enemies, companions, and UI all have a shared combat-state surface.

**Tech Stack:** Unreal Engine 5.7, C++, `UActorComponent`, existing `FEOCombatStats`.

---

## Tasks

### Task 1: Add Failing Combat Component Test

- [x] Create `Source/EverwildOdyssey/Private/Tests/EOCombatStatsComponentTests.cpp`.
- [x] Include `Gameplay/EOCombatStatsComponent.h`.
- [x] Test initialization, armor-mitigated damage, resource spending, and healing clamp.
- [x] Run the Unreal editor target build and verify it fails because the component header does not exist.

### Task 2: Implement Combat Component

- [x] Create `Source/EverwildOdyssey/Public/Gameplay/EOCombatStatsComponent.h`.
- [x] Create `Source/EverwildOdyssey/Private/Gameplay/EOCombatStatsComponent.cpp`.
- [x] Add the component to `AEOHeroCharacter`.
- [x] Run the Unreal editor target build and verify it succeeds.

### Task 3: Commit And Push

- [x] Run `Scripts/check-unreal.sh`.
- [x] Run `git status --short`.
- [ ] Commit with `feat: add combat stats component`.
- [ ] Push `feature/alpha-foundation`.
