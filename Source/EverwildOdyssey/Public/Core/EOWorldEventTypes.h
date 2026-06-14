#pragma once

#include "CoreMinimal.h"
#include "EOWorldEventTypes.generated.h"

UENUM(BlueprintType)
enum class EEOWorldEventState : uint8
{
    Dormant UMETA(DisplayName = "Dormant"),
    Warning UMETA(DisplayName = "Warning"),
    Active UMETA(DisplayName = "Active"),
    Resolved UMETA(DisplayName = "Resolved")
};

USTRUCT(BlueprintType)
struct FEOWorldEventRecord
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World Event")
    FName EventId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World Event")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World Event")
    EEOWorldEventState State = EEOWorldEventState::Dormant;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World Event")
    float RespawnSeconds = 180.0f;

    bool CanRespawn() const
    {
        return RespawnSeconds > 0.0f && State == EEOWorldEventState::Resolved;
    }
};
