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

    auto CreateMesh = [this, BasicShapeMaterial](const FString& Name, UStaticMesh* Mesh, const FVector& Location, const FRotator& Rotation, const FVector& Scale, const FLinearColor& TintColor, bool bBlocksMovement, FName SpecId, FName AssetRoleId)
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
            VisualSpec.AssetRoleId);
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
            VisualSpec.AssetRoleId);
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
