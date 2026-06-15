#pragma once

#include "CoreMinimal.h"
#include "Encounters/EOEncounterProfile.h"
#include "GameFramework/GameModeBase.h"
#include "EOGameMode.generated.h"

UCLASS()
class EVERWILDODYSSEY_API AEOGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    static constexpr int32 ExpectedOpeningEnemyCount = 11;

    AEOGameMode();

    static TArray<FEOEncounterEnemySpec> BuildOpeningEnemySpecs();
    static TMap<FName, FVector> BuildOpeningEnemyAnchorLocations();
    static TArray<FVector> BuildOpeningEnemySpawnLocations();

protected:
    virtual void BeginPlay() override;
};
