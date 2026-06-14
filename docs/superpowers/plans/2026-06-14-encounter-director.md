# Encounter Director Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Add a reusable encounter director that tracks enemy-wave lifecycle for world events such as Relic Surge.

**Architecture:** Implement `UEOEncounterDirectorComponent` as a focused actor component that owns encounter records and transitions them from dormant to active to cleared as enemies are defeated. Attach it to `AEOAlphaWorldScaffold` so the visible world scaffold has a combat-event state surface ready for enemy spawning, rewards, and quest hooks.

**Tech Stack:** Unreal Engine 5.7, C++, `UActorComponent`, `AActor`, Unreal automation tests.

---

## Files

- Create: `Source/EverwildOdyssey/Public/Gameplay/EOEncounterDirectorComponent.h`
- Create: `Source/EverwildOdyssey/Private/Gameplay/EOEncounterDirectorComponent.cpp`
- Create: `Source/EverwildOdyssey/Private/Tests/EOEncounterDirectorComponentTests.cpp`
- Modify: `Source/EverwildOdyssey/Public/Gameplay/EOAlphaWorldScaffold.h`
- Modify: `Source/EverwildOdyssey/Private/Gameplay/EOAlphaWorldScaffold.cpp`

## Tasks

### Task 1: Add Failing Encounter Director Test

- [x] Create `Source/EverwildOdyssey/Private/Tests/EOEncounterDirectorComponentTests.cpp`.
- [x] Include `Gameplay/EOEncounterDirectorComponent.h`.
- [x] Add a test named `EverwildOdyssey.Gameplay.EncounterDirectorComponent`.
- [x] Test initialization, start, enemy defeat progress, clear state, already-cleared rejection, and reset.
- [x] Run the Unreal editor target build and verify it fails because `Gameplay/EOEncounterDirectorComponent.h` does not exist.

Expected test behaviors:

```cpp
#if WITH_DEV_AUTOMATION_TESTS

#include "Gameplay/EOEncounterDirectorComponent.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEOEncounterDirectorComponentTest, "EverwildOdyssey.Gameplay.EncounterDirectorComponent", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FEOEncounterDirectorComponentTest::RunTest(const FString& Parameters)
{
    UEOEncounterDirectorComponent* Director = NewObject<UEOEncounterDirectorComponent>();

    FEOEncounterRecord RelicSurge;
    RelicSurge.EncounterId = TEXT("encounter.relic_surge.wave_one");
    RelicSurge.DisplayName = FText::FromString(TEXT("Relic Surge: First Wave"));
    RelicSurge.LinkedWorldEventId = TEXT("starfall.relic_surge");
    RelicSurge.TotalEnemies = 3;
    RelicSurge.RecommendedLevel = 1;

    Director->InitializeEncounters({ RelicSurge });

    TestEqual(TEXT("Director stores initialized encounter."), Director->GetEncounterCount(), 1);
    TestEqual(TEXT("Encounter starts dormant."), Director->GetEncounterState(TEXT("encounter.relic_surge.wave_one")), EEOEncounterState::Dormant);

    TestEqual(TEXT("Dormant encounter can start."), Director->StartEncounter(TEXT("encounter.relic_surge.wave_one")), EEOEncounterActionResult::Applied);
    TestEqual(TEXT("Started encounter is active."), Director->GetEncounterState(TEXT("encounter.relic_surge.wave_one")), EEOEncounterState::Active);
    TestEqual(TEXT("Started encounter fills remaining enemies."), Director->GetRemainingEnemies(TEXT("encounter.relic_surge.wave_one")), 3);

    TestEqual(TEXT("Enemy defeat advances encounter."), Director->RegisterEnemyDefeated(TEXT("encounter.relic_surge.wave_one")), EEOEncounterActionResult::Applied);
    TestEqual(TEXT("Remaining enemies decreases."), Director->GetRemainingEnemies(TEXT("encounter.relic_surge.wave_one")), 2);

    TestEqual(TEXT("Final defeats clear encounter."), Director->RegisterEnemyDefeated(TEXT("encounter.relic_surge.wave_one"), 5), EEOEncounterActionResult::Applied);
    TestEqual(TEXT("Encounter records cleared state."), Director->GetEncounterState(TEXT("encounter.relic_surge.wave_one")), EEOEncounterState::Cleared);
    TestEqual(TEXT("Cleared encounter has no remaining enemies."), Director->GetRemainingEnemies(TEXT("encounter.relic_surge.wave_one")), 0);

    TestEqual(TEXT("Cleared encounter cannot restart without reset."), Director->StartEncounter(TEXT("encounter.relic_surge.wave_one")), EEOEncounterActionResult::AlreadyCleared);
    TestEqual(TEXT("Encounter can reset."), Director->ResetEncounter(TEXT("encounter.relic_surge.wave_one")), EEOEncounterActionResult::Applied);
    TestEqual(TEXT("Reset encounter becomes dormant."), Director->GetEncounterState(TEXT("encounter.relic_surge.wave_one")), EEOEncounterState::Dormant);
    TestEqual(TEXT("Reset encounter restores remaining enemies."), Director->GetRemainingEnemies(TEXT("encounter.relic_surge.wave_one")), 3);

    return true;
}

#endif
```

