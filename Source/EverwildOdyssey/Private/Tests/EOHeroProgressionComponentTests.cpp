#if WITH_DEV_AUTOMATION_TESTS

#include "Gameplay/EOHeroProgressionComponent.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEOHeroProgressionComponentTest, "EverwildOdyssey.Gameplay.HeroProgressionComponent", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FEOHeroProgressionComponentTest::RunTest(const FString& Parameters)
{
    UEOHeroProgressionComponent* Progression = NewObject<UEOHeroProgressionComponent>();

    TestEqual(TEXT("Hero starts at level one."), Progression->GetHeroLevel(), 1);
    TestEqual(TEXT("Hero starts with no experience."), Progression->GetExperience(), 0);
    TestEqual(TEXT("First level threshold is one hundred."), Progression->GetExperienceForNextLevel(), 100);

    TestEqual(TEXT("Positive experience is applied."), Progression->AddExperience(60), EEOProgressionResult::Applied);
    TestEqual(TEXT("Experience accumulates before level up."), Progression->GetExperience(), 60);
    TestEqual(TEXT("Hero remains level one before threshold."), Progression->GetHeroLevel(), 1);

    TestEqual(TEXT("Experience can level up with carryover."), Progression->AddExperience(50), EEOProgressionResult::LeveledUp);
    TestEqual(TEXT("Hero reaches level two."), Progression->GetHeroLevel(), 2);
    TestEqual(TEXT("Carryover experience is retained."), Progression->GetExperience(), 10);
    TestEqual(TEXT("Second level threshold scales."), Progression->GetExperienceForNextLevel(), 200);

    TestEqual(TEXT("Invalid experience is rejected."), Progression->AddExperience(-5), EEOProgressionResult::InvalidExperience);

    return true;
}

#endif
