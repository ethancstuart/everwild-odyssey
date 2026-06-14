#if WITH_DEV_AUTOMATION_TESTS

#include "Gameplay/EOAbilityRuntimeComponent.h"
#include "Gameplay/EOCombatStatsComponent.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEOAbilityRuntimeComponentTest, "EverwildOdyssey.Gameplay.AbilityRuntimeComponent", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

namespace
{
FEOAbilityDefinition TestAbility(FName Id, const TCHAR* Name, float CooldownSeconds, float ResourceCost)
{
    FEOAbilityDefinition Ability;
    Ability.AbilityId = Id;
    Ability.DisplayName = FText::FromString(Name);
    Ability.CooldownSeconds = CooldownSeconds;
    Ability.ResourceCost = ResourceCost;
    return Ability;
}
}

bool FEOAbilityRuntimeComponentTest::RunTest(const FString& Parameters)
{
    UEOAbilityRuntimeComponent* Runtime = NewObject<UEOAbilityRuntimeComponent>();
    UEOCombatStatsComponent* CombatStats = NewObject<UEOCombatStatsComponent>();

    FEOCombatStats Stats;
    Stats.MaxResource = 50.0f;
    CombatStats->InitializeFromStats(Stats);

    Runtime->InitializeAbilities({
        TestAbility(TEXT("runeblade.cleave"), TEXT("Rune Cleave"), 6.0f, 20.0f),
        TestAbility(TEXT("runeblade.ultimate"), TEXT("Relic Surge"), 45.0f, 75.0f)
    });

    TestEqual(TEXT("Runtime stores initialized abilities."), Runtime->GetAbilityCount(), 2);

    const EEOAbilityActivationResult FirstActivation = Runtime->TryActivateAbility(TEXT("runeblade.cleave"), CombatStats);
    TestEqual(TEXT("Affordable ability activates."), FirstActivation, EEOAbilityActivationResult::Activated);
    TestEqual(TEXT("Activated ability spends resource."), CombatStats->GetCurrentResource(), 30.0f);
    TestEqual(TEXT("Activated ability starts cooldown."), Runtime->GetRemainingCooldown(TEXT("runeblade.cleave")), 6.0f);

    const EEOAbilityActivationResult CooldownActivation = Runtime->TryActivateAbility(TEXT("runeblade.cleave"), CombatStats);
    TestEqual(TEXT("Cooldown blocks repeat activation."), CooldownActivation, EEOAbilityActivationResult::OnCooldown);

    Runtime->AdvanceCooldowns(99.0f);
    TestEqual(TEXT("Cooldown advance clamps to zero."), Runtime->GetRemainingCooldown(TEXT("runeblade.cleave")), 0.0f);

    const EEOAbilityActivationResult ExpensiveActivation = Runtime->TryActivateAbility(TEXT("runeblade.ultimate"), CombatStats);
    TestEqual(TEXT("Unaffordable ability is rejected."), ExpensiveActivation, EEOAbilityActivationResult::InsufficientResource);

    return true;
}

#endif
