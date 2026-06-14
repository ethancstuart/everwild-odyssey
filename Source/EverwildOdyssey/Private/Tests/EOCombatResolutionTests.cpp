#if WITH_DEV_AUTOMATION_TESTS

#include "Gameplay/EOCombatResolution.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEOCombatResolutionTest, "EverwildOdyssey.Gameplay.CombatResolution", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FEOCombatResolutionTest::RunTest(const FString& Parameters)
{
    FEOCombatStats Stats;
    Stats.AttackPower = 16.0f;

    TestEqual(TEXT("Basic attack combines attack and gear power."), FEOCombatResolution::CalculateBasicAttackDamage(Stats, 4), 25.0f);

    Stats.AttackPower = -10.0f;
    TestEqual(TEXT("Basic attack clamps weak inputs to damage floor."), FEOCombatResolution::CalculateBasicAttackDamage(Stats, -5), 5.0f);

    Stats.AttackPower = 16.0f;
    Stats.SpellPower = 8.0f;
    TestEqual(TEXT("Ability damage mixes martial and spell scaling."), FEOCombatResolution::CalculateAbilityDamage(Stats, 4, false), 39.2f);
    TestEqual(TEXT("Ultimate ability damage gets a larger floor and bonus."), FEOCombatResolution::CalculateAbilityDamage(Stats, 4, true), 64.2f);

    return true;
}

#endif
