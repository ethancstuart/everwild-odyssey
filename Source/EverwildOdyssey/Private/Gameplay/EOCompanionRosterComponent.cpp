#include "Gameplay/EOCompanionRosterComponent.h"

UEOCompanionRosterComponent::UEOCompanionRosterComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

EEOCompanionRosterResult UEOCompanionRosterComponent::AddCompanion(const FEOCompanionProfile& Companion)
{
    if (!Companion.IsValidForAlpha())
    {
        return EEOCompanionRosterResult::InvalidCompanion;
    }

    if (HasCompanion(Companion.CompanionId))
    {
        return EEOCompanionRosterResult::DuplicateCompanion;
    }

    Companions.Add(Companion);
    return EEOCompanionRosterResult::Applied;
}

EEOCompanionRosterResult UEOCompanionRosterComponent::ActivateCompanion(FName CompanionId)
{
    if (ActiveCompanionIds.Contains(CompanionId))
    {
        return EEOCompanionRosterResult::AlreadyActive;
    }

    if (!HasCompanion(CompanionId))
    {
        return EEOCompanionRosterResult::MissingCompanion;
    }

    if (ActiveCompanionIds.Num() >= MaxActiveCompanions)
    {
        return EEOCompanionRosterResult::PartyFull;
    }

    ActiveCompanionIds.Add(CompanionId);
    return EEOCompanionRosterResult::Applied;
}

EEOCompanionRosterResult UEOCompanionRosterComponent::DeactivateCompanion(FName CompanionId)
{
    const int32 RemovedCount = ActiveCompanionIds.Remove(CompanionId);
    return RemovedCount > 0 ? EEOCompanionRosterResult::Applied : EEOCompanionRosterResult::NotActive;
}

bool UEOCompanionRosterComponent::HasCompanion(FName CompanionId) const
{
    return FindCompanion(CompanionId) != nullptr;
}

bool UEOCompanionRosterComponent::HasActiveRole(EEOPartyRole Role) const
{
    for (const FName& CompanionId : ActiveCompanionIds)
    {
        if (const FEOCompanionProfile* Companion = FindCompanion(CompanionId))
        {
            if (Companion->Role == Role)
            {
                return true;
            }
        }
    }

    return false;
}

const FEOCompanionProfile* UEOCompanionRosterComponent::FindCompanion(FName CompanionId) const
{
    return Companions.FindByPredicate([CompanionId](const FEOCompanionProfile& Companion)
    {
        return Companion.CompanionId == CompanionId;
    });
}
