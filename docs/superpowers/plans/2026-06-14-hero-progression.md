# Hero Progression Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Add hero XP and level progression, then award enemy XP once when defeated.

**Architecture:** Implement `UEOHeroProgressionComponent` as a focused actor component with deterministic level thresholds. Add one-time reward claiming to `AEOEnemyCharacter`, then let hero attacks and abilities grant XP when they defeat a target.

**Tech Stack:** Unreal Engine 5.7, C++, `UActorComponent`, existing hero/enemy combat classes, Unreal automation tests.

---

## Files

- Create: `Source/EverwildOdyssey/Public/Gameplay/EOHeroProgressionComponent.h`
- Create: `Source/EverwildOdyssey/Private/Gameplay/EOHeroProgressionComponent.cpp`
- Create: `Source/EverwildOdyssey/Private/Tests/EOHeroProgressionComponentTests.cpp`
- Modify: `Source/EverwildOdyssey/Public/Gameplay/EOEnemyCharacter.h`
- Modify: `Source/EverwildOdyssey/Private/Gameplay/EOEnemyCharacter.cpp`
- Modify: `Source/EverwildOdyssey/Public/Gameplay/EOHeroCharacter.h`
- Modify: `Source/EverwildOdyssey/Private/Gameplay/EOHeroCharacter.cpp`

## Tasks

### Task 1: Add Failing Progression Test

- [x] Create `Source/EverwildOdyssey/Private/Tests/EOHeroProgressionComponentTests.cpp`.
- [x] Include `Gameplay/EOHeroProgressionComponent.h`.
- [x] Add a test named `EverwildOdyssey.Gameplay.HeroProgressionComponent`.
- [x] Test initial level, XP gains, level-up carryover, next-level threshold, and invalid XP rejection.
- [x] Run the Unreal editor target build and verify it fails because `Gameplay/EOHeroProgressionComponent.h` does not exist.

Expected test behaviors:

```cpp
#if WITH_DEV_AUTOMATION_TESTS

#include "Gameplay/EOHeroProgressionComponent.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEOHeroProgressionComponentTest, "EverwildOdyssey.Gameplay.HeroProgressionComponent", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FEOHeroProgressionComponentTest::RunTest(const FString& Parameters)
{
    UEOHeroProgressionComponent* Progression = NewObject<UEOHeroProgressionComponent>();

    TestEqual(TEXT("Hero starts at level one."), Progression->GetHeroLevel(), 1);
    TestEqual(TEXT("Hero starts with no experience."), Progression->GetExperience(), 0);
    TestEqual(TEXT("First level threshold is one hundred."), Progression->GetExperienceForNextLevel(), 100);

    TestEqual(TEXT("Positive experience is applied."), Progression->AddExperience(60), EEOProgressionResult::Applied);
    TestEqual(TEXT("Experience accumulates before level up."), Progression->GetExperience(), 60);
    TestEqual(TEXT("Hero remains level one before threshold."), Progression->GetHeroLevel(), 1);

    TestEqual(TEXT("Experience can level up with carryover."), Progression->AddExperience(50), EEOProgressionResult::LeveledUp);
    TestEqual(TEXT("Hero reaches level two."), Progression->GetHeroLevel(), 2);
    TestEqual(TEXT("Carryover experience is retained."), Progression->GetExperience(), 10);
    TestEqual(TEXT("Second level threshold scales."), Progression->GetExperienceForNextLevel(), 200);

    TestEqual(TEXT("Invalid experience is rejected."), Progression->AddExperience(-5), EEOProgressionResult::InvalidExperience);

    return true;
}

#endif
```

Run:

```bash
"/Users/Shared/Epic Games/UE_5.7/Engine/Build/BatchFiles/Mac/Build.sh" EverwildOdysseyEditor Mac Development -Project="/Users/ethanstuart/Projects/everwild-odyssey/.worktrees/alpha-foundation/EverwildOdyssey.uproject" -WaitMutex
```

Expected: FAIL with an include error for `Gameplay/EOHeroProgressionComponent.h`.

### Task 2: Implement Hero Progression Component

- [x] Create `Source/EverwildOdyssey/Public/Gameplay/EOHeroProgressionComponent.h`.
- [x] Add `EEOProgressionResult` with values `Applied`, `LeveledUp`, and `InvalidExperience`.
- [x] Add methods `AddExperience`, `GetHeroLevel`, `GetExperience`, and `GetExperienceForNextLevel`.
- [x] Create `Source/EverwildOdyssey/Private/Gameplay/EOHeroProgressionComponent.cpp`.
- [x] Implement threshold as `HeroLevel * 100`, retaining carryover XP when leveling.
- [x] Run the Unreal editor target build and verify it succeeds.

### Task 3: Add One-Time Enemy Rewards

- [x] Add `ClaimExperienceReward` to `AEOEnemyCharacter`.
- [x] Add private `bExperienceRewardClaimed` to `AEOEnemyCharacter`.
- [x] Implement `ClaimExperienceReward` to return zero until the enemy is dead, then return `ExperienceReward` once.
- [x] Run the Unreal editor target build and verify it succeeds.

### Task 4: Attach Progression To Hero Combat

- [x] Forward declare `UEOHeroProgressionComponent` in `EOHeroCharacter.h`.
- [x] Add visible `Progression` component property.
- [x] Add public `GetProgressionComponent` getter.
- [x] Create the `Progression` default subobject in the hero constructor.
- [x] Add private `AwardEnemyIfDefeated(AEOEnemyCharacter* Enemy)`.
- [x] Call `AwardEnemyIfDefeated` after basic attack and ability damage.
- [x] Run the Unreal editor target build and verify it succeeds.

### Task 5: Commit And Push

- [x] Run `Scripts/check-unreal.sh`.
- [x] Run `git diff --check`.
- [x] Run `git status --short`.
- [ ] Commit with `feat: add hero progression rewards`.
- [ ] Push `feature/alpha-foundation`.
