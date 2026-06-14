#pragma once

#include "CoreMinimal.h"
#include "Core/EOCombatStats.h"
#include "GameFramework/Character.h"
#include "EOEnemyCharacter.generated.h"

class UEOCombatStatsComponent;
class UStaticMeshComponent;

USTRUCT(BlueprintType)
struct FEOEnemyArchetype
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
    FName EnemyId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
    FEOCombatStats BaseStats;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
    int32 ExperienceReward = 0;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
    float ThreatRadius = 800.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
    bool bElite = false;

    bool IsValidForAlpha() const
    {
        return !EnemyId.IsNone()
            && !DisplayName.IsEmpty()
            && BaseStats.IsValidForAlpha()
            && ExperienceReward > 0
            && ThreatRadius > 0.0f;
    }
};

UCLASS()
class EVERWILDODYSSEY_API AEOEnemyCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AEOEnemyCharacter();

    static FEOEnemyArchetype BuildRelicWispArchetype();

    UFUNCTION(BlueprintCallable, Category = "Enemy")
    void InitializeFromArchetype(const FEOEnemyArchetype& Archetype);

    UFUNCTION(BlueprintCallable, Category = "Enemy")
    float ApplyIncomingHit(float RawDamage);

    UFUNCTION(BlueprintCallable, Category = "Enemy")
    int32 ClaimExperienceReward();

    UFUNCTION(BlueprintPure, Category = "Enemy")
    UEOCombatStatsComponent* GetCombatStatsComponent() const { return CombatStats; }

    UFUNCTION(BlueprintPure, Category = "Enemy")
    FName GetEnemyId() const { return EnemyId; }

    UFUNCTION(BlueprintPure, Category = "Enemy")
    int32 GetExperienceReward() const { return ExperienceReward; }

    UFUNCTION(BlueprintPure, Category = "Enemy")
    float GetThreatRadius() const { return ThreatRadius; }

private:
    UPROPERTY(VisibleAnywhere, Category = "Enemy")
    TObjectPtr<UEOCombatStatsComponent> CombatStats;

    UPROPERTY(VisibleAnywhere, Category = "Enemy")
    TObjectPtr<UStaticMeshComponent> EnemySilhouette;

    UPROPERTY(VisibleAnywhere, Category = "Enemy")
    FName EnemyId = NAME_None;

    UPROPERTY(VisibleAnywhere, Category = "Enemy")
    int32 ExperienceReward = 0;

    UPROPERTY(VisibleAnywhere, Category = "Enemy")
    float ThreatRadius = 800.0f;

    UPROPERTY(VisibleAnywhere, Category = "Enemy")
    bool bExperienceRewardClaimed = false;
};
