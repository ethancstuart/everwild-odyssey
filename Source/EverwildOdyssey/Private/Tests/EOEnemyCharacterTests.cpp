#if WITH_DEV_AUTOMATION_TESTS

#include "Gameplay/EOEnemyCharacter.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEOEnemyCharacterTest, "EverwildOdyssey.Gameplay.EnemyCharacter", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FEOEnemyCharacterTest::RunTest(const FString& Parameters)
{
    const FEOEnemyArchetype RelicWisp = AEOEnemyCharacter::BuildRelicWispArchetype();

    TestTrue(TEXT("Relic Wisp archetype is valid."), RelicWisp.IsValidForAlpha());
    TestEqual(TEXT("Relic Wisp has stable id."), RelicWisp.EnemyId, FName(TEXT("enemy.starfall.relic_wisp")));
    TestTrue(TEXT("Relic Wisp rewards early progress."), RelicWisp.ExperienceReward >= 10);
    TestTrue(TEXT("Relic Wisp has readable threat radius."), RelicWisp.ThreatRadius >= 600.0f);
    TestTrue(TEXT("Relic Wisp can take damage."), RelicWisp.BaseStats.MaxHealth > 0.0f);

    return true;
}

#endif
