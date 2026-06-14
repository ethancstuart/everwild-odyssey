#if WITH_DEV_AUTOMATION_TESTS

#include "Gameplay/EOHeroCharacter.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEOPlayableFoundationTest, "EverwildOdyssey.Gameplay.PlayableFoundation", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FEOPlayableFoundationTest::RunTest(const FString& Parameters)
{
    TestEqual(TEXT("Default camera boom creates an isometric read."), AEOHeroCharacter::DefaultCameraBoomLength, 1200.0f);
    TestEqual(TEXT("Default camera pitch looks down at the playfield."), AEOHeroCharacter::DefaultCameraPitch, -60.0f);
    TestEqual(TEXT("Default camera yaw gives diagonal movement readability."), AEOHeroCharacter::DefaultCameraYaw, -45.0f);

    return true;
}

#endif
