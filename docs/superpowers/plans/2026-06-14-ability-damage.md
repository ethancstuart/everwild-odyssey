# Ability Damage Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Make hero ability inputs damage enemies after a successful ability activation.

**Architecture:** Extend `FEOCombatResolution` with deterministic ability damage math, then route `AbilityOne` and `AbilityTwo` through a shared hero helper that activates the ability, finds a nearby enemy, and applies damage. Keep this single-target for now so the combat loop is playable before expanding into arcs, projectiles, and hit volumes.

**Tech Stack:** Unreal Engine 5.7, C++, existing `AEOHeroCharacter`, `AEOEnemyCharacter`, `UEOAbilityRuntimeComponent`, `FEOCombatResolution`, Unreal automation tests.

---

## Files

- Modify: `Source/EverwildOdyssey/Public/Gameplay/EOCombatResolution.h`
- Modify: `Source/EverwildOdyssey/Private/Tests/EOCombatResolutionTests.cpp`
- Modify: `Source/EverwildOdyssey/Public/Gameplay/EOHeroCharacter.h`
- Modify: `Source/EverwildOdyssey/Private/Gameplay/EOHeroCharacter.cpp`

## Tasks

### Task 1: Add Failing Ability Damage Test

- [x] Modify `Source/EverwildOdyssey/Private/Tests/EOCombatResolutionTests.cpp`.
- [x] Add expectations for `FEOCombatResolution::CalculateAbilityDamage`.
- [x] Run the Unreal editor target build and verify it fails because `CalculateAbilityDamage` does not exist.

Expected additional test lines:

```cpp
    Stats.AttackPower = 16.0f;
    Stats.SpellPower = 8.0f;
    TestEqual(TEXT("Ability damage mixes martial and spell scaling."), FEOCombatResolution::CalculateAbilityDamage(Stats, 4, false), 39.0f);
    TestEqual(TEXT("Ultimate ability damage gets a larger floor and bonus."), FEOCombatResolution::CalculateAbilityDamage(Stats, 4, true), 64.0f);
```

Run:

```bash
"/Users/Shared/Epic Games/UE_5.7/Engine/Build/BatchFiles/Mac/Build.sh" EverwildOdysseyEditor Mac Development -Project="/Users/ethanstuart/Projects/everwild-odyssey/.worktrees/alpha-foundation/EverwildOdyssey.uproject" -WaitMutex
```

Expected: FAIL with an error that `CalculateAbilityDamage` is not a member of `FEOCombatResolution`.

### Task 2: Implement Ability Damage Math

- [x] Modify `Source/EverwildOdyssey/Public/Gameplay/EOCombatResolution.h`.
- [x] Add `FEOCombatResolution::CalculateAbilityDamage(const FEOCombatStats& AttackerStats, int32 EquippedPower, bool bUltimate)`.
- [x] Implement normal ability damage as `max(8, AttackPower * 1.4 + SpellPower * 0.6 + EquippedPower + 8)`.
- [x] Implement ultimate damage by adding `25` to the normal ability damage and clamping to at least `25`.
- [x] Run the Unreal editor target build and verify it succeeds.

### Task 3: Wire Hero Abilities To Enemy Hits

- [x] Add `DefaultAbilityAttackRange = 420.0f` to `AEOHeroCharacter`.
- [x] Add private helper `ActivateAbilityAndDamageNearestEnemy(int32 AbilityIndex, bool bUltimate)`.
- [x] Update `AbilityOne` to call helper with index `2` and `false`.
- [x] Update `AbilityTwo` to call helper with index `3` and `false`.
- [x] Run the Unreal editor target build and verify it succeeds.

### Task 4: Commit And Push

- [x] Run `Scripts/check-unreal.sh`.
- [x] Run `git diff --check`.
- [x] Run `git status --short`.
- [ ] Commit with `feat: wire ability damage`.
- [ ] Push `feature/alpha-foundation`.
