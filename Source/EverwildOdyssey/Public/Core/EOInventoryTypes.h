#pragma once

#include "CoreMinimal.h"
#include "Core/EOClassTypes.h"
#include "EOInventoryTypes.generated.h"

UENUM(BlueprintType)
enum class EEOGearSlot : uint8
{
    Weapon UMETA(DisplayName = "Weapon"),
    Armor UMETA(DisplayName = "Armor"),
    Trinket UMETA(DisplayName = "Trinket")
};

USTRUCT(BlueprintType)
struct FEOGearItem
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gear")
    FName ItemId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gear")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gear")
    EEOGearSlot Slot = EEOGearSlot::Weapon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gear")
    EEOGearRarity Rarity = EEOGearRarity::Common;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gear")
    int32 Power = 1;

    bool IsValidForAlpha() const
    {
        return !ItemId.IsNone() && !DisplayName.IsEmpty() && Power > 0;
    }
};
