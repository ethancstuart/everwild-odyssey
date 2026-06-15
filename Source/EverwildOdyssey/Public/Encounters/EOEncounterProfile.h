#pragma once

#include "CoreMinimal.h"
#include "EOEncounterProfile.generated.h"

USTRUCT(BlueprintType)
struct FEOEncounterEnemySpec
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Encounter")
    FName EnemyId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Encounter")
    FName RoleId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Encounter")
    FName AssetRoleId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Encounter")
    FName SpawnAnchorId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Encounter")
    int32 Level = 1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Encounter")
    bool bElite = false;

    bool IsValidForAlpha() const
    {
        return !EnemyId.IsNone()
            && !RoleId.IsNone()
            && !AssetRoleId.IsNone()
            && !SpawnAnchorId.IsNone()
            && Level > 0;
    }
};

USTRUCT(BlueprintType)
struct FEOEncounterPhaseSpec
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Encounter")
    FName PhaseId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Encounter")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Encounter")
    TArray<FEOEncounterEnemySpec> Enemies;

    bool IsValidForAlpha() const
    {
        if (PhaseId.IsNone() || DisplayName.IsEmpty() || Enemies.IsEmpty())
        {
            return false;
        }

        for (const FEOEncounterEnemySpec& EnemySpec : Enemies)
        {
            if (!EnemySpec.IsValidForAlpha())
            {
                return false;
            }
        }

        return true;
    }
};

USTRUCT(BlueprintType)
struct FEOEncounterProfile
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Encounter")
    FName EncounterId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Encounter")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Encounter")
    FName LinkedWorldEventId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Encounter")
    TArray<FEOEncounterPhaseSpec> Phases;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Encounter")
    int32 RewardExperience = 0;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Encounter")
    int32 RewardRelicShards = 0;

    bool IsValidForAlpha() const;
    int32 CountRole(FName RoleId) const;
    TArray<FVector> BuildSpawnLocationsFromAnchors(const TMap<FName, FVector>& AnchorLocations) const;
};

class FEOEncounterProfileCatalog
{
public:
    static FEOEncounterProfile BuildRelicSurgeProfile();
};
