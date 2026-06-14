#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Core/EOAbilityDefinition.h"
#include "EOAbilityRuntimeComponent.generated.h"

class UEOCombatStatsComponent;

UENUM(BlueprintType)
enum class EEOAbilityActivationResult : uint8
{
    Activated,
    InvalidAbility,
    InvalidCombatStats,
    DeadCaster,
    InsufficientResource,
    OnCooldown
};

UCLASS(ClassGroup = (Everwild), meta = (BlueprintSpawnableComponent))
class EVERWILDODYSSEY_API UEOAbilityRuntimeComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UEOAbilityRuntimeComponent();

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void InitializeAbilities(const TArray<FEOAbilityDefinition>& InAbilities);

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    EEOAbilityActivationResult TryActivateAbility(FName AbilityId, UEOCombatStatsComponent* CombatStats);

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    EEOAbilityActivationResult TryActivateAbilityByIndex(int32 AbilityIndex, UEOCombatStatsComponent* CombatStats);

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void AdvanceCooldowns(float DeltaSeconds);

    UFUNCTION(BlueprintPure, Category = "Abilities")
    float GetRemainingCooldown(FName AbilityId) const;

    UFUNCTION(BlueprintPure, Category = "Abilities")
    int32 GetAbilityCount() const { return Abilities.Num(); }

    UFUNCTION(BlueprintPure, Category = "Abilities")
    bool HasAbility(FName AbilityId) const;

private:
    const FEOAbilityDefinition* FindAbility(FName AbilityId) const;

    UPROPERTY(EditAnywhere, Category = "Abilities")
    TArray<FEOAbilityDefinition> Abilities;

    UPROPERTY(VisibleAnywhere, Category = "Abilities")
    TMap<FName, float> RemainingCooldowns;
};
