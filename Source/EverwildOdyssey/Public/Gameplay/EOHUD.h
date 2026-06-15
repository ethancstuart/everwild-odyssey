#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/EOHUDPresentationTypes.h"
#include "EOHUD.generated.h"

class AEOHeroCharacter;

USTRUCT(BlueprintType)
struct FEOHUDSnapshot
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    float Health = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    float MaxHealth = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    float Resource = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    float MaxResource = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    int32 Level = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    int32 Experience = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    int32 ExperienceForNextLevel = 100;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    int32 RelicShards = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    int32 RequiredRelicShards = 3;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    int32 ActiveEnemyCount = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    bool bQuestAccepted = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    bool bGateStabilized = false;

    bool IsValidForAlpha() const
    {
        return MaxHealth > 0.0f
            && Health >= 0.0f
            && Health <= MaxHealth
            && MaxResource > 0.0f
            && Resource >= 0.0f
            && Resource <= MaxResource
            && Level > 0
            && Experience >= 0
            && ExperienceForNextLevel > 0
            && RelicShards >= 0
            && RequiredRelicShards > 0
            && ActiveEnemyCount >= 0;
    }
};

UCLASS()
class EVERWILDODYSSEY_API AEOHUD : public AHUD
{
    GENERATED_BODY()

public:
    static constexpr int32 MaxVisiblePartyFrames = 3;
    static constexpr int32 MaxVisibleFeedEntries = 3;
    static constexpr int32 MaxVisibleActionSlots = 8;
    static constexpr int32 MaxVisibleMinimapMarkers = 5;

    virtual void DrawHUD() override;

    static FEOHUDSnapshot BuildSnapshot(const AEOHeroCharacter* Hero);
    static FEOHUDPresentationModel BuildPresentationModel(const FEOHUDSnapshot& Snapshot);
    static FString FormatVitalsLine(const FEOHUDSnapshot& Snapshot);
    static FString FormatProgressionLine(const FEOHUDSnapshot& Snapshot);
    static FString FormatObjectiveLine(const FEOHUDSnapshot& Snapshot);
    static FString FormatWorldEventLine(const FEOHUDSnapshot& Snapshot);
};
