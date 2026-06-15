#if WITH_DEV_AUTOMATION_TESTS

#include "Gameplay/EOHeroCharacter.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEOPlayableFoundationTest, "EverwildOdyssey.Gameplay.PlayableFoundation", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FEOPlayableFoundationTest::RunTest(const FString& Parameters)
{
    TestTrue(TEXT("Default camera boom supports heroic third-person readability."), AEOHeroCharacter::DefaultCameraBoomLength <= 720.0f);
    TestTrue(TEXT("Default camera pitch keeps hero and horizon readable."), AEOHeroCharacter::DefaultCameraPitch >= -28.0f && AEOHeroCharacter::DefaultCameraPitch <= -16.0f);
    TestEqual(TEXT("Default camera yaw gives diagonal movement readability."), AEOHeroCharacter::DefaultCameraYaw, -35.0f);
    TestTrue(TEXT("Default basic attack range supports generous controller melee."), AEOHeroCharacter::DefaultBasicAttackRange >= 280.0f);
    TestTrue(TEXT("Default ability range supports readable MMO-style pulls."), AEOHeroCharacter::DefaultAbilityAttackRange >= 460.0f);

    return true;
}

#endif
