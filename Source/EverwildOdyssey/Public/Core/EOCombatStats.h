#pragma once

#include "CoreMinimal.h"
#include "EOCombatStats.generated.h"

USTRUCT(BlueprintType)
struct FEOCombatStats
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float MaxHealth = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float MaxResource = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float AttackPower = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float SpellPower = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float Armor = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float MoveSpeed = 600.0f;

    bool IsValidForAlpha() const
    {
        return MaxHealth > 0.0f
            && MaxResource > 0.0f
            && AttackPower >= 0.0f
            && SpellPower >= 0.0f
            && Armor >= 0.0f
            && MoveSpeed > 0.0f;
    }
};
