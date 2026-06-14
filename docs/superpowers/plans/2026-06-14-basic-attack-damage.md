# Basic Attack Damage Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Make the hero's basic attack damage the nearest living enemy in range using hero stats and equipped gear power.

**Architecture:** Add a small `FEOCombatResolution` helper for deterministic damage calculations, then wire `AEOHeroCharacter::BasicAttack` to find the nearest `AEOEnemyCharacter` and apply a hit. Keep targeting simple and local for this slice so future lock-on, cones, projectiles, and AI threat can replace it cleanly.

**Tech Stack:** Unreal Engine 5.7, C++, existing `AEOHeroCharacter`, `AEOEnemyCharacter`, `UEOCombatStatsComponent`, `UEOInventoryComponent`, Unreal automation tests.

---

## Files

- Create: `Source/EverwildOdyssey/Public/Gameplay/EOCombatResolution.h`
- Create: `Source/EverwildOdyssey/Private/Tests/EOCombatResolutionTests.cpp`
- Modify: `Source/EverwildOdyssey/Public/Gameplay/EOHeroCharacter.h`
- Modify: `Source/EverwildOdyssey/Private/Gameplay/EOHeroCharacter.cpp`

## Tasks

### Task 1: Add Failing Combat Resolution Test

- [x] Create `Source/EverwildOdyssey/Private/Tests/EOCombatResolutionTests.cpp`.
- [x] Include `Gameplay/EOCombatResolution.h`.
- [x] Add a test named `EverwildOdyssey.Gameplay.CombatResolution`.
- [x] Test that basic attack damage adds attack power, equipped power, and a small floor, and clamps negative inputs.
- [x] Run the Unreal editor target build and verify it fails because `Gameplay/EOCombatResolution.h` does not exist.

Expected test behaviors:

```cpp
#if WITH_DEV_AUTOMATION_TESTS

#include "Gameplay/EOCombatResolution.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEOCombatResolutionTest, "EverwildOdyssey.Gameplay.CombatResolution", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FEOCombatResolutionTest::RunTest(const FString& Parameters)
{
    FEOCombatStats Stats;
    Stats.AttackPower = 16.0f;

    TestEqual(TEXT("Basic attack combines attack and gear power."), FEOCombatResolution::CalculateBasicAttackDamage(Stats, 4), 25.0f);

    Stats.AttackPower = -10.0f;
    TestEqual(TEXT("Basic attack clamps weak inputs to damage floor."), FEOCombatResolution::CalculateBasicAttackDamage(Stats, -5), 5.0f);

    return true;
}

#endif
```

Run:

```bash
"/Users/Shared/Epic Games/UE_5.7/Engine/Build/BatchFiles/Mac/Build.sh" EverwildOdysseyEditor Mac Development -Project="/Users/ethanstuart/Projects/everwild-odyssey/.worktrees/alpha-foundation/EverwildOdyssey.uproject" -WaitMutex
```

Expected: FAIL with an include error for `Gameplay/EOCombatResolution.h`.

### Task 2: Implement Combat Resolution Helper

- [x] Create `Source/EverwildOdyssey/Public/Gameplay/EOCombatResolution.h`.
- [x] Add `FEOCombatResolution::CalculateBasicAttackDamage`.
- [x] Implement basic attack damage as `max(5, AttackPower + EquippedPower + 5)`.
- [x] Run the Unreal editor target build and verify it succeeds.

### Task 3: Wire Hero Basic Attack To Enemies

- [x] Add `DefaultBasicAttackRange = 260.0f` to `AEOHeroCharacter`.
- [x] Add private `FindNearestEnemyInRange`.
- [x] Include `Gameplay/EOCombatResolution.h`, `Gameplay/EOEnemyCharacter.h`, `Gameplay/EOInventoryComponent.h`, and `Kismet/GameplayStatics.h` in `EOHeroCharacter.cpp`.
- [x] Update `BasicAttack` to find the nearest enemy in range and apply calculated damage.
- [x] Run the Unreal editor target build and verify it succeeds.

### Task 4: Commit And Push

- [x] Run `Scripts/check-unreal.sh`.
- [x] Run `git diff --check`.
- [x] Run `git status --short`.
- [ ] Commit with `feat: wire basic attack damage`.
- [ ] Push `feature/alpha-foundation`.
