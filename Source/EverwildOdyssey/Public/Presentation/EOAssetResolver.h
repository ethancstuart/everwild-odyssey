#pragma once

#include "CoreMinimal.h"
#include "Presentation/EOAssetRoleTypes.h"

class EVERWILDODYSSEY_API FEOAssetResolver
{
public:
    void Initialize(const TArray<FEOAssetRoleDefinition>& InRoles);
    FEOResolvedAssetRole ResolveRole(FName RoleId) const;
    FEOAssetResolutionSummary BuildSummary() const;
    FString BuildReadableReport() const;

private:
    TMap<FName, FEOAssetRoleDefinition> RoleById;

    bool CanLoadPreferredAsset(const FEOAssetRoleDefinition& Role) const;
};
