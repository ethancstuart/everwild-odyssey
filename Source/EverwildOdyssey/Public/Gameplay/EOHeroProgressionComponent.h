#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EOHeroProgressionComponent.generated.h"

UENUM(BlueprintType)
enum class EEOProgressionResult : uint8
{
    Applied,
    LeveledUp,
    InvalidExperience
};

UCLASS(ClassGroup = (Everwild), meta = (BlueprintSpawnableComponent))
class EVERWILDODYSSEY_API UEOHeroProgressionComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UEOHeroProgressionComponent();

    UFUNCTION(BlueprintCallable, Category = "Progression")
    EEOProgressionResult AddExperience(int32 Amount);

    UFUNCTION(BlueprintPure, Category = "Progression")
    int32 GetHeroLevel() const { return HeroLevel; }

    UFUNCTION(BlueprintPure, Category = "Progression")
    int32 GetExperience() const { return Experience; }

    UFUNCTION(BlueprintPure, Category = "Progression")
    int32 GetExperienceForNextLevel() const;

private:
    UPROPERTY(VisibleAnywhere, Category = "Progression")
    int32 HeroLevel = 1;

    UPROPERTY(VisibleAnywhere, Category = "Progression")
    int32 Experience = 0;
};
