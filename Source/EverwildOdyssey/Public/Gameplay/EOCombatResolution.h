#pragma once

#include "CoreMinimal.h"
#include "Core/EOCombatStats.h"

struct FEOCombatResolution
{
    static float CalculateBasicAttackDamage(const FEOCombatStats& AttackerStats, int32 EquippedPower)
    {
        return FMath::Max(5.0f, AttackerStats.AttackPower + FMath::Max(0, EquippedPower) + 5.0f);
    }

    static float CalculateAbilityDamage(const FEOCombatStats& AttackerStats, int32 EquippedPower, bool bUltimate)
    {
        const float NormalDamage = FMath::Max(
            8.0f,
            AttackerStats.AttackPower * 1.4f + AttackerStats.SpellPower * 0.6f + FMath::Max(0, EquippedPower) + 8.0f);

        return bUltimate ? FMath::Max(25.0f, NormalDamage + 25.0f) : NormalDamage;
    }
};
