#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EOEncounterDirectorComponent.generated.h"

UENUM(BlueprintType)
enum class EEOEncounterState : uint8
{
    Dormant,
    Active,
    Cleared
};

UENUM(BlueprintType)
enum class EEOEncounterActionResult : uint8
{
    Applied,
    UnknownEncounter,
    Unavailable,
    AlreadyActive,
    AlreadyCleared,
    NotActive
};

USTRUCT(BlueprintType)
struct FEOEncounterRecord
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Encounter")
    FName EncounterId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Encounter")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Encounter")
    FName LinkedWorldEventId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Encounter")
    EEOEncounterState State = EEOEncounterState::Dormant;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Encounter")
    int32 TotalEnemies = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Encounter")
    int32 RemainingEnemies = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Encounter")
    int32 RecommendedLevel = 1;

    bool IsValidForAlpha() const
    {
        return !EncounterId.IsNone()
            && !DisplayName.IsEmpty()
            && !LinkedWorldEventId.IsNone()
            && TotalEnemies > 0
            && RecommendedLevel > 0;
    }
};

UCLASS(ClassGroup = (Everwild), meta = (BlueprintSpawnableComponent))
class EVERWILDODYSSEY_API UEOEncounterDirectorComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UEOEncounterDirectorComponent();

    UFUNCTION(BlueprintCallable, Category = "Encounters")
    void InitializeEncounters(const TArray<FEOEncounterRecord>& InEncounters);

    UFUNCTION(BlueprintCallable, Category = "Encounters")
    EEOEncounterActionResult StartEncounter(FName EncounterId);

    UFUNCTION(BlueprintCallable, Category = "Encounters")
    EEOEncounterActionResult RegisterEnemyDefeated(FName EncounterId, int32 DefeatCount = 1);

    UFUNCTION(BlueprintCallable, Category = "Encounters")
    EEOEncounterActionResult ResetEncounter(FName EncounterId);

    UFUNCTION(BlueprintPure, Category = "Encounters")
    int32 GetEncounterCount() const { return Encounters.Num(); }

    UFUNCTION(BlueprintPure, Category = "Encounters")
    EEOEncounterState GetEncounterState(FName EncounterId) const;

    UFUNCTION(BlueprintPure, Category = "Encounters")
    int32 GetRemainingEnemies(FName EncounterId) const;

    UFUNCTION(BlueprintPure, Category = "Encounters")
    bool HasEncounter(FName EncounterId) const;

private:
    FEOEncounterRecord* FindEncounterMutable(FName EncounterId);
    const FEOEncounterRecord* FindEncounter(FName EncounterId) const;

    UPROPERTY(EditAnywhere, Category = "Encounters")
    TArray<FEOEncounterRecord> Encounters;
};
