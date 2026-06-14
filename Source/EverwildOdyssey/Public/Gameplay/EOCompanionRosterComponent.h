#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Core/EOCompanionTypes.h"
#include "EOCompanionRosterComponent.generated.h"

UENUM(BlueprintType)
enum class EEOCompanionRosterResult : uint8
{
    Applied,
    InvalidCompanion,
    DuplicateCompanion,
    MissingCompanion,
    AlreadyActive,
    NotActive,
    PartyFull
};

UCLASS(ClassGroup = (Everwild), meta = (BlueprintSpawnableComponent))
class EVERWILDODYSSEY_API UEOCompanionRosterComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    static constexpr int32 MaxActiveCompanions = 2;

    UEOCompanionRosterComponent();

    UFUNCTION(BlueprintCallable, Category = "Companions")
    EEOCompanionRosterResult AddCompanion(const FEOCompanionProfile& Companion);

    UFUNCTION(BlueprintCallable, Category = "Companions")
    EEOCompanionRosterResult ActivateCompanion(FName CompanionId);

    UFUNCTION(BlueprintCallable, Category = "Companions")
    EEOCompanionRosterResult DeactivateCompanion(FName CompanionId);

    UFUNCTION(BlueprintPure, Category = "Companions")
    int32 GetCompanionCount() const { return Companions.Num(); }

    UFUNCTION(BlueprintPure, Category = "Companions")
    int32 GetActiveCompanionCount() const { return ActiveCompanionIds.Num(); }

    UFUNCTION(BlueprintPure, Category = "Companions")
    bool HasCompanion(FName CompanionId) const;

    UFUNCTION(BlueprintPure, Category = "Companions")
    bool HasActiveRole(EEOPartyRole Role) const;

private:
    const FEOCompanionProfile* FindCompanion(FName CompanionId) const;

    UPROPERTY(EditAnywhere, Category = "Companions")
    TArray<FEOCompanionProfile> Companions;

    UPROPERTY(VisibleAnywhere, Category = "Companions")
    TArray<FName> ActiveCompanionIds;
};
