# Enemy Combatants Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Add the first reusable enemy combatant actor so Relic Surge can place hostile targets into the alpha world.

**Architecture:** Implement `AEOEnemyCharacter` as a lightweight enemy actor with a reusable archetype struct and the existing combat stats component. Spawn a small Relic Wisp group from `AEOGameMode` near the Starfall landmark so the world scaffold starts to feel like an encounter space instead of static scenery.

**Tech Stack:** Unreal Engine 5.7, C++, `ACharacter`, existing `UEOCombatStatsComponent`, engine primitive meshes, Unreal automation tests.

---

## Files

- Create: `Source/EverwildOdyssey/Public/Gameplay/EOEnemyCharacter.h`
- Create: `Source/EverwildOdyssey/Private/Gameplay/EOEnemyCharacter.cpp`
- Create: `Source/EverwildOdyssey/Private/Tests/EOEnemyCharacterTests.cpp`
- Modify: `Source/EverwildOdyssey/Private/Gameplay/EOGameMode.cpp`

## Tasks

### Task 1: Add Failing Enemy Character Test

- [x] Create `Source/EverwildOdyssey/Private/Tests/EOEnemyCharacterTests.cpp`.
- [x] Include `Gameplay/EOEnemyCharacter.h`.
- [x] Add a test named `EverwildOdyssey.Gameplay.EnemyCharacter`.
- [x] Test that the Relic Wisp archetype is valid, has a useful reward, has combat stats, and uses a readable threat radius.
- [x] Run the Unreal editor target build and verify it fails because `Gameplay/EOEnemyCharacter.h` does not exist.

Expected test behaviors:

```cpp
#if WITH_DEV_AUTOMATION_TESTS

#include "Gameplay/EOEnemyCharacter.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEOEnemyCharacterTest, "EverwildOdyssey.Gameplay.EnemyCharacter", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FEOEnemyCharacterTest::RunTest(const FString& Parameters)
{
    const FEOEnemyArchetype RelicWisp = AEOEnemyCharacter::BuildRelicWispArchetype();

    TestTrue(TEXT("Relic Wisp archetype is valid."), RelicWisp.IsValidForAlpha());
    TestEqual(TEXT("Relic Wisp has stable id."), RelicWisp.EnemyId, FName(TEXT("enemy.starfall.relic_wisp")));
    TestTrue(TEXT("Relic Wisp rewards early progress."), RelicWisp.ExperienceReward >= 10);
    TestTrue(TEXT("Relic Wisp has readable threat radius."), RelicWisp.ThreatRadius >= 600.0f);
    TestTrue(TEXT("Relic Wisp can take damage."), RelicWisp.BaseStats.MaxHealth > 0.0f);

    return true;
}

#endif
```

Run:

```bash
"/Users/Shared/Epic Games/UE_5.7/Engine/Build/BatchFiles/Mac/Build.sh" EverwildOdysseyEditor Mac Development -Project="/Users/ethanstuart/Projects/everwild-odyssey/.worktrees/alpha-foundation/EverwildOdyssey.uproject" -WaitMutex
```

Expected: FAIL with an include error for `Gameplay/EOEnemyCharacter.h`.

### Task 2: Implement Enemy Character

- [x] Create `Source/EverwildOdyssey/Public/Gameplay/EOEnemyCharacter.h`.
- [x] Add `FEOEnemyArchetype` with `EnemyId`, `DisplayName`, `BaseStats`, `ExperienceReward`, `ThreatRadius`, `bElite`, and `IsValidForAlpha`.
- [x] Add `AEOEnemyCharacter` with `BuildRelicWispArchetype`, `InitializeFromArchetype`, `ApplyIncomingHit`, `GetCombatStatsComponent`, `GetEnemyId`, `GetExperienceReward`, and `GetThreatRadius`.
- [x] Create `Source/EverwildOdyssey/Private/Gameplay/EOEnemyCharacter.cpp`.
- [x] Give the enemy a capsule, simple floating primitive mesh, combat stats component, and default Relic Wisp archetype.
- [x] Implement `ApplyIncomingHit` by forwarding damage to `UEOCombatStatsComponent`.
- [x] Run the Unreal editor target build and verify it succeeds.

### Task 3: Spawn Relic Wisps From Game Mode

- [x] Include `Gameplay/EOEnemyCharacter.h` in `EOGameMode.cpp`.
- [x] In `BeginPlay`, spawn three Relic Wisps near the Starfall Relic Surge landmark.
- [x] Initialize each spawned enemy from `AEOEnemyCharacter::BuildRelicWispArchetype()`.
- [x] Run the Unreal editor target build and verify it succeeds.

### Task 4: Commit And Push

- [x] Run `Scripts/check-unreal.sh`.
- [x] Run `git diff --check`.
- [x] Run `git status --short`.
- [ ] Commit with `feat: add enemy combatants`.
- [ ] Push `feature/alpha-foundation`.
