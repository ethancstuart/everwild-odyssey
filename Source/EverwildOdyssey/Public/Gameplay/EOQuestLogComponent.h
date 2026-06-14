#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Core/EOQuestTypes.h"
#include "Core/EOWorldEventTypes.h"
#include "EOQuestLogComponent.generated.h"

UENUM(BlueprintType)
enum class EEOQuestActionResult : uint8
{
    Applied,
    UnknownQuest,
    UnknownWorldEvent,
    Unavailable,
    AlreadyActive,
    NotActive,
    AlreadyCompleted,
    AlreadyRewarded
};

UCLASS(ClassGroup = (Everwild), meta = (BlueprintSpawnableComponent))
class EVERWILDODYSSEY_API UEOQuestLogComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UEOQuestLogComponent();

    UFUNCTION(BlueprintCallable, Category = "Quests")
    void InitializeQuests(const TArray<FEOQuestRecord>& InQuests);

    UFUNCTION(BlueprintCallable, Category = "Quests")
    EEOQuestActionResult StartQuest(FName QuestId);

    UFUNCTION(BlueprintCallable, Category = "Quests")
    EEOQuestActionResult AdvanceQuest(FName QuestId, int32 StepDelta = 1);

    UFUNCTION(BlueprintCallable, Category = "Quests")
    EEOQuestActionResult CompleteQuest(FName QuestId);

    UFUNCTION(BlueprintCallable, Category = "Quests")
    EEOQuestActionResult RewardQuest(FName QuestId);

    UFUNCTION(BlueprintPure, Category = "Quests")
    int32 GetQuestCount() const { return Quests.Num(); }

    UFUNCTION(BlueprintPure, Category = "Quests")
    EEOQuestState GetQuestState(FName QuestId) const;

    UFUNCTION(BlueprintPure, Category = "Quests")
    int32 GetQuestStepIndex(FName QuestId) const;

    UFUNCTION(BlueprintPure, Category = "Quests")
    bool HasQuest(FName QuestId) const;

    UFUNCTION(BlueprintCallable, Category = "World Events")
    void InitializeWorldEvents(const TArray<FEOWorldEventRecord>& InWorldEvents);

    UFUNCTION(BlueprintCallable, Category = "World Events")
    EEOQuestActionResult SetWorldEventState(FName EventId, EEOWorldEventState NewState);

    UFUNCTION(BlueprintPure, Category = "World Events")
    int32 GetWorldEventCount() const { return WorldEvents.Num(); }

    UFUNCTION(BlueprintPure, Category = "World Events")
    EEOWorldEventState GetWorldEventState(FName EventId) const;

    UFUNCTION(BlueprintPure, Category = "World Events")
    bool CanWorldEventRespawn(FName EventId) const;

    UFUNCTION(BlueprintPure, Category = "World Events")
    bool HasWorldEvent(FName EventId) const;

private:
    FEOQuestRecord* FindQuestMutable(FName QuestId);
    const FEOQuestRecord* FindQuest(FName QuestId) const;
    FEOWorldEventRecord* FindWorldEventMutable(FName EventId);
    const FEOWorldEventRecord* FindWorldEvent(FName EventId) const;

    UPROPERTY(EditAnywhere, Category = "Quests")
    TArray<FEOQuestRecord> Quests;

    UPROPERTY(EditAnywhere, Category = "World Events")
    TArray<FEOWorldEventRecord> WorldEvents;
};
