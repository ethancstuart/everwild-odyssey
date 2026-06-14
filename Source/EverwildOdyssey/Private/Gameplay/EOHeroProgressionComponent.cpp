#include "Gameplay/EOHeroProgressionComponent.h"

UEOHeroProgressionComponent::UEOHeroProgressionComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

EEOProgressionResult UEOHeroProgressionComponent::AddExperience(int32 Amount)
{
    if (Amount <= 0)
    {
        return EEOProgressionResult::InvalidExperience;
    }

    Experience += Amount;
    bool bLeveledUp = false;
    while (Experience >= GetExperienceForNextLevel())
    {
        Experience -= GetExperienceForNextLevel();
        HeroLevel++;
        bLeveledUp = true;
    }

    return bLeveledUp ? EEOProgressionResult::LeveledUp : EEOProgressionResult::Applied;
}

int32 UEOHeroProgressionComponent::GetExperienceForNextLevel() const
{
    return FMath::Max(1, HeroLevel) * 100;
}
