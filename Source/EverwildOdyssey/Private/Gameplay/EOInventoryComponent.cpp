#include "Gameplay/EOInventoryComponent.h"

UEOInventoryComponent::UEOInventoryComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

EEOInventoryActionResult UEOInventoryComponent::AddItem(const FEOGearItem& Item)
{
    if (!Item.IsValidForAlpha())
    {
        return EEOInventoryActionResult::InvalidItem;
    }

    if (HasItem(Item.ItemId))
    {
        return EEOInventoryActionResult::DuplicateItem;
    }

    Items.Add(Item);
    return EEOInventoryActionResult::Applied;
}

EEOInventoryActionResult UEOInventoryComponent::RemoveItem(FName ItemId)
{
    const int32 RemovedCount = Items.RemoveAll([ItemId](const FEOGearItem& Item)
    {
        return Item.ItemId == ItemId;
    });

    if (RemovedCount == 0)
    {
        return EEOInventoryActionResult::MissingItem;
    }

    bool bShouldRemoveEquippedSlot = false;
    EEOGearSlot EquippedSlotToRemove = EEOGearSlot::Weapon;

    for (const TPair<EEOGearSlot, FEOGearItem>& EquippedItem : EquippedItems)
    {
        if (EquippedItem.Value.ItemId == ItemId)
        {
            bShouldRemoveEquippedSlot = true;
            EquippedSlotToRemove = EquippedItem.Key;
            break;
        }
    }

    if (bShouldRemoveEquippedSlot)
    {
        EquippedItems.Remove(EquippedSlotToRemove);
    }

    return EEOInventoryActionResult::Applied;
}

EEOInventoryActionResult UEOInventoryComponent::EquipItem(FName ItemId)
{
    const FEOGearItem* Item = FindItem(ItemId);
    if (Item == nullptr)
    {
        return EEOInventoryActionResult::MissingItem;
    }

    EquippedItems.Add(Item->Slot, *Item);
    return EEOInventoryActionResult::Applied;
}

EEOInventoryActionResult UEOInventoryComponent::AutoEquipBestForSlot(EEOGearSlot Slot)
{
    const FEOGearItem* BestItem = nullptr;
    for (const FEOGearItem& Item : Items)
    {
        if (Item.Slot == Slot && (BestItem == nullptr || Item.Power > BestItem->Power))
        {
            BestItem = &Item;
        }
    }

    if (BestItem == nullptr)
    {
        return EEOInventoryActionResult::MissingSlot;
    }

    EquippedItems.Add(Slot, *BestItem);
    return EEOInventoryActionResult::Applied;
}

bool UEOInventoryComponent::HasItem(FName ItemId) const
{
    return FindItem(ItemId) != nullptr;
}

FName UEOInventoryComponent::GetEquippedItemId(EEOGearSlot Slot) const
{
    if (const FEOGearItem* Item = EquippedItems.Find(Slot))
    {
        return Item->ItemId;
    }

    return NAME_None;
}

int32 UEOInventoryComponent::GetEquippedPower(EEOGearSlot Slot) const
{
    if (const FEOGearItem* Item = EquippedItems.Find(Slot))
    {
        return Item->Power;
    }

    return 0;
}

int32 UEOInventoryComponent::GetTotalEquippedPower() const
{
    int32 TotalPower = 0;
    for (const TPair<EEOGearSlot, FEOGearItem>& EquippedItem : EquippedItems)
    {
        TotalPower += EquippedItem.Value.Power;
    }

    return TotalPower;
}

FEOGearItem* UEOInventoryComponent::FindItemMutable(FName ItemId)
{
    return Items.FindByPredicate([ItemId](const FEOGearItem& Item)
    {
        return Item.ItemId == ItemId;
    });
}

const FEOGearItem* UEOInventoryComponent::FindItem(FName ItemId) const
{
    return Items.FindByPredicate([ItemId](const FEOGearItem& Item)
    {
        return Item.ItemId == ItemId;
    });
}
