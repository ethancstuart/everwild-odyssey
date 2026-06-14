# Alpha World Scaffold Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Add a visible procedural alpha-world scaffold that gives the default map Dawnwatch and Starfall landmarks using engine primitives.

**Architecture:** Implement `AEOAlphaWorldScaffold` as an actor with deterministic landmark specs and primitive mesh components. Spawn it from `AEOGameMode` on begin play so the template map gains a readable vertical-slice layout without requiring checked-in binary map assets yet.

**Tech Stack:** Unreal Engine 5.7, C++, `AActor`, `UStaticMeshComponent`, existing engine basic shapes, Unreal automation tests.

---

## Files

- Create: `Source/EverwildOdyssey/Public/Gameplay/EOAlphaWorldScaffold.h`
- Create: `Source/EverwildOdyssey/Private/Gameplay/EOAlphaWorldScaffold.cpp`
- Create: `Source/EverwildOdyssey/Private/Tests/EOAlphaWorldScaffoldTests.cpp`
- Modify: `Source/EverwildOdyssey/Public/Gameplay/EOGameMode.h`
- Modify: `Source/EverwildOdyssey/Private/Gameplay/EOGameMode.cpp`

## Tasks

### Task 1: Add Failing World Scaffold Test

- [x] Create `Source/EverwildOdyssey/Private/Tests/EOAlphaWorldScaffoldTests.cpp`.
- [x] Include `Gameplay/EOAlphaWorldScaffold.h`.
- [x] Add a test named `EverwildOdyssey.Gameplay.AlphaWorldScaffold`.
- [x] Test that default landmarks contain six valid landmarks, including `dawnwatch.keep`, `starfall.gate`, and `starfall.relic_surge`.
- [x] Run the Unreal editor target build and verify it fails because `Gameplay/EOAlphaWorldScaffold.h` does not exist.

Expected test behaviors:

```cpp
#if WITH_DEV_AUTOMATION_TESTS

#include "Gameplay/EOAlphaWorldScaffold.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEOAlphaWorldScaffoldTest, "EverwildOdyssey.Gameplay.AlphaWorldScaffold", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FEOAlphaWorldScaffoldTest::RunTest(const FString& Parameters)
{
    const TArray<FEOAlphaLandmarkSpec> Landmarks = AEOAlphaWorldScaffold::BuildDefaultLandmarks();

    TestEqual(TEXT("Alpha world exposes expected landmark count."), Landmarks.Num(), AEOAlphaWorldScaffold::ExpectedLandmarkCount);

    TSet<FName> LandmarkIds;
    for (const FEOAlphaLandmarkSpec& Landmark : Landmarks)
    {
        TestTrue(TEXT("Each alpha landmark is valid."), Landmark.IsValidForAlpha());
        LandmarkIds.Add(Landmark.LandmarkId);
    }

    TestTrue(TEXT("Dawnwatch Keep landmark exists."), LandmarkIds.Contains(TEXT("dawnwatch.keep")));
    TestTrue(TEXT("Starfall Gate landmark exists."), LandmarkIds.Contains(TEXT("starfall.gate")));
    TestTrue(TEXT("Relic Surge landmark exists."), LandmarkIds.Contains(TEXT("starfall.relic_surge")));

    return true;
}

#endif
```

Run:

```bash
"/Users/Shared/Epic Games/UE_5.7/Engine/Build/BatchFiles/Mac/Build.sh" EverwildOdysseyEditor Mac Development -Project="/Users/ethanstuart/Projects/everwild-odyssey/.worktrees/alpha-foundation/EverwildOdyssey.uproject" -WaitMutex
```

Expected: FAIL with an include error for `Gameplay/EOAlphaWorldScaffold.h`.

### Task 2: Implement Scaffold Actor

- [x] Create `Source/EverwildOdyssey/Public/Gameplay/EOAlphaWorldScaffold.h`.
- [x] Add `FEOAlphaLandmarkSpec` with `LandmarkId`, `DisplayName`, `Location`, `Scale`, and `IsValidForAlpha`.
- [x] Add `AEOAlphaWorldScaffold::ExpectedLandmarkCount = 6`.
- [x] Add `BuildDefaultLandmarks`.
- [x] Create `Source/EverwildOdyssey/Private/Gameplay/EOAlphaWorldScaffold.cpp`.
- [x] Build six deterministic landmarks: Dawnwatch Keep, Starfall Gate, Training Ring, Relic Surge, Vale Road, and Skybridge Hub.
- [x] Add default subobject mesh components using engine basic shapes and place them according to the specs.
- [x] Run the Unreal editor target build and verify it succeeds.

### Task 3: Spawn Scaffold From Game Mode

- [x] Add `BeginPlay` override to `AEOGameMode`.
- [x] Include `Gameplay/EOAlphaWorldScaffold.h` in `EOGameMode.cpp`.
- [x] Spawn `AEOAlphaWorldScaffold` at the world origin if the world exists.
- [x] Run the Unreal editor target build and verify it succeeds.

### Task 4: Commit And Push

- [x] Run `Scripts/check-unreal.sh`.
- [x] Run `git diff --check`.
- [x] Run `git status --short`.
- [ ] Commit with `feat: add alpha world scaffold`.
- [ ] Push `feature/alpha-foundation`.
