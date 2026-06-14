#include "Gameplay/EOAlphaWorldScaffold.h"

#include "Components/ExponentialHeightFogComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/PostProcessComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SkyLightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Gameplay/EOEncounterDirectorComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

namespace
{
FEOAlphaLandmarkSpec Landmark(FName Id, const TCHAR* Name, const FVector& Location, const FVector& Scale, const FLinearColor& TintColor)
{
    FEOAlphaLandmarkSpec Spec;
    Spec.LandmarkId = Id;
    Spec.DisplayName = FText::FromString(Name);
    Spec.Location = Location;
    Spec.Scale = Scale;
    Spec.TintColor = TintColor;
    return Spec;
}

FEOAlphaScenicPropSpec ScenicProp(
    FName Id,
    FName Style,
    const FVector& Location,
    const FVector& Scale,
    const FLinearColor& TintColor,
    const FRotator& Rotation = FRotator::ZeroRotator,
    bool bBlocksMovement = true)
{
    FEOAlphaScenicPropSpec Spec;
    Spec.PropId = Id;
    Spec.VisualStyle = Style;
    Spec.Location = Location;
    Spec.Rotation = Rotation;
    Spec.Scale = Scale;
    Spec.TintColor = TintColor;
    Spec.bBlocksMovement = bBlocksMovement;
    return Spec;
}

FEOAlphaLightSpec AmbientLight(FName Id, const FVector& Location, const FLinearColor& Color, float Intensity, float Radius)
{
    FEOAlphaLightSpec Spec;
    Spec.LightId = Id;
    Spec.Location = Location;
    Spec.Color = Color;
    Spec.Intensity = Intensity;
    Spec.Radius = Radius;
    return Spec;
}

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

UStaticMesh* MeshForStyle(FName Style, UStaticMesh* CubeMesh, UStaticMesh* ConeMesh, UStaticMesh* CylinderMesh, UStaticMesh* SphereMesh)
{
    if (Style == TEXT("tree_canopy") || Style == TEXT("crystal") || Style == TEXT("hut_roof") || Style == TEXT("gate_spire") || Style == TEXT("flower") || Style == TEXT("grass_blade"))
    {
        return ConeMesh;
    }

    if (Style == TEXT("tree_trunk") || Style == TEXT("tower") || Style == TEXT("pillar") || Style == TEXT("lantern") || Style == TEXT("root"))
    {
        return CylinderMesh;
    }

    if (Style == TEXT("surge_orb") || Style == TEXT("shrine") || Style == TEXT("relic_core"))
    {
        return SphereMesh;
    }

    return CubeMesh;
}

UStaticMesh* MeshForLandmark(FName LandmarkId, UStaticMesh* CubeMesh, UStaticMesh* ConeMesh, UStaticMesh* CylinderMesh, UStaticMesh* SphereMesh)
{
    if (LandmarkId == TEXT("starfall.gate") || LandmarkId == TEXT("stormwatch.beacon"))
    {
        return ConeMesh;
    }

    if (LandmarkId == TEXT("dawnwatch.training_ring") || LandmarkId == TEXT("skybridge.hub"))
    {
        return CylinderMesh;
    }

    if (LandmarkId == TEXT("starfall.relic_surge") || LandmarkId == TEXT("moonwell.shrine"))
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

    auto CreateMesh = [this, BasicShapeMaterial](const FString& Name, UStaticMesh* Mesh, const FVector& Location, const FRotator& Rotation, const FVector& Scale, const FLinearColor& TintColor, bool bBlocksMovement, bool bApplyTint)
    {
        UStaticMeshComponent* MeshComponent = NewObject<UStaticMeshComponent>(this, MakeUniqueObjectName(this, UStaticMeshComponent::StaticClass(), FName(*Name)));
        MeshComponent->SetupAttachment(SceneRoot);
        MeshComponent->SetStaticMesh(Mesh);
        MeshComponent->SetRelativeLocation(Location);
        MeshComponent->SetRelativeRotation(Rotation);
        MeshComponent->SetRelativeScale3D(Scale);
        MeshComponent->SetCollisionEnabled(bBlocksMovement ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
        MeshComponent->SetCastShadow(true);
        if (bApplyTint)
        {
            if (UMaterialInstanceDynamic* TintedMaterial = BuildTintedMaterial(BasicShapeMaterial, MeshComponent, TintColor))
            {
                MeshComponent->SetMaterial(0, TintedMaterial);
            }
        }
        AddInstanceComponent(MeshComponent);
        MeshComponent->RegisterComponent();
        RuntimeWorldMeshes.Add(MeshComponent);
    };

    for (const FEOAlphaLandmarkSpec& LandmarkSpec : BuildDefaultLandmarks())
    {
        CreateMesh(
            FString::Printf(TEXT("Landmark_%s"), *LandmarkSpec.LandmarkId.ToString()),
            MeshForLandmark(LandmarkSpec.LandmarkId, CubeMesh, ConeMesh, CylinderMesh, SphereMesh),
            LandmarkSpec.Location,
            FRotator::ZeroRotator,
            LandmarkSpec.Scale,
            LandmarkSpec.TintColor,
            true,
            true);
    }

    for (const FEOAlphaScenicPropSpec& ScenicProp : BuildDefaultScenicProps())
    {
        if (ScenicProp.VisualStyle == TEXT("tree_trunk") && PcgTreeMesh != nullptr)
        {
            continue;
        }

        UStaticMesh* SelectedMesh = MeshForStyle(ScenicProp.VisualStyle, CubeMesh, ConeMesh, CylinderMesh, SphereMesh);
        FVector Location = ScenicProp.Location;
        FVector Scale = ScenicProp.Scale;
        bool bApplyTint = true;

        if (ScenicProp.VisualStyle == TEXT("tree_canopy") && PcgTreeMesh != nullptr)
        {
            SelectedMesh = PcgTreeMesh;
            Location.Z = 0.0f;
            Scale = FVector(0.72f, 0.72f, 0.72f);
            bApplyTint = false;
        }
        else if ((ScenicProp.VisualStyle == TEXT("cliff") || ScenicProp.VisualStyle == TEXT("ruin_block") || ScenicProp.VisualStyle == TEXT("road_marker")) && PcgBoulderMesh != nullptr)
        {
            SelectedMesh = PcgBoulderMesh;
            Scale = FVector(
                FMath::Max(0.72f, ScenicProp.Scale.X * 0.88f),
                FMath::Max(0.72f, ScenicProp.Scale.Y * 0.88f),
                FMath::Max(0.72f, ScenicProp.Scale.Z * 0.88f));
            bApplyTint = false;
        }

        CreateMesh(
            FString::Printf(TEXT("Scenic_%s"), *ScenicProp.PropId.ToString()),
            SelectedMesh,
            Location,
            ScenicProp.Rotation,
            Scale,
            ScenicProp.TintColor,
            ScenicProp.bBlocksMovement,
            bApplyTint);
    }

    for (const FEOAlphaLightSpec& LightSpec : BuildDefaultAmbientLights())
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
}

TArray<FEOAlphaLandmarkSpec> AEOAlphaWorldScaffold::BuildDefaultLandmarks()
{
    TArray<FEOAlphaLandmarkSpec> Landmarks;
    Landmarks.Reserve(ExpectedLandmarkCount);

    Landmarks.Add(Landmark(TEXT("dawnwatch.keep"), TEXT("Dawnwatch Keep"), FVector(-1120.0f, -720.0f, 150.0f), FVector(5.6f, 4.0f, 3.0f), FLinearColor(0.09f, 0.18f, 0.56f, 1.0f)));
    Landmarks.Add(Landmark(TEXT("starfall.gate"), TEXT("Starfall Gate"), FVector(620.0f, 130.0f, 280.0f), FVector(1.6f, 1.6f, 5.6f), FLinearColor(1.0f, 0.70f, 0.16f, 1.0f)));
    Landmarks.Add(Landmark(TEXT("dawnwatch.training_ring"), TEXT("Dawnwatch Training Ring"), FVector(-260.0f, 520.0f, 26.0f), FVector(3.8f, 3.8f, 0.22f), FLinearColor(0.12f, 0.62f, 0.24f, 1.0f)));
    Landmarks.Add(Landmark(TEXT("starfall.relic_surge"), TEXT("Relic Surge"), FVector(760.0f, -260.0f, 160.0f), FVector(1.9f, 1.9f, 2.7f), FLinearColor(0.78f, 0.22f, 1.0f, 1.0f)));
    Landmarks.Add(Landmark(TEXT("vale.road"), TEXT("Starfall Vale Road"), FVector(0.0f, 0.0f, 8.0f), FVector(17.0f, 0.48f, 0.08f), FLinearColor(0.24f, 0.19f, 0.14f, 1.0f)));
    Landmarks.Add(Landmark(TEXT("skybridge.hub"), TEXT("Skybridge Hub"), FVector(0.0f, -880.0f, 72.0f), FVector(3.4f, 3.4f, 0.42f), FLinearColor(0.06f, 0.62f, 0.78f, 1.0f)));
    Landmarks.Add(Landmark(TEXT("mirrorlake"), TEXT("Mirrorlake"), FVector(220.0f, 820.0f, 4.0f), FVector(7.0f, 4.4f, 0.05f), FLinearColor(0.10f, 0.42f, 0.92f, 0.82f)));
    Landmarks.Add(Landmark(TEXT("emberfall.forge"), TEXT("Emberfall Forge"), FVector(-760.0f, 210.0f, 96.0f), FVector(2.3f, 2.1f, 1.6f), FLinearColor(1.0f, 0.28f, 0.08f, 1.0f)));
    Landmarks.Add(Landmark(TEXT("mosscap.village"), TEXT("Mosscap Village"), FVector(-1020.0f, 520.0f, 68.0f), FVector(4.2f, 2.4f, 1.1f), FLinearColor(0.30f, 0.48f, 0.20f, 1.0f)));
    Landmarks.Add(Landmark(TEXT("elderroot.grove"), TEXT("Elderroot Grove"), FVector(-1260.0f, 60.0f, 130.0f), FVector(2.4f, 2.4f, 2.6f), FLinearColor(0.10f, 0.52f, 0.24f, 1.0f)));
    Landmarks.Add(Landmark(TEXT("moonwell.shrine"), TEXT("Moonwell Shrine"), FVector(360.0f, 560.0f, 96.0f), FVector(1.35f, 1.35f, 1.0f), FLinearColor(0.40f, 0.72f, 1.0f, 1.0f)));
    Landmarks.Add(Landmark(TEXT("shattered.colossus"), TEXT("Shattered Colossus"), FVector(1160.0f, -680.0f, 210.0f), FVector(2.2f, 1.0f, 4.2f), FLinearColor(0.45f, 0.42f, 0.36f, 1.0f)));
    Landmarks.Add(Landmark(TEXT("cloudstep.cliffs"), TEXT("Cloudstep Cliffs"), FVector(250.0f, -1220.0f, 230.0f), FVector(8.0f, 1.8f, 4.4f), FLinearColor(0.34f, 0.38f, 0.42f, 1.0f)));
    Landmarks.Add(Landmark(TEXT("stormwatch.beacon"), TEXT("Stormwatch Beacon"), FVector(-280.0f, -1180.0f, 240.0f), FVector(1.2f, 1.2f, 4.8f), FLinearColor(0.18f, 0.82f, 1.0f, 1.0f)));

    return Landmarks;
}

TArray<FEOAlphaScenicPropSpec> AEOAlphaWorldScaffold::BuildDefaultScenicProps()
{
    TArray<FEOAlphaScenicPropSpec> Props;
    Props.Reserve(ExpectedScenicPropCount);

    Props.Add(ScenicProp(TEXT("terrain.vale.floor"), TEXT("terrain"), FVector(0.0f, 0.0f, -8.0f), FVector(38.0f, 30.0f, 0.16f), FLinearColor(0.10f, 0.35f, 0.16f, 1.0f)));
    Props.Add(ScenicProp(TEXT("terrain.grove.floor"), TEXT("terrain"), FVector(-1180.0f, 160.0f, -5.0f), FVector(10.0f, 8.0f, 0.12f), FLinearColor(0.07f, 0.26f, 0.12f, 1.0f)));
    Props.Add(ScenicProp(TEXT("terrain.village.floor"), TEXT("terrain"), FVector(-1040.0f, 520.0f, -4.0f), FVector(8.0f, 5.5f, 0.12f), FLinearColor(0.18f, 0.32f, 0.16f, 1.0f)));
    Props.Add(ScenicProp(TEXT("terrain.surge.floor"), TEXT("terrain"), FVector(760.0f, -270.0f, -4.0f), FVector(8.5f, 6.5f, 0.12f), FLinearColor(0.20f, 0.12f, 0.26f, 1.0f)));
    Props.Add(ScenicProp(TEXT("mirrorlake.water.sheet"), TEXT("water"), FVector(220.0f, 820.0f, 8.0f), FVector(8.4f, 5.1f, 0.04f), FLinearColor(0.08f, 0.42f, 0.96f, 0.72f), FRotator::ZeroRotator, false));
    Props.Add(ScenicProp(TEXT("road.vale.main"), TEXT("road"), FVector(0.0f, 16.0f, 8.0f), FVector(18.5f, 0.42f, 0.06f), FLinearColor(0.30f, 0.22f, 0.15f, 1.0f)));
    Props.Add(ScenicProp(TEXT("road.village.branch"), TEXT("road"), FVector(-780.0f, 360.0f, 9.0f), FVector(5.4f, 0.34f, 0.06f), FLinearColor(0.30f, 0.22f, 0.15f, 1.0f), FRotator(0.0f, 28.0f, 0.0f)));
    Props.Add(ScenicProp(TEXT("bridge.skybridge.deck"), TEXT("bridge"), FVector(-40.0f, -760.0f, 42.0f), FVector(6.8f, 0.7f, 0.12f), FLinearColor(0.34f, 0.27f, 0.18f, 1.0f), FRotator(0.0f, -4.0f, 0.0f)));
    Props.Add(ScenicProp(TEXT("bridge.skybridge.left_rail"), TEXT("bridge"), FVector(-45.0f, -705.0f, 76.0f), FVector(6.5f, 0.08f, 0.34f), FLinearColor(0.18f, 0.58f, 0.66f, 1.0f), FRotator(0.0f, -4.0f, 0.0f)));
    Props.Add(ScenicProp(TEXT("bridge.skybridge.right_rail"), TEXT("bridge"), FVector(-36.0f, -815.0f, 76.0f), FVector(6.5f, 0.08f, 0.34f), FLinearColor(0.18f, 0.58f, 0.66f, 1.0f), FRotator(0.0f, -4.0f, 0.0f)));
    Props.Add(ScenicProp(TEXT("starfall.gate.plinth"), TEXT("shrine"), FVector(620.0f, 130.0f, 52.0f), FVector(1.35f, 1.35f, 0.34f), FLinearColor(1.0f, 0.58f, 0.20f, 1.0f)));
    Props.Add(ScenicProp(TEXT("starfall.gate.column.left"), TEXT("gate_arch"), FVector(518.0f, 130.0f, 190.0f), FVector(0.32f, 0.32f, 2.8f), FLinearColor(0.92f, 0.72f, 0.34f, 1.0f)));
    Props.Add(ScenicProp(TEXT("starfall.gate.column.right"), TEXT("gate_arch"), FVector(722.0f, 130.0f, 190.0f), FVector(0.32f, 0.32f, 2.8f), FLinearColor(0.92f, 0.72f, 0.34f, 1.0f)));
    Props.Add(ScenicProp(TEXT("starfall.gate.arch.top"), TEXT("gate_arch"), FVector(620.0f, 130.0f, 400.0f), FVector(2.4f, 0.24f, 0.32f), FLinearColor(1.0f, 0.82f, 0.38f, 1.0f)));
    Props.Add(ScenicProp(TEXT("starfall.gate.rune.left"), TEXT("crystal"), FVector(506.0f, 130.0f, 330.0f), FVector(0.18f, 0.18f, 1.2f), FLinearColor(0.36f, 0.84f, 1.0f, 1.0f), FRotator(0.0f, 0.0f, -10.0f), false));
    Props.Add(ScenicProp(TEXT("starfall.gate.rune.right"), TEXT("crystal"), FVector(734.0f, 130.0f, 330.0f), FVector(0.18f, 0.18f, 1.2f), FLinearColor(0.36f, 0.84f, 1.0f, 1.0f), FRotator(0.0f, 0.0f, 10.0f), false));
    Props.Add(ScenicProp(TEXT("starfall.gate.step.one"), TEXT("gate_step"), FVector(620.0f, 20.0f, 22.0f), FVector(2.3f, 0.46f, 0.14f), FLinearColor(0.54f, 0.44f, 0.36f, 1.0f)));
    Props.Add(ScenicProp(TEXT("starfall.gate.step.two"), TEXT("gate_step"), FVector(620.0f, -34.0f, 34.0f), FVector(2.0f, 0.42f, 0.14f), FLinearColor(0.60f, 0.50f, 0.40f, 1.0f)));
    Props.Add(ScenicProp(TEXT("starfall.gate.step.three"), TEXT("gate_step"), FVector(620.0f, -84.0f, 46.0f), FVector(1.7f, 0.38f, 0.14f), FLinearColor(0.66f, 0.55f, 0.42f, 1.0f)));

    Props.Add(ScenicProp(TEXT("keep.inner_keep"), TEXT("keep"), FVector(-1120.0f, -720.0f, 220.0f), FVector(3.2f, 2.8f, 3.4f), FLinearColor(0.12f, 0.18f, 0.40f, 1.0f)));
    Props.Add(ScenicProp(TEXT("keep.tower.nw"), TEXT("tower"), FVector(-1420.0f, -980.0f, 210.0f), FVector(0.95f, 0.95f, 3.4f), FLinearColor(0.14f, 0.20f, 0.46f, 1.0f)));
    Props.Add(ScenicProp(TEXT("keep.tower.ne"), TEXT("tower"), FVector(-820.0f, -980.0f, 210.0f), FVector(0.95f, 0.95f, 3.4f), FLinearColor(0.14f, 0.20f, 0.46f, 1.0f)));
    Props.Add(ScenicProp(TEXT("keep.tower.sw"), TEXT("tower"), FVector(-1420.0f, -460.0f, 210.0f), FVector(0.95f, 0.95f, 3.4f), FLinearColor(0.14f, 0.20f, 0.46f, 1.0f)));
    Props.Add(ScenicProp(TEXT("keep.tower.se"), TEXT("tower"), FVector(-820.0f, -460.0f, 210.0f), FVector(0.95f, 0.95f, 3.4f), FLinearColor(0.14f, 0.20f, 0.46f, 1.0f)));
    Props.Add(ScenicProp(TEXT("keep.wall.north"), TEXT("wall"), FVector(-1120.0f, -1010.0f, 96.0f), FVector(6.8f, 0.24f, 1.3f), FLinearColor(0.10f, 0.16f, 0.34f, 1.0f)));
    Props.Add(ScenicProp(TEXT("keep.wall.south"), TEXT("wall"), FVector(-1120.0f, -430.0f, 96.0f), FVector(6.8f, 0.24f, 1.3f), FLinearColor(0.10f, 0.16f, 0.34f, 1.0f)));
    Props.Add(ScenicProp(TEXT("keep.wall.west"), TEXT("wall"), FVector(-1448.0f, -720.0f, 96.0f), FVector(0.24f, 5.6f, 1.3f), FLinearColor(0.10f, 0.16f, 0.34f, 1.0f)));
    Props.Add(ScenicProp(TEXT("keep.wall.east"), TEXT("wall"), FVector(-792.0f, -720.0f, 96.0f), FVector(0.24f, 5.6f, 1.3f), FLinearColor(0.10f, 0.16f, 0.34f, 1.0f)));
    Props.Add(ScenicProp(TEXT("keep.gatehouse"), TEXT("wall"), FVector(-790.0f, -720.0f, 128.0f), FVector(0.42f, 1.6f, 2.0f), FLinearColor(0.16f, 0.22f, 0.48f, 1.0f)));
    Props.Add(ScenicProp(TEXT("keep.banner.blue"), TEXT("banner"), FVector(-760.0f, -620.0f, 190.0f), FVector(0.08f, 0.18f, 1.2f), FLinearColor(0.12f, 0.46f, 1.0f, 1.0f), FRotator::ZeroRotator, false));
    Props.Add(ScenicProp(TEXT("keep.banner.gold"), TEXT("banner"), FVector(-760.0f, -820.0f, 190.0f), FVector(0.08f, 0.18f, 1.2f), FLinearColor(1.0f, 0.76f, 0.20f, 1.0f), FRotator::ZeroRotator, false));

    for (int32 Index = 0; Index < 8; ++Index)
    {
        const float X = -1230.0f + static_cast<float>(Index % 4) * 135.0f;
        const float Y = 430.0f + static_cast<float>(Index / 4) * 165.0f;
        const FString HutId = FString::Printf(TEXT("village.hut.%02d"), Index);
        Props.Add(ScenicProp(FName(*HutId), TEXT("hut"), FVector(X, Y, 58.0f), FVector(0.95f, 0.85f, 0.72f), FLinearColor(0.34f, 0.26f, 0.18f, 1.0f)));
        Props.Add(ScenicProp(FName(*(HutId + TEXT(".roof"))), TEXT("hut_roof"), FVector(X, Y, 132.0f), FVector(1.05f, 1.05f, 0.72f), FLinearColor(0.18f, 0.42f, 0.20f, 1.0f)));
    }

    for (int32 Index = 0; Index < 18; ++Index)
    {
        const float AngleDegrees = static_cast<float>(Index) * 37.0f;
        const float Radius = 210.0f + static_cast<float>(Index % 5) * 56.0f;
        const float X = -1240.0f + FMath::Cos(FMath::DegreesToRadians(AngleDegrees)) * Radius;
        const float Y = 110.0f + FMath::Sin(FMath::DegreesToRadians(AngleDegrees)) * Radius;
        const FString TreeId = FString::Printf(TEXT("grove.tree.%02d"), Index);
        Props.Add(ScenicProp(FName(*(TreeId + TEXT(".trunk"))), TEXT("tree_trunk"), FVector(X, Y, 76.0f), FVector(0.18f, 0.18f, 1.36f), FLinearColor(0.24f, 0.13f, 0.06f, 1.0f)));
        Props.Add(ScenicProp(FName(*(TreeId + TEXT(".canopy"))), TEXT("tree_canopy"), FVector(X, Y, 196.0f), FVector(0.72f, 0.72f, 1.18f), FLinearColor(0.05f, 0.42f, 0.16f, 1.0f), FRotator(0.0f, AngleDegrees, 0.0f), false));
    }

    for (int32 Index = 0; Index < 16; ++Index)
    {
        const float X = -420.0f + static_cast<float>(Index % 8) * 142.0f;
        const float Y = 190.0f + static_cast<float>(Index / 8) * 118.0f + ((Index % 2 == 0) ? 0.0f : 42.0f);
        const FName Style = (Index % 3 == 0) ? TEXT("flower") : TEXT("grass_blade");
        const FLinearColor Color = (Style == TEXT("flower"))
            ? FLinearColor(1.0f, 0.84f, 0.28f, 1.0f)
            : FLinearColor(0.08f, 0.54f, 0.18f, 1.0f);
        Props.Add(ScenicProp(FName(*FString::Printf(TEXT("meadow.detail.%02d"), Index)), Style, FVector(X, Y, 34.0f), FVector(0.12f, 0.12f, 0.38f), Color, FRotator(0.0f, static_cast<float>(Index) * 23.0f, 0.0f), false));
    }

    for (int32 Index = 0; Index < 18; ++Index)
    {
        const float AngleDegrees = static_cast<float>(Index) * 20.0f;
        const float Radius = 150.0f + static_cast<float>(Index % 3) * 55.0f;
        const float X = 760.0f + FMath::Cos(FMath::DegreesToRadians(AngleDegrees)) * Radius;
        const float Y = -260.0f + FMath::Sin(FMath::DegreesToRadians(AngleDegrees)) * Radius;
        const FString CrystalId = FString::Printf(TEXT("surge.crystal.%02d"), Index);
        Props.Add(ScenicProp(FName(*CrystalId), TEXT("crystal"), FVector(X, Y, 74.0f), FVector(0.24f + 0.03f * (Index % 4), 0.24f + 0.03f * (Index % 4), 1.05f + 0.16f * (Index % 3)), FLinearColor(0.78f, 0.18f, 1.0f, 1.0f), FRotator(0.0f, AngleDegrees, 5.0f * (Index % 3))));
    }

    for (int32 Index = 0; Index < 6; ++Index)
    {
        const float AngleDegrees = static_cast<float>(Index) * 60.0f;
        const float X = 760.0f + FMath::Cos(FMath::DegreesToRadians(AngleDegrees)) * 92.0f;
        const float Y = -260.0f + FMath::Sin(FMath::DegreesToRadians(AngleDegrees)) * 92.0f;
        Props.Add(ScenicProp(FName(*FString::Printf(TEXT("surge.orb.%02d"), Index)), TEXT("surge_orb"), FVector(X, Y, 164.0f), FVector(0.28f, 0.28f, 0.28f), FLinearColor(1.0f, 0.25f, 0.86f, 1.0f), FRotator::ZeroRotator, false));
    }

    for (int32 Index = 0; Index < 8; ++Index)
    {
        Props.Add(ScenicProp(FName(*FString::Printf(TEXT("colossus.pillar.%02d"), Index)), TEXT("pillar"), FVector(1020.0f + (Index % 4) * 86.0f, -820.0f + (Index / 4) * 180.0f, 108.0f), FVector(0.32f, 0.32f, 2.0f + 0.22f * (Index % 3)), FLinearColor(0.42f, 0.40f, 0.34f, 1.0f), FRotator(0.0f, static_cast<float>(Index) * 11.0f, 0.0f)));
    }

    for (int32 Index = 0; Index < 8; ++Index)
    {
        Props.Add(ScenicProp(FName(*FString::Printf(TEXT("cloudstep.cliff.face.%02d"), Index)), TEXT("cliff"), FVector(-250.0f + Index * 150.0f, -1270.0f, 112.0f + 18.0f * (Index % 3)), FVector(1.4f, 0.34f, 2.0f + 0.18f * (Index % 4)), FLinearColor(0.31f, 0.35f, 0.38f, 1.0f), FRotator(0.0f, -8.0f + Index * 2.0f, 0.0f)));
    }

    for (int32 Index = 0; Index < 6; ++Index)
    {
        Props.Add(ScenicProp(FName(*FString::Printf(TEXT("colossus.block.%02d"), Index)), TEXT("ruin_block"), FVector(1080.0f + (Index % 3) * 135.0f, -600.0f + (Index / 3) * 105.0f, 46.0f), FVector(0.92f, 0.52f, 0.42f), FLinearColor(0.38f, 0.36f, 0.31f, 1.0f), FRotator(0.0f, 18.0f * Index, 0.0f)));
    }

    for (int32 Index = 0; Index < 6; ++Index)
    {
        Props.Add(ScenicProp(FName(*FString::Printf(TEXT("road.marker.%02d"), Index)), TEXT("road_marker"), FVector(-520.0f + Index * 190.0f, -62.0f + ((Index % 2 == 0) ? 0.0f : 120.0f), 24.0f), FVector(0.28f, 0.22f, 0.22f), FLinearColor(0.55f, 0.50f, 0.42f, 1.0f)));
    }

    for (int32 Index = 0; Index < 5; ++Index)
    {
        Props.Add(ScenicProp(FName(*FString::Printf(TEXT("road.lantern.%02d"), Index)), TEXT("lantern"), FVector(-450.0f + Index * 250.0f, 86.0f, 82.0f), FVector(0.10f, 0.10f, 1.15f), FLinearColor(1.0f, 0.68f, 0.24f, 1.0f), FRotator::ZeroRotator, false));
    }

    Props.Add(ScenicProp(TEXT("banner.road.dawnwatch.one"), TEXT("banner"), FVector(-320.0f, -118.0f, 92.0f), FVector(0.08f, 0.12f, 0.86f), FLinearColor(0.12f, 0.46f, 1.0f, 1.0f), FRotator(0.0f, -8.0f, 0.0f), false));
    Props.Add(ScenicProp(TEXT("banner.road.dawnwatch.two"), TEXT("banner"), FVector(230.0f, -118.0f, 92.0f), FVector(0.08f, 0.12f, 0.86f), FLinearColor(0.12f, 0.46f, 1.0f, 1.0f), FRotator(0.0f, 8.0f, 0.0f), false));
    Props.Add(ScenicProp(TEXT("banner.village.green"), TEXT("banner"), FVector(-890.0f, 360.0f, 82.0f), FVector(0.08f, 0.12f, 0.78f), FLinearColor(0.16f, 0.74f, 0.28f, 1.0f), FRotator(0.0f, 28.0f, 0.0f), false));
    Props.Add(ScenicProp(TEXT("banner.forge.orange"), TEXT("banner"), FVector(-680.0f, 156.0f, 88.0f), FVector(0.08f, 0.12f, 0.84f), FLinearColor(1.0f, 0.32f, 0.10f, 1.0f), FRotator(0.0f, -18.0f, 0.0f), false));

    return Props;
}

TArray<FEOAlphaLightSpec> AEOAlphaWorldScaffold::BuildDefaultAmbientLights()
{
    TArray<FEOAlphaLightSpec> Lights;
    Lights.Reserve(ExpectedAmbientLightCount);

    Lights.Add(AmbientLight(TEXT("light.starfall.gate"), FVector(620.0f, 130.0f, 360.0f), FLinearColor(1.0f, 0.72f, 0.22f, 1.0f), 9000.0f, 950.0f));
    Lights.Add(AmbientLight(TEXT("light.relic.surge.core"), FVector(760.0f, -260.0f, 220.0f), FLinearColor(0.82f, 0.20f, 1.0f, 1.0f), 11000.0f, 980.0f));
    Lights.Add(AmbientLight(TEXT("light.dawnwatch.keep"), FVector(-1040.0f, -720.0f, 240.0f), FLinearColor(0.26f, 0.48f, 1.0f, 1.0f), 5200.0f, 780.0f));
    Lights.Add(AmbientLight(TEXT("light.emberfall.forge"), FVector(-760.0f, 210.0f, 150.0f), FLinearColor(1.0f, 0.24f, 0.08f, 1.0f), 6200.0f, 600.0f));
    Lights.Add(AmbientLight(TEXT("light.moonwell"), FVector(360.0f, 560.0f, 150.0f), FLinearColor(0.42f, 0.74f, 1.0f, 1.0f), 5600.0f, 650.0f));
    Lights.Add(AmbientLight(TEXT("light.skybridge"), FVector(-40.0f, -760.0f, 150.0f), FLinearColor(0.18f, 0.74f, 0.92f, 1.0f), 4800.0f, 620.0f));
    Lights.Add(AmbientLight(TEXT("light.village.center"), FVector(-1040.0f, 535.0f, 120.0f), FLinearColor(1.0f, 0.58f, 0.24f, 1.0f), 4300.0f, 520.0f));
    Lights.Add(AmbientLight(TEXT("light.elderroot"), FVector(-1260.0f, 80.0f, 220.0f), FLinearColor(0.18f, 0.78f, 0.26f, 1.0f), 3600.0f, 740.0f));
    Lights.Add(AmbientLight(TEXT("light.crystal.north"), FVector(750.0f, -40.0f, 130.0f), FLinearColor(0.86f, 0.22f, 1.0f, 1.0f), 3600.0f, 470.0f));
    Lights.Add(AmbientLight(TEXT("light.crystal.south"), FVector(765.0f, -480.0f, 130.0f), FLinearColor(0.86f, 0.22f, 1.0f, 1.0f), 3600.0f, 470.0f));
    Lights.Add(AmbientLight(TEXT("light.crystal.east"), FVector(980.0f, -260.0f, 130.0f), FLinearColor(0.86f, 0.22f, 1.0f, 1.0f), 3600.0f, 470.0f));
    Lights.Add(AmbientLight(TEXT("light.crystal.west"), FVector(540.0f, -260.0f, 130.0f), FLinearColor(0.86f, 0.22f, 1.0f, 1.0f), 3600.0f, 470.0f));
    Lights.Add(AmbientLight(TEXT("light.road.one"), FVector(-450.0f, 86.0f, 130.0f), FLinearColor(1.0f, 0.68f, 0.24f, 1.0f), 2200.0f, 300.0f));
    Lights.Add(AmbientLight(TEXT("light.road.two"), FVector(50.0f, 86.0f, 130.0f), FLinearColor(1.0f, 0.68f, 0.24f, 1.0f), 2200.0f, 300.0f));
    Lights.Add(AmbientLight(TEXT("light.road.three"), FVector(550.0f, 86.0f, 130.0f), FLinearColor(1.0f, 0.68f, 0.24f, 1.0f), 2200.0f, 300.0f));
    Lights.Add(AmbientLight(TEXT("light.stormwatch"), FVector(-280.0f, -1180.0f, 330.0f), FLinearColor(0.16f, 0.82f, 1.0f, 1.0f), 5200.0f, 650.0f));

    return Lights;
}
