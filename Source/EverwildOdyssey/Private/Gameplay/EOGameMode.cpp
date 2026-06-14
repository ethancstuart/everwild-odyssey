#include "Gameplay/EOGameMode.h"

#include "Gameplay/EOHeroCharacter.h"
#include "Gameplay/EOPlayerController.h"

AEOGameMode::AEOGameMode()
{
    DefaultPawnClass = AEOHeroCharacter::StaticClass();
    PlayerControllerClass = AEOPlayerController::StaticClass();
}
