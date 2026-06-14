#include "Gameplay/EOGameMode.h"

#include "Gameplay/EOAlphaWorldScaffold.h"
#include "Gameplay/EOEnemyCharacter.h"
#include "Gameplay/EOHUD.h"
#include "Gameplay/EOHeroCharacter.h"
#include "Gameplay/EOPlayerController.h"

AEOGameMode::AEOGameMode()
{
    DefaultPawnClass = AEOHeroCharacter::StaticClass();
    PlayerControllerClass = AEOPlayerController::StaticClass();
    HUDClass = AEOHUD::StaticClass();
}

void AEOGameMode::BeginPlay()
{
    Super::BeginPlay();

    if (UWorld* World = GetWorld())
    {
        World->SpawnActor<AEOAlphaWorldScaffold>(AEOAlphaWorldScaffold::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);

        const FEOEnemyArchetype RelicWispArchetype = AEOEnemyCharacter::BuildRelicWispArchetype();
        const TArray<FVector> SpawnLocations = BuildOpeningEnemySpawnLocations();

        for (const FVector& SpawnLocation : SpawnLocations)
        {
            if (AEOEnemyCharacter* Enemy = World->SpawnActor<AEOEnemyCharacter>(AEOEnemyCharacter::StaticClass(), SpawnLocation, FRotator::ZeroRotator))
            {
                Enemy->InitializeFromArchetype(RelicWispArchetype);
            }
        }
    }
}

TArray<FVector> AEOGameMode::BuildOpeningEnemySpawnLocations()
{
    TArray<FVector> SpawnLocations;
    SpawnLocations.Reserve(ExpectedOpeningEnemyCount);

    SpawnLocations.Add(FVector(210.0f, -35.0f, 96.0f));
    SpawnLocations.Add(FVector(340.0f, -120.0f, 96.0f));
    SpawnLocations.Add(FVector(500.0f, -220.0f, 96.0f));

    return SpawnLocations;
}
