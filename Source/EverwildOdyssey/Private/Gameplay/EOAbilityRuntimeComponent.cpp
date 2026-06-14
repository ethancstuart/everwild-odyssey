#include "Gameplay/EOAbilityRuntimeComponent.h"

#include "Gameplay/EOCombatStatsComponent.h"

UEOAbilityRuntimeComponent::UEOAbilityRuntimeComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UEOAbilityRuntimeComponent::InitializeAbilities(const TArray<FEOAbilityDefinition>& InAbilities)
{
    Abilities.Reset();
    RemainingCooldowns.Reset();

    for (const FEOAbilityDefinition& Ability : InAbilities)
    {
        if (Ability.IsValidForAlpha())
        {
            Abilities.Add(Ability);
            RemainingCooldowns.Add(Ability.AbilityId, 0.0f);
        }
    }
}

EEOAbilityActivationResult UEOAbilityRuntimeComponent::TryActivateAbility(FName AbilityId, UEOCombatStatsComponent* CombatStats)
{
    const FEOAbilityDefinition* Ability = FindAbility(AbilityId);
    if (Ability == nullptr)
    {
        return EEOAbilityActivationResult::InvalidAbility;
    }

    if (CombatStats == nullptr)
    {
        return EEOAbilityActivationResult::InvalidCombatStats;
    }

    if (!CombatStats->IsAlive())
    {
        return EEOAbilityActivationResult::DeadCaster;
    }

    if (GetRemainingCooldown(AbilityId) > 0.0f)
    {
        return EEOAbilityActivationResult::OnCooldown;
    }

    if (!CombatStats->SpendResource(Ability->ResourceCost))
    {
        return EEOAbilityActivationResult::InsufficientResource;
    }

    RemainingCooldowns.Add(AbilityId, FMath::Max(0.0f, Ability->CooldownSeconds));
    return EEOAbilityActivationResult::Activated;
}

EEOAbilityActivationResult UEOAbilityRuntimeComponent::TryActivateAbilityByIndex(int32 AbilityIndex, UEOCombatStatsComponent* CombatStats)
{
    if (!Abilities.IsValidIndex(AbilityIndex))
    {
        return EEOAbilityActivationResult::InvalidAbility;
    }

    return TryActivateAbility(Abilities[AbilityIndex].AbilityId, CombatStats);
}

void UEOAbilityRuntimeComponent::AdvanceCooldowns(float DeltaSeconds)
{
    const float ClampedDelta = FMath::Max(0.0f, DeltaSeconds);
    for (TPair<FName, float>& Cooldown : RemainingCooldowns)
    {
        Cooldown.Value = FMath::Max(0.0f, Cooldown.Value - ClampedDelta);
    }
}

float UEOAbilityRuntimeComponent::GetRemainingCooldown(FName AbilityId) const
{
    if (const float* Cooldown = RemainingCooldowns.Find(AbilityId))
    {
        return *Cooldown;
    }

    return 0.0f;
}

bool UEOAbilityRuntimeComponent::HasAbility(FName AbilityId) const
{
    return FindAbility(AbilityId) != nullptr;
}

const FEOAbilityDefinition* UEOAbilityRuntimeComponent::FindAbility(FName AbilityId) const
{
    return Abilities.FindByPredicate([AbilityId](const FEOAbilityDefinition& Ability)
    {
        return Ability.AbilityId == AbilityId;
    });
}
