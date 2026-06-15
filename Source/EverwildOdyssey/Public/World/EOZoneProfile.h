#pragma once

#include "CoreMinimal.h"
#include "EOZoneProfile.generated.h"

UENUM(BlueprintType)
enum class EEOZoneArea : uint8
{
    DawnwatchGate,
    StarfallRoad,
    RelicSurgeCamp,
    SkyWatchEntrance,
    SidePocket,
    FutureBoundary
};

USTRUCT(BlueprintType)
struct FEOZoneVisualSpec
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone")
    FName Id = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone")
    FName AssetRoleId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone")
    EEOZoneArea Area = EEOZoneArea::StarfallRoad;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone")
    FVector Location = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone")
    FRotator Rotation = FRotator::ZeroRotator;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone")
    FVector Scale = FVector::OneVector;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone")
    FLinearColor Tint = FLinearColor::White;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone")
    bool bBlocksMovement = true;

    bool IsValidForAlpha() const
    {
        return !Id.IsNone()
            && !AssetRoleId.IsNone()
            && Scale.X > 0.0f
            && Scale.Y > 0.0f
            && Scale.Z > 0.0f
            && Tint.A > 0.0f;
    }
};

USTRUCT(BlueprintType)
struct FEOZoneLightSpec
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone")
    FName LightId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone")
    FVector Location = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone")
    FLinearColor Color = FLinearColor::White;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone")
    float Intensity = 3000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone")
    float Radius = 600.0f;

    bool IsValidForAlpha() const
    {
        return !LightId.IsNone() && Color.A > 0.0f && Intensity > 0.0f && Radius > 0.0f;
    }
};

USTRUCT(BlueprintType)
struct FEOZoneAnchor
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone")
    FName AnchorId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone")
    FVector Location = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone")
    FRotator Rotation = FRotator::ZeroRotator;

    bool IsValidForAlpha() const
    {
        return !AnchorId.IsNone() && !DisplayName.IsEmpty();
    }
};

USTRUCT(BlueprintType)
struct FEOZoneFutureHook
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone")
    FName HookId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone")
    FText LockedReason;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone")
    FName AnchorId = NAME_None;

    bool IsValidForAlpha() const
    {
        return !HookId.IsNone() && !DisplayName.IsEmpty() && !LockedReason.IsEmpty() && !AnchorId.IsNone();
    }
};

USTRUCT(BlueprintType)
struct FEOMinimapMarkerSpec
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone")
    FName MarkerId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone")
    FText Label;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone")
    FVector2D NormalizedPosition = FVector2D::ZeroVector;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone")
    FLinearColor Color = FLinearColor::White;

    bool IsValidForAlpha() const
    {
        return !MarkerId.IsNone()
            && !Label.IsEmpty()
            && NormalizedPosition.X >= 0.0f
            && NormalizedPosition.X <= 1.0f
            && NormalizedPosition.Y >= 0.0f
            && NormalizedPosition.Y <= 1.0f
            && Color.A > 0.0f;
    }
};

USTRUCT(BlueprintType)
struct FEOZoneProfile
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone")
    FName ZoneId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone")
    TArray<FEOZoneVisualSpec> Landmarks;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone")
    TArray<FEOZoneVisualSpec> ScenicProps;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone")
    TArray<FEOZoneLightSpec> Lights;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone")
    TArray<FEOZoneAnchor> Anchors;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone")
    TArray<FEOZoneFutureHook> FutureHooks;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zone")
    TArray<FEOMinimapMarkerSpec> MinimapMarkers;

    bool IsValidForAlpha() const;
    bool HasAnchor(FName AnchorId) const;
    bool HasFutureHook(FName HookId) const;
};

class FEOZoneProfileCatalog
{
public:
    static FEOZoneProfile BuildStarfallValeProfile();
};
