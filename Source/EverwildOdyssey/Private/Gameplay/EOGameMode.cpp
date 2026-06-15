#include "Gameplay/EOGameMode.h"

#include "Gameplay/EOAlphaWorldScaffold.h"
#include "Gameplay/EOEnemyCharacter.h"
#include "Gameplay/EOHUD.h"
#include "Gameplay/EOHeroCharacter.h"
#include "Gameplay/EOPlayerController.h"

namespace
{
FEOEnemyArchetype BuildRelicCasterArchetype()
{
    FEOEnemyArchetype Archetype = AEOEnemyCharacter::BuildRelicWispArchetype();
    Archetype.EnemyId = TEXT("enemy.starfall.relic_caster");
    Archetype.DisplayName = FText::FromString(TEXT("Relic Caster"));
    Archetype.BaseStats.MaxHealth = 36.0f;
    Archetype.BaseStats.MaxResource = 70.0f;
    Archetype.BaseStats.AttackPower = 5.0f;
    Archetype.BaseStats.SpellPower = 20.0f;
    Archetype.BaseStats.Armor = 0.5f;
    Archetype.ExperienceReward = 18;
    Archetype.ThreatRadius = 980.0f;
    Archetype.TintColor = FLinearColor(0.86f, 0.16f, 0.82f, 1.0f);
    Archetype.SilhouetteScale = FVector(0.58f, 0.58f, 1.25f);
    Archetype.ContactDamage = 4.0f;
    Archetype.ContactRange = 150.0f;
    Archetype.AttackCooldownSeconds = 1.6f;
    return Archetype;
}

const FEOEnemyArchetype& SelectOpeningEnemyArchetype(
    const FEOEncounterEnemySpec& EnemySpec,
    const FEOEnemyArchetype& MinionArchetype,
    const FEOEnemyArchetype& CasterArchetype,
    const FEOEnemyArchetype& EliteArchetype)
{
    if (EnemySpec.bElite || EnemySpec.RoleId == TEXT("enemy.role.elite"))
    {
        return EliteArchetype;
    }

    if (EnemySpec.RoleId == TEXT("enemy.role.caster"))
    {
        return CasterArchetype;
    }

    return MinionArchetype;
}
}

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
        const FEOEnemyArchetype RelicCasterArchetype = BuildRelicCasterArchetype();
        const FEOEnemyArchetype RelicSentinelArchetype = AEOEnemyCharacter::BuildRelicSentinelArchetype();
        const TArray<FEOEncounterEnemySpec> EnemySpecs = BuildOpeningEnemySpecs();
        const TArray<FVector> SpawnLocations = BuildOpeningEnemySpawnLocations();

        ensureMsgf(
            EnemySpecs.Num() == ExpectedOpeningEnemyCount,
            TEXT("Opening encounter enemy spec count (%d) must match expected opening enemy count (%d)."),
            EnemySpecs.Num(),
            ExpectedOpeningEnemyCount);

        ensureMsgf(
            SpawnLocations.Num() == EnemySpecs.Num(),
            TEXT("Opening encounter spawn location count (%d) must match enemy spec count (%d)."),
            SpawnLocations.Num(),
            EnemySpecs.Num());

        for (int32 Index = 0; Index < EnemySpecs.Num(); ++Index)
        {
            const FEOEncounterEnemySpec& EnemySpec = EnemySpecs[Index];
            const FVector SpawnLocation = SpawnLocations.IsValidIndex(Index) ? SpawnLocations[Index] : FVector::ZeroVector;
            const FRotator SpawnRotation = (FVector::ZeroVector - SpawnLocation).Rotation();
            FActorSpawnParameters SpawnParameters;
            SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

            if (AEOEnemyCharacter* Enemy = World->SpawnActor<AEOEnemyCharacter>(AEOEnemyCharacter::StaticClass(), SpawnLocation, SpawnRotation, SpawnParameters))
            {
                Enemy->InitializeFromArchetype(SelectOpeningEnemyArchetype(EnemySpec, RelicWispArchetype, RelicCasterArchetype, RelicSentinelArchetype));
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
