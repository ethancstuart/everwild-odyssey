#include "Gameplay/EOGameMode.h"

#include "Gameplay/EOAlphaWorldScaffold.h"
#include "Gameplay/EOHeroCharacter.h"
#include "Gameplay/EOPlayerController.h"

AEOGameMode::AEOGameMode()
{
    DefaultPawnClass = AEOHeroCharacter::StaticClass();
    PlayerControllerClass = AEOPlayerController::StaticClass();
}

void AEOGameMode::BeginPlay()
{
    Super::BeginPlay();

    if (UWorld* World = GetWorld())
    {
        World->SpawnActor<AEOAlphaWorldScaffold>(AEOAlphaWorldScaffold::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
    }
}
