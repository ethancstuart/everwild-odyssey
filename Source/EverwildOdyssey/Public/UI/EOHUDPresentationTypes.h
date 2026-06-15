#pragma once

#include "CoreMinimal.h"
#include "World/EOZoneProfile.h"
#include "EOHUDPresentationTypes.generated.h"

USTRUCT(BlueprintType)
struct FEOHUDUnitFrame
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD")
    FName RoleId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD")
    FString TargetRole;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD")
    float Health = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD")
    float MaxHealth = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD")
    FLinearColor RoleColor = FLinearColor::White;

    bool IsValidForAlpha() const;
};

USTRUCT(BlueprintType)
struct FEOHUDActionSlot
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD")
    FName ActionId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD")
    FString KeyboardGlyph;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD")
    FString ControllerGlyph;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD")
    float CooldownRemaining = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD")
    FLinearColor SlotColor = FLinearColor::White;

    bool IsValidForAlpha() const;
};

USTRUCT(BlueprintType)
struct FEOHUDEventFeedEntry
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD")
    FText Message;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD")
    FLinearColor Color = FLinearColor::White;

    bool IsValidForAlpha() const;
};

USTRUCT(BlueprintType)
struct FEOHUDPresentationModel
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD")
    FEOHUDUnitFrame PlayerFrame;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD")
    TArray<FEOHUDUnitFrame> PartyFrames;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD")
    FEOHUDUnitFrame TargetFrame;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD")
    TArray<FEOHUDActionSlot> ActionSlots;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD")
    FText QuestTitle;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD")
    FText QuestObjective;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD")
    FText WorldEventText;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD")
    TArray<FEOMinimapMarkerSpec> MinimapMarkers;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD")
    TArray<FEOHUDEventFeedEntry> FeedEntries;

    bool IsValidForAlpha() const;
    FString FindGlyphForAction(FName ActionId) const;
    static FEOHUDPresentationModel BuildAlphaFixture();
};
