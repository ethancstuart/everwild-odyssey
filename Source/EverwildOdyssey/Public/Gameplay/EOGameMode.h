#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EOGameMode.generated.h"

UCLASS()
class EVERWILDODYSSEY_API AEOGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AEOGameMode();

protected:
    virtual void BeginPlay() override;
};
