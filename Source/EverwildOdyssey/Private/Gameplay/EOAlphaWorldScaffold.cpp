#include "Gameplay/EOAlphaWorldScaffold.h"

#include "Components/ExponentialHeightFogComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/PostProcessComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SkyLightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Gameplay/EOEncounterDirectorComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Presentation/EOAssetRoleTypes.h"
#include "World/EOZoneProfile.h"

namespace
{
constexpr double MinimapMarkerWorldWidth = 2400.0;
constexpr double MinimapMarkerWorldDepth = 1800.0;
constexpr double MinimapMarkerHeight = 180.0;

enum class EEOProxyMeshShape : uint8
{
    Cube,
    Cone,
    Cylinder,
    Sphere,
    LightBeam
};

struct FEOShowcaseAssemblyPartSpec
{
    FName PartId = NAME_None;
    FName AssemblyTag = NAME_None;
    FName AssetRoleId = NAME_None;
    EEOProxyMeshShape Shape = EEOProxyMeshShape::Cube;
    FVector Location = FVector::ZeroVector;
    FRotator Rotation = FRotator::ZeroRotator;
    FVector Scale = FVector::OneVector;
    FLinearColor Tint = FLinearColor::White;
    bool bBlocksMovement = false;
};

UMaterialInstanceDynamic* BuildTintedMaterial(UMaterialInterface* BaseMaterial, UObject* Outer, const FLinearColor& TintColor)
{
    if (BaseMaterial == nullptr)
    {
        return nullptr;
    }

    UMaterialInstanceDynamic* TintedMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, Outer);
    TintedMaterial->SetVectorParameterValue(TEXT("Color"), TintColor);
    return TintedMaterial;
}

FLinearColor ResolveProfileTint(FName AssetRoleId, const FLinearColor& ProfileTint)
{
    if (ProfileTint.A > 0.0f)
    {
        return ProfileTint;
    }

    FEOAssetRoleDefinition AssetRole;
    if (FEOAssetRoleCatalog::TryGetRoleDefinition(AssetRoleId, AssetRole))
    {
        return AssetRole.DebugTint;
    }

    return FLinearColor::White;
}

FEOShowcaseAssemblyPartSpec AssemblyPart(const TCHAR* Id, const TCHAR* AssemblyTag, const TCHAR* AssetRoleId, EEOProxyMeshShape Shape, const FVector& Location, const FVector& Scale, const FLinearColor& Tint, const FRotator& Rotation = FRotator::ZeroRotator, bool bBlocksMovement = false)
{
    FEOShowcaseAssemblyPartSpec Result;
    Result.PartId = FName(Id);
    Result.AssemblyTag = FName(AssemblyTag);
    Result.AssetRoleId = FName(AssetRoleId);
    Result.Shape = Shape;
    Result.Location = Location;
    Result.Rotation = Rotation;
    Result.Scale = Scale;
    Result.Tint = Tint;
    Result.bBlocksMovement = bBlocksMovement;
    return Result;
}

UStaticMesh* ProxyMeshForRole(FName AssetRoleId, UStaticMesh* CubeMesh, UStaticMesh* ConeMesh, UStaticMesh* CylinderMesh, UStaticMesh* SphereMesh)
{
    if (AssetRoleId == TEXT("zone.relic_surge.crystal")
        || AssetRoleId == TEXT("zone.road.signpost")
        || AssetRoleId == TEXT("zone.skywatch.entrance")
        || AssetRoleId == TEXT("zone.future.bridge_blocker")
        || AssetRoleId == TEXT("zone.foliage.tree"))
    {
        return ConeMesh;
    }

    if (AssetRoleId == TEXT("zone.dawnwatch.wall")
        || AssetRoleId == TEXT("zone.road.lamp")
        || AssetRoleId == TEXT("zone.training.dummy"))
    {
        return CylinderMesh;
    }

    if (AssetRoleId == TEXT("zone.relic_surge.core")
        || AssetRoleId == TEXT("zone.scholar.station")
        || AssetRoleId == TEXT("zone.foliage.rock"))
    {
        return SphereMesh;
    }

    return CubeMesh;
}

UStaticMesh* MeshForAssemblyShape(EEOProxyMeshShape Shape, UStaticMesh* CubeMesh, UStaticMesh* ConeMesh, UStaticMesh* CylinderMesh, UStaticMesh* SphereMesh, UStaticMesh* LightBeamMesh)
{
    switch (Shape)
    {
    case EEOProxyMeshShape::Cone:
        return ConeMesh;
    case EEOProxyMeshShape::Cylinder:
        return CylinderMesh;
    case EEOProxyMeshShape::Sphere:
        return SphereMesh;
    case EEOProxyMeshShape::LightBeam:
        return LightBeamMesh != nullptr ? LightBeamMesh : ConeMesh;
    case EEOProxyMeshShape::Cube:
    default:
        return CubeMesh;
    }
}

