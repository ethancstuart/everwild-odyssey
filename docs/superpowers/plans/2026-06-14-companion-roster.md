# Companion Roster Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Add a companion roster runtime so the solo alpha can carry an MMO-style party composition.

**Architecture:** Implement `UEOCompanionRosterComponent` around the existing `FEOCompanionProfile` data model. Attach it to the hero with two default companions so future companion AI, party UI, and support abilities have a stable roster surface.

**Tech Stack:** Unreal Engine 5.7, C++, `UActorComponent`, existing `FEOCompanionProfile`, Unreal automation tests.

---

## Files

- Create: `Source/EverwildOdyssey/Public/Gameplay/EOCompanionRosterComponent.h`
- Create: `Source/EverwildOdyssey/Private/Gameplay/EOCompanionRosterComponent.cpp`
- Create: `Source/EverwildOdyssey/Private/Tests/EOCompanionRosterComponentTests.cpp`
- Modify: `Source/EverwildOdyssey/Public/Gameplay/EOHeroCharacter.h`
- Modify: `Source/EverwildOdyssey/Private/Gameplay/EOHeroCharacter.cpp`

## Tasks

### Task 1: Add Failing Companion Roster Test

- [x] Create `Source/EverwildOdyssey/Private/Tests/EOCompanionRosterComponentTests.cpp`.
- [x] Include `Gameplay/EOCompanionRosterComponent.h`.
- [x] Add a test named `EverwildOdyssey.Gameplay.CompanionRosterComponent`.
- [x] Test valid companion add, invalid rejection, duplicate rejection, activation, active cap, and deactivation.
- [x] Run the Unreal editor target build and verify it fails because `Gameplay/EOCompanionRosterComponent.h` does not exist.

Expected test behaviors:

```cpp
#if WITH_DEV_AUTOMATION_TESTS

#include "Gameplay/EOCompanionRosterComponent.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEOCompanionRosterComponentTest, "EverwildOdyssey.Gameplay.CompanionRosterComponent", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

namespace
{
FEOCompanionProfile TestCompanion(FName Id, const TCHAR* Name, EEOPartyRole Role)
{
    FEOCompanionProfile Companion;
    Companion.CompanionId = Id;
    Companion.DisplayName = FText::FromString(Name);
    Companion.Role = Role;
    Companion.FollowDistance = 320.0f;
    Companion.AbilityCheckInterval = 1.0f;
    return Companion;
}
}

bool FEOCompanionRosterComponentTest::RunTest(const FString& Parameters)
{
    UEOCompanionRosterComponent* Roster = NewObject<UEOCompanionRosterComponent>();

    const FEOCompanionProfile Mira = TestCompanion(TEXT("companion.mira"), TEXT("Mira of the Dawnwatch"), EEOPartyRole::Support);
    const FEOCompanionProfile Tor = TestCompanion(TEXT("companion.tor"), TEXT("Tor Valeguard"), EEOPartyRole::Defender);
    const FEOCompanionProfile InvalidCompanion;

    TestEqual(TEXT("Valid companion can be added."), Roster->AddCompanion(Mira), EEOCompanionRosterResult::Applied);
    TestEqual(TEXT("Second companion can be added."), Roster->AddCompanion(Tor), EEOCompanionRosterResult::Applied);
    TestEqual(TEXT("Invalid companion is rejected."), Roster->AddCompanion(InvalidCompanion), EEOCompanionRosterResult::InvalidCompanion);
    TestEqual(TEXT("Duplicate companion is rejected."), Roster->AddCompanion(Mira), EEOCompanionRosterResult::DuplicateCompanion);
    TestEqual(TEXT("Roster stores valid companions."), Roster->GetCompanionCount(), 2);

    TestEqual(TEXT("Companion can activate."), Roster->ActivateCompanion(TEXT("companion.mira")), EEOCompanionRosterResult::Applied);
    TestEqual(TEXT("Second companion can activate."), Roster->ActivateCompanion(TEXT("companion.tor")), EEOCompanionRosterResult::Applied);
    TestEqual(TEXT("Active companion count is tracked."), Roster->GetActiveCompanionCount(), 2);
    TestTrue(TEXT("Support role is active."), Roster->HasActiveRole(EEOPartyRole::Support));

    TestEqual(TEXT("Active cap blocks extra activation."), Roster->ActivateCompanion(TEXT("companion.mira")), EEOCompanionRosterResult::AlreadyActive);
    TestEqual(TEXT("Companion can deactivate."), Roster->DeactivateCompanion(TEXT("companion.mira")), EEOCompanionRosterResult::Applied);
    TestEqual(TEXT("Active count decreases."), Roster->GetActiveCompanionCount(), 1);

    return true;
}

#endif
```

Run:

```bash
"/Users/Shared/Epic Games/UE_5.7/Engine/Build/BatchFiles/Mac/Build.sh" EverwildOdysseyEditor Mac Development -Project="/Users/ethanstuart/Projects/everwild-odyssey/.worktrees/alpha-foundation/EverwildOdyssey.uproject" -WaitMutex
```

Expected: FAIL with an include error for `Gameplay/EOCompanionRosterComponent.h`.

### Task 2: Implement Companion Roster Component

- [x] Create `Source/EverwildOdyssey/Public/Gameplay/EOCompanionRosterComponent.h`.
- [x] Add `EEOCompanionRosterResult` with values `Applied`, `InvalidCompanion`, `DuplicateCompanion`, `MissingCompanion`, `AlreadyActive`, `NotActive`, and `PartyFull`.
- [x] Add methods `AddCompanion`, `ActivateCompanion`, `DeactivateCompanion`, `GetCompanionCount`, `GetActiveCompanionCount`, `HasCompanion`, and `HasActiveRole`.
- [x] Create `Source/EverwildOdyssey/Private/Gameplay/EOCompanionRosterComponent.cpp`.
- [x] Store companions in `TArray<FEOCompanionProfile>` and active companion IDs in `TArray<FName>`.
- [x] Set active companion cap to `2`.
- [x] Run the Unreal editor target build and verify it succeeds.

### Task 3: Attach Companion Roster To Hero

- [x] Forward declare `UEOCompanionRosterComponent` in `EOHeroCharacter.h`.
- [x] Add visible `CompanionRoster` component property.
- [x] Add public `GetCompanionRosterComponent` getter.
- [x] Create the `CompanionRoster` default subobject in the hero constructor.
- [x] In `BeginPlay`, add and activate two default companions: Mira support and Tor defender.
- [x] Run the Unreal editor target build and verify it succeeds.

### Task 4: Commit And Push

- [x] Run `Scripts/check-unreal.sh`.
- [x] Run `git diff --check`.
- [x] Run `git status --short`.
- [ ] Commit with `feat: add companion roster`.
- [ ] Push `feature/alpha-foundation`.
