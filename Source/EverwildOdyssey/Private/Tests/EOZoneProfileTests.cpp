#if WITH_DEV_AUTOMATION_TESTS

#include "World/EOZoneProfile.h"
#include "Misc/AutomationTest.h"
#include "Presentation/EOAssetRoleTypes.h"

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

    TSet<FName> LandmarkIds;
    for (const FEOZoneVisualSpec& Landmark : Profile.Landmarks)
    {
        TestFalse(TEXT("Landmark ids are unique."), LandmarkIds.Contains(Landmark.Id));
        LandmarkIds.Add(Landmark.Id);

        FEOAssetRoleDefinition AssetRole;
        TestTrue(TEXT("Landmark asset role resolves."), FEOAssetRoleCatalog::TryGetRoleDefinition(Landmark.AssetRoleId, AssetRole));
    }

    TSet<FName> ScenicPropIds;
    for (const FEOZoneVisualSpec& ScenicProp : Profile.ScenicProps)
    {
        TestFalse(TEXT("Scenic prop ids are unique."), ScenicPropIds.Contains(ScenicProp.Id));
        ScenicPropIds.Add(ScenicProp.Id);

        FEOAssetRoleDefinition AssetRole;
        TestTrue(TEXT("Scenic prop asset role resolves."), FEOAssetRoleCatalog::TryGetRoleDefinition(ScenicProp.AssetRoleId, AssetRole));
    }

    TSet<FName> AnchorIds;
    for (const FEOZoneAnchor& Anchor : Profile.Anchors)
    {
        TestFalse(TEXT("Anchor ids are unique."), AnchorIds.Contains(Anchor.AnchorId));
        AnchorIds.Add(Anchor.AnchorId);
    }

    for (const FEOZoneFutureHook& FutureHook : Profile.FutureHooks)
    {
        TestTrue(TEXT("Future hook anchor exists."), Profile.HasAnchor(FutureHook.AnchorId));
    }

    FEOZoneAnchor SpawnVistaAnchor;
    TestTrue(TEXT("Known anchor lookup succeeds."), Profile.TryGetAnchor(TEXT("anchor.spawn_vista"), SpawnVistaAnchor));
    TestFalse(TEXT("Known anchor has a display name."), SpawnVistaAnchor.DisplayName.IsEmpty());

    FEOZoneFutureHook BriarfenHook;
    TestTrue(TEXT("Known future hook lookup succeeds."), Profile.TryGetFutureHook(TEXT("future.briarfen_march"), BriarfenHook));
    TestEqual(TEXT("Known future hook has expected anchor."), BriarfenHook.AnchorId, FName(TEXT("anchor.future.briarfen")));

    FEOZoneAnchor UnknownAnchor;
    TestFalse(TEXT("Unknown anchor lookup fails."), Profile.TryGetAnchor(TEXT("anchor.unknown"), UnknownAnchor));

    FEOZoneFutureHook UnknownHook;
    TestFalse(TEXT("Unknown future hook lookup fails."), Profile.TryGetFutureHook(TEXT("future.unknown"), UnknownHook));

    FEOZoneProfile BrokenProfile = Profile;
    if (BrokenProfile.FutureHooks.Num() > 0)
    {
        BrokenProfile.FutureHooks[0].AnchorId = TEXT("anchor.missing_future");
        TestFalse(TEXT("Profile with a future hook pointing to a missing anchor is invalid."), BrokenProfile.IsValidForAlpha());
    }

    return true;
}

#endif