TArray<FEOShowcaseAssemblyPartSpec> BuildShowcaseAssemblyParts()
{
    TArray<FEOShowcaseAssemblyPartSpec> Parts;
    Parts.Reserve(128);

    const FLinearColor DawnwatchStone(0.08f, 0.18f, 0.52f, 1.0f);
    const FLinearColor DawnwatchTrim(0.16f, 0.44f, 1.0f, 1.0f);
    const FLinearColor BannerGold(1.0f, 0.70f, 0.18f, 1.0f);
    const FLinearColor LampWarm(1.0f, 0.46f, 0.10f, 1.0f);

    Parts.Add(AssemblyPart(TEXT("assembly.dawnwatch_gate.base"), TEXT("visual.assembly.dawnwatch_gate"), TEXT("zone.dawnwatch.gate"), EEOProxyMeshShape::Cube, FVector(-950.0f, -560.0f, 100.0f), FVector(5.6f, 0.72f, 1.05f), DawnwatchStone, FRotator::ZeroRotator, true));
    Parts.Add(AssemblyPart(TEXT("assembly.dawnwatch_gate.lintel"), TEXT("visual.assembly.dawnwatch_gate"), TEXT("zone.dawnwatch.gate"), EEOProxyMeshShape::Cube, FVector(-950.0f, -560.0f, 345.0f), FVector(4.8f, 0.68f, 0.55f), DawnwatchTrim, FRotator::ZeroRotator, true));
    Parts.Add(AssemblyPart(TEXT("assembly.dawnwatch_gate.left_door"), TEXT("visual.assembly.dawnwatch_gate"), TEXT("zone.dawnwatch.gate"), EEOProxyMeshShape::Cube, FVector(-1010.0f, -520.0f, 165.0f), FVector(0.46f, 0.22f, 1.95f), FLinearColor(0.06f, 0.12f, 0.26f, 1.0f), FRotator(0.0f, 8.0f, 0.0f), true));
    Parts.Add(AssemblyPart(TEXT("assembly.dawnwatch_gate.right_door"), TEXT("visual.assembly.dawnwatch_gate"), TEXT("zone.dawnwatch.gate"), EEOProxyMeshShape::Cube, FVector(-890.0f, -520.0f, 165.0f), FVector(0.46f, 0.22f, 1.95f), FLinearColor(0.06f, 0.12f, 0.26f, 1.0f), FRotator(0.0f, -8.0f, 0.0f), true));

    for (int32 Side = 0; Side < 2; ++Side)
    {
        const float SideSign = Side == 0 ? -1.0f : 1.0f;
        const FString SideName = Side == 0 ? TEXT("left") : TEXT("right");
        const float TowerX = -950.0f + SideSign * 260.0f;

        Parts.Add(AssemblyPart(*FString::Printf(TEXT("assembly.dawnwatch_gate.%s_tower_core"), *SideName), TEXT("visual.assembly.dawnwatch_gate"), TEXT("zone.dawnwatch.wall"), EEOProxyMeshShape::Cylinder, FVector(TowerX, -590.0f, 245.0f), FVector(1.25f, 1.25f, 3.45f), DawnwatchStone, FRotator::ZeroRotator, true));
        Parts.Add(AssemblyPart(*FString::Printf(TEXT("assembly.dawnwatch_gate.%s_tower_band"), *SideName), TEXT("visual.assembly.dawnwatch_gate"), TEXT("zone.dawnwatch.wall"), EEOProxyMeshShape::Cylinder, FVector(TowerX, -590.0f, 420.0f), FVector(1.42f, 1.42f, 0.28f), DawnwatchTrim, FRotator::ZeroRotator, true));
        Parts.Add(AssemblyPart(*FString::Printf(TEXT("assembly.dawnwatch_gate.%s_roof"), *SideName), TEXT("visual.assembly.dawnwatch_gate"), TEXT("zone.dawnwatch.gate"), EEOProxyMeshShape::Cone, FVector(TowerX, -590.0f, 565.0f), FVector(1.65f, 1.65f, 1.28f), BannerGold, FRotator::ZeroRotator, false));
        Parts.Add(AssemblyPart(*FString::Printf(TEXT("assembly.dawnwatch_gate.%s_wall_wing_inner"), *SideName), TEXT("visual.assembly.dawnwatch_gate"), TEXT("zone.dawnwatch.wall"), EEOProxyMeshShape::Cube, FVector(-950.0f + SideSign * 405.0f, -602.0f, 180.0f), FVector(2.25f, 0.42f, 1.55f), DawnwatchStone, FRotator(0.0f, SideSign * 4.0f, 0.0f), true));
        Parts.Add(AssemblyPart(*FString::Printf(TEXT("assembly.dawnwatch_gate.%s_wall_wing_outer"), *SideName), TEXT("visual.assembly.dawnwatch_gate"), TEXT("zone.dawnwatch.wall"), EEOProxyMeshShape::Cube, FVector(-950.0f + SideSign * 600.0f, -615.0f, 145.0f), FVector(1.95f, 0.36f, 1.05f), DawnwatchStone, FRotator(0.0f, SideSign * 7.0f, 0.0f), true));

        for (int32 BannerIndex = 0; BannerIndex < 2; ++BannerIndex)
        {
            const float BannerX = TowerX + SideSign * (52.0f + BannerIndex * 50.0f);
            Parts.Add(AssemblyPart(*FString::Printf(TEXT("assembly.dawnwatch_gate.%s_banner_pole_%02d"), *SideName, BannerIndex), TEXT("visual.assembly.dawnwatch_gate"), TEXT("zone.dawnwatch.banner"), EEOProxyMeshShape::Cylinder, FVector(BannerX, -500.0f, 330.0f), FVector(0.12f, 0.12f, 1.95f), BannerGold, FRotator::ZeroRotator, false));
            Parts.Add(AssemblyPart(*FString::Printf(TEXT("assembly.dawnwatch_gate.%s_banner_cloth_%02d"), *SideName, BannerIndex), TEXT("visual.assembly.dawnwatch_gate"), TEXT("zone.dawnwatch.banner"), EEOProxyMeshShape::Cube, FVector(BannerX + SideSign * 18.0f, -493.0f, 285.0f), FVector(0.34f, 0.06f, 0.78f), DawnwatchTrim, FRotator(0.0f, SideSign * 9.0f, 0.0f), false));
        }
    }

    for (int32 Index = 0; Index < 10; ++Index)
    {
        const float X = -1310.0f + static_cast<float>(Index) * 80.0f;
        Parts.Add(AssemblyPart(*FString::Printf(TEXT("assembly.dawnwatch_gate.crenel_%02d"), Index), TEXT("visual.assembly.dawnwatch_gate"), TEXT("zone.dawnwatch.wall"), EEOProxyMeshShape::Cube, FVector(X, -594.0f, 445.0f), FVector(0.38f, 0.34f, 0.42f), DawnwatchTrim, FRotator::ZeroRotator, true));
    }

    for (int32 Index = 0; Index < 6; ++Index)
    {
        const float X = -1200.0f + static_cast<float>(Index) * 100.0f;
        Parts.Add(AssemblyPart(*FString::Printf(TEXT("assembly.dawnwatch_gate.warm_lamp_%02d"), Index), TEXT("visual.assembly.dawnwatch_gate"), TEXT("zone.road.lamp"), EEOProxyMeshShape::Sphere, FVector(X, -470.0f, 250.0f), FVector(0.22f, 0.22f, 0.22f), LampWarm, FRotator::ZeroRotator, false));
    }

    const FLinearColor RoadBase(0.58f, 0.40f, 0.20f, 1.0f);
    const FLinearColor RoadGlow(1.0f, 0.78f, 0.32f, 1.0f);
    for (int32 Index = 0; Index < 14; ++Index)
    {
        const float X = -690.0f + static_cast<float>(Index) * 105.0f;
        const float Y = -185.0f + FMath::Sin(static_cast<float>(Index) * 0.58f) * 48.0f;
        Parts.Add(AssemblyPart(*FString::Printf(TEXT("assembly.road_ribbon.segment_%02d"), Index), TEXT("visual.assembly.road_ribbon"), TEXT("zone.road.signpost"), EEOProxyMeshShape::Cube, FVector(X, Y, 17.0f), FVector(1.22f, 0.62f, 0.08f), RoadBase, FRotator(0.0f, 9.0f + Index * 2.0f, 0.0f), false));
        Parts.Add(AssemblyPart(*FString::Printf(TEXT("assembly.road_ribbon.left_marker_%02d"), Index), TEXT("visual.assembly.road_ribbon"), TEXT("zone.road.lamp"), EEOProxyMeshShape::Cylinder, FVector(X, Y + 82.0f, 64.0f), FVector(0.16f, 0.16f, 0.68f), RoadGlow, FRotator::ZeroRotator, false));
        Parts.Add(AssemblyPart(*FString::Printf(TEXT("assembly.road_ribbon.right_marker_%02d"), Index), TEXT("visual.assembly.road_ribbon"), TEXT("zone.road.lamp"), EEOProxyMeshShape::Cylinder, FVector(X, Y - 82.0f, 64.0f), FVector(0.16f, 0.16f, 0.68f), RoadGlow, FRotator::ZeroRotator, false));
    }

    const FLinearColor RelicPurple(0.88f, 0.12f, 1.0f, 1.0f);
    const FLinearColor RelicCyan(0.28f, 0.90f, 1.0f, 1.0f);
    const FVector RelicCenter(760.0f, -260.0f, 0.0f);
    Parts.Add(AssemblyPart(TEXT("assembly.relic_beam.platform"), TEXT("visual.assembly.relic_beam"), TEXT("zone.relic_surge.core"), EEOProxyMeshShape::Cylinder, FVector(RelicCenter.X, RelicCenter.Y, 38.0f), FVector(2.9f, 2.9f, 0.24f), FLinearColor(0.24f, 0.08f, 0.38f, 1.0f), FRotator::ZeroRotator, true));
    Parts.Add(AssemblyPart(TEXT("assembly.relic_beam.column"), TEXT("visual.assembly.relic_beam"), TEXT("zone.relic_surge.core"), EEOProxyMeshShape::LightBeam, FVector(RelicCenter.X, RelicCenter.Y, 520.0f), FVector(1.0f, 1.0f, 6.4f), RelicCyan, FRotator::ZeroRotator, false));

    for (int32 Index = 0; Index < 16; ++Index)
    {
        const float Angle = static_cast<float>(Index) * 22.5f;
        const float Radians = FMath::DegreesToRadians(Angle);
        const FVector Location(RelicCenter.X + FMath::Cos(Radians) * 210.0f, RelicCenter.Y + FMath::Sin(Radians) * 210.0f, 128.0f + (Index % 2) * 24.0f);
        Parts.Add(AssemblyPart(*FString::Printf(TEXT("assembly.relic_beam.crystal_ring_%02d"), Index), TEXT("visual.assembly.relic_beam"), TEXT("zone.relic_surge.crystal"), EEOProxyMeshShape::Cone, Location, FVector(0.34f, 0.34f, 1.28f), Index % 2 == 0 ? RelicPurple : RelicCyan, FRotator(0.0f, Angle, 18.0f), false));
    }

    for (int32 Index = 0; Index < 12; ++Index)
    {
        const float Angle = static_cast<float>(Index) * 30.0f + 12.0f;
        const float Radians = FMath::DegreesToRadians(Angle);
        const float Radius = 105.0f + static_cast<float>(Index % 3) * 42.0f;
        const FVector Location(RelicCenter.X + FMath::Cos(Radians) * Radius, RelicCenter.Y + FMath::Sin(Radians) * Radius, 380.0f + static_cast<float>(Index % 4) * 46.0f);
        Parts.Add(AssemblyPart(*FString::Printf(TEXT("assembly.relic_beam.floating_shard_%02d"), Index), TEXT("visual.assembly.relic_beam"), TEXT("zone.relic_surge.crystal"), EEOProxyMeshShape::Cone, Location, FVector(0.22f, 0.22f, 0.92f), RelicPurple, FRotator(18.0f, Angle, 32.0f), false));
    }

    for (int32 Index = 0; Index < 8; ++Index)
    {
        const float Angle = static_cast<float>(Index) * 45.0f;
        const float Radians = FMath::DegreesToRadians(Angle);
        const FVector Location(RelicCenter.X + FMath::Cos(Radians) * 330.0f, RelicCenter.Y + FMath::Sin(Radians) * 330.0f, 95.0f);
        Parts.Add(AssemblyPart(*FString::Printf(TEXT("assembly.relic_beam.warning_pylon_%02d"), Index), TEXT("visual.assembly.relic_beam"), TEXT("zone.future.bridge_blocker"), EEOProxyMeshShape::Cylinder, Location, FVector(0.28f, 0.28f, 1.2f), FLinearColor(1.0f, 0.10f, 0.04f, 1.0f), FRotator::ZeroRotator, true));
    }

    const FLinearColor SkywatchBlue(0.18f, 0.66f, 1.0f, 1.0f);
    const FLinearColor SkywatchCore(0.46f, 0.92f, 1.0f, 1.0f);
    const FVector SkywatchCenter(1260.0f, -760.0f, 0.0f);
    Parts.Add(AssemblyPart(TEXT("assembly.skywatch_entrance.left_column"), TEXT("visual.assembly.skywatch_entrance"), TEXT("zone.skywatch.entrance"), EEOProxyMeshShape::Cube, FVector(SkywatchCenter.X - 145.0f, SkywatchCenter.Y, 255.0f), FVector(0.62f, 0.72f, 4.1f), SkywatchBlue, FRotator::ZeroRotator, true));
    Parts.Add(AssemblyPart(TEXT("assembly.skywatch_entrance.right_column"), TEXT("visual.assembly.skywatch_entrance"), TEXT("zone.skywatch.entrance"), EEOProxyMeshShape::Cube, FVector(SkywatchCenter.X + 145.0f, SkywatchCenter.Y, 255.0f), FVector(0.62f, 0.72f, 4.1f), SkywatchBlue, FRotator::ZeroRotator, true));
    Parts.Add(AssemblyPart(TEXT("assembly.skywatch_entrance.top_cap"), TEXT("visual.assembly.skywatch_entrance"), TEXT("zone.skywatch.entrance"), EEOProxyMeshShape::Cube, FVector(SkywatchCenter.X, SkywatchCenter.Y, 515.0f), FVector(3.75f, 0.74f, 0.58f), SkywatchCore, FRotator::ZeroRotator, true));
    Parts.Add(AssemblyPart(TEXT("assembly.skywatch_entrance.portal_core"), TEXT("visual.assembly.skywatch_entrance"), TEXT("zone.skywatch.entrance"), EEOProxyMeshShape::Sphere, FVector(SkywatchCenter.X, SkywatchCenter.Y + 8.0f, 255.0f), FVector(1.2f, 0.28f, 2.1f), SkywatchCore, FRotator::ZeroRotator, false));
    Parts.Add(AssemblyPart(TEXT("assembly.skywatch_entrance.blue_beam"), TEXT("visual.assembly.skywatch_entrance"), TEXT("zone.skywatch.entrance"), EEOProxyMeshShape::LightBeam, FVector(SkywatchCenter.X, SkywatchCenter.Y, 580.0f), FVector(0.72f, 0.72f, 3.8f), SkywatchCore, FRotator::ZeroRotator, false));

    for (int32 Side = 0; Side < 2; ++Side)
    {
        const float SideSign = Side == 0 ? -1.0f : 1.0f;
        for (int32 Index = 0; Index < 3; ++Index)
        {
            Parts.Add(AssemblyPart(*FString::Printf(TEXT("assembly.skywatch_entrance.spire_%d_%02d"), Side, Index), TEXT("visual.assembly.skywatch_entrance"), TEXT("zone.skywatch.entrance"), EEOProxyMeshShape::Cone, FVector(SkywatchCenter.X + SideSign * (235.0f + Index * 58.0f), SkywatchCenter.Y - 12.0f, 320.0f + Index * 52.0f), FVector(0.52f, 0.52f, 1.5f), SkywatchBlue, FRotator::ZeroRotator, false));
        }
    }

    for (int32 Index = 0; Index < 6; ++Index)
    {
        Parts.Add(AssemblyPart(*FString::Printf(TEXT("assembly.skywatch_entrance.step_%02d"), Index), TEXT("visual.assembly.skywatch_entrance"), TEXT("zone.skywatch.entrance"), EEOProxyMeshShape::Cube, FVector(SkywatchCenter.X, SkywatchCenter.Y + 125.0f + Index * 34.0f, 18.0f + Index * 8.0f), FVector(2.5f - Index * 0.16f, 0.28f, 0.11f), SkywatchBlue, FRotator::ZeroRotator, true));
        Parts.Add(AssemblyPart(*FString::Printf(TEXT("assembly.skywatch_entrance.rune_%02d"), Index), TEXT("visual.assembly.skywatch_entrance"), TEXT("zone.skywatch.entrance"), EEOProxyMeshShape::Sphere, FVector(SkywatchCenter.X - 150.0f + Index * 60.0f, SkywatchCenter.Y + 48.0f, 130.0f + (Index % 2) * 80.0f), FVector(0.24f, 0.08f, 0.24f), SkywatchCore, FRotator::ZeroRotator, false));
    }

    struct FFutureBoundaryAnchor
    {
        const TCHAR* Name;
        FVector Location;
        FLinearColor Tint;
    };

    const FFutureBoundaryAnchor FutureAnchors[] = {
        { TEXT("briarfen"), FVector(-1280.0f, 420.0f, 0.0f), FLinearColor(0.16f, 0.80f, 0.22f, 1.0f) },
        { TEXT("moonwell"), FVector(260.0f, 660.0f, 0.0f), FLinearColor(0.35f, 0.76f, 1.0f, 1.0f) },
        { TEXT("sunspire"), FVector(100.0f, -1060.0f, 0.0f), FLinearColor(1.0f, 0.68f, 0.18f, 1.0f) }
    };

    for (const FFutureBoundaryAnchor& Anchor : FutureAnchors)
    {
        Parts.Add(AssemblyPart(*FString::Printf(TEXT("assembly.future_boundaries.%s_blocker_left"), Anchor.Name), TEXT("visual.assembly.future_boundaries"), TEXT("zone.future.bridge_blocker"), EEOProxyMeshShape::Cube, Anchor.Location + FVector(-85.0f, 0.0f, 105.0f), FVector(0.52f, 0.36f, 1.5f), Anchor.Tint, FRotator(0.0f, -12.0f, 0.0f), true));
        Parts.Add(AssemblyPart(*FString::Printf(TEXT("assembly.future_boundaries.%s_blocker_right"), Anchor.Name), TEXT("visual.assembly.future_boundaries"), TEXT("zone.future.bridge_blocker"), EEOProxyMeshShape::Cube, Anchor.Location + FVector(85.0f, 0.0f, 105.0f), FVector(0.52f, 0.36f, 1.5f), Anchor.Tint, FRotator(0.0f, 12.0f, 0.0f), true));
        Parts.Add(AssemblyPart(*FString::Printf(TEXT("assembly.future_boundaries.%s_crossbar"), Anchor.Name), TEXT("visual.assembly.future_boundaries"), TEXT("zone.future.bridge_blocker"), EEOProxyMeshShape::Cube, Anchor.Location + FVector(0.0f, 0.0f, 195.0f), FVector(2.15f, 0.32f, 0.32f), Anchor.Tint, FRotator::ZeroRotator, true));
        Parts.Add(AssemblyPart(*FString::Printf(TEXT("assembly.future_boundaries.%s_sign"), Anchor.Name), TEXT("visual.assembly.future_boundaries"), TEXT("zone.road.signpost"), EEOProxyMeshShape::Cube, Anchor.Location + FVector(0.0f, -62.0f, 132.0f), FVector(1.0f, 0.08f, 0.55f), FLinearColor(0.05f, 0.08f, 0.12f, 1.0f), FRotator::ZeroRotator, false));
        Parts.Add(AssemblyPart(*FString::Printf(TEXT("assembly.future_boundaries.%s_spire_left"), Anchor.Name), TEXT("visual.assembly.future_boundaries"), TEXT("zone.future.bridge_blocker"), EEOProxyMeshShape::Cone, Anchor.Location + FVector(-135.0f, 10.0f, 245.0f), FVector(0.42f, 0.42f, 1.1f), Anchor.Tint, FRotator::ZeroRotator, false));
        Parts.Add(AssemblyPart(*FString::Printf(TEXT("assembly.future_boundaries.%s_spire_right"), Anchor.Name), TEXT("visual.assembly.future_boundaries"), TEXT("zone.future.bridge_blocker"), EEOProxyMeshShape::Cone, Anchor.Location + FVector(135.0f, 10.0f, 245.0f), FVector(0.42f, 0.42f, 1.1f), Anchor.Tint, FRotator::ZeroRotator, false));
    }

    return Parts;
}
}

