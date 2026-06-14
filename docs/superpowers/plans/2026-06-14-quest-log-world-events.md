# Quest Log And World Events Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Add a stateful quest log and world-event runtime so the alpha can track story progress, interaction outcomes, and zone event state.

**Architecture:** Implement `UEOQuestLogComponent` as a focused actor component that owns quest records and world-event records. Attach it to `AEOHeroCharacter` so future NPCs, interactables, save/load, HUD, and zone directors can use one progression surface.

**Tech Stack:** Unreal Engine 5.7, C++, `UActorComponent`, existing `FEOQuestRecord`, existing `FEOWorldEventRecord`, Unreal automation tests.

---

## Files

- Create: `Source/EverwildOdyssey/Public/Gameplay/EOQuestLogComponent.h`
- Create: `Source/EverwildOdyssey/Private/Gameplay/EOQuestLogComponent.cpp`
- Create: `Source/EverwildOdyssey/Private/Tests/EOQuestLogComponentTests.cpp`
- Modify: `Source/EverwildOdyssey/Public/Gameplay/EOHeroCharacter.h`
- Modify: `Source/EverwildOdyssey/Private/Gameplay/EOHeroCharacter.cpp`

## Tasks

### Task 1: Add Failing Quest Log Test

- [x] Create `Source/EverwildOdyssey/Private/Tests/EOQuestLogComponentTests.cpp`.
- [x] Include `Gameplay/EOQuestLogComponent.h`.
- [x] Add a test named `EverwildOdyssey.Gameplay.QuestLogComponent`.
- [x] Test quest initialization, starting, advancing, completing, rewarding, locked quest rejection, and world-event state changes.
- [x] Run the Unreal editor target build and verify it fails because `Gameplay/EOQuestLogComponent.h` does not exist.

Expected test behaviors:

```cpp
#if WITH_DEV_AUTOMATION_TESTS

#include "Gameplay/EOQuestLogComponent.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEOQuestLogComponentTest, "EverwildOdyssey.Gameplay.QuestLogComponent", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

namespace
{
FEOQuestRecord TestQuest(FName Id, const TCHAR* Name, EEOQuestState State)
{
    FEOQuestRecord Quest;
    Quest.QuestId = Id;
    Quest.DisplayName = FText::FromString(Name);
    Quest.State = State;
    return Quest;
}

FEOWorldEventRecord TestWorldEvent(FName Id, const TCHAR* Name, EEOWorldEventState State)
{
    FEOWorldEventRecord Event;
    Event.EventId = Id;
    Event.DisplayName = FText::FromString(Name);
    Event.State = State;
    Event.RespawnSeconds = 120.0f;
    return Event;
}
}

bool FEOQuestLogComponentTest::RunTest(const FString& Parameters)
{
    UEOQuestLogComponent* QuestLog = NewObject<UEOQuestLogComponent>();

    QuestLog->InitializeQuests({
        TestQuest(TEXT("dawnwatch.starfall_arrival"), TEXT("The Starfall Gate"), EEOQuestState::Available),
        TestQuest(TEXT("dawnwatch.locked_depths"), TEXT("The Locked Depths"), EEOQuestState::Locked)
    });

    QuestLog->InitializeWorldEvents({
        TestWorldEvent(TEXT("starfall.relic_surge"), TEXT("Relic Surge"), EEOWorldEventState::Warning)
    });

    TestEqual(TEXT("Quest log stores initialized quests."), QuestLog->GetQuestCount(), 2);
    TestEqual(TEXT("World-event log stores initialized events."), QuestLog->GetWorldEventCount(), 1);

    TestEqual(TEXT("Available quest can start."), QuestLog->StartQuest(TEXT("dawnwatch.starfall_arrival")), EEOQuestActionResult::Applied);
    TestEqual(TEXT("Started quest becomes active."), QuestLog->GetQuestState(TEXT("dawnwatch.starfall_arrival")), EEOQuestState::Active);

    TestEqual(TEXT("Active quest advances."), QuestLog->AdvanceQuest(TEXT("dawnwatch.starfall_arrival")), EEOQuestActionResult::Applied);
    TestEqual(TEXT("Quest step increments."), QuestLog->GetQuestStepIndex(TEXT("dawnwatch.starfall_arrival")), 1);

    TestEqual(TEXT("Active quest can complete."), QuestLog->CompleteQuest(TEXT("dawnwatch.starfall_arrival")), EEOQuestActionResult::Applied);
    TestEqual(TEXT("Completed quest can reward."), QuestLog->RewardQuest(TEXT("dawnwatch.starfall_arrival")), EEOQuestActionResult::Applied);
    TestEqual(TEXT("Rewarded quest records state."), QuestLog->GetQuestState(TEXT("dawnwatch.starfall_arrival")), EEOQuestState::Rewarded);

    TestEqual(TEXT("Locked quest cannot start."), QuestLog->StartQuest(TEXT("dawnwatch.locked_depths")), EEOQuestActionResult::Unavailable);

    TestEqual(TEXT("World event can become active."), QuestLog->SetWorldEventState(TEXT("starfall.relic_surge"), EEOWorldEventState::Active), EEOQuestActionResult::Applied);
    TestEqual(TEXT("World event records active state."), QuestLog->GetWorldEventState(TEXT("starfall.relic_surge")), EEOWorldEventState::Active);
    TestEqual(TEXT("World event can resolve."), QuestLog->SetWorldEventState(TEXT("starfall.relic_surge"), EEOWorldEventState::Resolved), EEOQuestActionResult::Applied);
    TestTrue(TEXT("Resolved world event can respawn."), QuestLog->CanWorldEventRespawn(TEXT("starfall.relic_surge")));

    return true;
}

#endif
```

