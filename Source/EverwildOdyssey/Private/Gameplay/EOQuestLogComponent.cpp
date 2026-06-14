#include "Gameplay/EOQuestLogComponent.h"

UEOQuestLogComponent::UEOQuestLogComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UEOQuestLogComponent::InitializeQuests(const TArray<FEOQuestRecord>& InQuests)
{
    Quests.Reset();

    for (const FEOQuestRecord& Quest : InQuests)
    {
        if (!Quest.QuestId.IsNone())
        {
            Quests.Add(Quest);
        }
    }
}

EEOQuestActionResult UEOQuestLogComponent::StartQuest(FName QuestId)
{
    FEOQuestRecord* Quest = FindQuestMutable(QuestId);
    if (Quest == nullptr)
    {
        return EEOQuestActionResult::UnknownQuest;
    }

    switch (Quest->State)
    {
    case EEOQuestState::Available:
        Quest->State = EEOQuestState::Active;
        Quest->StepIndex = FMath::Max(0, Quest->StepIndex);
        return EEOQuestActionResult::Applied;
    case EEOQuestState::Active:
        return EEOQuestActionResult::AlreadyActive;
    case EEOQuestState::Completed:
        return EEOQuestActionResult::AlreadyCompleted;
    case EEOQuestState::Rewarded:
        return EEOQuestActionResult::AlreadyRewarded;
    case EEOQuestState::Locked:
    default:
        return EEOQuestActionResult::Unavailable;
    }
}

EEOQuestActionResult UEOQuestLogComponent::AdvanceQuest(FName QuestId, int32 StepDelta)
{
    FEOQuestRecord* Quest = FindQuestMutable(QuestId);
    if (Quest == nullptr)
    {
        return EEOQuestActionResult::UnknownQuest;
    }

    if (Quest->State != EEOQuestState::Active)
    {
        return EEOQuestActionResult::NotActive;
    }

    Quest->StepIndex += FMath::Max(1, StepDelta);
    return EEOQuestActionResult::Applied;
}

EEOQuestActionResult UEOQuestLogComponent::CompleteQuest(FName QuestId)
{
    FEOQuestRecord* Quest = FindQuestMutable(QuestId);
    if (Quest == nullptr)
    {
        return EEOQuestActionResult::UnknownQuest;
    }

    if (Quest->State == EEOQuestState::Completed)
    {
        return EEOQuestActionResult::AlreadyCompleted;
    }

    if (Quest->State == EEOQuestState::Rewarded)
    {
        return EEOQuestActionResult::AlreadyRewarded;
    }

    if (Quest->State != EEOQuestState::Active)
    {
        return EEOQuestActionResult::NotActive;
    }

    Quest->State = EEOQuestState::Completed;
    return EEOQuestActionResult::Applied;
}

EEOQuestActionResult UEOQuestLogComponent::RewardQuest(FName QuestId)
{
    FEOQuestRecord* Quest = FindQuestMutable(QuestId);
    if (Quest == nullptr)
    {
        return EEOQuestActionResult::UnknownQuest;
    }

    if (Quest->State == EEOQuestState::Rewarded)
    {
        return EEOQuestActionResult::AlreadyRewarded;
    }

    if (Quest->State != EEOQuestState::Completed)
    {
        return EEOQuestActionResult::NotActive;
    }

    Quest->State = EEOQuestState::Rewarded;
    return EEOQuestActionResult::Applied;
}

EEOQuestState UEOQuestLogComponent::GetQuestState(FName QuestId) const
{
    if (const FEOQuestRecord* Quest = FindQuest(QuestId))
    {
        return Quest->State;
    }

    return EEOQuestState::Locked;
}

int32 UEOQuestLogComponent::GetQuestStepIndex(FName QuestId) const
{
    if (const FEOQuestRecord* Quest = FindQuest(QuestId))
    {
        return Quest->StepIndex;
    }

    return INDEX_NONE;
}

bool UEOQuestLogComponent::HasQuest(FName QuestId) const
{
    return FindQuest(QuestId) != nullptr;
}

void UEOQuestLogComponent::InitializeWorldEvents(const TArray<FEOWorldEventRecord>& InWorldEvents)
{
    WorldEvents.Reset();

    for (const FEOWorldEventRecord& Event : InWorldEvents)
    {
        if (!Event.EventId.IsNone())
        {
            WorldEvents.Add(Event);
        }
    }
}

EEOQuestActionResult UEOQuestLogComponent::SetWorldEventState(FName EventId, EEOWorldEventState NewState)
{
    FEOWorldEventRecord* Event = FindWorldEventMutable(EventId);
    if (Event == nullptr)
    {
        return EEOQuestActionResult::UnknownWorldEvent;
    }

    Event->State = NewState;
    return EEOQuestActionResult::Applied;
}

EEOWorldEventState UEOQuestLogComponent::GetWorldEventState(FName EventId) const
{
    if (const FEOWorldEventRecord* Event = FindWorldEvent(EventId))
    {
        return Event->State;
    }

    return EEOWorldEventState::Dormant;
}

bool UEOQuestLogComponent::CanWorldEventRespawn(FName EventId) const
{
    if (const FEOWorldEventRecord* Event = FindWorldEvent(EventId))
    {
        return Event->CanRespawn();
    }

    return false;
}

bool UEOQuestLogComponent::HasWorldEvent(FName EventId) const
{
    return FindWorldEvent(EventId) != nullptr;
}

FEOQuestRecord* UEOQuestLogComponent::FindQuestMutable(FName QuestId)
{
    return Quests.FindByPredicate([QuestId](const FEOQuestRecord& Quest)
    {
        return Quest.QuestId == QuestId;
    });
}

const FEOQuestRecord* UEOQuestLogComponent::FindQuest(FName QuestId) const
{
    return Quests.FindByPredicate([QuestId](const FEOQuestRecord& Quest)
    {
        return Quest.QuestId == QuestId;
    });
}

FEOWorldEventRecord* UEOQuestLogComponent::FindWorldEventMutable(FName EventId)
{
    return WorldEvents.FindByPredicate([EventId](const FEOWorldEventRecord& Event)
    {
        return Event.EventId == EventId;
    });
}

const FEOWorldEventRecord* UEOQuestLogComponent::FindWorldEvent(FName EventId) const
{
    return WorldEvents.FindByPredicate([EventId](const FEOWorldEventRecord& Event)
    {
        return Event.EventId == EventId;
    });
}
