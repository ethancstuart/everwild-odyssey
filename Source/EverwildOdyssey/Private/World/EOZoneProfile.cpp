#include "World/EOZoneProfile.h"

namespace
{
constexpr int32 StarfallValeScenicPropCount = 1 + 56 + 48 + 48 + 42;

FText Txt(const TCHAR* Value)
{
    return FText::FromString(Value);
}

FEOZoneVisualSpec Visual(const TCHAR* Id, const TCHAR* Role, EEOZoneArea Area, const FVector& Location, const FVector& Scale, const FLinearColor& Tint, const FRotator& Rotation = FRotator::ZeroRotator, bool bBlocksMovement = true)
{
    FEOZoneVisualSpec Result;
    Result.Id = FName(Id);
    Result.AssetRoleId = FName(Role);
    Result.Area = Area;
    Result.Location = Location;
    Result.Rotation = Rotation;
    Result.Scale = Scale;
    Result.Tint = Tint;
    Result.bBlocksMovement = bBlocksMovement;
    return Result;
}

FEOZoneLightSpec Light(const TCHAR* Id, const FVector& Location, const FLinearColor& Color, float Intensity, float Radius)
{
    FEOZoneLightSpec Result;
    Result.LightId = FName(Id);
    Result.Location = Location;
    Result.Color = Color;
    Result.Intensity = Intensity;
    Result.Radius = Radius;
    return Result;
}

FEOZoneAnchor Anchor(const TCHAR* Id, const TCHAR* Name, const FVector& Location, const FRotator& Rotation = FRotator::ZeroRotator)
{
    FEOZoneAnchor Result;
    Result.AnchorId = FName(Id);
    Result.DisplayName = Txt(Name);
    Result.Location = Location;
    Result.Rotation = Rotation;
    return Result;
}

FEOZoneFutureHook Hook(const TCHAR* Id, const TCHAR* Name, const TCHAR* Reason, const TCHAR* AnchorId)
{
    FEOZoneFutureHook Result;
    Result.HookId = FName(Id);
    Result.DisplayName = Txt(Name);
    Result.LockedReason = Txt(Reason);
    Result.AnchorId = FName(AnchorId);
    return Result;
}

FEOMinimapMarkerSpec MinimapMarker(const TCHAR* Id, const TCHAR* Label, const FVector2D& Position, const FLinearColor& Color)
{
    FEOMinimapMarkerSpec Result;
    Result.MarkerId = FName(Id);
    Result.Label = Txt(Label);
    Result.NormalizedPosition = Position;
    Result.Color = Color;
    return Result;
}
}

bool FEOZoneProfile::IsValidForAlpha() const
{
    if (ZoneId.IsNone() || DisplayName.IsEmpty())
    {
        return false;
    }

    for (const FEOZoneVisualSpec& VisualSpec : Landmarks)
    {
        if (!VisualSpec.IsValidForAlpha())
        {
            return false;
        }
    }

    for (const FEOZoneVisualSpec& VisualSpec : ScenicProps)
    {
        if (!VisualSpec.IsValidForAlpha())
        {
            return false;
        }
    }

    for (const FEOZoneLightSpec& LightSpec : Lights)
    {
        if (!LightSpec.IsValidForAlpha())
        {
            return false;
        }
    }

    for (const FEOZoneAnchor& AnchorSpec : Anchors)
    {
        if (!AnchorSpec.IsValidForAlpha())
        {
            return false;
        }
    }

    for (const FEOZoneFutureHook& FutureHook : FutureHooks)
    {
        if (!FutureHook.IsValidForAlpha() || !HasAnchor(FutureHook.AnchorId))
        {
            return false;
        }
    }

    for (const FEOMinimapMarkerSpec& MarkerSpec : MinimapMarkers)
    {
        if (!MarkerSpec.IsValidForAlpha())
        {
            return false;
        }
    }

    return true;
}

bool FEOZoneProfile::HasAnchor(FName AnchorId) const
{
    FEOZoneAnchor Anchor;
    return TryGetAnchor(AnchorId, Anchor);
}

