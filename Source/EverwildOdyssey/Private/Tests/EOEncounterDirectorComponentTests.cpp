#if WITH_DEV_AUTOMATION_TESTS

#include "Gameplay/EOEncounterDirectorComponent.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEOEncounterDirectorComponentTest, "EverwildOdyssey.Gameplay.EncounterDirectorComponent", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FEOEncounterDirectorComponentTest::RunTest(const FString& Parameters)
{
    UEOEncounterDirectorComponent* Director = NewObject<UEOEncounterDirectorComponent>();

    FEOEncounterRecord RelicSurge;
    RelicSurge.EncounterId = TEXT("encounter.relic_surge.wave_one");
    RelicSurge.DisplayName = FText::FromString(TEXT("Relic Surge: First Wave"));
    RelicSurge.LinkedWorldEventId = TEXT("starfall.relic_surge");
    RelicSurge.TotalEnemies = 3;
    RelicSurge.RecommendedLevel = 1;

    Director->InitializeEncounters({ RelicSurge });

    TestEqual(TEXT("Director stores initialized encounter."), Director->GetEncounterCount(), 1);
    TestEqual(TEXT("Encounter starts dormant."), Director->GetEncounterState(TEXT("encounter.relic_surge.wave_one")), EEOEncounterState::Dormant);

    TestEqual(TEXT("Dormant encounter can start."), Director->StartEncounter(TEXT("encounter.relic_surge.wave_one")), EEOEncounterActionResult::Applied);
    TestEqual(TEXT("Started encounter is active."), Director->GetEncounterState(TEXT("encounter.relic_surge.wave_one")), EEOEncounterState::Active);
    TestEqual(TEXT("Started encounter fills remaining enemies."), Director->GetRemainingEnemies(TEXT("encounter.relic_surge.wave_one")), 3);

    TestEqual(TEXT("Enemy defeat advances encounter."), Director->RegisterEnemyDefeated(TEXT("encounter.relic_surge.wave_one")), EEOEncounterActionResult::Applied);
    TestEqual(TEXT("Remaining enemies decreases."), Director->GetRemainingEnemies(TEXT("encounter.relic_surge.wave_one")), 2);

    TestEqual(TEXT("Final defeats clear encounter."), Director->RegisterEnemyDefeated(TEXT("encounter.relic_surge.wave_one"), 5), EEOEncounterActionResult::Applied);
    TestEqual(TEXT("Encounter records cleared state."), Director->GetEncounterState(TEXT("encounter.relic_surge.wave_one")), EEOEncounterState::Cleared);
    TestEqual(TEXT("Cleared encounter has no remaining enemies."), Director->GetRemainingEnemies(TEXT("encounter.relic_surge.wave_one")), 0);

    TestEqual(TEXT("Cleared encounter cannot restart without reset."), Director->StartEncounter(TEXT("encounter.relic_surge.wave_one")), EEOEncounterActionResult::AlreadyCleared);
    TestEqual(TEXT("Encounter can reset."), Director->ResetEncounter(TEXT("encounter.relic_surge.wave_one")), EEOEncounterActionResult::Applied);
    TestEqual(TEXT("Reset encounter becomes dormant."), Director->GetEncounterState(TEXT("encounter.relic_surge.wave_one")), EEOEncounterState::Dormant);
    TestEqual(TEXT("Reset encounter restores remaining enemies."), Director->GetRemainingEnemies(TEXT("encounter.relic_surge.wave_one")), 3);

    return true;
}

#endif
