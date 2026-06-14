#if WITH_DEV_AUTOMATION_TESTS

#include "Gameplay/EOCompanionRosterComponent.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEOCompanionRosterComponentTest, "EverwildOdyssey.Gameplay.CompanionRosterComponent", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

namespace
{
FEOCompanionProfile TestCompanion(FName Id, const TCHAR* Name, EEOPartyRole Role)
{
    FEOCompanionProfile Companion;
    Companion.CompanionId = Id;
    Companion.DisplayName = FText::FromString(Name);
    Companion.Role = Role;
    Companion.FollowDistance = 320.0f;
    Companion.AbilityCheckInterval = 1.0f;
    return Companion;
}
}

bool FEOCompanionRosterComponentTest::RunTest(const FString& Parameters)
{
    UEOCompanionRosterComponent* Roster = NewObject<UEOCompanionRosterComponent>();

    const FEOCompanionProfile Mira = TestCompanion(TEXT("companion.mira"), TEXT("Mira of the Dawnwatch"), EEOPartyRole::Support);
    const FEOCompanionProfile Tor = TestCompanion(TEXT("companion.tor"), TEXT("Tor Valeguard"), EEOPartyRole::Defender);
    const FEOCompanionProfile InvalidCompanion;

    TestEqual(TEXT("Valid companion can be added."), Roster->AddCompanion(Mira), EEOCompanionRosterResult::Applied);
    TestEqual(TEXT("Second companion can be added."), Roster->AddCompanion(Tor), EEOCompanionRosterResult::Applied);
    TestEqual(TEXT("Invalid companion is rejected."), Roster->AddCompanion(InvalidCompanion), EEOCompanionRosterResult::InvalidCompanion);
    TestEqual(TEXT("Duplicate companion is rejected."), Roster->AddCompanion(Mira), EEOCompanionRosterResult::DuplicateCompanion);
    TestEqual(TEXT("Roster stores valid companions."), Roster->GetCompanionCount(), 2);

    TestEqual(TEXT("Companion can activate."), Roster->ActivateCompanion(TEXT("companion.mira")), EEOCompanionRosterResult::Applied);
    TestEqual(TEXT("Second companion can activate."), Roster->ActivateCompanion(TEXT("companion.tor")), EEOCompanionRosterResult::Applied);
    TestEqual(TEXT("Active companion count is tracked."), Roster->GetActiveCompanionCount(), 2);
    TestTrue(TEXT("Support role is active."), Roster->HasActiveRole(EEOPartyRole::Support));

    TestEqual(TEXT("Active cap blocks extra activation."), Roster->ActivateCompanion(TEXT("companion.mira")), EEOCompanionRosterResult::AlreadyActive);
    TestEqual(TEXT("Companion can deactivate."), Roster->DeactivateCompanion(TEXT("companion.mira")), EEOCompanionRosterResult::Applied);
    TestEqual(TEXT("Active count decreases."), Roster->GetActiveCompanionCount(), 1);

    return true;
}

#endif
