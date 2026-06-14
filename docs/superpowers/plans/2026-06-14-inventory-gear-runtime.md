# Inventory Gear Runtime Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Add a runtime inventory component that stores gear, rejects invalid loot, equips items by slot, and computes equipped power for early MMO-style progression.

**Architecture:** Implement `UEOInventoryComponent` as a focused actor component over the existing `FEOGearItem` data model. Attach it to `AEOHeroCharacter` with a starter weapon so future quest rewards, enemy drops, UI, and save/load all use one gear surface.

**Tech Stack:** Unreal Engine 5.7, C++, `UActorComponent`, existing `FEOGearItem`, Unreal automation tests.

---

## Files

- Create: `Source/EverwildOdyssey/Public/Gameplay/EOInventoryComponent.h`
- Create: `Source/EverwildOdyssey/Private/Gameplay/EOInventoryComponent.cpp`
- Create: `Source/EverwildOdyssey/Private/Tests/EOInventoryComponentTests.cpp`
- Modify: `Source/EverwildOdyssey/Public/Gameplay/EOHeroCharacter.h`
- Modify: `Source/EverwildOdyssey/Private/Gameplay/EOHeroCharacter.cpp`

## Tasks

### Task 1: Add Failing Inventory Test

- [x] Create `Source/EverwildOdyssey/Private/Tests/EOInventoryComponentTests.cpp`.
- [x] Include `Gameplay/EOInventoryComponent.h`.
- [x] Add a test named `EverwildOdyssey.Gameplay.InventoryComponent`.
- [x] Test adding valid gear, rejecting invalid gear, rejecting duplicates, auto-equipping the strongest weapon, equipping a weaker weapon manually, and rejecting missing items.
- [x] Run the Unreal editor target build and verify it fails because `Gameplay/EOInventoryComponent.h` does not exist.

Expected test behaviors:

```cpp
#if WITH_DEV_AUTOMATION_TESTS

#include "Gameplay/EOInventoryComponent.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEOInventoryComponentTest, "EverwildOdyssey.Gameplay.InventoryComponent", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

namespace
{
FEOGearItem TestGear(FName Id, const TCHAR* Name, EEOGearSlot Slot, EEOGearRarity Rarity, int32 Power)
{
    FEOGearItem Item;
    Item.ItemId = Id;
    Item.DisplayName = FText::FromString(Name);
    Item.Slot = Slot;
    Item.Rarity = Rarity;
    Item.Power = Power;
    return Item;
}
}

bool FEOInventoryComponentTest::RunTest(const FString& Parameters)
{
    UEOInventoryComponent* Inventory = NewObject<UEOInventoryComponent>();

    const FEOGearItem TrainingBlade = TestGear(TEXT("weapon.training_blade"), TEXT("Training Blade"), EEOGearSlot::Weapon, EEOGearRarity::Common, 4);
    const FEOGearItem StarforgedBlade = TestGear(TEXT("relic.starforged_blade"), TEXT("Starforged Blade"), EEOGearSlot::Weapon, EEOGearRarity::Relic, 12);
    const FEOGearItem EmptyItem;

    TestEqual(TEXT("Valid gear can be added."), Inventory->AddItem(TrainingBlade), EEOInventoryActionResult::Applied);
    TestEqual(TEXT("Second valid gear can be added."), Inventory->AddItem(StarforgedBlade), EEOInventoryActionResult::Applied);
    TestEqual(TEXT("Invalid gear is rejected."), Inventory->AddItem(EmptyItem), EEOInventoryActionResult::InvalidItem);
    TestEqual(TEXT("Duplicate gear is rejected."), Inventory->AddItem(TrainingBlade), EEOInventoryActionResult::DuplicateItem);
    TestEqual(TEXT("Inventory stores valid items only."), Inventory->GetItemCount(), 2);

    TestEqual(TEXT("Best weapon can auto-equip."), Inventory->AutoEquipBestForSlot(EEOGearSlot::Weapon), EEOInventoryActionResult::Applied);
    TestEqual(TEXT("Auto-equip picks strongest weapon."), Inventory->GetEquippedItemId(EEOGearSlot::Weapon), FName(TEXT("relic.starforged_blade")));
    TestEqual(TEXT("Equipped weapon contributes power."), Inventory->GetEquippedPower(EEOGearSlot::Weapon), 12);

    TestEqual(TEXT("Known weaker weapon can be equipped manually."), Inventory->EquipItem(TEXT("weapon.training_blade")), EEOInventoryActionResult::Applied);
    TestEqual(TEXT("Manual equip updates slot."), Inventory->GetEquippedPower(EEOGearSlot::Weapon), 4);
    TestEqual(TEXT("Missing item cannot equip."), Inventory->EquipItem(TEXT("missing.sword")), EEOInventoryActionResult::MissingItem);

    return true;
}

#endif
```

Run:

```bash
"/Users/Shared/Epic Games/UE_5.7/Engine/Build/BatchFiles/Mac/Build.sh" EverwildOdysseyEditor Mac Development -Project="/Users/ethanstuart/Projects/everwild-odyssey/.worktrees/alpha-foundation/EverwildOdyssey.uproject" -WaitMutex
```

Expected: FAIL with an include error for `Gameplay/EOInventoryComponent.h`.

### Task 2: Implement Inventory Component

- [x] Create `Source/EverwildOdyssey/Public/Gameplay/EOInventoryComponent.h`.
- [x] Add `EEOInventoryActionResult` with values `Applied`, `InvalidItem`, `DuplicateItem`, `MissingItem`, and `MissingSlot`.
- [x] Add methods `AddItem`, `RemoveItem`, `EquipItem`, `AutoEquipBestForSlot`, `GetItemCount`, `HasItem`, `GetEquippedItemId`, `GetEquippedPower`, and `GetTotalEquippedPower`.
- [x] Create `Source/EverwildOdyssey/Private/Gameplay/EOInventoryComponent.cpp`.
- [x] Store carried items in `TArray<FEOGearItem>` and equipped gear in `TMap<EEOGearSlot, FEOGearItem>`.
- [x] Implement auto-equip by choosing the highest power item in the requested slot.
- [x] Run the Unreal editor target build and verify it succeeds.

### Task 3: Attach Inventory To Hero

- [x] Forward declare `UEOInventoryComponent` in `EOHeroCharacter.h`.
- [x] Add a visible `Inventory` component property to `AEOHeroCharacter`.
- [x] Add a public `GetInventoryComponent` getter.
- [x] Create the `Inventory` default subobject in the hero constructor.
- [x] In `BeginPlay`, add and equip a common Dawnwatch training blade.
- [x] Run the Unreal editor target build and verify it succeeds.

### Task 4: Commit And Push

- [x] Run `Scripts/check-unreal.sh`.
- [x] Run `git diff --check`.
- [x] Run `git status --short`.
- [ ] Commit with `feat: add inventory gear runtime`.
- [ ] Push `feature/alpha-foundation`.
