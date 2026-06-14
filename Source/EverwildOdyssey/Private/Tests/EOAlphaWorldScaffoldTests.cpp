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
        TestTrue(TEXT("Each alpha landmark has a readable tint."), Landmark.TintColor.A > 0.5f && Landmark.TintColor != FLinearColor::White);
        LandmarkIds.Add(Landmark.LandmarkId);
    }

    TestTrue(TEXT("Dawnwatch Keep landmark exists."), LandmarkIds.Contains(TEXT("dawnwatch.keep")));
    TestTrue(TEXT("Starfall Gate landmark exists."), LandmarkIds.Contains(TEXT("starfall.gate")));
    TestTrue(TEXT("Relic Surge landmark exists."), LandmarkIds.Contains(TEXT("starfall.relic_surge")));

    const TArray<FEOAlphaScenicPropSpec> ScenicProps = AEOAlphaWorldScaffold::BuildDefaultScenicProps();
    TestEqual(TEXT("Alpha world exposes expected scenic prop count."), ScenicProps.Num(), AEOAlphaWorldScaffold::ExpectedScenicPropCount);

    TSet<FName> ScenicStyles;
    for (const FEOAlphaScenicPropSpec& ScenicProp : ScenicProps)
    {
        TestTrue(TEXT("Each scenic prop is valid."), ScenicProp.IsValidForAlpha());
        ScenicStyles.Add(ScenicProp.VisualStyle);
    }

    TestTrue(TEXT("Alpha world has forest dressing."), ScenicStyles.Contains(TEXT("tree_canopy")));
    TestTrue(TEXT("Alpha world has crystalline surge dressing."), ScenicStyles.Contains(TEXT("crystal")));
    TestTrue(TEXT("Alpha world has village dressing."), ScenicStyles.Contains(TEXT("hut")));
    TestTrue(TEXT("Alpha world has water dressing."), ScenicStyles.Contains(TEXT("water")));

    const TArray<FEOAlphaLightSpec> AmbientLights = AEOAlphaWorldScaffold::BuildDefaultAmbientLights();
    TestEqual(TEXT("Alpha world exposes expected ambient light count."), AmbientLights.Num(), AEOAlphaWorldScaffold::ExpectedAmbientLightCount);

    for (const FEOAlphaLightSpec& AmbientLight : AmbientLights)
    {
        TestTrue(TEXT("Each ambient light is valid."), AmbientLight.IsValidForAlpha());
    }

    return true;
}

#endif
