#if WITH_DEV_AUTOMATION_TESTS

#include "Gameplay/EOAlphaWorldScaffold.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEOAlphaWorldScaffoldTest, "EverwildOdyssey.Gameplay.AlphaWorldScaffold", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FEOAlphaWorldScaffoldTest::RunTest(const FString& Parameters)
{
    const TArray<FEOAlphaLandmarkSpec> Landmarks = AEOAlphaWorldScaffold::BuildDefaultLandmarks();

    TestEqual(TEXT("Alpha world exposes expected landmark count."), Landmarks.Num(), AEOAlphaWorldScaffold::ExpectedLandmarkCount);

    TSet<FName> LandmarkIds;
    for (const FEOAlphaLandmarkSpec& Landmark : Landmarks)
    {
        TestTrue(TEXT("Each alpha landmark is valid."), Landmark.IsValidForAlpha());
        LandmarkIds.Add(Landmark.LandmarkId);
    }

    TestTrue(TEXT("Dawnwatch Keep landmark exists."), LandmarkIds.Contains(TEXT("dawnwatch.keep")));
    TestTrue(TEXT("Starfall Gate landmark exists."), LandmarkIds.Contains(TEXT("starfall.gate")));
    TestTrue(TEXT("Relic Surge landmark exists."), LandmarkIds.Contains(TEXT("starfall.relic_surge")));

    return true;
}

#endif
