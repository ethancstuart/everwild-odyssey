#pragma once

#include "CoreMinimal.h"
#include "Core/EOClassTypes.h"
#include "Core/EOInventoryTypes.h"
#include "Core/EOQuestTypes.h"
#include "Core/EOWorldEventTypes.h"
#include "EOSaveTypes.generated.h"

USTRUCT(BlueprintType)
struct FEOSaveProfile
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save")
    FString SlotName = TEXT("AlphaSlot");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save")
    EEOHeroClass SelectedClass = EEOHeroClass::Runeblade;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save")
    int32 HeroLevel = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save")
    int32 Experience = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save")
    TArray<FEOGearItem> Inventory;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save")
    TArray<FEOQuestRecord> Quests;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save")
    TArray<FEOWorldEventRecord> WorldEvents;

    bool IsValidForAlpha() const
    {
        return !SlotName.IsEmpty() && HeroLevel > 0 && Experience >= 0;
    }
};
