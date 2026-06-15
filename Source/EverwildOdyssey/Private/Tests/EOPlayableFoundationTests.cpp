#if WITH_DEV_AUTOMATION_TESTS

#include "Gameplay/EOHeroCharacter.h"
#include "GameFramework/InputSettings.h"
#include "InputCoreTypes.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEOPlayableFoundationTest, "EverwildOdyssey.Gameplay.PlayableFoundation", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FEOPlayableFoundationTest::RunTest(const FString& Parameters)
{
    const auto CountAxisMappings = [](const FName AxisName, const FKey& Key)
    {
        const UInputSettings* InputSettings = GetDefault<UInputSettings>();
        TArray<FInputAxisKeyMapping> Mappings;
        InputSettings->GetAxisMappingByName(AxisName, Mappings);

        int32 MatchCount = 0;
        for (const FInputAxisKeyMapping& Mapping : Mappings)
        {
            if (Mapping.Key == Key)
            {
                ++MatchCount;
            }
        }

        return MatchCount;
    };

    TestTrue(TEXT("Default camera boom supports heroic third-person readability."), AEOHeroCharacter::DefaultCameraBoomLength <= 720.0f);
    TestTrue(TEXT("Default camera pitch keeps hero and horizon readable."), AEOHeroCharacter::DefaultCameraPitch >= AEOHeroCharacter::MinCameraPitch && AEOHeroCharacter::DefaultCameraPitch <= AEOHeroCharacter::MaxCameraPitch);
    TestEqual(TEXT("Default camera yaw gives diagonal movement readability."), AEOHeroCharacter::DefaultCameraYaw, -35.0f);
    TestTrue(TEXT("Camera pitch clamp keeps the horizon within heroic readability."), AEOHeroCharacter::MinCameraPitch >= -28.0f && AEOHeroCharacter::MaxCameraPitch <= -16.0f);
    TestTrue(TEXT("Camera pitch clamp has a usable pitch band."), AEOHeroCharacter::MinCameraPitch < AEOHeroCharacter::MaxCameraPitch);
    TestTrue(TEXT("Gamepad yaw rate is positive and controlled."), AEOHeroCharacter::GamepadCameraYawRate > 0.0f && AEOHeroCharacter::GamepadCameraYawRate <= 180.0f);
    TestTrue(TEXT("Gamepad pitch rate is positive and controlled."), AEOHeroCharacter::GamepadCameraPitchRate > 0.0f && AEOHeroCharacter::GamepadCameraPitchRate <= 120.0f);
    TestTrue(TEXT("Default basic attack range supports generous controller melee."), AEOHeroCharacter::DefaultBasicAttackRange >= 280.0f);
    TestTrue(TEXT("Default ability range supports readable MMO-style pulls."), AEOHeroCharacter::DefaultAbilityAttackRange >= 460.0f);
    TestEqual(TEXT("Mouse X uses immediate camera yaw input."), CountAxisMappings(TEXT("CameraYaw"), EKeys::MouseX), 1);
    TestEqual(TEXT("Mouse Y uses immediate camera pitch input."), CountAxisMappings(TEXT("CameraPitch"), EKeys::MouseY), 1);
    TestEqual(TEXT("Gamepad right X uses frame-scaled camera yaw rate input."), CountAxisMappings(TEXT("CameraYawRate"), EKeys::Gamepad_RightX), 1);
    TestEqual(TEXT("Gamepad right Y uses frame-scaled camera pitch rate input."), CountAxisMappings(TEXT("CameraPitchRate"), EKeys::Gamepad_RightY), 1);
    TestEqual(TEXT("Gamepad right X is not mapped to immediate camera yaw."), CountAxisMappings(TEXT("CameraYaw"), EKeys::Gamepad_RightX), 0);
    TestEqual(TEXT("Gamepad right Y is not mapped to immediate camera pitch."), CountAxisMappings(TEXT("CameraPitch"), EKeys::Gamepad_RightY), 0);

    return true;
}

#endif
