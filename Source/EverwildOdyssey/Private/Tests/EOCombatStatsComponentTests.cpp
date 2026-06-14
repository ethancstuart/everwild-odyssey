#if WITH_DEV_AUTOMATION_TESTS

#include "Gameplay/EOCombatStatsComponent.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEOCombatStatsComponentTest, "EverwildOdyssey.Gameplay.CombatStatsComponent", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FEOCombatStatsComponentTest::RunTest(const FString& Parameters)
{
    UEOCombatStatsComponent* Component = NewObject<UEOCombatStatsComponent>();

    FEOCombatStats Stats;
    Stats.MaxHealth = 120.0f;
    Stats.MaxResource = 80.0f;
    Stats.Armor = 5.0f;
    Component->InitializeFromStats(Stats);

    TestEqual(TEXT("Initialization fills health."), Component->GetCurrentHealth(), 120.0f);
    TestEqual(TEXT("Initialization fills resource."), Component->GetCurrentResource(), 80.0f);

    const float DamageApplied = Component->ApplyDamage(25.0f);
    TestEqual(TEXT("Armor mitigates incoming damage."), DamageApplied, 20.0f);
    TestEqual(TEXT("Damage reduces health."), Component->GetCurrentHealth(), 100.0f);

    TestTrue(TEXT("Resource spend succeeds when affordable."), Component->SpendResource(30.0f));
    TestEqual(TEXT("Resource spend reduces resource."), Component->GetCurrentResource(), 50.0f);
    TestFalse(TEXT("Resource spend fails when unaffordable."), Component->SpendResource(90.0f));

    Component->RestoreHealth(999.0f);
    TestEqual(TEXT("Healing clamps to max health."), Component->GetCurrentHealth(), 120.0f);

    return true;
}

#endif