Run:

```bash
"/Users/Shared/Epic Games/UE_5.7/Engine/Build/BatchFiles/Mac/Build.sh" EverwildOdysseyEditor Mac Development -Project="/Users/ethanstuart/Projects/everwild-odyssey/.worktrees/alpha-foundation/EverwildOdyssey.uproject" -WaitMutex
```

Expected: FAIL with an include error for `Gameplay/EOQuestLogComponent.h`.

### Task 2: Implement Quest Log Component

- [x] Create `Source/EverwildOdyssey/Public/Gameplay/EOQuestLogComponent.h`.
- [x] Add `EEOQuestActionResult` with values `Applied`, `UnknownQuest`, `UnknownWorldEvent`, `Unavailable`, `AlreadyActive`, `NotActive`, `AlreadyCompleted`, and `AlreadyRewarded`.
- [x] Add quest methods `InitializeQuests`, `StartQuest`, `AdvanceQuest`, `CompleteQuest`, `RewardQuest`, `GetQuestCount`, `GetQuestState`, `GetQuestStepIndex`, and `HasQuest`.
- [x] Add world-event methods `InitializeWorldEvents`, `SetWorldEventState`, `GetWorldEventCount`, `GetWorldEventState`, `CanWorldEventRespawn`, and `HasWorldEvent`.
- [x] Create `Source/EverwildOdyssey/Private/Gameplay/EOQuestLogComponent.cpp`.
- [x] Implement quest methods by mutating `FEOQuestRecord` state in a local `TArray`.
- [x] Implement world-event methods by mutating `FEOWorldEventRecord` state in a local `TArray`.
- [x] Run the Unreal editor target build and verify it succeeds.

### Task 3: Attach Quest Log To Hero

- [x] Forward declare `UEOQuestLogComponent` in `EOHeroCharacter.h`.
- [x] Add a visible `QuestLog` component property to `AEOHeroCharacter`.
- [x] Add a public `GetQuestLogComponent` getter.
- [x] Create the `QuestLog` default subobject in the hero constructor.
- [x] In `BeginPlay`, initialize one available Dawnwatch quest and one warning Starfall world event.
- [x] Update `Interact` to call `StartQuest(TEXT("dawnwatch.starfall_arrival"))` and log the result.
- [x] Run the Unreal editor target build and verify it succeeds.

### Task 4: Commit And Push

- [x] Run `Scripts/check-unreal.sh`.
- [x] Run `git diff --check`.
- [x] Run `git status --short`.
- [ ] Commit with `feat: add quest log world events`.
- [ ] Push `feature/alpha-foundation`.
