#if WITH_DEV_AUTOMATION_TESTS

#include "Gameplay/EOGameMode.h"
#include "Gameplay/EOHeroCharacter.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEOGameModeTest, "EverwildOdyssey.Gameplay.GameMode", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FEOGameModeTest::RunTest(const FString& Parameters)
{
    const TArray<FEOEncounterEnemySpec> EnemySpecs = AEOGameMode::BuildOpeningEnemySpecs();
    const TMap<FName, FVector> AnchorLocations = AEOGameMode::BuildOpeningEnemyAnchorLocations();
    const TArray<FVector> SpawnLocations = AEOGameMode::BuildOpeningEnemySpawnLocations();

    TestEqual(TEXT("Opening combat exposes the encounter profile enemy count."), SpawnLocations.Num(), 11);
    TestEqual(TEXT("Opening combat exposes expected profile enemy specs."), EnemySpecs.Num(), AEOGameMode::ExpectedOpeningEnemyCount);
    TestEqual(TEXT("Opening combat exposes one spawn per expected enemy."), SpawnLocations.Num(), AEOGameMode::ExpectedOpeningEnemyCount);
    TestTrue(TEXT("Opening encounter has required anchors."), AnchorLocations.Contains(TEXT("anchor.relic_surge.elite")));
    TestTrue(
        TEXT("The first opening enemy starts inside basic attack range."),
        SpawnLocations.Num() > 0 && FVector::DistSquared(FVector::ZeroVector, SpawnLocations[0]) <= FMath::Square(AEOHeroCharacter::DefaultBasicAttackRange));

    for (const FEOEncounterEnemySpec& EnemySpec : EnemySpecs)
    {
        TestTrue(TEXT("Every opening enemy spawn anchor resolves."), AnchorLocations.Contains(EnemySpec.SpawnAnchorId));
    }

    for (const FVector& SpawnLocation : SpawnLocations)
    {
        TestTrue(TEXT("Opening enemy spawns are above the floor."), SpawnLocation.Z >= 80.0f);
    }

    for (int32 LeftIndex = 0; LeftIndex < SpawnLocations.Num(); ++LeftIndex)
    {
        for (int32 RightIndex = LeftIndex + 1; RightIndex < SpawnLocations.Num(); ++RightIndex)
        {
            TestFalse(
                TEXT("Opening enemy spawns do not share exact duplicate locations."),
                SpawnLocations[LeftIndex].Equals(SpawnLocations[RightIndex], 0.0f));
        }
    }

    TestTrue(TEXT("Opening encounter reads as a zone event."), AEOGameMode::ExpectedOpeningEnemyCount >= 10);

    return true;
}

#endif