AEOAlphaWorldScaffold::AEOAlphaWorldScaffold()
{
    PrimaryActorTick.bCanEverTick = false;

    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("AlphaWorldRoot"));
    RootComponent = SceneRoot;

    EncounterDirector = CreateDefaultSubobject<UEOEncounterDirectorComponent>(TEXT("EncounterDirector"));

    ValeSkyLight = CreateDefaultSubobject<USkyLightComponent>(TEXT("ValeSkyLight"));
    ValeSkyLight->SetupAttachment(SceneRoot);
    ValeSkyLight->SetIntensity(0.85f);
    ValeSkyLight->SetLightColor(FLinearColor(0.55f, 0.68f, 1.0f, 1.0f));

    ValeFog = CreateDefaultSubobject<UExponentialHeightFogComponent>(TEXT("ValeFog"));
    ValeFog->SetupAttachment(SceneRoot);
    ValeFog->SetFogDensity(0.018f);
    ValeFog->SetFogHeightFalloff(0.24f);
    ValeFog->SetFogInscatteringColor(FLinearColor(0.42f, 0.56f, 0.78f, 1.0f));

    ValeColorGrade = CreateDefaultSubobject<UPostProcessComponent>(TEXT("ValeColorGrade"));
    ValeColorGrade->SetupAttachment(SceneRoot);
    ValeColorGrade->bUnbound = true;
    ValeColorGrade->BlendWeight = 1.0f;
    ValeColorGrade->Settings.bOverride_BloomIntensity = true;
    ValeColorGrade->Settings.BloomIntensity = 0.6f;
    ValeColorGrade->Settings.bOverride_VignetteIntensity = true;
    ValeColorGrade->Settings.VignetteIntensity = 0.18f;
    ValeColorGrade->Settings.bOverride_ColorSaturation = true;
    ValeColorGrade->Settings.ColorSaturation = FVector4(1.18f, 1.12f, 1.08f, 1.0f);
    ValeColorGrade->Settings.bOverride_ColorContrast = true;
    ValeColorGrade->Settings.ColorContrast = FVector4(1.08f, 1.06f, 1.04f, 1.0f);
}

