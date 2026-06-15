#include "Presentation/EOAssetResolver.h"

void FEOAssetResolver::Initialize(const TArray<FEOAssetRoleDefinition>& InRoles)
{
    RoleById.Reset();
    for (const FEOAssetRoleDefinition& Role : InRoles)
    {
        if (Role.IsValidForAlpha())
        {
            RoleById.Add(Role.RoleId, Role);
        }
    }
}

FEOResolvedAssetRole FEOAssetResolver::ResolveRole(FName RoleId) const
{
    FEOResolvedAssetRole Result;
    Result.RoleId = RoleId;

    const FEOAssetRoleDefinition* Role = RoleById.Find(RoleId);
    if (Role == nullptr)
    {
        Result.State = EEOAssetResolutionState::Missing;
        return Result;
    }

    Result.FallbackRoleId = Role->FallbackRoleId;

    if (CanLoadPreferredAsset(*Role))
    {
        Result.State = EEOAssetResolutionState::RealAsset;
        Result.ResolvedPath = Role->PreferredAssetPath;
        return Result;
    }

    if (Role->Requirement == EEOAssetRoleRequirement::RequiredForShowcase)
    {
        Result.State = EEOAssetResolutionState::Missing;
        return Result;
    }

    Result.State = EEOAssetResolutionState::ProxyAsset;
    return Result;
}

FEOAssetResolutionSummary FEOAssetResolver::BuildSummary() const
{
    FEOAssetResolutionSummary Summary;
    Summary.TotalRoles = RoleById.Num();

    for (const TPair<FName, FEOAssetRoleDefinition>& Pair : RoleById)
    {
        const FEOResolvedAssetRole Resolution = ResolveRole(Pair.Key);
        if (Resolution.State == EEOAssetResolutionState::RealAsset)
        {
            ++Summary.RealAssetRoles;
        }
        else if (Resolution.State == EEOAssetResolutionState::ProxyAsset)
        {
            ++Summary.ProxyRoles;
        }
        else if (Pair.Value.Requirement == EEOAssetRoleRequirement::RequiredForShowcase)
        {
            ++Summary.MissingRequiredRoles;
        }
    }

    return Summary;
}

FString FEOAssetResolver::BuildReadableReport() const
{
    const FEOAssetResolutionSummary Summary = BuildSummary();
    return FString::Printf(
        TEXT("Asset roles: total=%d real=%d proxy=%d missing_required=%d showcase_ready=%s"),
        Summary.TotalRoles,
        Summary.RealAssetRoles,
        Summary.ProxyRoles,
        Summary.MissingRequiredRoles,
        Summary.CanLaunchShowcase() ? TEXT("true") : TEXT("false"));
}

bool FEOAssetResolver::CanLoadPreferredAsset(const FEOAssetRoleDefinition& Role) const
{
    if (!Role.PreferredAssetPath.IsValid())
    {
        return false;
    }

    return Role.PreferredAssetPath.TryLoad() != nullptr;
}
