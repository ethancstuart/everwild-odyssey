# Ability Runtime Component Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Add a reusable ability runtime that validates class abilities, spends combat resources, applies cooldowns, and gives clear activation failure reasons.

**Architecture:** Implement `UEOAbilityRuntimeComponent` as a focused actor component that owns equipped ability definitions and cooldown state. Keep visual effects, targeting, projectiles, and animation outside this slice so future gameplay systems can consume a stable activation surface first.

**Tech Stack:** Unreal Engine 5.7, C++, `UActorComponent`, existing `FEOAbilityDefinition`, existing `UEOCombatStatsComponent`, Unreal automation tests.

---

## Files

- Create: `Source/EverwildOdyssey/Public/Gameplay/EOAbilityRuntimeComponent.h`
- Create: `Source/EverwildOdyssey/Private/Gameplay/EOAbilityRuntimeComponent.cpp`
- Create: `Source/EverwildOdyssey/Private/Tests/EOAbilityRuntimeComponentTests.cpp`
- Modify: `Source/EverwildOdyssey/Public/Gameplay/EOHeroCharacter.h`
- Modify: `Source/EverwildOdyssey/Private/Gameplay/EOHeroCharacter.cpp`

## Tasks

### Task 1: Add Failing Ability Runtime Test

- [x] Create `Source/EverwildOdyssey/Private/Tests/EOAbilityRuntimeComponentTests.cpp`.
- [x] Include `Gameplay/EOAbilityRuntimeComponent.h` and `Gameplay/EOCombatStatsComponent.h`.
- [x] Add a test named `EverwildOdyssey.Gameplay.AbilityRuntimeComponent`.
- [x] Test these behaviors in one sequence:
  - initializing two abilities stores both abilities
  - activating an affordable ability spends its resource and starts its cooldown
  - activating the same ability during cooldown returns `EEOAbilityActivationResult::OnCooldown`
  - advancing cooldown time clamps the remaining cooldown at zero
  - activating an unaffordable ability returns `EEOAbilityActivationResult::InsufficientResource`
- [x] Run the Unreal editor target build and verify it fails because `Gameplay/EOAbilityRuntimeComponent.h` does not exist.

Expected test body:

```cpp
#if WITH_DEV_AUTOMATION_TESTS

#include "Gameplay/EOAbilityRuntimeComponent.h"
#include "Gameplay/EOCombatStatsComponent.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEOAbilityRuntimeComponentTest, "EverwildOdyssey.Gameplay.AbilityRuntimeComponent", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

namespace
{
FEOAbilityDefinition TestAbility(FName Id, const TCHAR* Name, float CooldownSeconds, float ResourceCost)
{
    FEOAbilityDefinition Ability;
    Ability.AbilityId = Id;
    Ability.DisplayName = FText::FromString(Name);
    Ability.CooldownSeconds = CooldownSeconds;
    Ability.ResourceCost = ResourceCost;
    return Ability;
}
}

bool FEOAbilityRuntimeComponentTest::RunTest(const FString& Parameters)
{
    UEOAbilityRuntimeComponent* Runtime = NewObject<UEOAbilityRuntimeComponent>();
    UEOCombatStatsComponent* CombatStats = NewObject<UEOCombatStatsComponent>();

    FEOCombatStats Stats;
    Stats.MaxResource = 50.0f;
    CombatStats->InitializeFromStats(Stats);

    Runtime->InitializeAbilities({
        TestAbility(TEXT("runeblade.cleave"), TEXT("Rune Cleave"), 6.0f, 20.0f),
        TestAbility(TEXT("runeblade.ultimate"), TEXT("Relic Surge"), 45.0f, 75.0f)
    });

    TestEqual(TEXT("Runtime stores initialized abilities."), Runtime->GetAbilityCount(), 2);

    const EEOAbilityActivationResult FirstActivation = Runtime->TryActivateAbility(TEXT("runeblade.cleave"), CombatStats);
    TestEqual(TEXT("Affordable ability activates."), FirstActivation, EEOAbilityActivationResult::Activated);
    TestEqual(TEXT("Activated ability spends resource."), CombatStats->GetCurrentResource(), 30.0f);
    TestEqual(TEXT("Activated ability starts cooldown."), Runtime->GetRemainingCooldown(TEXT("runeblade.cleave")), 6.0f);

    const EEOAbilityActivationResult CooldownActivation = Runtime->TryActivateAbility(TEXT("runeblade.cleave"), CombatStats);
    TestEqual(TEXT("Cooldown blocks repeat activation."), CooldownActivation, EEOAbilityActivationResult::OnCooldown);

    Runtime->AdvanceCooldowns(99.0f);
    TestEqual(TEXT("Cooldown advance clamps to zero."), Runtime->GetRemainingCooldown(TEXT("runeblade.cleave")), 0.0f);

    const EEOAbilityActivationResult ExpensiveActivation = Runtime->TryActivateAbility(TEXT("runeblade.ultimate"), CombatStats);
    TestEqual(TEXT("Unaffordable ability is rejected."), ExpensiveActivation, EEOAbilityActivationResult::InsufficientResource);

    return true;
}

#endif
```

Run:

```bash
"/Users/Shared/Epic Games/UE_5.7/Engine/Build/BatchFiles/Mac/Build.sh" EverwildOdysseyEditor Mac Development -Project="/Users/ethanstuart/Projects/everwild-odyssey/.worktrees/alpha-foundation/EverwildOdyssey.uproject" -WaitMutex
```

Expected: FAIL with an include error for `Gameplay/EOAbilityRuntimeComponent.h`.

### Task 2: Implement Ability Runtime Component

- [x] Create `Source/EverwildOdyssey/Public/Gameplay/EOAbilityRuntimeComponent.h`.
- [x] Add `EEOAbilityActivationResult` with values `Activated`, `InvalidAbility`, `InvalidCombatStats`, `DeadCaster`, `InsufficientResource`, and `OnCooldown`.
- [x] Add `InitializeAbilities`, `TryActivateAbility`, `TryActivateAbilityByIndex`, `AdvanceCooldowns`, `GetRemainingCooldown`, `GetAbilityCount`, and `HasAbility`.
- [x] Store abilities in a `TArray<FEOAbilityDefinition>` and cooldowns in a `TMap<FName, float>`.
- [x] Create `Source/EverwildOdyssey/Private/Gameplay/EOAbilityRuntimeComponent.cpp`.
- [x] Implement activation so it rejects invalid abilities, invalid combat stats, dead casters, insufficient resource, and cooldowns before spending resources.
- [x] Run the Unreal editor target build and verify it succeeds.

### Task 3: Attach Runtime To Hero

- [x] Forward declare `UEOAbilityRuntimeComponent` in `EOHeroCharacter.h`.
- [x] Add a visible `AbilityRuntime` component property to `AEOHeroCharacter`.
- [x] Add public getters for `CombatStats` and `AbilityRuntime`.
- [x] Override `BeginPlay`.
- [x] In `BeginPlay`, initialize the default `Runeblade` class stats and starting abilities from `FEOWildClassCatalog`.
- [x] Update `AbilityOne` and `AbilityTwo` to call `TryActivateAbilityByIndex(2, CombatStats)` and `TryActivateAbilityByIndex(3, CombatStats)`.
- [x] Run the Unreal editor target build and verify it succeeds.

### Task 4: Commit And Push

- [x] Run `Scripts/check-unreal.sh`.
- [x] Run `git diff --check`.
- [x] Run `git status --short`.
- [ ] Commit with `feat: add ability runtime component`.
- [ ] Push `feature/alpha-foundation`.
