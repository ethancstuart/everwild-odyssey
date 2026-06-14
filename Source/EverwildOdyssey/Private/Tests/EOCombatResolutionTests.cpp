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

    return true;
}

#endif
