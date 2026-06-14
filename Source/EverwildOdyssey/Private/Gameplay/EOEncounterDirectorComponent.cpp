#include "Gameplay/EOEncounterDirectorComponent.h"

UEOEncounterDirectorComponent::UEOEncounterDirectorComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UEOEncounterDirectorComponent::InitializeEncounters(const TArray<FEOEncounterRecord>& InEncounters)
{
    Encounters.Reset();

    for (FEOEncounterRecord Encounter : InEncounters)
    {
        if (Encounter.IsValidForAlpha())
        {
            Encounter.RemainingEnemies = FMath::Clamp(Encounter.RemainingEnemies, 0, Encounter.TotalEnemies);
            if (Encounter.State == EEOEncounterState::Dormant && Encounter.RemainingEnemies == 0)
            {
                Encounter.RemainingEnemies = Encounter.TotalEnemies;
            }
            Encounters.Add(Encounter);
        }
    }
}

EEOEncounterActionResult UEOEncounterDirectorComponent::StartEncounter(FName EncounterId)
{
    FEOEncounterRecord* Encounter = FindEncounterMutable(EncounterId);
    if (Encounter == nullptr)
    {
        return EEOEncounterActionResult::UnknownEncounter;
    }

    if (Encounter->State == EEOEncounterState::Active)
    {
        return EEOEncounterActionResult::AlreadyActive;
    }

    if (Encounter->State == EEOEncounterState::Cleared)
    {
        return EEOEncounterActionResult::AlreadyCleared;
    }

    Encounter->State = EEOEncounterState::Active;
    Encounter->RemainingEnemies = FMath::Max(1, Encounter->TotalEnemies);
    return EEOEncounterActionResult::Applied;
}

EEOEncounterActionResult UEOEncounterDirectorComponent::RegisterEnemyDefeated(FName EncounterId, int32 DefeatCount)
{
    FEOEncounterRecord* Encounter = FindEncounterMutable(EncounterId);
    if (Encounter == nullptr)
    {
        return EEOEncounterActionResult::UnknownEncounter;
    }

    if (Encounter->State == EEOEncounterState::Cleared)
    {
        return EEOEncounterActionResult::AlreadyCleared;
    }

    if (Encounter->State != EEOEncounterState::Active)
    {
        return EEOEncounterActionResult::NotActive;
    }

    Encounter->RemainingEnemies = FMath::Max(0, Encounter->RemainingEnemies - FMath::Max(1, DefeatCount));
    if (Encounter->RemainingEnemies == 0)
    {
        Encounter->State = EEOEncounterState::Cleared;
    }

    return EEOEncounterActionResult::Applied;
}

EEOEncounterActionResult UEOEncounterDirectorComponent::ResetEncounter(FName EncounterId)
{
    FEOEncounterRecord* Encounter = FindEncounterMutable(EncounterId);
    if (Encounter == nullptr)
    {
        return EEOEncounterActionResult::UnknownEncounter;
    }

    Encounter->State = EEOEncounterState::Dormant;
    Encounter->RemainingEnemies = FMath::Max(1, Encounter->TotalEnemies);
    return EEOEncounterActionResult::Applied;
}

EEOEncounterState UEOEncounterDirectorComponent::GetEncounterState(FName EncounterId) const
{
    if (const FEOEncounterRecord* Encounter = FindEncounter(EncounterId))
    {
        return Encounter->State;
    }

    return EEOEncounterState::Dormant;
}

int32 UEOEncounterDirectorComponent::GetRemainingEnemies(FName EncounterId) const
{
    if (const FEOEncounterRecord* Encounter = FindEncounter(EncounterId))
    {
        return Encounter->RemainingEnemies;
    }

    return INDEX_NONE;
}

bool UEOEncounterDirectorComponent::HasEncounter(FName EncounterId) const
{
    return FindEncounter(EncounterId) != nullptr;
}

FEOEncounterRecord* UEOEncounterDirectorComponent::FindEncounterMutable(FName EncounterId)
{
    return Encounters.FindByPredicate([EncounterId](const FEOEncounterRecord& Encounter)
    {
        return Encounter.EncounterId == EncounterId;
    });
}

const FEOEncounterRecord* UEOEncounterDirectorComponent::FindEncounter(FName EncounterId) const
{
    return Encounters.FindByPredicate([EncounterId](const FEOEncounterRecord& Encounter)
    {
        return Encounter.EncounterId == EncounterId;
    });
}
