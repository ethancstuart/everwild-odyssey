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
        const TArray<FEOEncounterEnemySpec> EnemySpecs = BuildOpeningEnemySpecs();
        const TArray<FVector> SpawnLocations = BuildOpeningEnemySpawnLocations();

        for (int32 Index = 0; Index < EnemySpecs.Num() && Index < SpawnLocations.Num(); ++Index)
        {
            const FEOEncounterEnemySpec& EnemySpec = EnemySpecs[Index];
            const FVector SpawnLocation = SpawnLocations[Index];
            const FRotator SpawnRotation = (FVector::ZeroVector - SpawnLocation).Rotation();
            FActorSpawnParameters SpawnParameters;
            SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

            if (AEOEnemyCharacter* Enemy = World->SpawnActor<AEOEnemyCharacter>(AEOEnemyCharacter::StaticClass(), SpawnLocation, SpawnRotation, SpawnParameters))
            {
                const bool bUseSentinel = EnemySpec.bElite || EnemySpec.RoleId == TEXT("enemy.role.elite");
                Enemy->InitializeFromArchetype(bUseSentinel ? RelicSentinelArchetype : RelicWispArchetype);
            }
        }
    }
}

TArray<FEOEncounterEnemySpec> AEOGameMode::BuildOpeningEnemySpecs()
{
    const FEOEncounterProfile Profile = FEOEncounterProfileCatalog::BuildRelicSurgeProfile();
    TArray<FEOEncounterEnemySpec> EnemySpecs;
    EnemySpecs.Reserve(ExpectedOpeningEnemyCount);

    for (const FEOEncounterPhaseSpec& PhaseSpec : Profile.Phases)
    {
        EnemySpecs.Append(PhaseSpec.Enemies);
    }

    return EnemySpecs;
}

TMap<FName, FVector> AEOGameMode::BuildOpeningEnemyAnchorLocations()
{
    TMap<FName, FVector> AnchorLocations;
    AnchorLocations.Add(TEXT("anchor.road.first_contact"), FVector(205.0f, -34.0f, 96.0f));
    AnchorLocations.Add(TEXT("anchor.relic_surge.entry"), FVector(520.0f, -235.0f, 96.0f));
    AnchorLocations.Add(TEXT("anchor.relic_surge.center"), FVector(760.0f, -260.0f, 96.0f));
    AnchorLocations.Add(TEXT("anchor.relic_surge.caster_north"), FVector(760.0f, -70.0f, 96.0f));
    AnchorLocations.Add(TEXT("anchor.relic_surge.caster_east"), FVector(970.0f, -260.0f, 96.0f));
    AnchorLocations.Add(TEXT("anchor.relic_surge.elite"), FVector(840.0f, -310.0f, 96.0f));
    return AnchorLocations;
}

TArray<FVector> AEOGameMode::BuildOpeningEnemySpawnLocations()
{
    return FEOEncounterProfileCatalog::BuildRelicSurgeProfile()
        .BuildSpawnLocationsFromAnchors(BuildOpeningEnemyAnchorLocations());
}
