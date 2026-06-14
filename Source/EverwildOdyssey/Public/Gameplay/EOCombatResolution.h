#pragma once

#include "CoreMinimal.h"
#include "Core/EOCombatStats.h"

struct FEOCombatResolution
{
    static float CalculateBasicAttackDamage(const FEOCombatStats& AttackerStats, int32 EquippedPower)
    {
        return FMath::Max(5.0f, AttackerStats.AttackPower + FMath::Max(0, EquippedPower) + 5.0f);
    }
};