void AEOAlphaWorldScaffold::BeginPlay()
{
    Super::BeginPlay();

    SpawnVerticalSliceWorld();

    FEOEncounterRecord RelicSurgeEncounter;
    RelicSurgeEncounter.EncounterId = TEXT("encounter.relic_surge.wave_one");
    RelicSurgeEncounter.DisplayName = FText::FromString(TEXT("Relic Surge: First Wave"));
    RelicSurgeEncounter.LinkedWorldEventId = TEXT("starfall.relic_surge");
    RelicSurgeEncounter.State = EEOEncounterState::Dormant;
    RelicSurgeEncounter.TotalEnemies = 12;
    RelicSurgeEncounter.RecommendedLevel = 1;

    EncounterDirector->InitializeEncounters({ RelicSurgeEncounter });
}

void AEOAlphaWorldScaffold::SpawnVerticalSliceWorld()
{
    const FEOZoneProfile ZoneProfile = BuildDefaultZoneProfile();

    UStaticMesh* CubeMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube"));
    UStaticMesh* ConeMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cone.Cone"));
    UStaticMesh* CylinderMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
    UStaticMesh* SphereMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Sphere.Sphere"));
    UStaticMesh* LightBeamMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/EngineVolumetrics/LightBeam/Mesh/S_EV_SimpleLightBeam_03.S_EV_SimpleLightBeam_03"));
    UStaticMesh* PcgTreeMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/PCG/SampleContent/SimpleForest/Meshes/PCG_Tree_01.PCG_Tree_01"));
    if (PcgTreeMesh == nullptr)
    {
        PcgTreeMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/PCGBiomeSample/Meshes/PCG_Tree_01.PCG_Tree_01"));
    }
    UStaticMesh* PcgBoulderMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/PCG/SampleContent/SimpleForest/Meshes/PCG_Boulder_02.PCG_Boulder_02"));
    if (PcgBoulderMesh == nullptr)
    {
        PcgBoulderMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/PCGBiomeSample/Meshes/PCG_Boulder_01.PCG_Boulder_01"));
    }
    UMaterialInterface* BasicShapeMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));

    if (CubeMesh == nullptr || ConeMesh == nullptr || CylinderMesh == nullptr || SphereMesh == nullptr)
    {
        return;
    }

    auto CreateMesh = [this, BasicShapeMaterial](const FString& Name, UStaticMesh* Mesh, const FVector& Location, const FRotator& Rotation, const FVector& Scale, const FLinearColor& TintColor, bool bBlocksMovement, FName SpecId, FName AssetRoleId, FName AssemblyTag)
    {
        UStaticMeshComponent* MeshComponent = NewObject<UStaticMeshComponent>(this, MakeUniqueObjectName(this, UStaticMeshComponent::StaticClass(), FName(*Name)));
        MeshComponent->SetupAttachment(SceneRoot);
        MeshComponent->SetStaticMesh(Mesh);
        MeshComponent->SetRelativeLocation(Location);
        MeshComponent->SetRelativeRotation(Rotation);
        MeshComponent->SetRelativeScale3D(Scale);
        MeshComponent->SetCollisionEnabled(bBlocksMovement ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
        MeshComponent->SetCastShadow(true);
        MeshComponent->ComponentTags.Add(SpecId);
        MeshComponent->ComponentTags.Add(AssetRoleId);
        if (!AssemblyTag.IsNone())
        {
            MeshComponent->ComponentTags.Add(AssemblyTag);
        }
        if (UMaterialInstanceDynamic* TintedMaterial = BuildTintedMaterial(BasicShapeMaterial, MeshComponent, TintColor))
        {
            MeshComponent->SetMaterial(0, TintedMaterial);
        }
        AddInstanceComponent(MeshComponent);
        MeshComponent->RegisterComponent();
        RuntimeWorldMeshes.Add(MeshComponent);
    };

    auto CreateMarkerMesh = [this, BasicShapeMaterial](const FEOMinimapMarkerSpec& MarkerSpec, UStaticMesh* Mesh)
    {
        UStaticMeshComponent* MarkerComponent = NewObject<UStaticMeshComponent>(this, MakeUniqueObjectName(this, UStaticMeshComponent::StaticClass(), FName(*FString::Printf(TEXT("Marker_%s"), *MarkerSpec.MarkerId.ToString()))));
        MarkerComponent->SetupAttachment(SceneRoot);
        MarkerComponent->SetStaticMesh(Mesh);
        MarkerComponent->SetRelativeLocation(FVector(
            (MarkerSpec.NormalizedPosition.X - 0.5f) * MinimapMarkerWorldWidth,
            (0.5f - MarkerSpec.NormalizedPosition.Y) * MinimapMarkerWorldDepth,
            MinimapMarkerHeight));
        MarkerComponent->SetRelativeScale3D(FVector(0.16f, 0.16f, 0.05f));
        MarkerComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        MarkerComponent->SetCastShadow(false);
        MarkerComponent->ComponentTags.Add(MarkerSpec.MarkerId);
        if (UMaterialInstanceDynamic* TintedMaterial = BuildTintedMaterial(BasicShapeMaterial, MarkerComponent, MarkerSpec.Color))
        {
            MarkerComponent->SetMaterial(0, TintedMaterial);
        }
        AddInstanceComponent(MarkerComponent);
        MarkerComponent->RegisterComponent();
        RuntimeWorldMarkerMeshes.Add(MarkerComponent);
    };

    for (const FEOZoneVisualSpec& VisualSpec : ZoneProfile.Landmarks)
    {
        CreateMesh(
            FString::Printf(TEXT("Landmark_%s"), *VisualSpec.Id.ToString()),
            ProxyMeshForRole(VisualSpec.AssetRoleId, CubeMesh, ConeMesh, CylinderMesh, SphereMesh),
            VisualSpec.Location,
            VisualSpec.Rotation,
            VisualSpec.Scale,
            ResolveProfileTint(VisualSpec.AssetRoleId, VisualSpec.Tint),
            true,
            VisualSpec.Id,
            VisualSpec.AssetRoleId,
            NAME_None);
    }

    for (const FEOShowcaseAssemblyPartSpec& PartSpec : BuildShowcaseAssemblyParts())
    {
        CreateMesh(
            FString::Printf(TEXT("Assembly_%s"), *PartSpec.PartId.ToString()),
            MeshForAssemblyShape(PartSpec.Shape, CubeMesh, ConeMesh, CylinderMesh, SphereMesh, LightBeamMesh),
            PartSpec.Location,
            PartSpec.Rotation,
            PartSpec.Scale,
            PartSpec.Tint,
            PartSpec.bBlocksMovement,
            PartSpec.PartId,
            PartSpec.AssetRoleId,
            PartSpec.AssemblyTag);
    }

    for (const FEOZoneVisualSpec& VisualSpec : ZoneProfile.ScenicProps)
    {
        UStaticMesh* SelectedMesh = ProxyMeshForRole(VisualSpec.AssetRoleId, CubeMesh, ConeMesh, CylinderMesh, SphereMesh);
        FVector Location = VisualSpec.Location;
        FVector Scale = VisualSpec.Scale;

        if (VisualSpec.AssetRoleId == TEXT("zone.foliage.tree") && PcgTreeMesh != nullptr)
        {
            SelectedMesh = PcgTreeMesh;
            Location.Z = 0.0f;
        }
        else if (VisualSpec.AssetRoleId == TEXT("zone.foliage.rock") && PcgBoulderMesh != nullptr)
        {
            SelectedMesh = PcgBoulderMesh;
            Scale = FVector(
                FMath::Max(0.72f, VisualSpec.Scale.X * 0.88f),
                FMath::Max(0.72f, VisualSpec.Scale.Y * 0.88f),
                FMath::Max(0.72f, VisualSpec.Scale.Z * 0.88f));
        }

        CreateMesh(
            FString::Printf(TEXT("Scenic_%s"), *VisualSpec.Id.ToString()),
            SelectedMesh,
            Location,
            VisualSpec.Rotation,
            Scale,
            ResolveProfileTint(VisualSpec.AssetRoleId, VisualSpec.Tint),
            VisualSpec.bBlocksMovement,
            VisualSpec.Id,
            VisualSpec.AssetRoleId,
            NAME_None);
    }

    for (const FEOZoneLightSpec& LightSpec : ZoneProfile.Lights)
    {
        UPointLightComponent* LightComponent = NewObject<UPointLightComponent>(this, MakeUniqueObjectName(this, UPointLightComponent::StaticClass(), LightSpec.LightId));
        LightComponent->SetupAttachment(SceneRoot);
        LightComponent->SetRelativeLocation(LightSpec.Location);
        LightComponent->SetLightColor(LightSpec.Color);
        LightComponent->SetIntensity(LightSpec.Intensity);
        LightComponent->SetAttenuationRadius(LightSpec.Radius);
        AddInstanceComponent(LightComponent);
        LightComponent->RegisterComponent();
        RuntimeWorldLights.Add(LightComponent);
    }

    for (const FEOMinimapMarkerSpec& MarkerSpec : ZoneProfile.MinimapMarkers)
    {
        CreateMarkerMesh(MarkerSpec, CylinderMesh);
    }
}

