#pragma once

#include "CoreMinimal.h"
#include "Core/EOClassTypes.h"
#include "EOQuestTypes.generated.h"

UENUM(BlueprintType)
enum class EEOQuestState : uint8
{
    Locked UMETA(DisplayName = "Locked"),
    Available UMETA(DisplayName = "Available"),
    Active UMETA(DisplayName = "Active"),
    Completed UMETA(DisplayName = "Completed"),
    Rewarded UMETA(DisplayName = "Rewarded")
};

USTRUCT(BlueprintType)
struct FEOQuestRecord
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    FName QuestId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    EEOQuestState State = EEOQuestState::Locked;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    int32 StepIndex = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    EEOFaction ReportedToFaction = EEOFaction::DawnwatchCommand;

    bool CanAdvance() const
    {
        return State == EEOQuestState::Active && StepIndex >= 0;
    }
};
