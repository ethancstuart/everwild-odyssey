#if WITH_DEV_AUTOMATION_TESTS

#include "Gameplay/EOQuestLogComponent.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEOQuestLogComponentTest, "EverwildOdyssey.Gameplay.QuestLogComponent", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

namespace
{
FEOQuestRecord TestQuest(FName Id, const TCHAR* Name, EEOQuestState State)
{
    FEOQuestRecord Quest;
    Quest.QuestId = Id;
    Quest.DisplayName = FText::FromString(Name);
    Quest.State = State;
    return Quest;
}

FEOWorldEventRecord TestWorldEvent(FName Id, const TCHAR* Name, EEOWorldEventState State)
{
    FEOWorldEventRecord Event;
    Event.EventId = Id;
    Event.DisplayName = FText::FromString(Name);
    Event.State = State;
    Event.RespawnSeconds = 120.0f;
    return Event;
}
}

bool FEOQuestLogComponentTest::RunTest(const FString& Parameters)
{
    UEOQuestLogComponent* QuestLog = NewObject<UEOQuestLogComponent>();

    QuestLog->InitializeQuests({
        TestQuest(TEXT("dawnwatch.starfall_arrival"), TEXT("The Starfall Gate"), EEOQuestState::Available),
        TestQuest(TEXT("dawnwatch.locked_depths"), TEXT("The Locked Depths"), EEOQuestState::Locked)
    });

    QuestLog->InitializeWorldEvents({
        TestWorldEvent(TEXT("starfall.relic_surge"), TEXT("Relic Surge"), EEOWorldEventState::Warning)
    });

    TestEqual(TEXT("Quest log stores initialized quests."), QuestLog->GetQuestCount(), 2);
    TestEqual(TEXT("World-event log stores initialized events."), QuestLog->GetWorldEventCount(), 1);

    TestEqual(TEXT("Available quest can start."), QuestLog->StartQuest(TEXT("dawnwatch.starfall_arrival")), EEOQuestActionResult::Applied);
    TestEqual(TEXT("Started quest becomes active."), QuestLog->GetQuestState(TEXT("dawnwatch.starfall_arrival")), EEOQuestState::Active);

    TestEqual(TEXT("Active quest advances."), QuestLog->AdvanceQuest(TEXT("dawnwatch.starfall_arrival")), EEOQuestActionResult::Applied);
    TestEqual(TEXT("Quest step increments."), QuestLog->GetQuestStepIndex(TEXT("dawnwatch.starfall_arrival")), 1);

    TestEqual(TEXT("Active quest can complete."), QuestLog->CompleteQuest(TEXT("dawnwatch.starfall_arrival")), EEOQuestActionResult::Applied);
    TestEqual(TEXT("Completed quest can reward."), QuestLog->RewardQuest(TEXT("dawnwatch.starfall_arrival")), EEOQuestActionResult::Applied);
    TestEqual(TEXT("Rewarded quest records state."), QuestLog->GetQuestState(TEXT("dawnwatch.starfall_arrival")), EEOQuestState::Rewarded);

    TestEqual(TEXT("Locked quest cannot start."), QuestLog->StartQuest(TEXT("dawnwatch.locked_depths")), EEOQuestActionResult::Unavailable);

    TestEqual(TEXT("World event can become active."), QuestLog->SetWorldEventState(TEXT("starfall.relic_surge"), EEOWorldEventState::Active), EEOQuestActionResult::Applied);
    TestEqual(TEXT("World event records active state."), QuestLog->GetWorldEventState(TEXT("starfall.relic_surge")), EEOWorldEventState::Active);
    TestEqual(TEXT("World event can resolve."), QuestLog->SetWorldEventState(TEXT("starfall.relic_surge"), EEOWorldEventState::Resolved), EEOQuestActionResult::Applied);
    TestTrue(TEXT("Resolved world event can respawn."), QuestLog->CanWorldEventRespawn(TEXT("starfall.relic_surge")));

    return true;
}

#endif
