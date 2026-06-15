#pragma once

#include "CoreMinimal.h"
#include "UObject/SoftObjectPath.h"
#include "EOAssetRoleTypes.generated.h"

UENUM(BlueprintType)
enum class EEOAssetRoleCategory : uint8
{
    Hero,
    Companion,
    Enemy,
    Environment,
    Prop,
    VFX,
    UI,
    Audio
};

UENUM(BlueprintType)
enum class EEOAssetRoleRequirement : uint8
{
    RequiredForShowcase,
    OptionalEnhancement,
    DebugProxyAllowed
};

UENUM(BlueprintType)
enum class EEOAssetResolutionState : uint8
{
    RealAsset,
    ProxyAsset,
    Missing
};

USTRUCT(BlueprintType)
struct FEOAssetRoleDefinition
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asset Role")
    FName RoleId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asset Role")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asset Role")
    EEOAssetRoleCategory Category = EEOAssetRoleCategory::Environment;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asset Role")
    EEOAssetRoleRequirement Requirement = EEOAssetRoleRequirement::DebugProxyAllowed;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asset Role")
    FSoftObjectPath PreferredAssetPath;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asset Role")
    FName FallbackRoleId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asset Role")
    FVector ScaleNormalization = FVector::OneVector;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asset Role")
    FLinearColor DebugTint = FLinearColor::White;

    bool IsValidForAlpha() const
    {
        return !RoleId.IsNone()
            && !DisplayName.IsEmpty()
            && ScaleNormalization.X > 0.0f
            && ScaleNormalization.Y > 0.0f
            && ScaleNormalization.Z > 0.0f
            && DebugTint.A > 0.0f;
    }
};

USTRUCT(BlueprintType)
struct FEOResolvedAssetRole
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asset Role")
    FName RoleId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asset Role")
    EEOAssetResolutionState State = EEOAssetResolutionState::Missing;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asset Role")
    FSoftObjectPath ResolvedPath;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asset Role")
    FName FallbackRoleId = NAME_None;

    bool IsResolved() const
    {
        return State == EEOAssetResolutionState::RealAsset || State == EEOAssetResolutionState::ProxyAsset;
    }
};

USTRUCT(BlueprintType)
struct FEOAssetResolutionSummary
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asset Role")
    int32 TotalRoles = 0;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asset Role")
    int32 RealAssetRoles = 0;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asset Role")
    int32 ProxyRoles = 0;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asset Role")
    int32 MissingRequiredRoles = 0;

    bool CanLaunchShowcase() const
    {
        return TotalRoles > 0 && MissingRequiredRoles == 0 && RealAssetRoles >= 12;
    }
};

class FEOAssetRoleCatalog
{
public:
    static TArray<FEOAssetRoleDefinition> BuildDefaultRoles();
    static bool TryGetRoleDefinition(FName RoleId, FEOAssetRoleDefinition& OutRole);
};
