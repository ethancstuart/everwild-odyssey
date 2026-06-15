#if WITH_DEV_AUTOMATION_TESTS

#include "Presentation/EOAssetResolver.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEOAssetResolverTest, "EverwildOdyssey.Presentation.AssetResolver", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FEOAssetResolverTest::RunTest(const FString& Parameters)
{
    FEOAssetRoleDefinition RequiredRole;
    RequiredRole.RoleId = TEXT("zone.test.required");
    RequiredRole.DisplayName = FText::FromString(TEXT("Required Test Role"));
    RequiredRole.Category = EEOAssetRoleCategory::Environment;
    RequiredRole.Requirement = EEOAssetRoleRequirement::RequiredForShowcase;
    RequiredRole.PreferredAssetPath = FSoftObjectPath(TEXT("/Game/Missing/SM_NotPresent.SM_NotPresent"));
    RequiredRole.DebugTint = FLinearColor(1.0f, 0.0f, 1.0f, 1.0f);

    FEOAssetRoleDefinition DebugProxyRole;
    DebugProxyRole.RoleId = TEXT("zone.test.proxy");
    DebugProxyRole.DisplayName = FText::FromString(TEXT("Proxy Test Role"));
    DebugProxyRole.Category = EEOAssetRoleCategory::Prop;
    DebugProxyRole.Requirement = EEOAssetRoleRequirement::DebugProxyAllowed;
    DebugProxyRole.PreferredAssetPath = FSoftObjectPath(TEXT("/Game/Missing/SM_NotPresentEither.SM_NotPresentEither"));
    DebugProxyRole.DebugTint = FLinearColor::Gray;

    FEOAssetResolver Resolver;
    Resolver.Initialize({ RequiredRole, DebugProxyRole });

    const FEOResolvedAssetRole RequiredResult = Resolver.ResolveRole(TEXT("zone.test.required"));
    TestEqual(TEXT("Missing required role reports missing."), RequiredResult.State, EEOAssetResolutionState::Missing);

    const FEOResolvedAssetRole ProxyResult = Resolver.ResolveRole(TEXT("zone.test.proxy"));
    TestEqual(TEXT("Debug proxy role reports proxy asset."), ProxyResult.State, EEOAssetResolutionState::ProxyAsset);

    const FEOAssetResolutionSummary Summary = Resolver.BuildSummary();
    TestEqual(TEXT("Summary counts all roles."), Summary.TotalRoles, 2);
    TestEqual(TEXT("Summary counts required misses."), Summary.MissingRequiredRoles, 1);
    TestEqual(TEXT("Summary counts proxy roles."), Summary.ProxyRoles, 1);
    TestEqual(TEXT("Summary categories account for every role."), Summary.RealAssetRoles + Summary.ProxyRoles + Summary.MissingRequiredRoles, Summary.TotalRoles);
    TestFalse(TEXT("Missing required role blocks showcase."), Summary.CanLaunchShowcase());

    return true;
}

#endif
