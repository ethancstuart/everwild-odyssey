#include "Gameplay/EOCombatStatsComponent.h"

UEOCombatStatsComponent::UEOCombatStatsComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    InitializeFromStats(Stats);
}

void UEOCombatStatsComponent::InitializeFromStats(const FEOCombatStats& InStats)
{
    Stats = InStats;
    CurrentHealth = FMath::Max(0.0f, Stats.MaxHealth);
    CurrentResource = FMath::Max(0.0f, Stats.MaxResource);
}

float UEOCombatStatsComponent::ApplyDamage(float RawDamage)
{
    const float MitigatedDamage = FMath::Max(0.0f, RawDamage - Stats.Armor);
    CurrentHealth = FMath::Clamp(CurrentHealth - MitigatedDamage, 0.0f, Stats.MaxHealth);
    return MitigatedDamage;
}

void UEOCombatStatsComponent::RestoreHealth(float Amount)
{
    CurrentHealth = FMath::Clamp(CurrentHealth + FMath::Max(0.0f, Amount), 0.0f, Stats.MaxHealth);
}

bool UEOCombatStatsComponent::SpendResource(float Cost)
{
    const float ClampedCost = FMath::Max(0.0f, Cost);
    if (CurrentResource < ClampedCost)
    {
        return false;
    }

    CurrentResource -= ClampedCost;
    return true;
}

bool UEOCombatStatsComponent::IsAlive() const
{
    return CurrentHealth > 0.0f;
}
