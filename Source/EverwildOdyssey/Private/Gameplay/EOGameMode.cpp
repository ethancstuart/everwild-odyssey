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
        const FEOEnemyArchetype RelicSentinelArchetype = AEOEnemyCharacter::BuildRelicSentinelArchetype();
        const TArray<FVector> SpawnLocations = BuildOpeningEnemySpawnLocations();

        for (int32 Index = 0; Index < SpawnLocations.Num(); ++Index)
        {
            const FVector SpawnLocation = SpawnLocations[Index];
            const FRotator SpawnRotation = (FVector::ZeroVector - SpawnLocation).Rotation();
            FActorSpawnParameters SpawnParameters;
            SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

            if (AEOEnemyCharacter* Enemy = World->SpawnActor<AEOEnemyCharacter>(AEOEnemyCharacter::StaticClass(), SpawnLocation, SpawnRotation, SpawnParameters))
            {
                Enemy->InitializeFromArchetype((Index == 4 || Index == 8 || Index == 11) ? RelicSentinelArchetype : RelicWispArchetype);
            }
        }
    }
}

TArray<FVector> AEOGameMode::BuildOpeningEnemySpawnLocations()
{
    TArray<FVector> SpawnLocations;
    SpawnLocations.Reserve(ExpectedOpeningEnemyCount);

    SpawnLocations.Add(FVector(205.0f, -34.0f, 96.0f));
    SpawnLocations.Add(FVector(390.0f, -95.0f, 96.0f));
    SpawnLocations.Add(FVector(520.0f, -235.0f, 96.0f));
    SpawnLocations.Add(FVector(705.0f, -120.0f, 96.0f));
    SpawnLocations.Add(FVector(835.0f, -250.0f, 96.0f));
    SpawnLocations.Add(FVector(690.0f, -420.0f, 96.0f));
    SpawnLocations.Add(FVector(890.0f, -70.0f, 96.0f));
    SpawnLocations.Add(FVector(1030.0f, -280.0f, 96.0f));
    SpawnLocations.Add(FVector(770.0f, -610.0f, 96.0f));
    SpawnLocations.Add(FVector(1090.0f, -490.0f, 96.0f));
    SpawnLocations.Add(FVector(590.0f, -560.0f, 96.0f));
    SpawnLocations.Add(FVector(980.0f, -650.0f, 96.0f));

    return SpawnLocations;
}
