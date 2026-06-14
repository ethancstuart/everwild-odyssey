#if WITH_DEV_AUTOMATION_TESTS

#include "Gameplay/EOHUD.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEOHUDTest, "EverwildOdyssey.Gameplay.HUD", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FEOHUDTest::RunTest(const FString& Parameters)
{
    FEOHUDSnapshot Snapshot;
    Snapshot.Health = 120.0f;
    Snapshot.MaxHealth = 135.0f;
    Snapshot.Resource = 65.0f;
    Snapshot.MaxResource = 100.0f;
    Snapshot.Level = 2;
    Snapshot.Experience = 25;
    Snapshot.ExperienceForNextLevel = 200;

    TestTrue(TEXT("HUD snapshot is valid."), Snapshot.IsValidForAlpha());
    TestEqual(TEXT("HUD formats progression line."), AEOHUD::FormatProgressionLine(Snapshot), FString(TEXT("Level 2  XP 25 / 200")));
    TestEqual(TEXT("HUD formats vitality line."), AEOHUD::FormatVitalsLine(Snapshot), FString(TEXT("HP 120 / 135  Resource 65 / 100")));

    return true;
}

#endif