int32 AEOAlphaWorldScaffold::ExpectedLandmarkCount()
{
    return BuildDefaultZoneProfile().Landmarks.Num();
}

int32 AEOAlphaWorldScaffold::ExpectedScenicPropCount()
{
    return BuildDefaultZoneProfile().ScenicProps.Num();
}

int32 AEOAlphaWorldScaffold::ExpectedShowcaseAssemblyPartCount()
{
    return BuildShowcaseAssemblyParts().Num();
}

int32 AEOAlphaWorldScaffold::ExpectedAmbientLightCount()
{
    return BuildDefaultZoneProfile().Lights.Num();
}

int32 AEOAlphaWorldScaffold::ExpectedMinimapMarkerCount()
{
    return BuildDefaultZoneProfile().MinimapMarkers.Num();
}

FEOZoneProfile AEOAlphaWorldScaffold::BuildDefaultZoneProfile()
{
    return FEOZoneProfileCatalog::BuildStarfallValeProfile();
}

TArray<FEOZoneVisualSpec> AEOAlphaWorldScaffold::BuildDefaultLandmarks()
{
    return BuildDefaultZoneProfile().Landmarks;
}

TArray<FEOZoneVisualSpec> AEOAlphaWorldScaffold::BuildDefaultScenicProps()
{
    return BuildDefaultZoneProfile().ScenicProps;
}

TArray<FEOZoneLightSpec> AEOAlphaWorldScaffold::BuildDefaultAmbientLights()
{
    return BuildDefaultZoneProfile().Lights;
}

TArray<FEOMinimapMarkerSpec> AEOAlphaWorldScaffold::BuildDefaultMinimapMarkers()
{
    return BuildDefaultZoneProfile().MinimapMarkers;
}
