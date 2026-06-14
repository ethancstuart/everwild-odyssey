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
