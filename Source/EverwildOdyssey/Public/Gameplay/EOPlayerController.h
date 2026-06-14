#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EOPlayerController.generated.h"

UCLASS()
class EVERWILDODYSSEY_API AEOPlayerController : public APlayerController
{
    GENERATED_BODY()

protected:
    virtual void BeginPlay() override;
};
