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

    FEOHUDPresentationModel EmptyPartyModel = Model;
    EmptyPartyModel.PartyFrames.Empty();
    TestFalse(TEXT("HUD presentation rejects empty party frames."), EmptyPartyModel.IsValidForAlpha());

    FEOHUDPresentationModel MissingGlyphModel = Model;
    MissingGlyphModel.ActionSlots[0].ControllerGlyph.Reset();
    TestFalse(TEXT("HUD presentation rejects missing action glyphs."), MissingGlyphModel.IsValidForAlpha());

    FEOHUDPresentationModel InvalidHealthModel = Model;
    InvalidHealthModel.PlayerFrame.Health = InvalidHealthModel.PlayerFrame.MaxHealth + 1.0f;
    TestFalse(TEXT("HUD presentation rejects invalid player health."), InvalidHealthModel.IsValidForAlpha());

    return true;
}

#endif
