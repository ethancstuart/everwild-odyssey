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
        const FVector SpawnLocations[] = {
            FVector(460.0f, -40.0f, 96.0f),
            FVector(620.0f, -130.0f, 96.0f),
            FVector(520.0f, -280.0f, 96.0f)
        };

        for (const FVector& SpawnLocation : SpawnLocations)
        {
            if (AEOEnemyCharacter* Enemy = World->SpawnActor<AEOEnemyCharacter>(AEOEnemyCharacter::StaticClass(), SpawnLocation, FRotator::ZeroRotator))
            {
                Enemy->InitializeFromArchetype(RelicWispArchetype);
            }
        }
    }
}
