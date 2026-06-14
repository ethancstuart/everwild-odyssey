#if WITH_DEV_AUTOMATION_TESTS

#include "Gameplay/EOHeroCharacter.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEOPlayableFoundationTest, "EverwildOdyssey.Gameplay.PlayableFoundation", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FEOPlayableFoundationTest::RunTest(const FString& Parameters)
{
    TestEqual(TEXT("Default camera boom creates an adventure MMO read."), AEOHeroCharacter::DefaultCameraBoomLength, 960.0f);
    TestEqual(TEXT("Default camera pitch keeps the hero and horizon readable."), AEOHeroCharacter::DefaultCameraPitch, -48.0f);
    TestEqual(TEXT("Default camera yaw gives diagonal movement readability."), AEOHeroCharacter::DefaultCameraYaw, -42.0f);

    return true;
}

#endif
