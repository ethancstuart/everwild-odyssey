#include "Gameplay/EOAlphaWorldScaffold.h"

#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Gameplay/EOEncounterDirectorComponent.h"
#include "UObject/ConstructorHelpers.h"

namespace
{
FEOAlphaLandmarkSpec Landmark(FName Id, const TCHAR* Name, const FVector& Location, const FVector& Scale)
{
    FEOAlphaLandmarkSpec Spec;
    Spec.LandmarkId = Id;
    Spec.DisplayName = FText::FromString(Name);
    Spec.Location = Location;
    Spec.Scale = Scale;
    return Spec;
}

void ConfigureLandmark(UStaticMeshComponent* Component, const FEOAlphaLandmarkSpec& Spec, UStaticMesh* Mesh)
{
    Component->SetStaticMesh(Mesh);
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

    const TArray<FEOAlphaLandmarkSpec> Landmarks = BuildDefaultLandmarks();
    if (Landmarks.Num() == ExpectedLandmarkCount)
    {
        ConfigureLandmark(DawnwatchKeep, Landmarks[0], CubeMesh.Object);
        ConfigureLandmark(StarfallGate, Landmarks[1], ConeMesh.Object);
        ConfigureLandmark(TrainingRing, Landmarks[2], CylinderMesh.Object);
        ConfigureLandmark(RelicSurge, Landmarks[3], SphereMesh.Object);
        ConfigureLandmark(ValeRoad, Landmarks[4], CubeMesh.Object);
        ConfigureLandmark(SkybridgeHub, Landmarks[5], CylinderMesh.Object);
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

    Landmarks.Add(Landmark(TEXT("dawnwatch.keep"), TEXT("Dawnwatch Keep"), FVector(-520.0f, -300.0f, 70.0f), FVector(4.0f, 3.0f, 1.4f)));
    Landmarks.Add(Landmark(TEXT("starfall.gate"), TEXT("Starfall Gate"), FVector(260.0f, 120.0f, 170.0f), FVector(0.9f, 0.9f, 3.4f)));
    Landmarks.Add(Landmark(TEXT("dawnwatch.training_ring"), TEXT("Dawnwatch Training Ring"), FVector(-220.0f, 420.0f, 24.0f), FVector(2.8f, 2.8f, 0.18f)));
    Landmarks.Add(Landmark(TEXT("starfall.relic_surge"), TEXT("Relic Surge"), FVector(560.0f, -140.0f, 135.0f), FVector(1.25f, 1.25f, 2.7f)));
    Landmarks.Add(Landmark(TEXT("vale.road"), TEXT("Starfall Vale Road"), FVector(0.0f, 0.0f, 8.0f), FVector(11.0f, 0.35f, 0.08f)));
    Landmarks.Add(Landmark(TEXT("skybridge.hub"), TEXT("Skybridge Hub"), FVector(0.0f, -650.0f, 64.0f), FVector(2.6f, 2.6f, 0.35f)));

    return Landmarks;
}
