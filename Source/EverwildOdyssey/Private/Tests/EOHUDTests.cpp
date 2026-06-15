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
    Snapshot.RelicShards = 2;
    Snapshot.RequiredRelicShards = 3;
    Snapshot.ActiveEnemyCount = 7;
    Snapshot.bQuestAccepted = true;
    Snapshot.bGateStabilized = false;

    TestTrue(TEXT("HUD snapshot is valid."), Snapshot.IsValidForAlpha());
    TestEqual(TEXT("HUD formats progression line."), AEOHUD::FormatProgressionLine(Snapshot), FString(TEXT("Level 2  XP 25 / 200")));
    TestEqual(TEXT("HUD formats vitality line."), AEOHUD::FormatVitalsLine(Snapshot), FString(TEXT("HP 120 / 135  Resource 65 / 100")));
    TestEqual(TEXT("HUD formats objective line."), AEOHUD::FormatObjectiveLine(Snapshot), FString(TEXT("Objective: Collect relic shards 2 / 3, then stabilize the Starfall Gate")));
    TestEqual(TEXT("HUD formats world event line."), AEOHUD::FormatWorldEventLine(Snapshot), FString(TEXT("World Event: Relic Surge active - 7 hostiles remain")));
    TestEqual(TEXT("HUD caps visible party frames."), AEOHUD::MaxVisiblePartyFrames, 3);
    TestEqual(TEXT("HUD caps visible feed entries."), AEOHUD::MaxVisibleFeedEntries, 3);
    TestEqual(TEXT("HUD caps visible action slots."), AEOHUD::MaxVisibleActionSlots, 8);
    TestEqual(TEXT("HUD caps visible minimap markers."), AEOHUD::MaxVisibleMinimapMarkers, 5);
    const FEOHUDPresentationModel Model = AEOHUD::BuildPresentationModel(Snapshot);
    TestTrue(TEXT("HUD presentation model is valid."), Model.IsValidForAlpha());
    TestEqual(TEXT("HUD presentation carries controller attack glyph."), Model.FindGlyphForAction(TEXT("BasicAttack")), FString(TEXT("X")));
    TestEqual(TEXT("HUD presentation carries quest objective."), Model.QuestObjective.ToString(), FString(TEXT("Objective: Collect relic shards 2 / 3, then stabilize the Starfall Gate")));

    return true;
}

#endif
