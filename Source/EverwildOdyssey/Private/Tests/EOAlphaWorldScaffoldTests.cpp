#if WITH_DEV_AUTOMATION_TESTS

#include "Gameplay/EOAlphaWorldScaffold.h"
#include "Misc/AutomationTest.h"
#include "World/EOZoneProfile.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEOAlphaWorldScaffoldTest, "EverwildOdyssey.Gameplay.AlphaWorldScaffold", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FEOAlphaWorldScaffoldTest::RunTest(const FString& Parameters)
{
    const FEOZoneProfile Profile = FEOZoneProfileCatalog::BuildStarfallValeProfile();
    TestTrue(TEXT("Alpha world consumes a valid Starfall Vale profile."), Profile.IsValidForAlpha());
    TestEqual(TEXT("Scaffold expected landmark count matches profile."), AEOAlphaWorldScaffold::ExpectedLandmarkCount(), Profile.Landmarks.Num());
    TestEqual(TEXT("Scaffold expected scenic prop count matches profile."), AEOAlphaWorldScaffold::ExpectedScenicPropCount(), Profile.ScenicProps.Num());
    TestEqual(TEXT("Scaffold expected ambient light count matches profile."), AEOAlphaWorldScaffold::ExpectedAmbientLightCount(), Profile.Lights.Num());

    const TArray<FEOZoneVisualSpec> Landmarks = AEOAlphaWorldScaffold::BuildDefaultLandmarks();

    TestEqual(TEXT("Alpha world exposes expected landmark count."), Landmarks.Num(), AEOAlphaWorldScaffold::ExpectedLandmarkCount());

    TSet<FName> LandmarkIds;
    for (const FEOZoneVisualSpec& Landmark : Landmarks)
    {
        TestTrue(TEXT("Each alpha landmark is valid."), Landmark.IsValidForAlpha());
        TestTrue(TEXT("Each alpha landmark has a readable tint."), Landmark.Tint.A > 0.5f && Landmark.Tint != FLinearColor::White);
        LandmarkIds.Add(Landmark.Id);
    }

    TestTrue(TEXT("Dawnwatch Gate landmark exists."), LandmarkIds.Contains(TEXT("landmark.dawnwatch.gate")));
    TestTrue(TEXT("Starfall Road landmark exists."), LandmarkIds.Contains(TEXT("landmark.starfall_road")));
    TestTrue(TEXT("Relic Surge landmark exists."), LandmarkIds.Contains(TEXT("landmark.relic_core")));

    const TArray<FEOZoneVisualSpec> ScenicProps = AEOAlphaWorldScaffold::BuildDefaultScenicProps();
    TestEqual(TEXT("Alpha world exposes expected scenic prop count."), ScenicProps.Num(), AEOAlphaWorldScaffold::ExpectedScenicPropCount());

    TSet<FName> AssetRoles;
    for (const FEOZoneVisualSpec& ScenicProp : ScenicProps)
    {
        TestTrue(TEXT("Each scenic prop is valid."), ScenicProp.IsValidForAlpha());
        AssetRoles.Add(ScenicProp.AssetRoleId);
    }

    TestTrue(TEXT("Alpha world has road dressing."), AssetRoles.Contains(TEXT("zone.road.lamp")));
    TestTrue(TEXT("Alpha world has crystalline surge dressing."), AssetRoles.Contains(TEXT("zone.relic_surge.crystal")));
    TestTrue(TEXT("Alpha world has hub dressing."), AssetRoles.Contains(TEXT("zone.vendor.crates")));
    TestTrue(TEXT("Alpha world has foliage dressing."), AssetRoles.Contains(TEXT("zone.foliage.tree")));

    const TArray<FEOZoneLightSpec> AmbientLights = AEOAlphaWorldScaffold::BuildDefaultAmbientLights();
    TestEqual(TEXT("Alpha world exposes expected ambient light count."), AmbientLights.Num(), AEOAlphaWorldScaffold::ExpectedAmbientLightCount());

    for (const FEOZoneLightSpec& AmbientLight : AmbientLights)
    {
        TestTrue(TEXT("Each ambient light is valid."), AmbientLight.IsValidForAlpha());
    }

    return true;
}

#endif
