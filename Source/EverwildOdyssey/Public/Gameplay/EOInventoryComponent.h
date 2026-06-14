#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Core/EOInventoryTypes.h"
#include "EOInventoryComponent.generated.h"

UENUM(BlueprintType)
enum class EEOInventoryActionResult : uint8
{
    Applied,
    InvalidItem,
    DuplicateItem,
    MissingItem,
    MissingSlot
};

UCLASS(ClassGroup = (Everwild), meta = (BlueprintSpawnableComponent))
class EVERWILDODYSSEY_API UEOInventoryComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UEOInventoryComponent();

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    EEOInventoryActionResult AddItem(const FEOGearItem& Item);

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    EEOInventoryActionResult RemoveItem(FName ItemId);

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    EEOInventoryActionResult EquipItem(FName ItemId);

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    EEOInventoryActionResult AutoEquipBestForSlot(EEOGearSlot Slot);

    UFUNCTION(BlueprintPure, Category = "Inventory")
    int32 GetItemCount() const { return Items.Num(); }

    UFUNCTION(BlueprintPure, Category = "Inventory")
    bool HasItem(FName ItemId) const;

    UFUNCTION(BlueprintPure, Category = "Inventory")
    FName GetEquippedItemId(EEOGearSlot Slot) const;

    UFUNCTION(BlueprintPure, Category = "Inventory")
    int32 GetEquippedPower(EEOGearSlot Slot) const;

    UFUNCTION(BlueprintPure, Category = "Inventory")
    int32 GetTotalEquippedPower() const;

private:
    FEOGearItem* FindItemMutable(FName ItemId);
    const FEOGearItem* FindItem(FName ItemId) const;

    UPROPERTY(EditAnywhere, Category = "Inventory")
    TArray<FEOGearItem> Items;

    UPROPERTY(VisibleAnywhere, Category = "Inventory")
    TMap<EEOGearSlot, FEOGearItem> EquippedItems;
};
