#if WITH_DEV_AUTOMATION_TESTS

#include "Core/EOSaveTypes.h"
#include "Tests/EOTestData.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEOProgressionModelTest, "EverwildOdyssey.Core.ProgressionModel", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FEOProgressionModelTest::RunTest(const FString& Parameters)
{
    const FEOGearItem RelicWeapon = EOTestData::MakeRelicWeapon();
    TestTrue(TEXT("Relic weapon is valid."), RelicWeapon.IsValidForAlpha());
    TestEqual(TEXT("Relic weapon rarity is Relic."), static_cast<uint8>(RelicWeapon.Rarity), static_cast<uint8>(EEOGearRarity::Relic));

    const FEOQuestRecord MainQuest = EOTestData::MakeActiveMainQuest();
    TestTrue(TEXT("Active main quest can advance."), MainQuest.CanAdvance());
    TestEqual(TEXT("Quest records faction choice."), static_cast<uint8>(MainQuest.ReportedToFaction), static_cast<uint8>(EEOFaction::StarfallCollegium));

    const FEOWorldEventRecord Event = EOTestData::MakeResolvedRelicSurge();
    TestTrue(TEXT("Resolved event can respawn."), Event.CanRespawn());

    FEOSaveProfile Save;
    Save.SelectedClass = EEOHeroClass::Pathwarden;
    Save.HeroLevel = 2;
    Save.Experience = 125;
    Save.Inventory.Add(RelicWeapon);
    Save.Quests.Add(MainQuest);
    Save.WorldEvents.Add(Event);

    TestTrue(TEXT("Save profile is alpha-valid."), Save.IsValidForAlpha());
    TestEqual(TEXT("Save tracks inventory."), Save.Inventory.Num(), 1);
    TestEqual(TEXT("Save tracks quests."), Save.Quests.Num(), 1);
    TestEqual(TEXT("Save tracks world events."), Save.WorldEvents.Num(), 1);

    return true;
}

#endif
