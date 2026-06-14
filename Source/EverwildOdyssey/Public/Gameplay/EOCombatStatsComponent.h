#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Core/EOCombatStats.h"
#include "EOCombatStatsComponent.generated.h"

UCLASS(ClassGroup = (Everwild), meta = (BlueprintSpawnableComponent))
class EVERWILDODYSSEY_API UEOCombatStatsComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UEOCombatStatsComponent();

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void InitializeFromStats(const FEOCombatStats& InStats);

    UFUNCTION(BlueprintCallable, Category = "Combat")
    float ApplyDamage(float RawDamage);

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void RestoreHealth(float Amount);

    UFUNCTION(BlueprintCallable, Category = "Combat")
    bool SpendResource(float Cost);

    UFUNCTION(BlueprintPure, Category = "Combat")
    bool IsAlive() const;

    UFUNCTION(BlueprintPure, Category = "Combat")
    float GetCurrentHealth() const { return CurrentHealth; }

    UFUNCTION(BlueprintPure, Category = "Combat")
    float GetCurrentResource() const { return CurrentResource; }

    UFUNCTION(BlueprintPure, Category = "Combat")
    const FEOCombatStats& GetStats() const { return Stats; }

private:
    UPROPERTY(EditAnywhere, Category = "Combat")
    FEOCombatStats Stats;

    UPROPERTY(VisibleAnywhere, Category = "Combat")
    float CurrentHealth = 100.0f;

    UPROPERTY(VisibleAnywhere, Category = "Combat")
    float CurrentResource = 100.0f;
};
