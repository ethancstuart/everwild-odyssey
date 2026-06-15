#if WITH_DEV_AUTOMATION_TESTS

#include "UI/EOHUDPresentationTypes.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEOHUDPresentationTypesTest, "EverwildOdyssey.UI.HUDPresentationTypes", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FEOHUDPresentationTypesTest::RunTest(const FString& Parameters)
{
    FEOHUDPresentationModel Model = FEOHUDPresentationModel::BuildAlphaFixture();
    TestTrue(TEXT("HUD presentation fixture is valid."), Model.IsValidForAlpha());
    TestEqual(TEXT("HUD has two companion frames."), Model.PartyFrames.Num(), 2);
    TestEqual(TEXT("HUD has eight action slots."), Model.ActionSlots.Num(), 8);
    TestTrue(TEXT("HUD has minimap markers."), Model.MinimapMarkers.Num() >= 8);
    TestTrue(TEXT("HUD has event feed entries."), Model.FeedEntries.Num() >= 3);
    TestEqual(TEXT("HUD exposes controller attack glyph."), Model.FindGlyphForAction(TEXT("BasicAttack")), FString(TEXT("X")));
    TestTrue(TEXT("Target frame has an elite-ready role."), Model.TargetFrame.TargetRole == TEXT("Elite") || Model.TargetFrame.TargetRole == TEXT("Minion"));

    return true;
}

#endif
