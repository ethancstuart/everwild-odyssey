#include "Gameplay/EOPlayerController.h"

void AEOPlayerController::BeginPlay()
{
    Super::BeginPlay();

    bShowMouseCursor = false;

    FInputModeGameOnly InputMode;
    SetInputMode(InputMode);
}
