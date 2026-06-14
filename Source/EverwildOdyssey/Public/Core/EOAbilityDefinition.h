#pragma once

#include "CoreMinimal.h"
#include "EOAbilityDefinition.generated.h"

USTRUCT(BlueprintType)
struct FEOAbilityDefinition
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
    FName AbilityId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
    FText Description;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
    float CooldownSeconds = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
    float ResourceCost = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
    bool bIsUltimate = false;

    bool IsValidForAlpha() const
    {
        return !AbilityId.IsNone()
            && !DisplayName.IsEmpty()
            && CooldownSeconds >= 0.0f
            && ResourceCost >= 0.0f;
    }
};
