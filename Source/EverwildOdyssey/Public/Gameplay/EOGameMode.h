#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EOGameMode.generated.h"

UCLASS()
class EVERWILDODYSSEY_API AEOGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    static constexpr int32 ExpectedOpeningEnemyCount = 3;

    AEOGameMode();

    static TArray<FVector> BuildOpeningEnemySpawnLocations();

protected:
    virtual void BeginPlay() override;
};