Run:

```bash
"/Users/Shared/Epic Games/UE_5.7/Engine/Build/BatchFiles/Mac/Build.sh" EverwildOdysseyEditor Mac Development -Project="/Users/ethanstuart/Projects/everwild-odyssey/.worktrees/alpha-foundation/EverwildOdyssey.uproject" -WaitMutex
```

Expected: FAIL with an include error for `Gameplay/EOEncounterDirectorComponent.h`.

### Task 2: Implement Encounter Director Component

- [x] Create `Source/EverwildOdyssey/Public/Gameplay/EOEncounterDirectorComponent.h`.
- [x] Add `EEOEncounterState` with values `Dormant`, `Active`, and `Cleared`.
- [x] Add `EEOEncounterActionResult` with values `Applied`, `UnknownEncounter`, `Unavailable`, `AlreadyActive`, `AlreadyCleared`, and `NotActive`.
- [x] Add `FEOEncounterRecord` with `EncounterId`, `DisplayName`, `LinkedWorldEventId`, `State`, `TotalEnemies`, `RemainingEnemies`, `RecommendedLevel`, and `IsValidForAlpha`.
- [x] Add methods `InitializeEncounters`, `StartEncounter`, `RegisterEnemyDefeated`, `ResetEncounter`, `GetEncounterCount`, `GetEncounterState`, `GetRemainingEnemies`, and `HasEncounter`.
- [x] Create `Source/EverwildOdyssey/Private/Gameplay/EOEncounterDirectorComponent.cpp`.
- [x] Implement lifecycle transitions with clamped remaining enemy counts.
- [x] Run the Unreal editor target build and verify it succeeds.

### Task 3: Attach Encounter Director To World Scaffold

- [x] Forward declare `UEOEncounterDirectorComponent` in `EOAlphaWorldScaffold.h`.
- [x] Add a visible `EncounterDirector` component property to `AEOAlphaWorldScaffold`.
- [x] Add a public `GetEncounterDirectorComponent` getter.
- [x] Override `BeginPlay`.
- [x] Create the `EncounterDirector` default subobject in the scaffold constructor.
- [x] In `BeginPlay`, initialize one dormant Relic Surge encounter with three enemies.
- [x] Run the Unreal editor target build and verify it succeeds.

### Task 4: Commit And Push

- [x] Run `Scripts/check-unreal.sh`.
- [x] Run `git diff --check`.
- [x] Run `git status --short`.
- [ ] Commit with `feat: add encounter director`.
- [ ] Push `feature/alpha-foundation`.
