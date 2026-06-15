#if WITH_DEV_AUTOMATION_TESTS

#include "Presentation/EOAssetRoleTypes.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEOAssetRoleTypesTest, "EverwildOdyssey.Presentation.AssetRoleTypes", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FEOAssetRoleTypesTest::RunTest(const FString& Parameters)
{
    const TArray<FEOAssetRoleDefinition> Roles = FEOAssetRoleCatalog::BuildDefaultRoles();
    TestTrue(TEXT("The default role catalog is large enough for the heroic MMO slice."), Roles.Num() >= 26);

    int32 RequiredCount = 0;
    TSet<FName> RoleIds;
    for (const FEOAssetRoleDefinition& Role : Roles)
    {
        TestTrue(TEXT("Each asset role is valid."), Role.IsValidForAlpha());
        TestFalse(TEXT("Asset role ids are unique."), RoleIds.Contains(Role.RoleId));
        RoleIds.Add(Role.RoleId);
        RequiredCount += Role.Requirement == EEOAssetRoleRequirement::RequiredForShowcase ? 1 : 0;
    }

    TestTrue(TEXT("Showcase-critical roles exist."), RequiredCount >= 12);
    TestTrue(TEXT("Hero body role exists."), RoleIds.Contains(TEXT("hero.runeblade.body")));
    TestTrue(TEXT("Dawnwatch gate role exists."), RoleIds.Contains(TEXT("zone.dawnwatch.gate")));
    TestTrue(TEXT("Relic surge core role exists."), RoleIds.Contains(TEXT("zone.relic_surge.core")));
    TestTrue(TEXT("Sky-Watch entrance role exists."), RoleIds.Contains(TEXT("zone.skywatch.entrance")));
    TestTrue(TEXT("Action slot role exists."), RoleIds.Contains(TEXT("ui.action.slot")));

    FEOAssetResolutionSummary Summary;
    Summary.TotalRoles = Roles.Num();
    Summary.RealAssetRoles = 15;
    Summary.ProxyRoles = Roles.Num() - 15;
    Summary.MissingRequiredRoles = 0;
    TestTrue(TEXT("Summary with no missing required roles is showcase-ready enough to launch."), Summary.CanLaunchShowcase());

    Summary.MissingRequiredRoles = 1;
    TestFalse(TEXT("Summary with a missing required role fails showcase readiness."), Summary.CanLaunchShowcase());

    return true;
}

#endif
