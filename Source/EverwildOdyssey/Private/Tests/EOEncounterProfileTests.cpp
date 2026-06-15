#if WITH_DEV_AUTOMATION_TESTS

#include "Encounters/EOEncounterProfile.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEOEncounterProfileTest, "EverwildOdyssey.Encounters.EncounterProfile", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FEOEncounterProfileTest::RunTest(const FString& Parameters)
{
    const FEOEncounterProfile Profile = FEOEncounterProfileCatalog::BuildRelicSurgeProfile();
    TestTrue(TEXT("Relic surge profile is valid."), Profile.IsValidForAlpha());
    TestEqual(TEXT("Relic surge has three phases."), Profile.Phases.Num(), 3);
    TestTrue(TEXT("Relic surge contains minions."), Profile.CountRole(TEXT("enemy.role.minion")) >= 6);
    TestTrue(TEXT("Relic surge contains casters."), Profile.CountRole(TEXT("enemy.role.caster")) >= 2);
    TestTrue(TEXT("Relic surge contains an elite."), Profile.CountRole(TEXT("enemy.role.elite")) >= 1);
    TestTrue(TEXT("Relic surge rewards XP."), Profile.RewardExperience >= 100);
    TestTrue(TEXT("Relic surge rewards shards."), Profile.RewardRelicShards >= 3);

    return true;
}

#endif
