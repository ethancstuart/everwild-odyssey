#pragma once

#include "CoreMinimal.h"
#include "Core/EOAbilityDefinition.h"
#include "Core/EOClassTypes.h"
#include "Core/EOCombatStats.h"
#include "EOHeroClassDefinition.generated.h"

USTRUCT(BlueprintType)
struct FEOHeroClassDefinition
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Class")
    EEOHeroClass HeroClass = EEOHeroClass::Runeblade;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Class")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Class")
    FText FantasySummary;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Class")
    EEOPartyRole PrimaryRole = EEOPartyRole::Striker;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Class")
    EEOResourceType ResourceType = EEOResourceType::Stamina;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Class")
    FEOCombatStats BaseStats;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Class")
    TArray<FEOAbilityDefinition> StartingAbilities;

    bool IsValidForAlpha() const
    {
        if (DisplayName.IsEmpty() || FantasySummary.IsEmpty() || !BaseStats.IsValidForAlpha())
        {
            return false;
        }

        int32 UltimateCount = 0;
        for (const FEOAbilityDefinition& Ability : StartingAbilities)
        {
            if (!Ability.IsValidForAlpha())
            {
                return false;
            }
            UltimateCount += Ability.bIsUltimate ? 1 : 0;
        }

        return StartingAbilities.Num() == 5 && UltimateCount == 1;
    }
};

class FEOWildClassCatalog
{
public:
    static TArray<FEOHeroClassDefinition> BuildStartingClasses();
    static bool TryGetClassDefinition(EEOHeroClass HeroClass, FEOHeroClassDefinition& OutDefinition);
};