bool FEOZoneProfile::HasFutureHook(FName HookId) const
{
    FEOZoneFutureHook FutureHook;
    return TryGetFutureHook(HookId, FutureHook);
}

bool FEOZoneProfile::TryGetAnchor(FName AnchorId, FEOZoneAnchor& OutAnchor) const
{
    for (const FEOZoneAnchor& AnchorSpec : Anchors)
    {
        if (AnchorSpec.AnchorId == AnchorId)
        {
            OutAnchor = AnchorSpec;
            return true;
        }
    }

    return false;
}

bool FEOZoneProfile::TryGetFutureHook(FName HookId, FEOZoneFutureHook& OutHook) const
{
    for (const FEOZoneFutureHook& HookSpec : FutureHooks)
    {
        if (HookSpec.HookId == HookId)
        {
            OutHook = HookSpec;
            return true;
        }
    }

    return false;
}

FEOZoneProfile FEOZoneProfileCatalog::BuildStarfallValeProfile()
{
    FEOZoneProfile Profile;
    Profile.ZoneId = TEXT("zone.starfall_vale");
    Profile.DisplayName = Txt(TEXT("Starfall Vale"));

    Profile.Landmarks = {
        Visual(TEXT("landmark.dawnwatch.gate"), TEXT("zone.dawnwatch.gate"), EEOZoneArea::DawnwatchGate, FVector(-950.0f, -520.0f, 190.0f), FVector(3.4f, 1.2f, 2.6f), FLinearColor(0.10f, 0.30f, 0.90f, 1.0f)),
        Visual(TEXT("landmark.dawnwatch.left_tower"), TEXT("zone.dawnwatch.wall"), EEOZoneArea::DawnwatchGate, FVector(-1160.0f, -690.0f, 220.0f), FVector(1.1f, 1.1f, 3.2f), FLinearColor(0.12f, 0.22f, 0.68f, 1.0f)),
        Visual(TEXT("landmark.dawnwatch.right_tower"), TEXT("zone.dawnwatch.wall"), EEOZoneArea::DawnwatchGate, FVector(-760.0f, -690.0f, 220.0f), FVector(1.1f, 1.1f, 3.2f), FLinearColor(0.12f, 0.22f, 0.68f, 1.0f)),
        Visual(TEXT("landmark.training_yard"), TEXT("zone.training.dummy"), EEOZoneArea::DawnwatchGate, FVector(-620.0f, -180.0f, 80.0f), FVector(1.3f, 1.3f, 1.2f), FLinearColor(0.72f, 0.44f, 0.20f, 1.0f)),
        Visual(TEXT("landmark.scholar_station"), TEXT("zone.scholar.station"), EEOZoneArea::DawnwatchGate, FVector(-1180.0f, -250.0f, 80.0f), FVector(1.4f, 1.0f, 1.0f), FLinearColor(0.24f, 0.82f, 1.0f, 1.0f)),
        Visual(TEXT("landmark.vendor_corner"), TEXT("zone.vendor.crates"), EEOZoneArea::DawnwatchGate, FVector(-980.0f, -120.0f, 60.0f), FVector(1.6f, 1.1f, 0.8f), FLinearColor(0.72f, 0.52f, 0.24f, 1.0f)),
        Visual(TEXT("landmark.starfall_road"), TEXT("zone.road.signpost"), EEOZoneArea::StarfallRoad, FVector(-300.0f, -35.0f, 70.0f), FVector(1.0f, 1.0f, 1.3f), FLinearColor(0.82f, 0.62f, 0.32f, 1.0f)),
        Visual(TEXT("landmark.road_lantern_one"), TEXT("zone.road.lamp"), EEOZoneArea::StarfallRoad, FVector(-420.0f, 80.0f, 88.0f), FVector(1.0f, 1.0f, 1.2f), FLinearColor(1.0f, 0.68f, 0.24f, 1.0f)),
        Visual(TEXT("landmark.road_lantern_two"), TEXT("zone.road.lamp"), EEOZoneArea::StarfallRoad, FVector(40.0f, 80.0f, 88.0f), FVector(1.0f, 1.0f, 1.2f), FLinearColor(1.0f, 0.68f, 0.24f, 1.0f)),
        Visual(TEXT("landmark.road_lantern_three"), TEXT("zone.road.lamp"), EEOZoneArea::StarfallRoad, FVector(480.0f, 20.0f, 88.0f), FVector(1.0f, 1.0f, 1.2f), FLinearColor(1.0f, 0.68f, 0.24f, 1.0f)),
        Visual(TEXT("landmark.relic_core"), TEXT("zone.relic_surge.core"), EEOZoneArea::RelicSurgeCamp, FVector(760.0f, -260.0f, 180.0f), FVector(1.9f, 1.9f, 2.7f), FLinearColor(0.82f, 0.20f, 1.0f, 1.0f)),
        Visual(TEXT("landmark.relic_crystal_north"), TEXT("zone.relic_surge.crystal"), EEOZoneArea::RelicSurgeCamp, FVector(760.0f, -60.0f, 95.0f), FVector(0.8f, 0.8f, 1.9f), FLinearColor(0.76f, 0.20f, 1.0f, 1.0f)),
        Visual(TEXT("landmark.relic_crystal_south"), TEXT("zone.relic_surge.crystal"), EEOZoneArea::RelicSurgeCamp, FVector(760.0f, -470.0f, 95.0f), FVector(0.8f, 0.8f, 1.9f), FLinearColor(0.76f, 0.20f, 1.0f, 1.0f)),
        Visual(TEXT("landmark.relic_crystal_east"), TEXT("zone.relic_surge.crystal"), EEOZoneArea::RelicSurgeCamp, FVector(970.0f, -260.0f, 95.0f), FVector(0.8f, 0.8f, 1.9f), FLinearColor(0.76f, 0.20f, 1.0f, 1.0f)),
        Visual(TEXT("landmark.skywatch_entrance"), TEXT("zone.skywatch.entrance"), EEOZoneArea::SkyWatchEntrance, FVector(1260.0f, -760.0f, 220.0f), FVector(2.7f, 1.3f, 3.4f), FLinearColor(0.34f, 0.74f, 1.0f, 1.0f)),
        Visual(TEXT("landmark.briarfen_gate"), TEXT("zone.future.bridge_blocker"), EEOZoneArea::FutureBoundary, FVector(-1280.0f, 420.0f, 75.0f), FVector(2.0f, 0.8f, 1.1f), FLinearColor(0.22f, 0.66f, 0.24f, 1.0f)),
        Visual(TEXT("landmark.moonwell_gate"), TEXT("zone.future.bridge_blocker"), EEOZoneArea::FutureBoundary, FVector(260.0f, 660.0f, 75.0f), FVector(2.0f, 0.8f, 1.1f), FLinearColor(0.36f, 0.72f, 1.0f, 1.0f)),
        Visual(TEXT("landmark.sunspire_pass"), TEXT("zone.future.bridge_blocker"), EEOZoneArea::FutureBoundary, FVector(100.0f, -1060.0f, 90.0f), FVector(2.3f, 0.9f, 1.2f), FLinearColor(1.0f, 0.72f, 0.26f, 1.0f))
    };

    Profile.ScenicProps.Reserve(StarfallValeScenicPropCount);
    Profile.ScenicProps.Add(Visual(TEXT("terrain.vale.floor"), TEXT("zone.foliage.rock"), EEOZoneArea::StarfallRoad, FVector(0.0f, 0.0f, -10.0f), FVector(40.0f, 31.0f, 0.14f), FLinearColor(0.10f, 0.34f, 0.16f, 1.0f)));
    for (int32 Index = 0; Index < 56; ++Index)
    {
        const float X = -780.0f + static_cast<float>(Index % 14) * 145.0f;
        const float Y = 160.0f + static_cast<float>(Index / 14) * 125.0f;
        Profile.ScenicProps.Add(Visual(*FString::Printf(TEXT("foliage.tree.%02d"), Index), TEXT("zone.foliage.tree"), EEOZoneArea::StarfallRoad, FVector(X, Y, 0.0f), FVector(0.72f, 0.72f, 0.72f), FLinearColor(0.08f, 0.42f, 0.16f, 1.0f), FRotator(0.0f, Index * 17.0f, 0.0f), false));
    }
    for (int32 Index = 0; Index < 48; ++Index)
    {
        const float X = -640.0f + static_cast<float>(Index % 16) * 125.0f;
        const float Y = -95.0f + ((Index % 2 == 0) ? 0.0f : 130.0f);
        Profile.ScenicProps.Add(Visual(*FString::Printf(TEXT("road.dressing.%02d"), Index), (Index % 3 == 0) ? TEXT("zone.road.lamp") : TEXT("zone.foliage.rock"), EEOZoneArea::StarfallRoad, FVector(X, Y, 56.0f), FVector(0.5f, 0.5f, 0.75f), FLinearColor(0.70f, 0.55f, 0.36f, 1.0f), FRotator(0.0f, Index * 9.0f, 0.0f), Index % 3 != 0));
    }
    for (int32 Index = 0; Index < 48; ++Index)
    {
        const float Angle = Index * 7.5f;
        const float Radius = 170.0f + static_cast<float>(Index % 6) * 35.0f;
        Profile.ScenicProps.Add(Visual(*FString::Printf(TEXT("surge.crystal.cluster.%02d"), Index), TEXT("zone.relic_surge.crystal"), EEOZoneArea::RelicSurgeCamp, FVector(760.0f + FMath::Cos(FMath::DegreesToRadians(Angle)) * Radius, -260.0f + FMath::Sin(FMath::DegreesToRadians(Angle)) * Radius, 74.0f), FVector(0.28f, 0.28f, 1.15f), FLinearColor(0.82f, 0.16f, 1.0f, 1.0f), FRotator(0.0f, Angle, 8.0f), false));
    }
    for (int32 Index = 0; Index < 42; ++Index)
    {
        const float X = -1220.0f + static_cast<float>(Index % 7) * 95.0f;
        const float Y = -720.0f + static_cast<float>(Index / 7) * 82.0f;
        Profile.ScenicProps.Add(Visual(*FString::Printf(TEXT("hub.dressing.%02d"), Index), (Index % 4 == 0) ? TEXT("zone.dawnwatch.banner") : TEXT("zone.vendor.crates"), EEOZoneArea::DawnwatchGate, FVector(X, Y, 70.0f), FVector(0.7f, 0.5f, 0.75f), FLinearColor(0.20f, 0.40f, 0.94f, 1.0f), FRotator(0.0f, Index * 13.0f, 0.0f), false));
    }

    Profile.Lights = {
        Light(TEXT("light.spawn.vista"), FVector(-900.0f, -450.0f, 280.0f), FLinearColor(0.30f, 0.52f, 1.0f, 1.0f), 5200.0f, 900.0f),
        Light(TEXT("light.gate.gold"), FVector(-810.0f, -520.0f, 220.0f), FLinearColor(1.0f, 0.72f, 0.22f, 1.0f), 4200.0f, 550.0f),
        Light(TEXT("light.scholar.cyan"), FVector(-1180.0f, -250.0f, 180.0f), FLinearColor(0.30f, 0.84f, 1.0f, 1.0f), 3600.0f, 480.0f),
        Light(TEXT("light.road.one"), FVector(-420.0f, 80.0f, 130.0f), FLinearColor(1.0f, 0.66f, 0.20f, 1.0f), 2400.0f, 320.0f),
        Light(TEXT("light.road.two"), FVector(40.0f, 80.0f, 130.0f), FLinearColor(1.0f, 0.66f, 0.20f, 1.0f), 2400.0f, 320.0f),
        Light(TEXT("light.road.three"), FVector(480.0f, 20.0f, 130.0f), FLinearColor(1.0f, 0.66f, 0.20f, 1.0f), 2400.0f, 320.0f),
        Light(TEXT("light.relic.core"), FVector(760.0f, -260.0f, 260.0f), FLinearColor(0.82f, 0.18f, 1.0f, 1.0f), 12000.0f, 1050.0f),
        Light(TEXT("light.skywatch"), FVector(1260.0f, -760.0f, 320.0f), FLinearColor(0.36f, 0.74f, 1.0f, 1.0f), 8200.0f, 880.0f),
        Light(TEXT("light.future.briarfen"), FVector(-1280.0f, 420.0f, 180.0f), FLinearColor(0.22f, 0.72f, 0.24f, 1.0f), 3400.0f, 520.0f),
        Light(TEXT("light.future.moonwell"), FVector(260.0f, 660.0f, 180.0f), FLinearColor(0.36f, 0.72f, 1.0f, 1.0f), 3400.0f, 520.0f),
        Light(TEXT("light.future.sunspire"), FVector(100.0f, -1060.0f, 190.0f), FLinearColor(1.0f, 0.68f, 0.26f, 1.0f), 3400.0f, 560.0f),
        Light(TEXT("light.combat.warning.n"), FVector(760.0f, -60.0f, 150.0f), FLinearColor(1.0f, 0.18f, 0.08f, 1.0f), 2800.0f, 360.0f),
        Light(TEXT("light.combat.warning.s"), FVector(760.0f, -470.0f, 150.0f), FLinearColor(1.0f, 0.18f, 0.08f, 1.0f), 2800.0f, 360.0f),
        Light(TEXT("light.combat.warning.e"), FVector(970.0f, -260.0f, 150.0f), FLinearColor(1.0f, 0.18f, 0.08f, 1.0f), 2800.0f, 360.0f),
        Light(TEXT("light.companion.camp"), FVector(-1040.0f, -340.0f, 130.0f), FLinearColor(1.0f, 0.54f, 0.20f, 1.0f), 3000.0f, 420.0f),
        Light(TEXT("light.training.yard"), FVector(-620.0f, -180.0f, 130.0f), FLinearColor(1.0f, 0.70f, 0.30f, 1.0f), 2800.0f, 400.0f),
        Light(TEXT("light.vendor.corner"), FVector(-980.0f, -120.0f, 120.0f), FLinearColor(1.0f, 0.64f, 0.25f, 1.0f), 2400.0f, 360.0f),
        Light(TEXT("light.dungeon.promise"), FVector(1140.0f, -650.0f, 260.0f), FLinearColor(0.48f, 0.86f, 1.0f, 1.0f), 4800.0f, 640.0f)
    };

    Profile.Anchors = {
        Anchor(TEXT("anchor.spawn_vista"), TEXT("Spawn Vista"), FVector(-720.0f, -420.0f, 96.0f), FRotator(0.0f, 28.0f, 0.0f)),
        Anchor(TEXT("anchor.hub.commander"), TEXT("Dawnwatch Commander"), FVector(-880.0f, -290.0f, 96.0f)),
        Anchor(TEXT("anchor.hub.training"), TEXT("Training Yard"), FVector(-620.0f, -180.0f, 96.0f)),
        Anchor(TEXT("anchor.hub.vendor"), TEXT("Vendor Corner"), FVector(-980.0f, -120.0f, 96.0f)),
        Anchor(TEXT("anchor.hub.scholar"), TEXT("Scholarium Station"), FVector(-1180.0f, -250.0f, 96.0f)),
        Anchor(TEXT("anchor.road.first_contact"), TEXT("First Road Contact"), FVector(180.0f, -20.0f, 96.0f)),
        Anchor(TEXT("anchor.road.side_pocket"), TEXT("Ruined Shrine Pocket"), FVector(310.0f, 420.0f, 96.0f)),
        Anchor(TEXT("anchor.relic_surge.entry"), TEXT("Relic Surge Entry"), FVector(520.0f, -170.0f, 96.0f)),
        Anchor(TEXT("anchor.relic_surge.center"), TEXT("Relic Surge Center"), FVector(760.0f, -260.0f, 96.0f)),
        Anchor(TEXT("anchor.relic_surge.caster_north"), TEXT("Caster North"), FVector(760.0f, -70.0f, 96.0f)),
        Anchor(TEXT("anchor.relic_surge.caster_east"), TEXT("Caster East"), FVector(970.0f, -260.0f, 96.0f)),
        Anchor(TEXT("anchor.relic_surge.elite"), TEXT("Elite Spawn"), FVector(840.0f, -310.0f, 96.0f)),
        Anchor(TEXT("anchor.skywatch.reveal"), TEXT("Sky-Watch Reveal"), FVector(1120.0f, -640.0f, 96.0f)),
        Anchor(TEXT("anchor.future.briarfen"), TEXT("Briarfen Road"), FVector(-1280.0f, 420.0f, 96.0f)),
        Anchor(TEXT("anchor.future.moonwell"), TEXT("Moonwell Road"), FVector(260.0f, 660.0f, 96.0f)),
        Anchor(TEXT("anchor.future.sunspire"), TEXT("Sunspire Pass"), FVector(100.0f, -1060.0f, 96.0f))
    };

    Profile.FutureHooks = {
        Hook(TEXT("future.briarfen_march"), TEXT("Briarfen March"), TEXT("Pathwarden scouts have not cleared the wetlands."), TEXT("anchor.future.briarfen")),
        Hook(TEXT("future.moonwell_archive"), TEXT("Moonwell Archive"), TEXT("The Scholarium seal needs a stabilized relic key."), TEXT("anchor.future.moonwell")),
        Hook(TEXT("future.sunspire_expanse"), TEXT("Sunspire Expanse"), TEXT("The southern pass is blocked by a relic storm."), TEXT("anchor.future.sunspire")),
        Hook(TEXT("future.skywatch_dungeon"), TEXT("Sky-Watch Entrance"), TEXT("The entrance opens after the relic surge is stabilized."), TEXT("anchor.skywatch.reveal"))
    };

    Profile.MinimapMarkers = {
        MinimapMarker(TEXT("map.dawnwatch"), TEXT("Dawnwatch"), FVector2D(0.18f, 0.32f), FLinearColor(0.18f, 0.48f, 1.0f, 1.0f)),
        MinimapMarker(TEXT("map.commander"), TEXT("Commander"), FVector2D(0.22f, 0.40f), FLinearColor(1.0f, 0.82f, 0.24f, 1.0f)),
        MinimapMarker(TEXT("map.vendor"), TEXT("Vendor"), FVector2D(0.18f, 0.46f), FLinearColor(0.74f, 0.52f, 0.24f, 1.0f)),
        MinimapMarker(TEXT("map.road"), TEXT("Starfall Road"), FVector2D(0.46f, 0.46f), FLinearColor(0.76f, 0.58f, 0.34f, 1.0f)),
        MinimapMarker(TEXT("map.side_pocket"), TEXT("Shrine"), FVector2D(0.50f, 0.30f), FLinearColor(0.42f, 0.82f, 1.0f, 1.0f)),
        MinimapMarker(TEXT("map.surge"), TEXT("Relic Surge"), FVector2D(0.68f, 0.52f), FLinearColor(0.88f, 0.20f, 1.0f, 1.0f)),
        MinimapMarker(TEXT("map.skywatch"), TEXT("Sky-Watch"), FVector2D(0.86f, 0.72f), FLinearColor(0.42f, 0.82f, 1.0f, 1.0f)),
        MinimapMarker(TEXT("map.briarfen"), TEXT("Briarfen"), FVector2D(0.08f, 0.18f), FLinearColor(0.24f, 0.72f, 0.28f, 1.0f)),
        MinimapMarker(TEXT("map.moonwell"), TEXT("Moonwell"), FVector2D(0.54f, 0.12f), FLinearColor(0.38f, 0.74f, 1.0f, 1.0f))
    };

    return Profile;
}
