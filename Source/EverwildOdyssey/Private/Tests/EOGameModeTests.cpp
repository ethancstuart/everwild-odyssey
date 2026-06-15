#if WITH_DEV_AUTOMATION_TESTS

#include "Gameplay/EOGameMode.h"
#include "Gameplay/EOHeroCharacter.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEOGameModeTest, "EverwildOdyssey.Gameplay.GameMode", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FEOGameModeTest::RunTest(const FString& Parameters)
{
    const TArray<FVector> SpawnLocations = AEOGameMode::BuildOpeningEnemySpawnLocations();

    TestEqual(TEXT("Opening combat exposes the encounter profile enemy count."), SpawnLocations.Num(), 11);
    TestEqual(TEXT("Opening combat exposes expected profile enemy specs."), AEOGameMode::BuildOpeningEnemySpecs().Num(), AEOGameMode::ExpectedOpeningEnemyCount);
    TestTrue(TEXT("Opening encounter has required anchors."), AEOGameMode::BuildOpeningEnemyAnchorLocations().Contains(TEXT("anchor.relic_surge.elite")));
    TestTrue(
        TEXT("The first opening enemy starts inside basic attack range."),
        SpawnLocations.Num() > 0 && FVector::DistSquared(FVector::ZeroVector, SpawnLocations[0]) <= FMath::Square(AEOHeroCharacter::DefaultBasicAttackRange));

    for (const FVector& SpawnLocation : SpawnLocations)
    {
        TestTrue(TEXT("Opening enemy spawns are above the floor."), SpawnLocation.Z >= 80.0f);
    }

    TestTrue(TEXT("Opening encounter reads as a zone event."), AEOGameMode::ExpectedOpeningEnemyCount >= 10);

    return true;
}

#endif
