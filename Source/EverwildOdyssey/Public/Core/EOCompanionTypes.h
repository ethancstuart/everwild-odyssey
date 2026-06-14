#pragma once

#include "CoreMinimal.h"
#include "Core/EOClassTypes.h"
#include "EOCompanionTypes.generated.h"

USTRUCT(BlueprintType)
struct FEOCompanionProfile
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Companion")
    FName CompanionId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Companion")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Companion")
    EEOPartyRole Role = EEOPartyRole::Support;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Companion")
    float FollowDistance = 300.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Companion")
    float AbilityCheckInterval = 1.25f;

    bool IsValidForAlpha() const
    {
        return !CompanionId.IsNone() && !DisplayName.IsEmpty() && FollowDistance > 0.0f && AbilityCheckInterval > 0.0f;
    }
};
