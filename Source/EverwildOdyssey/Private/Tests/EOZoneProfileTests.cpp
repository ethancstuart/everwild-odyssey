#if WITH_DEV_AUTOMATION_TESTS

#include "World/EOZoneProfile.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEOZoneProfileTest, "EverwildOdyssey.World.ZoneProfile", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FEOZoneProfileTest::RunTest(const FString& Parameters)
{
    const FEOZoneProfile Profile = FEOZoneProfileCatalog::BuildStarfallValeProfile();

    TestTrue(TEXT("Zone profile is valid."), Profile.IsValidForAlpha());
    TestEqual(TEXT("Zone id is Starfall Vale."), Profile.ZoneId, FName(TEXT("zone.starfall_vale")));
    TestTrue(TEXT("Zone has dense landmarks."), Profile.Landmarks.Num() >= 18);
    TestTrue(TEXT("Zone has dense scenic props."), Profile.ScenicProps.Num() >= 190);
    TestTrue(TEXT("Zone has enough light anchors."), Profile.Lights.Num() >= 18);
    TestTrue(TEXT("Zone has gameplay anchors."), Profile.Anchors.Num() >= 14);
    TestTrue(TEXT("Zone has future hooks."), Profile.FutureHooks.Num() >= 4);
    TestTrue(TEXT("Zone has minimap markers."), Profile.MinimapMarkers.Num() >= 8);

    TestTrue(TEXT("Spawn vista anchor exists."), Profile.HasAnchor(TEXT("anchor.spawn_vista")));
    TestTrue(TEXT("Relic surge center anchor exists."), Profile.HasAnchor(TEXT("anchor.relic_surge.center")));
    TestTrue(TEXT("Sky-Watch reveal anchor exists."), Profile.HasAnchor(TEXT("anchor.skywatch.reveal")));
    TestTrue(TEXT("Future Briarfen hook exists."), Profile.HasFutureHook(TEXT("future.briarfen_march")));
    TestTrue(TEXT("Future Moonwell hook exists."), Profile.HasFutureHook(TEXT("future.moonwell_archive")));

    return true;
}

#endif
