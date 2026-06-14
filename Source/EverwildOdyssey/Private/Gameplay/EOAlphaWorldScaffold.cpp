#include "Gameplay/EOAlphaWorldScaffold.h"

#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Gameplay/EOEncounterDirectorComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"

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

void ConfigureLandmark(UStaticMeshComponent* Component, const FEOAlphaLandmarkSpec& Spec, UStaticMesh* Mesh, UMaterialInterface* BaseMaterial)
{
    Component->SetStaticMesh(Mesh);
    if (BaseMaterial != nullptr)
    {
        UMaterialInstanceDynamic* TintedMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, Component);
        TintedMaterial->SetVectorParameterValue(TEXT("Color"), Spec.TintColor);
        Component->SetMaterial(0, TintedMaterial);
    }

    Component->SetRelativeLocation(Spec.Location);
    Component->SetRelativeScale3D(Spec.Scale);
    Component->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}
}

AEOAlphaWorldScaffold::AEOAlphaWorldScaffold()
{
    PrimaryActorTick.bCanEverTick = false;

    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("AlphaWorldRoot"));
    RootComponent = SceneRoot;

    EncounterDirector = CreateDefaultSubobject<UEOEncounterDirectorComponent>(TEXT("EncounterDirector"));

    DawnwatchKeep = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DawnwatchKeep"));
    DawnwatchKeep->SetupAttachment(SceneRoot);

    StarfallGate = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StarfallGate"));
    StarfallGate->SetupAttachment(SceneRoot);

    TrainingRing = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TrainingRing"));
    TrainingRing->SetupAttachment(SceneRoot);

    RelicSurge = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RelicSurge"));
    RelicSurge->SetupAttachment(SceneRoot);

    ValeRoad = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ValeRoad"));
    ValeRoad->SetupAttachment(SceneRoot);

    SkybridgeHub = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SkybridgeHub"));
    SkybridgeHub->SetupAttachment(SceneRoot);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> ConeMesh(TEXT("/Engine/BasicShapes/Cone.Cone"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderMesh(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> BasicShapeMaterial(TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));

    const TArray<FEOAlphaLandmarkSpec> Landmarks = BuildDefaultLandmarks();
    if (Landmarks.Num() == ExpectedLandmarkCount)
    {
        ConfigureLandmark(DawnwatchKeep, Landmarks[0], CubeMesh.Object, BasicShapeMaterial.Object);
        ConfigureLandmark(StarfallGate, Landmarks[1], ConeMesh.Object, BasicShapeMaterial.Object);
        ConfigureLandmark(TrainingRing, Landmarks[2], CylinderMesh.Object, BasicShapeMaterial.Object);
        ConfigureLandmark(RelicSurge, Landmarks[3], SphereMesh.Object, BasicShapeMaterial.Object);
        ConfigureLandmark(ValeRoad, Landmarks[4], CubeMesh.Object, BasicShapeMaterial.Object);
        ConfigureLandmark(SkybridgeHub, Landmarks[5], CylinderMesh.Object, BasicShapeMaterial.Object);
    }
}

void AEOAlphaWorldScaffold::BeginPlay()
{
    Super::BeginPlay();

    FEOEncounterRecord RelicSurgeEncounter;
    RelicSurgeEncounter.EncounterId = TEXT("encounter.relic_surge.wave_one");
    RelicSurgeEncounter.DisplayName = FText::FromString(TEXT("Relic Surge: First Wave"));
    RelicSurgeEncounter.LinkedWorldEventId = TEXT("starfall.relic_surge");
    RelicSurgeEncounter.State = EEOEncounterState::Dormant;
    RelicSurgeEncounter.TotalEnemies = 3;
    RelicSurgeEncounter.RecommendedLevel = 1;

    EncounterDirector->InitializeEncounters({ RelicSurgeEncounter });
}

TArray<FEOAlphaLandmarkSpec> AEOAlphaWorldScaffold::BuildDefaultLandmarks()
{
    TArray<FEOAlphaLandmarkSpec> Landmarks;
    Landmarks.Reserve(ExpectedLandmarkCount);

    Landmarks.Add(Landmark(TEXT("dawnwatch.keep"), TEXT("Dawnwatch Keep"), FVector(-520.0f, -300.0f, 70.0f), FVector(4.0f, 3.0f, 1.4f), FLinearColor(0.08f, 0.18f, 0.78f, 1.0f)));
    Landmarks.Add(Landmark(TEXT("starfall.gate"), TEXT("Starfall Gate"), FVector(260.0f, 120.0f, 170.0f), FVector(0.9f, 0.9f, 3.4f), FLinearColor(1.0f, 0.72f, 0.18f, 1.0f)));
    Landmarks.Add(Landmark(TEXT("dawnwatch.training_ring"), TEXT("Dawnwatch Training Ring"), FVector(-220.0f, 420.0f, 24.0f), FVector(2.8f, 2.8f, 0.18f), FLinearColor(0.12f, 0.62f, 0.24f, 1.0f)));
    Landmarks.Add(Landmark(TEXT("starfall.relic_surge"), TEXT("Relic Surge"), FVector(560.0f, -140.0f, 135.0f), FVector(1.25f, 1.25f, 2.7f), FLinearColor(0.75f, 0.20f, 1.0f, 1.0f)));
    Landmarks.Add(Landmark(TEXT("vale.road"), TEXT("Starfall Vale Road"), FVector(0.0f, 0.0f, 8.0f), FVector(11.0f, 0.35f, 0.08f), FLinearColor(0.18f, 0.17f, 0.15f, 1.0f)));
    Landmarks.Add(Landmark(TEXT("skybridge.hub"), TEXT("Skybridge Hub"), FVector(0.0f, -650.0f, 64.0f), FVector(2.6f, 2.6f, 0.35f), FLinearColor(0.06f, 0.62f, 0.72f, 1.0f)));

    return Landmarks;
}
