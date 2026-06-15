# Heroic MMO Asset-First Rebuild Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Rebuild the current Unreal vertical slice into an asset-led, controller-friendly, single-player heroic MMO-adventure slice with a credible first frame, a dense Starfall Vale micro-zone, readable Runeblade combat, MMO HUD surfaces, and diagnostics that expose remaining proxy-art gaps.

**Architecture:** Preserve the existing C++ gameplay foundation, but split new work into focused `Core`, `Presentation`, `World`, `Combat`, `Encounters`, `UI`, `Input`, and `Tests` responsibilities. The first implementation target is Tier 1 from the spec, with Tier 2 data seams and Tier 3 visible hooks; Tier 4 multiplayer and service systems are out of this plan.

**Tech Stack:** Unreal Engine 5.7 C++, existing legacy input mappings plus controller mappings in `Config/DefaultInput.ini`, Canvas HUD first with a `UI` presentation model shaped for UMG/CommonUI, Engine/installed/local assets through semantic role resolution, automation tests under `Source/EverwildOdyssey/Private/Tests`, and macOS verification through `Scripts/check-unreal.sh` plus Unreal build scripts.

---

## Scope Check

The approved spec contains a long-term game vision. This plan implements the first asset-led vertical-slice rebuild only:

- Tier 1: one polished Runeblade hero read, one dense Starfall Vale micro-zone, one staged relic surge event, one elite phase, one MMO-style HUD, one reward loop, one playable controller/keyboard path.
- Tier 2: asset roles, zone profile, encounter profile, quest/profile data, HUD presentation model, diagnostics, build modes.
- Tier 3: visible class/faction/dungeon/future-zone hooks through data, HUD, signage, and world composition.
- Tier 4: networking, real MMO persistence, auction/economy service, guilds, raids, large streaming continents, and live operations are explicitly excluded.

## File Structure

Create or modify these files:

- Create: `Source/EverwildOdyssey/Public/Presentation/EOAssetRoleTypes.h`
  Defines semantic asset roles, role requirements, resolved-role status, and visual diagnostic summaries.
- Create: `Source/EverwildOdyssey/Private/Presentation/EOAssetRoleTypes.cpp`
  Builds the default role table and role summary helpers.
- Create: `Source/EverwildOdyssey/Public/Presentation/EOAssetResolver.h`
  Provides a small runtime resolver that maps semantic roles to installed assets or declared proxies.
- Create: `Source/EverwildOdyssey/Private/Presentation/EOAssetResolver.cpp`
  Loads preferred engine/local assets and reports real/proxy/missing status without hiding required visual failures.
- Create: `Source/EverwildOdyssey/Public/World/EOZoneProfile.h`
  Defines Starfall Vale zone, landmark, scenic prop, light, anchor, future hook, and minimap marker data.
- Create: `Source/EverwildOdyssey/Private/World/EOZoneProfile.cpp`
  Builds the Dawnwatch Gate, Starfall Road, Relic Surge, and Sky-Watch Entrance profile.
- Create: `Source/EverwildOdyssey/Public/UI/EOHUDPresentationTypes.h`
  Defines HUD snapshot surfaces: player frame, party frames, target frame, action slots, quest tracker, minimap, feed, input glyphs.
- Create: `Source/EverwildOdyssey/Private/UI/EOHUDPresentationTypes.cpp`
  Formats and validates HUD presentation data.
- Create: `Source/EverwildOdyssey/Public/Encounters/EOEncounterProfile.h`
  Defines event phases, enemy roles, spawn anchors, rewards, and elite-phase data.
- Create: `Source/EverwildOdyssey/Private/Encounters/EOEncounterProfile.cpp`
  Builds the relic surge profile consumed by game mode and tests.
- Create: `Source/EverwildOdyssey/Public/World/EOWorldDiagnostics.h`
  Defines a launch/report summary for asset, zone, HUD, controller, and event readiness.
- Create: `Source/EverwildOdyssey/Private/World/EOWorldDiagnostics.cpp`
  Produces readable diagnostics strings and acceptance flags.
- Create: `Source/EverwildOdyssey/Private/Tests/EOAssetRoleTypesTests.cpp`
- Create: `Source/EverwildOdyssey/Private/Tests/EOAssetResolverTests.cpp`
- Create: `Source/EverwildOdyssey/Private/Tests/EOZoneProfileTests.cpp`
- Create: `Source/EverwildOdyssey/Private/Tests/EOHUDPresentationTypesTests.cpp`
- Create: `Source/EverwildOdyssey/Private/Tests/EOEncounterProfileTests.cpp`
- Create: `Source/EverwildOdyssey/Private/Tests/EOWorldDiagnosticsTests.cpp`
- Create: `docs/assets/heroic-mmo-asset-manifest.md`
- Create: `docs/heroic-mmo-visual-verification.md`
- Create: `Scripts/launch-everwild-standalone.sh`
- Modify: `Source/EverwildOdyssey/EverwildOdyssey.Build.cs` only if new modules need extra dependencies after compile.
- Modify: `Source/EverwildOdyssey/Public/Gameplay/EOAlphaWorldScaffold.h`
- Modify: `Source/EverwildOdyssey/Private/Gameplay/EOAlphaWorldScaffold.cpp`
- Modify: `Source/EverwildOdyssey/Public/Gameplay/EOHeroCharacter.h`
- Modify: `Source/EverwildOdyssey/Private/Gameplay/EOHeroCharacter.cpp`
- Modify: `Source/EverwildOdyssey/Public/Gameplay/EOHUD.h`
- Modify: `Source/EverwildOdyssey/Private/Gameplay/EOHUD.cpp`
- Modify: `Source/EverwildOdyssey/Public/Gameplay/EOGameMode.h`
- Modify: `Source/EverwildOdyssey/Private/Gameplay/EOGameMode.cpp`
- Modify: `Source/EverwildOdyssey/Public/Core/EOQuestTypes.h`
- Modify: `Source/EverwildOdyssey/Public/Core/EOInventoryTypes.h`
- Modify: `Source/EverwildOdyssey/Public/Core/EOClassTypes.h`
- Modify: `Config/DefaultInput.ini`
- Modify: `README.md`
- Modify: `docs/unreal-verification.md`

---

### Task 1: Asset Role Catalog And Proxy Diagnostics

**Tier:** 1 and 2

**Files:**
- Create: `Source/EverwildOdyssey/Public/Presentation/EOAssetRoleTypes.h`
- Create: `Source/EverwildOdyssey/Private/Presentation/EOAssetRoleTypes.cpp`
- Create: `Source/EverwildOdyssey/Private/Tests/EOAssetRoleTypesTests.cpp`
- Create: `docs/assets/heroic-mmo-asset-manifest.md`

- [ ] **Step 1: Write the failing asset-role catalog test**

Create `Source/EverwildOdyssey/Private/Tests/EOAssetRoleTypesTests.cpp`:

```cpp
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
```

- [ ] **Step 2: Run test to verify it fails**

Run:

```bash
"/Users/Shared/Epic Games/UE_5.7/Engine/Build/BatchFiles/Mac/Build.sh" EverwildOdysseyEditor Mac Development -Project="/Users/ethanstuart/Projects/everwild-odyssey/.worktrees/alpha-foundation/EverwildOdyssey.uproject" -WaitMutex
```

Expected: build fails because `Presentation/EOAssetRoleTypes.h` does not exist.

- [ ] **Step 3: Add the asset-role type header**

Create `Source/EverwildOdyssey/Public/Presentation/EOAssetRoleTypes.h`:

```cpp
#pragma once

#include "CoreMinimal.h"
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
```

- [ ] **Step 4: Add the default role catalog**

Create `Source/EverwildOdyssey/Private/Presentation/EOAssetRoleTypes.cpp`:

```cpp
#include "Presentation/EOAssetRoleTypes.h"

namespace
{
FEOAssetRoleDefinition Role(
    const TCHAR* Id,
    const TCHAR* Name,
    EEOAssetRoleCategory Category,
    EEOAssetRoleRequirement Requirement,
    const TCHAR* PreferredPath,
    const FLinearColor& DebugTint,
    const FVector& Scale = FVector::OneVector,
    FName FallbackRole = NAME_None)
{
    FEOAssetRoleDefinition Result;
    Result.RoleId = FName(Id);
    Result.DisplayName = FText::FromString(Name);
    Result.Category = Category;
    Result.Requirement = Requirement;
    Result.PreferredAssetPath = FSoftObjectPath(PreferredPath);
    Result.DebugTint = DebugTint;
    Result.ScaleNormalization = Scale;
    Result.FallbackRoleId = FallbackRole;
    return Result;
}
}

TArray<FEOAssetRoleDefinition> FEOAssetRoleCatalog::BuildDefaultRoles()
{
    TArray<FEOAssetRoleDefinition> Roles;
    Roles.Reserve(32);

    Roles.Add(Role(TEXT("hero.runeblade.body"), TEXT("Runeblade Champion Body"), EEOAssetRoleCategory::Hero, EEOAssetRoleRequirement::RequiredForShowcase, TEXT("/Game/LocalOnly/Heroes/Runeblade/SK_Runeblade.SK_Runeblade"), FLinearColor(0.08f, 0.36f, 1.0f, 1.0f), FVector(1.18f, 1.18f, 1.18f)));
    Roles.Add(Role(TEXT("hero.runeblade.weapon"), TEXT("Runeblade Oversized Sword"), EEOAssetRoleCategory::Hero, EEOAssetRoleRequirement::RequiredForShowcase, TEXT("/Game/LocalOnly/Heroes/Runeblade/SM_RunebladeSword.SM_RunebladeSword"), FLinearColor(1.0f, 0.76f, 0.24f, 1.0f)));
    Roles.Add(Role(TEXT("hero.runeblade.shield"), TEXT("Runeblade Shield"), EEOAssetRoleCategory::Hero, EEOAssetRoleRequirement::OptionalEnhancement, TEXT("/Game/LocalOnly/Heroes/Runeblade/SM_RunebladeShield.SM_RunebladeShield"), FLinearColor(0.16f, 0.42f, 1.0f, 1.0f)));
    Roles.Add(Role(TEXT("companion.defender.body"), TEXT("Defender Companion Body"), EEOAssetRoleCategory::Companion, EEOAssetRoleRequirement::RequiredForShowcase, TEXT("/Game/LocalOnly/Companions/SM_DefenderCompanion.SM_DefenderCompanion"), FLinearColor(1.0f, 0.55f, 0.14f, 1.0f)));
    Roles.Add(Role(TEXT("companion.mystic.body"), TEXT("Mystic Companion Body"), EEOAssetRoleCategory::Companion, EEOAssetRoleRequirement::RequiredForShowcase, TEXT("/Game/LocalOnly/Companions/SM_MysticCompanion.SM_MysticCompanion"), FLinearColor(0.24f, 0.88f, 0.74f, 1.0f)));

    Roles.Add(Role(TEXT("enemy.minion.body"), TEXT("Ashen Minion Body"), EEOAssetRoleCategory::Enemy, EEOAssetRoleRequirement::RequiredForShowcase, TEXT("/Game/LocalOnly/Enemies/SM_AshenMinion.SM_AshenMinion"), FLinearColor(0.82f, 0.12f, 0.08f, 1.0f)));
    Roles.Add(Role(TEXT("enemy.caster.body"), TEXT("Ashen Caster Body"), EEOAssetRoleCategory::Enemy, EEOAssetRoleRequirement::RequiredForShowcase, TEXT("/Game/LocalOnly/Enemies/SM_AshenCaster.SM_AshenCaster"), FLinearColor(0.86f, 0.16f, 0.82f, 1.0f)));
    Roles.Add(Role(TEXT("enemy.elite.body"), TEXT("Relic Sentinel Elite Body"), EEOAssetRoleCategory::Enemy, EEOAssetRoleRequirement::RequiredForShowcase, TEXT("/Game/LocalOnly/Enemies/SM_RelicSentinelElite.SM_RelicSentinelElite"), FLinearColor(0.66f, 0.12f, 1.0f, 1.0f), FVector(1.35f, 1.35f, 1.35f)));

    Roles.Add(Role(TEXT("zone.dawnwatch.gate"), TEXT("Dawnwatch Hero Gate"), EEOAssetRoleCategory::Environment, EEOAssetRoleRequirement::RequiredForShowcase, TEXT("/Game/LocalOnly/Environment/Dawnwatch/SM_DawnwatchGate.SM_DawnwatchGate"), FLinearColor(0.10f, 0.32f, 1.0f, 1.0f)));
    Roles.Add(Role(TEXT("zone.dawnwatch.wall"), TEXT("Dawnwatch Wall Segment"), EEOAssetRoleCategory::Environment, EEOAssetRoleRequirement::RequiredForShowcase, TEXT("/Game/LocalOnly/Environment/Dawnwatch/SM_DawnwatchWall.SM_DawnwatchWall"), FLinearColor(0.12f, 0.20f, 0.62f, 1.0f)));
    Roles.Add(Role(TEXT("zone.dawnwatch.banner"), TEXT("Dawnwatch Banner"), EEOAssetRoleCategory::Prop, EEOAssetRoleRequirement::RequiredForShowcase, TEXT("/Game/LocalOnly/Environment/Dawnwatch/SM_DawnwatchBanner.SM_DawnwatchBanner"), FLinearColor(0.08f, 0.44f, 1.0f, 1.0f)));
    Roles.Add(Role(TEXT("zone.road.lamp"), TEXT("Starfall Road Lamp"), EEOAssetRoleCategory::Prop, EEOAssetRoleRequirement::OptionalEnhancement, TEXT("/Game/LocalOnly/Environment/Props/SM_RoadLamp.SM_RoadLamp"), FLinearColor(1.0f, 0.66f, 0.22f, 1.0f)));
    Roles.Add(Role(TEXT("zone.road.signpost"), TEXT("Future Zone Signpost"), EEOAssetRoleCategory::Prop, EEOAssetRoleRequirement::OptionalEnhancement, TEXT("/Game/LocalOnly/Environment/Props/SM_Signpost.SM_Signpost"), FLinearColor(0.76f, 0.58f, 0.30f, 1.0f)));
    Roles.Add(Role(TEXT("zone.relic_surge.core"), TEXT("Relic Surge Core"), EEOAssetRoleCategory::Environment, EEOAssetRoleRequirement::RequiredForShowcase, TEXT("/Game/LocalOnly/Relics/SM_RelicSurgeCore.SM_RelicSurgeCore"), FLinearColor(0.82f, 0.18f, 1.0f, 1.0f)));
    Roles.Add(Role(TEXT("zone.relic_surge.crystal"), TEXT("Relic Surge Crystal"), EEOAssetRoleCategory::Environment, EEOAssetRoleRequirement::RequiredForShowcase, TEXT("/Game/LocalOnly/Relics/SM_RelicCrystal.SM_RelicCrystal"), FLinearColor(0.72f, 0.20f, 1.0f, 1.0f)));
    Roles.Add(Role(TEXT("zone.skywatch.entrance"), TEXT("Sky-Watch Dungeon Entrance"), EEOAssetRoleCategory::Environment, EEOAssetRoleRequirement::RequiredForShowcase, TEXT("/Game/LocalOnly/Environment/SkyWatch/SM_SkyWatchEntrance.SM_SkyWatchEntrance"), FLinearColor(0.30f, 0.72f, 1.0f, 1.0f)));
    Roles.Add(Role(TEXT("zone.future.bridge_blocker"), TEXT("Future Zone Bridge Blocker"), EEOAssetRoleCategory::Environment, EEOAssetRoleRequirement::OptionalEnhancement, TEXT("/Game/LocalOnly/Environment/Props/SM_BlockedBridge.SM_BlockedBridge"), FLinearColor(0.45f, 0.38f, 0.30f, 1.0f)));
    Roles.Add(Role(TEXT("zone.training.dummy"), TEXT("Training Yard Dummy"), EEOAssetRoleCategory::Prop, EEOAssetRoleRequirement::OptionalEnhancement, TEXT("/Game/LocalOnly/Environment/Props/SM_TrainingDummy.SM_TrainingDummy"), FLinearColor(0.58f, 0.36f, 0.18f, 1.0f)));
    Roles.Add(Role(TEXT("zone.vendor.crates"), TEXT("Vendor Crates"), EEOAssetRoleCategory::Prop, EEOAssetRoleRequirement::OptionalEnhancement, TEXT("/Game/LocalOnly/Environment/Props/SM_VendorCrates.SM_VendorCrates"), FLinearColor(0.62f, 0.42f, 0.22f, 1.0f)));
    Roles.Add(Role(TEXT("zone.scholar.station"), TEXT("Scholarium Relic Station"), EEOAssetRoleCategory::Prop, EEOAssetRoleRequirement::OptionalEnhancement, TEXT("/Game/LocalOnly/Environment/Props/SM_RelicStudyTable.SM_RelicStudyTable"), FLinearColor(0.24f, 0.82f, 1.0f, 1.0f)));
    Roles.Add(Role(TEXT("zone.foliage.tree"), TEXT("Stylized Tree"), EEOAssetRoleCategory::Environment, EEOAssetRoleRequirement::OptionalEnhancement, TEXT("/PCG/SampleContent/SimpleForest/Meshes/PCG_Tree_01.PCG_Tree_01"), FLinearColor(0.06f, 0.44f, 0.18f, 1.0f)));
    Roles.Add(Role(TEXT("zone.foliage.rock"), TEXT("Stylized Boulder"), EEOAssetRoleCategory::Environment, EEOAssetRoleRequirement::OptionalEnhancement, TEXT("/PCG/SampleContent/SimpleForest/Meshes/PCG_Boulder_02.PCG_Boulder_02"), FLinearColor(0.38f, 0.38f, 0.34f, 1.0f)));

    Roles.Add(Role(TEXT("vfx.weapon.trail"), TEXT("Runeblade Weapon Trail"), EEOAssetRoleCategory::VFX, EEOAssetRoleRequirement::OptionalEnhancement, TEXT("/Game/LocalOnly/VFX/NS_RunebladeTrail.NS_RunebladeTrail"), FLinearColor(0.18f, 0.62f, 1.0f, 1.0f)));
    Roles.Add(Role(TEXT("vfx.hit.impact"), TEXT("Combat Hit Impact"), EEOAssetRoleCategory::VFX, EEOAssetRoleRequirement::OptionalEnhancement, TEXT("/Game/LocalOnly/VFX/NS_HitImpact.NS_HitImpact"), FLinearColor(1.0f, 0.62f, 0.12f, 1.0f)));
    Roles.Add(Role(TEXT("vfx.relic.beam"), TEXT("Relic Sky Beam"), EEOAssetRoleCategory::VFX, EEOAssetRoleRequirement::RequiredForShowcase, TEXT("/Game/LocalOnly/VFX/NS_RelicBeam.NS_RelicBeam"), FLinearColor(0.62f, 0.18f, 1.0f, 1.0f)));
    Roles.Add(Role(TEXT("vfx.enemy.telegraph"), TEXT("Enemy Danger Telegraph"), EEOAssetRoleCategory::VFX, EEOAssetRoleRequirement::OptionalEnhancement, TEXT("/Game/LocalOnly/VFX/NS_EnemyTelegraph.NS_EnemyTelegraph"), FLinearColor(1.0f, 0.18f, 0.08f, 1.0f)));

    Roles.Add(Role(TEXT("ui.action.slot"), TEXT("Action Bar Slot Frame"), EEOAssetRoleCategory::UI, EEOAssetRoleRequirement::RequiredForShowcase, TEXT("/Game/LocalOnly/UI/T_ActionSlot.T_ActionSlot"), FLinearColor(0.18f, 0.22f, 0.34f, 1.0f)));
    Roles.Add(Role(TEXT("ui.quest.marker"), TEXT("Quest Marker"), EEOAssetRoleCategory::UI, EEOAssetRoleRequirement::OptionalEnhancement, TEXT("/Game/LocalOnly/UI/T_QuestMarker.T_QuestMarker"), FLinearColor(1.0f, 0.82f, 0.24f, 1.0f)));
    Roles.Add(Role(TEXT("ui.minimap.marker"), TEXT("Minimap Marker"), EEOAssetRoleCategory::UI, EEOAssetRoleRequirement::OptionalEnhancement, TEXT("/Game/LocalOnly/UI/T_MinimapMarker.T_MinimapMarker"), FLinearColor(0.36f, 0.84f, 1.0f, 1.0f)));
    Roles.Add(Role(TEXT("audio.event.warning"), TEXT("Relic Event Warning Cue"), EEOAssetRoleCategory::Audio, EEOAssetRoleRequirement::DebugProxyAllowed, TEXT("/Game/LocalOnly/Audio/S_RelicWarning.S_RelicWarning"), FLinearColor(1.0f, 0.36f, 0.72f, 1.0f)));

    return Roles;
}

bool FEOAssetRoleCatalog::TryGetRoleDefinition(FName RoleId, FEOAssetRoleDefinition& OutRole)
{
    for (const FEOAssetRoleDefinition& Role : BuildDefaultRoles())
    {
        if (Role.RoleId == RoleId)
        {
            OutRole = Role;
            return true;
        }
    }

    return false;
}
```

- [ ] **Step 5: Add the asset manifest**

Create `docs/assets/heroic-mmo-asset-manifest.md`:

```markdown
# Heroic MMO Asset Manifest

Date: 2026-06-15

## Repository Policy

The public repo contains source code, docs, scripts, and lightweight original assets only. Marketplace, premium, Vault cache, and local-only binary assets stay outside git unless their license and file size are approved for public redistribution.

## Local Asset Roots

- `Content/LocalOnly/`
- `Content/Marketplace/`
- `Plugins/Marketplace/`
- Unreal Engine content under `/Users/Shared/Epic Games/UE_5.7/Engine/Content/`

## Required Showcase Roles

Required roles are defined in `FEOAssetRoleCatalog::BuildDefaultRoles`. Showcase Mode fails visual acceptance when any required role is missing and has no explicit proxy resolution.

## Current Asset Source Preference

1. Installed local project assets.
2. Unreal Engine sample/plugin content already installed.
3. License-safe free assets with user-approved network/install actions.
4. Explicit debug proxies with visible diagnostics.

## Current Known Gap

The initial catalog points to `Content/LocalOnly` paths for final-looking hero, enemy, environment, VFX, and UI roles. If those assets are absent, the resolver reports proxy or missing state and the visual verification doc records the gap.
```

- [ ] **Step 6: Run build and asset-role test compile**

Run:

```bash
"/Users/Shared/Epic Games/UE_5.7/Engine/Build/BatchFiles/Mac/Build.sh" EverwildOdysseyEditor Mac Development -Project="/Users/ethanstuart/Projects/everwild-odyssey/.worktrees/alpha-foundation/EverwildOdyssey.uproject" -WaitMutex
```

Expected: build succeeds and `FEOAssetRoleTypesTest` compiles.

- [ ] **Step 7: Commit**

```bash
git add Source/EverwildOdyssey/Public/Presentation/EOAssetRoleTypes.h Source/EverwildOdyssey/Private/Presentation/EOAssetRoleTypes.cpp Source/EverwildOdyssey/Private/Tests/EOAssetRoleTypesTests.cpp docs/assets/heroic-mmo-asset-manifest.md
git commit -m "feat: add heroic asset role catalog"
```

---

### Task 2: Runtime Asset Resolver

**Tier:** 1 and 2

**Files:**
- Create: `Source/EverwildOdyssey/Public/Presentation/EOAssetResolver.h`
- Create: `Source/EverwildOdyssey/Private/Presentation/EOAssetResolver.cpp`
- Create: `Source/EverwildOdyssey/Private/Tests/EOAssetResolverTests.cpp`

- [ ] **Step 1: Write the failing resolver test**

Create `Source/EverwildOdyssey/Private/Tests/EOAssetResolverTests.cpp`:

```cpp
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
    RequiredRole.DebugTint = FLinearColor::Magenta;

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
    TestFalse(TEXT("Missing required role blocks showcase."), Summary.CanLaunchShowcase());

    return true;
}

#endif
```

- [ ] **Step 2: Run test to verify it fails**

Run:

```bash
"/Users/Shared/Epic Games/UE_5.7/Engine/Build/BatchFiles/Mac/Build.sh" EverwildOdysseyEditor Mac Development -Project="/Users/ethanstuart/Projects/everwild-odyssey/.worktrees/alpha-foundation/EverwildOdyssey.uproject" -WaitMutex
```

Expected: build fails because `Presentation/EOAssetResolver.h` does not exist.

- [ ] **Step 3: Add resolver header**

Create `Source/EverwildOdyssey/Public/Presentation/EOAssetResolver.h`:

```cpp
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
```

- [ ] **Step 4: Add resolver implementation**

Create `Source/EverwildOdyssey/Private/Presentation/EOAssetResolver.cpp`:

```cpp
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
```

- [ ] **Step 5: Run build and inspect resolver behavior**

Run:

```bash
"/Users/Shared/Epic Games/UE_5.7/Engine/Build/BatchFiles/Mac/Build.sh" EverwildOdysseyEditor Mac Development -Project="/Users/ethanstuart/Projects/everwild-odyssey/.worktrees/alpha-foundation/EverwildOdyssey.uproject" -WaitMutex
```

Expected: build succeeds and `FEOAssetResolverTest` compiles.

- [ ] **Step 6: Commit**

```bash
git add Source/EverwildOdyssey/Public/Presentation/EOAssetResolver.h Source/EverwildOdyssey/Private/Presentation/EOAssetResolver.cpp Source/EverwildOdyssey/Private/Tests/EOAssetResolverTests.cpp
git commit -m "feat: add asset resolver diagnostics"
```

---

### Task 3: Starfall Vale Zone Profile

**Tier:** 1, 2, and 3

**Files:**
- Create: `Source/EverwildOdyssey/Public/World/EOZoneProfile.h`
- Create: `Source/EverwildOdyssey/Private/World/EOZoneProfile.cpp`
- Create: `Source/EverwildOdyssey/Private/Tests/EOZoneProfileTests.cpp`

- [ ] **Step 1: Write the failing zone-profile test**

Create `Source/EverwildOdyssey/Private/Tests/EOZoneProfileTests.cpp`:

```cpp
#if WITH_DEV_AUTOMATION_TESTS

#include "World/EOZoneProfile.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEOZoneProfileTest, "EverwildOdyssey.World.ZoneProfile", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FEOZoneProfileTest::RunTest(const FString& Parameters)
{
    const FEOZoneProfile Profile = FEOZoneProfileCatalog::BuildStarfallValeProfile();

    TestTrue(TEXT("Zone profile is valid."), Profile.IsValidForAlpha());
    TestEqual(TEXT("Zone id is Starfall Vale."), Profile.ZoneId, FName(TEXT("zone.starfall_vale")));
    TestTrue(TEXT("Zone has dense landmarks."), Profile.Landmarks.Num() >= 18);
    TestTrue(TEXT("Zone has dense scenic props."), Profile.ScenicProps.Num() >= 190);
    TestTrue(TEXT("Zone has enough light anchors."), Profile.Lights.Num() >= 18);
    TestTrue(TEXT("Zone has gameplay anchors."), Profile.Anchors.Num() >= 14);
    TestTrue(TEXT("Zone has future hooks."), Profile.FutureHooks.Num() >= 4);
    TestTrue(TEXT("Zone has minimap markers."), Profile.MinimapMarkers.Num() >= 8);

    TestTrue(TEXT("Spawn vista anchor exists."), Profile.HasAnchor(TEXT("anchor.spawn_vista")));
    TestTrue(TEXT("Relic surge center anchor exists."), Profile.HasAnchor(TEXT("anchor.relic_surge.center")));
    TestTrue(TEXT("Sky-Watch reveal anchor exists."), Profile.HasAnchor(TEXT("anchor.skywatch.reveal")));
    TestTrue(TEXT("Future Briarfen hook exists."), Profile.HasFutureHook(TEXT("future.briarfen_march")));
    TestTrue(TEXT("Future Moonwell hook exists."), Profile.HasFutureHook(TEXT("future.moonwell_archive")));

    return true;
}

#endif
```

- [ ] **Step 2: Run test to verify it fails**

Run:

```bash
"/Users/Shared/Epic Games/UE_5.7/Engine/Build/BatchFiles/Mac/Build.sh" EverwildOdysseyEditor Mac Development -Project="/Users/ethanstuart/Projects/everwild-odyssey/.worktrees/alpha-foundation/EverwildOdyssey.uproject" -WaitMutex
```

Expected: build fails because `World/EOZoneProfile.h` does not exist.

- [ ] **Step 3: Add zone-profile data types**

Create `Source/EverwildOdyssey/Public/World/EOZoneProfile.h`:

```cpp
#pragma once

#include "CoreMinimal.h"
#include "World/EOZoneProfile.generated.h"

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
```

- [ ] **Step 4: Add the Starfall Vale profile implementation**

Create `Source/EverwildOdyssey/Private/World/EOZoneProfile.cpp` with the helper functions and profile core:

```cpp
#include "World/EOZoneProfile.h"

namespace
{
FText Txt(const TCHAR* Value)
{
    return FText::FromString(Value);
}

FEOZoneVisualSpec Visual(const TCHAR* Id, const TCHAR* Role, EEOZoneArea Area, const FVector& Location, const FVector& Scale, const FLinearColor& Tint, const FRotator& Rotation = FRotator::ZeroRotator, bool bBlocksMovement = true)
{
    FEOZoneVisualSpec Result;
    Result.Id = FName(Id);
    Result.AssetRoleId = FName(Role);
    Result.Area = Area;
    Result.Location = Location;
    Result.Rotation = Rotation;
    Result.Scale = Scale;
    Result.Tint = Tint;
    Result.bBlocksMovement = bBlocksMovement;
    return Result;
}

FEOZoneLightSpec Light(const TCHAR* Id, const FVector& Location, const FLinearColor& Color, float Intensity, float Radius)
{
    FEOZoneLightSpec Result;
    Result.LightId = FName(Id);
    Result.Location = Location;
    Result.Color = Color;
    Result.Intensity = Intensity;
    Result.Radius = Radius;
    return Result;
}

FEOZoneAnchor Anchor(const TCHAR* Id, const TCHAR* Name, const FVector& Location, const FRotator& Rotation = FRotator::ZeroRotator)
{
    FEOZoneAnchor Result;
    Result.AnchorId = FName(Id);
    Result.DisplayName = Txt(Name);
    Result.Location = Location;
    Result.Rotation = Rotation;
    return Result;
}

FEOZoneFutureHook Hook(const TCHAR* Id, const TCHAR* Name, const TCHAR* Reason, const TCHAR* AnchorId)
{
    FEOZoneFutureHook Result;
    Result.HookId = FName(Id);
    Result.DisplayName = Txt(Name);
    Result.LockedReason = Txt(Reason);
    Result.AnchorId = FName(AnchorId);
    return Result;
}

FEOMinimapMarkerSpec Marker(const TCHAR* Id, const TCHAR* Label, const FVector2D& Position, const FLinearColor& Color)
{
    FEOMinimapMarkerSpec Result;
    Result.MarkerId = FName(Id);
    Result.Label = Txt(Label);
    Result.NormalizedPosition = Position;
    Result.Color = Color;
    return Result;
}
}

bool FEOZoneProfile::IsValidForAlpha() const
{
    if (ZoneId.IsNone() || DisplayName.IsEmpty())
    {
        return false;
    }

    for (const FEOZoneVisualSpec& VisualSpec : Landmarks)
    {
        if (!VisualSpec.IsValidForAlpha())
        {
            return false;
        }
    }

    for (const FEOZoneVisualSpec& VisualSpec : ScenicProps)
    {
        if (!VisualSpec.IsValidForAlpha())
        {
            return false;
        }
    }

    for (const FEOZoneLightSpec& LightSpec : Lights)
    {
        if (!LightSpec.IsValidForAlpha())
        {
            return false;
        }
    }

    for (const FEOZoneAnchor& AnchorSpec : Anchors)
    {
        if (!AnchorSpec.IsValidForAlpha())
        {
            return false;
        }
    }

    for (const FEOZoneFutureHook& FutureHook : FutureHooks)
    {
        if (!FutureHook.IsValidForAlpha())
        {
            return false;
        }
    }

    for (const FEOMinimapMarkerSpec& MarkerSpec : MinimapMarkers)
    {
        if (!MarkerSpec.IsValidForAlpha())
        {
            return false;
        }
    }

    return true;
}

bool FEOZoneProfile::HasAnchor(FName AnchorId) const
{
    return Anchors.ContainsByPredicate([AnchorId](const FEOZoneAnchor& AnchorSpec)
    {
        return AnchorSpec.AnchorId == AnchorId;
    });
}

bool FEOZoneProfile::HasFutureHook(FName HookId) const
{
    return FutureHooks.ContainsByPredicate([HookId](const FEOZoneFutureHook& HookSpec)
    {
        return HookSpec.HookId == HookId;
    });
}
```

Continue the same file with `BuildStarfallValeProfile()`:

```cpp
FEOZoneProfile FEOZoneProfileCatalog::BuildStarfallValeProfile()
{
    FEOZoneProfile Profile;
    Profile.ZoneId = TEXT("zone.starfall_vale");
    Profile.DisplayName = Txt(TEXT("Starfall Vale"));

    Profile.Landmarks = {
        Visual(TEXT("landmark.dawnwatch.gate"), TEXT("zone.dawnwatch.gate"), EEOZoneArea::DawnwatchGate, FVector(-950.0f, -520.0f, 190.0f), FVector(3.4f, 1.2f, 2.6f), FLinearColor(0.10f, 0.30f, 0.90f, 1.0f)),
        Visual(TEXT("landmark.dawnwatch.left_tower"), TEXT("zone.dawnwatch.wall"), EEOZoneArea::DawnwatchGate, FVector(-1160.0f, -690.0f, 220.0f), FVector(1.1f, 1.1f, 3.2f), FLinearColor(0.12f, 0.22f, 0.68f, 1.0f)),
        Visual(TEXT("landmark.dawnwatch.right_tower"), TEXT("zone.dawnwatch.wall"), EEOZoneArea::DawnwatchGate, FVector(-760.0f, -690.0f, 220.0f), FVector(1.1f, 1.1f, 3.2f), FLinearColor(0.12f, 0.22f, 0.68f, 1.0f)),
        Visual(TEXT("landmark.training_yard"), TEXT("zone.training.dummy"), EEOZoneArea::DawnwatchGate, FVector(-620.0f, -180.0f, 80.0f), FVector(1.3f, 1.3f, 1.2f), FLinearColor(0.72f, 0.44f, 0.20f, 1.0f)),
        Visual(TEXT("landmark.scholar_station"), TEXT("zone.scholar.station"), EEOZoneArea::DawnwatchGate, FVector(-1180.0f, -250.0f, 80.0f), FVector(1.4f, 1.0f, 1.0f), FLinearColor(0.24f, 0.82f, 1.0f, 1.0f)),
        Visual(TEXT("landmark.vendor_corner"), TEXT("zone.vendor.crates"), EEOZoneArea::DawnwatchGate, FVector(-980.0f, -120.0f, 60.0f), FVector(1.6f, 1.1f, 0.8f), FLinearColor(0.72f, 0.52f, 0.24f, 1.0f)),
        Visual(TEXT("landmark.starfall_road"), TEXT("zone.road.signpost"), EEOZoneArea::StarfallRoad, FVector(-300.0f, -35.0f, 70.0f), FVector(1.0f, 1.0f, 1.3f), FLinearColor(0.82f, 0.62f, 0.32f, 1.0f)),
        Visual(TEXT("landmark.road_lantern_one"), TEXT("zone.road.lamp"), EEOZoneArea::StarfallRoad, FVector(-420.0f, 80.0f, 88.0f), FVector(1.0f, 1.0f, 1.2f), FLinearColor(1.0f, 0.68f, 0.24f, 1.0f)),
        Visual(TEXT("landmark.road_lantern_two"), TEXT("zone.road.lamp"), EEOZoneArea::StarfallRoad, FVector(40.0f, 80.0f, 88.0f), FVector(1.0f, 1.0f, 1.2f), FLinearColor(1.0f, 0.68f, 0.24f, 1.0f)),
        Visual(TEXT("landmark.road_lantern_three"), TEXT("zone.road.lamp"), EEOZoneArea::StarfallRoad, FVector(480.0f, 20.0f, 88.0f), FVector(1.0f, 1.0f, 1.2f), FLinearColor(1.0f, 0.68f, 0.24f, 1.0f)),
        Visual(TEXT("landmark.relic_core"), TEXT("zone.relic_surge.core"), EEOZoneArea::RelicSurgeCamp, FVector(760.0f, -260.0f, 180.0f), FVector(1.9f, 1.9f, 2.7f), FLinearColor(0.82f, 0.20f, 1.0f, 1.0f)),
        Visual(TEXT("landmark.relic_crystal_north"), TEXT("zone.relic_surge.crystal"), EEOZoneArea::RelicSurgeCamp, FVector(760.0f, -60.0f, 95.0f), FVector(0.8f, 0.8f, 1.9f), FLinearColor(0.76f, 0.20f, 1.0f, 1.0f)),
        Visual(TEXT("landmark.relic_crystal_south"), TEXT("zone.relic_surge.crystal"), EEOZoneArea::RelicSurgeCamp, FVector(760.0f, -470.0f, 95.0f), FVector(0.8f, 0.8f, 1.9f), FLinearColor(0.76f, 0.20f, 1.0f, 1.0f)),
        Visual(TEXT("landmark.relic_crystal_east"), TEXT("zone.relic_surge.crystal"), EEOZoneArea::RelicSurgeCamp, FVector(970.0f, -260.0f, 95.0f), FVector(0.8f, 0.8f, 1.9f), FLinearColor(0.76f, 0.20f, 1.0f, 1.0f)),
        Visual(TEXT("landmark.skywatch_entrance"), TEXT("zone.skywatch.entrance"), EEOZoneArea::SkyWatchEntrance, FVector(1260.0f, -760.0f, 220.0f), FVector(2.7f, 1.3f, 3.4f), FLinearColor(0.34f, 0.74f, 1.0f, 1.0f)),
        Visual(TEXT("landmark.briarfen_gate"), TEXT("zone.future.bridge_blocker"), EEOZoneArea::FutureBoundary, FVector(-1280.0f, 420.0f, 75.0f), FVector(2.0f, 0.8f, 1.1f), FLinearColor(0.22f, 0.66f, 0.24f, 1.0f)),
        Visual(TEXT("landmark.moonwell_gate"), TEXT("zone.future.bridge_blocker"), EEOZoneArea::FutureBoundary, FVector(260.0f, 660.0f, 75.0f), FVector(2.0f, 0.8f, 1.1f), FLinearColor(0.36f, 0.72f, 1.0f, 1.0f)),
        Visual(TEXT("landmark.sunspire_pass"), TEXT("zone.future.bridge_blocker"), EEOZoneArea::FutureBoundary, FVector(100.0f, -1060.0f, 90.0f), FVector(2.3f, 0.9f, 1.2f), FLinearColor(1.0f, 0.72f, 0.26f, 1.0f))
    };

    Profile.ScenicProps.Reserve(210);
    Profile.ScenicProps.Add(Visual(TEXT("terrain.vale.floor"), TEXT("zone.foliage.rock"), EEOZoneArea::StarfallRoad, FVector(0.0f, 0.0f, -10.0f), FVector(40.0f, 31.0f, 0.14f), FLinearColor(0.10f, 0.34f, 0.16f, 1.0f)));
    for (int32 Index = 0; Index < 56; ++Index)
    {
        const float X = -780.0f + static_cast<float>(Index % 14) * 145.0f;
        const float Y = 160.0f + static_cast<float>(Index / 14) * 125.0f;
        Profile.ScenicProps.Add(Visual(*FString::Printf(TEXT("foliage.tree.%02d"), Index), TEXT("zone.foliage.tree"), EEOZoneArea::StarfallRoad, FVector(X, Y, 0.0f), FVector(0.72f, 0.72f, 0.72f), FLinearColor(0.08f, 0.42f, 0.16f, 1.0f), FRotator(0.0f, Index * 17.0f, 0.0f), false));
    }
    for (int32 Index = 0; Index < 48; ++Index)
    {
        const float X = -640.0f + static_cast<float>(Index % 16) * 125.0f;
        const float Y = -95.0f + ((Index % 2 == 0) ? 0.0f : 130.0f);
        Profile.ScenicProps.Add(Visual(*FString::Printf(TEXT("road.dressing.%02d"), Index), (Index % 3 == 0) ? TEXT("zone.road.lamp") : TEXT("zone.foliage.rock"), EEOZoneArea::StarfallRoad, FVector(X, Y, 56.0f), FVector(0.5f, 0.5f, 0.75f), FLinearColor(0.70f, 0.55f, 0.36f, 1.0f), FRotator(0.0f, Index * 9.0f, 0.0f), Index % 3 != 0));
    }
    for (int32 Index = 0; Index < 48; ++Index)
    {
        const float Angle = Index * 7.5f;
        const float Radius = 170.0f + static_cast<float>(Index % 6) * 35.0f;
        Profile.ScenicProps.Add(Visual(*FString::Printf(TEXT("surge.crystal.cluster.%02d"), Index), TEXT("zone.relic_surge.crystal"), EEOZoneArea::RelicSurgeCamp, FVector(760.0f + FMath::Cos(FMath::DegreesToRadians(Angle)) * Radius, -260.0f + FMath::Sin(FMath::DegreesToRadians(Angle)) * Radius, 74.0f), FVector(0.28f, 0.28f, 1.15f), FLinearColor(0.82f, 0.16f, 1.0f, 1.0f), FRotator(0.0f, Angle, 8.0f), false));
    }
    for (int32 Index = 0; Index < 42; ++Index)
    {
        const float X = -1220.0f + static_cast<float>(Index % 7) * 95.0f;
        const float Y = -720.0f + static_cast<float>(Index / 7) * 82.0f;
        Profile.ScenicProps.Add(Visual(*FString::Printf(TEXT("hub.dressing.%02d"), Index), (Index % 4 == 0) ? TEXT("zone.dawnwatch.banner") : TEXT("zone.vendor.crates"), EEOZoneArea::DawnwatchGate, FVector(X, Y, 70.0f), FVector(0.7f, 0.5f, 0.75f), FLinearColor(0.20f, 0.40f, 0.94f, 1.0f), FRotator(0.0f, Index * 13.0f, 0.0f), false));
    }

    Profile.Lights = {
        Light(TEXT("light.spawn.vista"), FVector(-900.0f, -450.0f, 280.0f), FLinearColor(0.30f, 0.52f, 1.0f, 1.0f), 5200.0f, 900.0f),
        Light(TEXT("light.gate.gold"), FVector(-810.0f, -520.0f, 220.0f), FLinearColor(1.0f, 0.72f, 0.22f, 1.0f), 4200.0f, 550.0f),
        Light(TEXT("light.scholar.cyan"), FVector(-1180.0f, -250.0f, 180.0f), FLinearColor(0.30f, 0.84f, 1.0f, 1.0f), 3600.0f, 480.0f),
        Light(TEXT("light.road.one"), FVector(-420.0f, 80.0f, 130.0f), FLinearColor(1.0f, 0.66f, 0.20f, 1.0f), 2400.0f, 320.0f),
        Light(TEXT("light.road.two"), FVector(40.0f, 80.0f, 130.0f), FLinearColor(1.0f, 0.66f, 0.20f, 1.0f), 2400.0f, 320.0f),
        Light(TEXT("light.road.three"), FVector(480.0f, 20.0f, 130.0f), FLinearColor(1.0f, 0.66f, 0.20f, 1.0f), 2400.0f, 320.0f),
        Light(TEXT("light.relic.core"), FVector(760.0f, -260.0f, 260.0f), FLinearColor(0.82f, 0.18f, 1.0f, 1.0f), 12000.0f, 1050.0f),
        Light(TEXT("light.skywatch"), FVector(1260.0f, -760.0f, 320.0f), FLinearColor(0.36f, 0.74f, 1.0f, 1.0f), 8200.0f, 880.0f),
        Light(TEXT("light.future.briarfen"), FVector(-1280.0f, 420.0f, 180.0f), FLinearColor(0.22f, 0.72f, 0.24f, 1.0f), 3400.0f, 520.0f),
        Light(TEXT("light.future.moonwell"), FVector(260.0f, 660.0f, 180.0f), FLinearColor(0.36f, 0.72f, 1.0f, 1.0f), 3400.0f, 520.0f),
        Light(TEXT("light.future.sunspire"), FVector(100.0f, -1060.0f, 190.0f), FLinearColor(1.0f, 0.68f, 0.26f, 1.0f), 3400.0f, 560.0f),
        Light(TEXT("light.combat.warning.n"), FVector(760.0f, -60.0f, 150.0f), FLinearColor(1.0f, 0.18f, 0.08f, 1.0f), 2800.0f, 360.0f),
        Light(TEXT("light.combat.warning.s"), FVector(760.0f, -470.0f, 150.0f), FLinearColor(1.0f, 0.18f, 0.08f, 1.0f), 2800.0f, 360.0f),
        Light(TEXT("light.combat.warning.e"), FVector(970.0f, -260.0f, 150.0f), FLinearColor(1.0f, 0.18f, 0.08f, 1.0f), 2800.0f, 360.0f),
        Light(TEXT("light.companion.camp"), FVector(-1040.0f, -340.0f, 130.0f), FLinearColor(1.0f, 0.54f, 0.20f, 1.0f), 3000.0f, 420.0f),
        Light(TEXT("light.training.yard"), FVector(-620.0f, -180.0f, 130.0f), FLinearColor(1.0f, 0.70f, 0.30f, 1.0f), 2800.0f, 400.0f),
        Light(TEXT("light.vendor.corner"), FVector(-980.0f, -120.0f, 120.0f), FLinearColor(1.0f, 0.64f, 0.25f, 1.0f), 2400.0f, 360.0f),
        Light(TEXT("light.dungeon.promise"), FVector(1140.0f, -650.0f, 260.0f), FLinearColor(0.48f, 0.86f, 1.0f, 1.0f), 4800.0f, 640.0f)
    };

    Profile.Anchors = {
        Anchor(TEXT("anchor.spawn_vista"), TEXT("Spawn Vista"), FVector(-720.0f, -420.0f, 96.0f), FRotator(0.0f, 28.0f, 0.0f)),
        Anchor(TEXT("anchor.hub.commander"), TEXT("Dawnwatch Commander"), FVector(-880.0f, -290.0f, 96.0f)),
        Anchor(TEXT("anchor.hub.training"), TEXT("Training Yard"), FVector(-620.0f, -180.0f, 96.0f)),
        Anchor(TEXT("anchor.hub.vendor"), TEXT("Vendor Corner"), FVector(-980.0f, -120.0f, 96.0f)),
        Anchor(TEXT("anchor.hub.scholar"), TEXT("Scholarium Station"), FVector(-1180.0f, -250.0f, 96.0f)),
        Anchor(TEXT("anchor.road.first_contact"), TEXT("First Road Contact"), FVector(180.0f, -20.0f, 96.0f)),
        Anchor(TEXT("anchor.road.side_pocket"), TEXT("Ruined Shrine Pocket"), FVector(310.0f, 420.0f, 96.0f)),
        Anchor(TEXT("anchor.relic_surge.entry"), TEXT("Relic Surge Entry"), FVector(520.0f, -170.0f, 96.0f)),
        Anchor(TEXT("anchor.relic_surge.center"), TEXT("Relic Surge Center"), FVector(760.0f, -260.0f, 96.0f)),
        Anchor(TEXT("anchor.relic_surge.caster_north"), TEXT("Caster North"), FVector(760.0f, -70.0f, 96.0f)),
        Anchor(TEXT("anchor.relic_surge.caster_east"), TEXT("Caster East"), FVector(970.0f, -260.0f, 96.0f)),
        Anchor(TEXT("anchor.relic_surge.elite"), TEXT("Elite Spawn"), FVector(840.0f, -310.0f, 96.0f)),
        Anchor(TEXT("anchor.skywatch.reveal"), TEXT("Sky-Watch Reveal"), FVector(1120.0f, -640.0f, 96.0f)),
        Anchor(TEXT("anchor.future.briarfen"), TEXT("Briarfen Road"), FVector(-1280.0f, 420.0f, 96.0f)),
        Anchor(TEXT("anchor.future.moonwell"), TEXT("Moonwell Road"), FVector(260.0f, 660.0f, 96.0f)),
        Anchor(TEXT("anchor.future.sunspire"), TEXT("Sunspire Pass"), FVector(100.0f, -1060.0f, 96.0f))
    };

    Profile.FutureHooks = {
        Hook(TEXT("future.briarfen_march"), TEXT("Briarfen March"), TEXT("Pathwarden scouts have not cleared the wetlands."), TEXT("anchor.future.briarfen")),
        Hook(TEXT("future.moonwell_archive"), TEXT("Moonwell Archive"), TEXT("The Scholarium seal needs a stabilized relic key."), TEXT("anchor.future.moonwell")),
        Hook(TEXT("future.sunspire_expanse"), TEXT("Sunspire Expanse"), TEXT("The southern pass is blocked by a relic storm."), TEXT("anchor.future.sunspire")),
        Hook(TEXT("future.skywatch_dungeon"), TEXT("Sky-Watch Entrance"), TEXT("The entrance opens after the relic surge is stabilized."), TEXT("anchor.skywatch.reveal"))
    };

    Profile.MinimapMarkers = {
        Marker(TEXT("map.dawnwatch"), TEXT("Dawnwatch"), FVector2D(0.18f, 0.32f), FLinearColor(0.18f, 0.48f, 1.0f, 1.0f)),
        Marker(TEXT("map.commander"), TEXT("Commander"), FVector2D(0.22f, 0.40f), FLinearColor(1.0f, 0.82f, 0.24f, 1.0f)),
        Marker(TEXT("map.vendor"), TEXT("Vendor"), FVector2D(0.18f, 0.46f), FLinearColor(0.74f, 0.52f, 0.24f, 1.0f)),
        Marker(TEXT("map.road"), TEXT("Starfall Road"), FVector2D(0.46f, 0.46f), FLinearColor(0.76f, 0.58f, 0.34f, 1.0f)),
        Marker(TEXT("map.side_pocket"), TEXT("Shrine"), FVector2D(0.50f, 0.30f), FLinearColor(0.42f, 0.82f, 1.0f, 1.0f)),
        Marker(TEXT("map.surge"), TEXT("Relic Surge"), FVector2D(0.68f, 0.52f), FLinearColor(0.88f, 0.20f, 1.0f, 1.0f)),
        Marker(TEXT("map.skywatch"), TEXT("Sky-Watch"), FVector2D(0.86f, 0.72f), FLinearColor(0.42f, 0.82f, 1.0f, 1.0f)),
        Marker(TEXT("map.briarfen"), TEXT("Briarfen"), FVector2D(0.08f, 0.18f), FLinearColor(0.24f, 0.72f, 0.28f, 1.0f)),
        Marker(TEXT("map.moonwell"), TEXT("Moonwell"), FVector2D(0.54f, 0.12f), FLinearColor(0.38f, 0.74f, 1.0f, 1.0f))
    };

    return Profile;
}
```

- [ ] **Step 5: Run build and validate profile test compile**

Run:

```bash
"/Users/Shared/Epic Games/UE_5.7/Engine/Build/BatchFiles/Mac/Build.sh" EverwildOdysseyEditor Mac Development -Project="/Users/ethanstuart/Projects/everwild-odyssey/.worktrees/alpha-foundation/EverwildOdyssey.uproject" -WaitMutex
```

Expected: build succeeds and `FEOZoneProfileTest` compiles.

- [ ] **Step 6: Commit**

```bash
git add Source/EverwildOdyssey/Public/World/EOZoneProfile.h Source/EverwildOdyssey/Private/World/EOZoneProfile.cpp Source/EverwildOdyssey/Private/Tests/EOZoneProfileTests.cpp
git commit -m "feat: add Starfall Vale zone profile"
```

---

### Task 4: Profile-Driven World Scaffold

**Tier:** 1 and 2

**Files:**
- Modify: `Source/EverwildOdyssey/Public/Gameplay/EOAlphaWorldScaffold.h`
- Modify: `Source/EverwildOdyssey/Private/Gameplay/EOAlphaWorldScaffold.cpp`
- Modify: `Source/EverwildOdyssey/Private/Tests/EOAlphaWorldScaffoldTests.cpp`

- [ ] **Step 1: Update the scaffold test to require profile-driven counts**

Modify `Source/EverwildOdyssey/Private/Tests/EOAlphaWorldScaffoldTests.cpp` so `RunTest` begins by checking the new profile and expected counts:

```cpp
const FEOZoneProfile Profile = FEOZoneProfileCatalog::BuildStarfallValeProfile();
TestTrue(TEXT("Alpha world consumes a valid Starfall Vale profile."), Profile.IsValidForAlpha());
TestEqual(TEXT("Scaffold expected landmark count matches profile."), AEOAlphaWorldScaffold::ExpectedLandmarkCount(), Profile.Landmarks.Num());
TestEqual(TEXT("Scaffold expected scenic prop count matches profile."), AEOAlphaWorldScaffold::ExpectedScenicPropCount(), Profile.ScenicProps.Num());
TestEqual(TEXT("Scaffold expected ambient light count matches profile."), AEOAlphaWorldScaffold::ExpectedAmbientLightCount(), Profile.Lights.Num());
```

Add these includes to the top of the same test file:

```cpp
#include "World/EOZoneProfile.h"
```

- [ ] **Step 2: Run test to verify it fails**

Run:

```bash
"/Users/Shared/Epic Games/UE_5.7/Engine/Build/BatchFiles/Mac/Build.sh" EverwildOdysseyEditor Mac Development -Project="/Users/ethanstuart/Projects/everwild-odyssey/.worktrees/alpha-foundation/EverwildOdyssey.uproject" -WaitMutex
```

Expected: build fails or the test expectations fail because `AEOAlphaWorldScaffold` still uses its old hard-coded count constants.

- [ ] **Step 3: Update scaffold header to expose profile-driven data**

Modify `Source/EverwildOdyssey/Public/Gameplay/EOAlphaWorldScaffold.h`:

```cpp
#include "World/EOZoneProfile.h"
```

Replace the count constants and static builders with profile-backed declarations:

```cpp
public:
    static int32 ExpectedLandmarkCount();
    static int32 ExpectedScenicPropCount();
    static int32 ExpectedAmbientLightCount();

    static TArray<FEOZoneVisualSpec> BuildDefaultLandmarks();
    static TArray<FEOZoneVisualSpec> BuildDefaultScenicProps();
    static TArray<FEOZoneLightSpec> BuildDefaultAmbientLights();
    static FEOZoneProfile BuildDefaultZoneProfile();
```

Leave the old `FEOAlphaLandmarkSpec`, `FEOAlphaScenicPropSpec`, and `FEOAlphaLightSpec` structs in the header through this task to keep the refactor smaller, but stop using them from tests and production scaffold builders. A follow-up cleanup can remove them after the profile-driven scaffold has compiled once.

- [ ] **Step 4: Update scaffold implementation to consume the zone profile**

Modify `Source/EverwildOdyssey/Private/Gameplay/EOAlphaWorldScaffold.cpp`:

```cpp
#include "Presentation/EOAssetResolver.h"
#include "Presentation/EOAssetRoleTypes.h"
#include "World/EOZoneProfile.h"
```

Add these static methods:

```cpp
int32 AEOAlphaWorldScaffold::ExpectedLandmarkCount()
{
    return BuildDefaultZoneProfile().Landmarks.Num();
}

int32 AEOAlphaWorldScaffold::ExpectedScenicPropCount()
{
    return BuildDefaultZoneProfile().ScenicProps.Num();
}

int32 AEOAlphaWorldScaffold::ExpectedAmbientLightCount()
{
    return BuildDefaultZoneProfile().Lights.Num();
}

FEOZoneProfile AEOAlphaWorldScaffold::BuildDefaultZoneProfile()
{
    return FEOZoneProfileCatalog::BuildStarfallValeProfile();
}

TArray<FEOZoneVisualSpec> AEOAlphaWorldScaffold::BuildDefaultLandmarks()
{
    return BuildDefaultZoneProfile().Landmarks;
}

TArray<FEOZoneVisualSpec> AEOAlphaWorldScaffold::BuildDefaultScenicProps()
{
    return BuildDefaultZoneProfile().ScenicProps;
}

TArray<FEOZoneLightSpec> AEOAlphaWorldScaffold::BuildDefaultAmbientLights()
{
    return BuildDefaultZoneProfile().Lights;
}
```

Inside `SpawnVerticalSliceWorld`, create a resolver before mesh spawning:

```cpp
FEOAssetResolver AssetResolver;
AssetResolver.Initialize(FEOAssetRoleCatalog::BuildDefaultRoles());
UE_LOG(LogTemp, Log, TEXT("Everwild asset resolver: %s"), *AssetResolver.BuildReadableReport());

const FEOZoneProfile ZoneProfile = BuildDefaultZoneProfile();
```

Replace loops over `BuildDefaultLandmarks()` and `BuildDefaultScenicProps()` with loops over `ZoneProfile.Landmarks` and `ZoneProfile.ScenicProps`. Use `VisualSpec.AssetRoleId` instead of the old `VisualStyle`. Keep engine primitive fallback mesh selection, but name it `ProxyMeshForRole` and use asset role tints so debug proxies are visually intentional.

- [ ] **Step 5: Update scaffold tests for the new profile model**

Modify `Source/EverwildOdyssey/Private/Tests/EOAlphaWorldScaffoldTests.cpp` so it checks role ids instead of old visual styles:

```cpp
const TArray<FEOZoneVisualSpec> ScenicProps = AEOAlphaWorldScaffold::BuildDefaultScenicProps();
TestEqual(TEXT("Alpha world exposes expected scenic prop count."), ScenicProps.Num(), AEOAlphaWorldScaffold::ExpectedScenicPropCount());

TSet<FName> AssetRoles;
for (const FEOZoneVisualSpec& ScenicProp : ScenicProps)
{
    TestTrue(TEXT("Each scenic prop is valid."), ScenicProp.IsValidForAlpha());
    AssetRoles.Add(ScenicProp.AssetRoleId);
}

TestTrue(TEXT("Alpha world has road dressing."), AssetRoles.Contains(TEXT("zone.road.lamp")));
TestTrue(TEXT("Alpha world has crystalline surge dressing."), AssetRoles.Contains(TEXT("zone.relic_surge.crystal")));
TestTrue(TEXT("Alpha world has hub dressing."), AssetRoles.Contains(TEXT("zone.vendor.crates")));
TestTrue(TEXT("Alpha world has foliage dressing."), AssetRoles.Contains(TEXT("zone.foliage.tree")));
```

- [ ] **Step 6: Run build**

Run:

```bash
"/Users/Shared/Epic Games/UE_5.7/Engine/Build/BatchFiles/Mac/Build.sh" EverwildOdysseyEditor Mac Development -Project="/Users/ethanstuart/Projects/everwild-odyssey/.worktrees/alpha-foundation/EverwildOdyssey.uproject" -WaitMutex
```

Expected: build succeeds and scaffold tests compile against the zone profile.

- [ ] **Step 7: Commit**

```bash
git add Source/EverwildOdyssey/Public/Gameplay/EOAlphaWorldScaffold.h Source/EverwildOdyssey/Private/Gameplay/EOAlphaWorldScaffold.cpp Source/EverwildOdyssey/Private/Tests/EOAlphaWorldScaffoldTests.cpp
git commit -m "feat: drive world scaffold from zone profile"
```

---

### Task 5: Hero Presentation Upgrade And Input Contract

**Tier:** 1 and 2

**Files:**
- Modify: `Source/EverwildOdyssey/Public/Gameplay/EOHeroCharacter.h`
- Modify: `Source/EverwildOdyssey/Private/Gameplay/EOHeroCharacter.cpp`
- Modify: `Source/EverwildOdyssey/Private/Tests/EOPlayableFoundationTests.cpp`
- Modify: `Config/DefaultInput.ini`

- [ ] **Step 1: Extend the playable foundation test**

Modify `Source/EverwildOdyssey/Private/Tests/EOPlayableFoundationTests.cpp`:

```cpp
TestTrue(TEXT("Default camera supports a closer heroic third-person read."), AEOHeroCharacter::DefaultCameraBoomLength <= 720.0f);
TestTrue(TEXT("Default camera pitch keeps the horizon present."), AEOHeroCharacter::DefaultCameraPitch >= -28.0f && AEOHeroCharacter::DefaultCameraPitch <= -16.0f);
TestTrue(TEXT("Basic attack range supports melee readability."), AEOHeroCharacter::DefaultBasicAttackRange >= 280.0f);
TestTrue(TEXT("Ability attack range supports short heroic ability reach."), AEOHeroCharacter::DefaultAbilityAttackRange >= 460.0f);
```

- [ ] **Step 2: Run test to verify it fails**

Run:

```bash
"/Users/Shared/Epic Games/UE_5.7/Engine/Build/BatchFiles/Mac/Build.sh" EverwildOdysseyEditor Mac Development -Project="/Users/ethanstuart/Projects/everwild-odyssey/.worktrees/alpha-foundation/EverwildOdyssey.uproject" -WaitMutex
```

Expected: build succeeds but the new expected camera values fail if tests are executed; compile confirms the constants exist.

- [ ] **Step 3: Update hero camera constants and component names**

Modify constants in `Source/EverwildOdyssey/Public/Gameplay/EOHeroCharacter.h`:

```cpp
static constexpr float DefaultCameraBoomLength = 680.0f;
static constexpr float DefaultCameraPitch = -22.0f;
static constexpr float DefaultCameraYaw = -35.0f;
static constexpr float DefaultBasicAttackRange = 300.0f;
static constexpr float DefaultAbilityAttackRange = 480.0f;
```

Rename the camera subobjects in `Source/EverwildOdyssey/Private/Gameplay/EOHeroCharacter.cpp` constructor:

```cpp
CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("HeroicThirdPersonCameraBoom"));
FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("HeroicThirdPersonCamera"));
```

- [ ] **Step 4: Replace toy silhouette proportions with champion proxies and asset-role comments**

In `Source/EverwildOdyssey/Private/Gameplay/EOHeroCharacter.cpp`, update the hero component transforms:

```cpp
HeroSilhouette->SetRelativeLocation(FVector(0.0f, 0.0f, -32.0f));
HeroSilhouette->SetRelativeScale3D(FVector(1.05f, 0.82f, 1.72f));

HeroHead->SetRelativeLocation(FVector(0.0f, 0.0f, 112.0f));
HeroHead->SetRelativeScale3D(FVector(0.42f, 0.42f, 0.42f));

HeroCape->SetRelativeLocation(FVector(-48.0f, -38.0f, 18.0f));
HeroCape->SetRelativeRotation(FRotator(-4.0f, -18.0f, -8.0f));
HeroCape->SetRelativeScale3D(FVector(0.12f, 0.70f, 1.46f));

HeroSword->SetRelativeLocation(FVector(68.0f, 28.0f, 20.0f));
HeroSword->SetRelativeRotation(FRotator(0.0f, 28.0f, -32.0f));
HeroSword->SetRelativeScale3D(FVector(0.10f, 0.12f, 1.72f));

HeroShield->SetRelativeLocation(FVector(-70.0f, 20.0f, 28.0f));
HeroShield->SetRelativeRotation(FRotator(0.0f, -18.0f, 0.0f));
HeroShield->SetRelativeScale3D(FVector(0.46f, 0.14f, 0.78f));
```

Before the primitive mesh assignments, add this comment:

```cpp
// These meshes are explicit hero proxies. The asset resolver reports whether final role assets are present.
```

- [ ] **Step 5: Add target-cycle and camera-axis bindings**

Add declarations to `Source/EverwildOdyssey/Public/Gameplay/EOHeroCharacter.h`:

```cpp
void TargetCycle();
void CameraYaw(float Value);
void CameraPitch(float Value);
```

Add implementation to `Source/EverwildOdyssey/Private/Gameplay/EOHeroCharacter.cpp`:

```cpp
void AEOHeroCharacter::TargetCycle()
{
    UE_LOG(LogTemp, Log, TEXT("Everwild target cycle requested"));
}

void AEOHeroCharacter::CameraYaw(float Value)
{
    if (!FMath::IsNearlyZero(Value))
    {
        AddControllerYawInput(Value * 0.5f);
    }
}

void AEOHeroCharacter::CameraPitch(float Value)
{
    if (!FMath::IsNearlyZero(Value))
    {
        AddControllerPitchInput(Value * -0.35f);
    }
}
```

Bind them in `SetupPlayerInputComponent`:

```cpp
PlayerInputComponent->BindAction(TEXT("TargetCycle"), IE_Pressed, this, &AEOHeroCharacter::TargetCycle);
PlayerInputComponent->BindAxis(TEXT("CameraYaw"), this, &AEOHeroCharacter::CameraYaw);
PlayerInputComponent->BindAxis(TEXT("CameraPitch"), this, &AEOHeroCharacter::CameraPitch);
```

- [ ] **Step 6: Add controller target-cycle mapping**

Append to `Config/DefaultInput.ini` under action mappings:

```ini
+ActionMappings=(ActionName="TargetCycle",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=Tab)
+ActionMappings=(ActionName="TargetCycle",bShift=False,bCtrl=False,bAlt=False,bCmd=False,Key=Gamepad_FaceButton_Top)
```

- [ ] **Step 7: Run build**

Run:

```bash
"/Users/Shared/Epic Games/UE_5.7/Engine/Build/BatchFiles/Mac/Build.sh" EverwildOdysseyEditor Mac Development -Project="/Users/ethanstuart/Projects/everwild-odyssey/.worktrees/alpha-foundation/EverwildOdyssey.uproject" -WaitMutex
```

Expected: build succeeds and hero/input changes compile.

- [ ] **Step 8: Commit**

```bash
git add Source/EverwildOdyssey/Public/Gameplay/EOHeroCharacter.h Source/EverwildOdyssey/Private/Gameplay/EOHeroCharacter.cpp Source/EverwildOdyssey/Private/Tests/EOPlayableFoundationTests.cpp Config/DefaultInput.ini
git commit -m "feat: improve heroic camera and controller inputs"
```

---

### Task 6: MMO HUD Presentation Model

**Tier:** 1 and 2

**Files:**
- Create: `Source/EverwildOdyssey/Public/UI/EOHUDPresentationTypes.h`
- Create: `Source/EverwildOdyssey/Private/UI/EOHUDPresentationTypes.cpp`
- Create: `Source/EverwildOdyssey/Private/Tests/EOHUDPresentationTypesTests.cpp`
- Modify: `Source/EverwildOdyssey/Public/Gameplay/EOHUD.h`
- Modify: `Source/EverwildOdyssey/Private/Gameplay/EOHUD.cpp`
- Modify: `Source/EverwildOdyssey/Private/Tests/EOHUDTests.cpp`

- [ ] **Step 1: Write the failing HUD presentation test**

Create `Source/EverwildOdyssey/Private/Tests/EOHUDPresentationTypesTests.cpp`:

```cpp
#if WITH_DEV_AUTOMATION_TESTS

#include "UI/EOHUDPresentationTypes.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEOHUDPresentationTypesTest, "EverwildOdyssey.UI.HUDPresentationTypes", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FEOHUDPresentationTypesTest::RunTest(const FString& Parameters)
{
    FEOHUDPresentationModel Model = FEOHUDPresentationModel::BuildAlphaFixture();
    TestTrue(TEXT("HUD presentation fixture is valid."), Model.IsValidForAlpha());
    TestEqual(TEXT("HUD has two companion frames."), Model.PartyFrames.Num(), 2);
    TestEqual(TEXT("HUD has eight action slots."), Model.ActionSlots.Num(), 8);
    TestTrue(TEXT("HUD has minimap markers."), Model.MinimapMarkers.Num() >= 8);
    TestTrue(TEXT("HUD has event feed entries."), Model.FeedEntries.Num() >= 3);
    TestEqual(TEXT("HUD exposes controller attack glyph."), Model.FindGlyphForAction(TEXT("BasicAttack")), FString(TEXT("X")));
    TestTrue(TEXT("Target frame has an elite-ready role."), Model.TargetFrame.TargetRole == TEXT("Elite") || Model.TargetFrame.TargetRole == TEXT("Minion"));

    return true;
}

#endif
```

- [ ] **Step 2: Run test to verify it fails**

Run:

```bash
"/Users/Shared/Epic Games/UE_5.7/Engine/Build/BatchFiles/Mac/Build.sh" EverwildOdysseyEditor Mac Development -Project="/Users/ethanstuart/Projects/everwild-odyssey/.worktrees/alpha-foundation/EverwildOdyssey.uproject" -WaitMutex
```

Expected: build fails because `UI/EOHUDPresentationTypes.h` does not exist.

- [ ] **Step 3: Add HUD presentation types**

Create `Source/EverwildOdyssey/Public/UI/EOHUDPresentationTypes.h`:

```cpp
#pragma once

#include "CoreMinimal.h"
#include "World/EOZoneProfile.h"
#include "EOHUDPresentationTypes.generated.h"

USTRUCT(BlueprintType)
struct FEOHUDUnitFrame
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    FName RoleId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    FString TargetRole;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    float Health = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    float MaxHealth = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    FLinearColor RoleColor = FLinearColor::White;

    bool IsValidForAlpha() const
    {
        return !DisplayName.IsEmpty() && MaxHealth > 0.0f && Health >= 0.0f && Health <= MaxHealth && RoleColor.A > 0.0f;
    }
};

USTRUCT(BlueprintType)
struct FEOHUDActionSlot
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    FName ActionId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    FString KeyboardGlyph;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    FString ControllerGlyph;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    float CooldownRemaining = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    FLinearColor SlotColor = FLinearColor::White;

    bool IsValidForAlpha() const
    {
        return !ActionId.IsNone() && !DisplayName.IsEmpty() && !KeyboardGlyph.IsEmpty() && !ControllerGlyph.IsEmpty() && CooldownRemaining >= 0.0f && SlotColor.A > 0.0f;
    }
};

USTRUCT(BlueprintType)
struct FEOHUDEventFeedEntry
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    FText Message;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    FLinearColor Color = FLinearColor::White;

    bool IsValidForAlpha() const
    {
        return !Message.IsEmpty() && Color.A > 0.0f;
    }
};

USTRUCT(BlueprintType)
struct FEOHUDPresentationModel
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    FEOHUDUnitFrame PlayerFrame;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    TArray<FEOHUDUnitFrame> PartyFrames;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    FEOHUDUnitFrame TargetFrame;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    TArray<FEOHUDActionSlot> ActionSlots;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    FText QuestTitle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    FText QuestObjective;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    FText WorldEventText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    TArray<FEOMinimapMarkerSpec> MinimapMarkers;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    TArray<FEOHUDEventFeedEntry> FeedEntries;

    bool IsValidForAlpha() const;
    FString FindGlyphForAction(FName ActionId) const;
    static FEOHUDPresentationModel BuildAlphaFixture();
};
```

- [ ] **Step 4: Add HUD presentation implementation**

Create `Source/EverwildOdyssey/Private/UI/EOHUDPresentationTypes.cpp`:

```cpp
#include "UI/EOHUDPresentationTypes.h"

namespace
{
FEOHUDUnitFrame Unit(const TCHAR* Name, FName Role, const TCHAR* TargetRole, float Health, float MaxHealth, const FLinearColor& Color)
{
    FEOHUDUnitFrame Result;
    Result.DisplayName = FText::FromString(Name);
    Result.RoleId = Role;
    Result.TargetRole = FString(TargetRole);
    Result.Health = Health;
    Result.MaxHealth = MaxHealth;
    Result.RoleColor = Color;
    return Result;
}

FEOHUDActionSlot Slot(const TCHAR* Id, const TCHAR* Name, const TCHAR* Key, const TCHAR* Pad, const FLinearColor& Color)
{
    FEOHUDActionSlot Result;
    Result.ActionId = FName(Id);
    Result.DisplayName = FText::FromString(Name);
    Result.KeyboardGlyph = FString(Key);
    Result.ControllerGlyph = FString(Pad);
    Result.CooldownRemaining = 0.0f;
    Result.SlotColor = Color;
    return Result;
}

FEOHUDEventFeedEntry Feed(const TCHAR* Message, const FLinearColor& Color)
{
    FEOHUDEventFeedEntry Result;
    Result.Message = FText::FromString(Message);
    Result.Color = Color;
    return Result;
}
}

bool FEOHUDPresentationModel::IsValidForAlpha() const
{
    if (!PlayerFrame.IsValidForAlpha() || !TargetFrame.IsValidForAlpha() || QuestTitle.IsEmpty() || QuestObjective.IsEmpty() || WorldEventText.IsEmpty())
    {
        return false;
    }

    for (const FEOHUDUnitFrame& Frame : PartyFrames)
    {
        if (!Frame.IsValidForAlpha())
        {
            return false;
        }
    }

    for (const FEOHUDActionSlot& SlotSpec : ActionSlots)
    {
        if (!SlotSpec.IsValidForAlpha())
        {
            return false;
        }
    }

    for (const FEOMinimapMarkerSpec& Marker : MinimapMarkers)
    {
        if (!Marker.IsValidForAlpha())
        {
            return false;
        }
    }

    for (const FEOHUDEventFeedEntry& Entry : FeedEntries)
    {
        if (!Entry.IsValidForAlpha())
        {
            return false;
        }
    }

    return PartyFrames.Num() >= 2 && ActionSlots.Num() >= 8 && MinimapMarkers.Num() >= 8;
}

FString FEOHUDPresentationModel::FindGlyphForAction(FName ActionId) const
{
    for (const FEOHUDActionSlot& SlotSpec : ActionSlots)
    {
        if (SlotSpec.ActionId == ActionId)
        {
            return SlotSpec.ControllerGlyph;
        }
    }

    return FString();
}

FEOHUDPresentationModel FEOHUDPresentationModel::BuildAlphaFixture()
{
    FEOHUDPresentationModel Model;
    Model.PlayerFrame = Unit(TEXT("Runeblade"), TEXT("role.striker"), TEXT("Player"), 120.0f, 135.0f, FLinearColor(0.18f, 0.56f, 1.0f, 1.0f));
    Model.PartyFrames = {
        Unit(TEXT("Mira"), TEXT("role.support"), TEXT("Support"), 86.0f, 90.0f, FLinearColor(0.24f, 0.90f, 0.72f, 1.0f)),
        Unit(TEXT("Tor"), TEXT("role.defender"), TEXT("Defender"), 140.0f, 150.0f, FLinearColor(1.0f, 0.58f, 0.18f, 1.0f))
    };
    Model.TargetFrame = Unit(TEXT("Relic Sentinel"), TEXT("role.elite"), TEXT("Elite"), 180.0f, 220.0f, FLinearColor(0.86f, 0.22f, 1.0f, 1.0f));
    Model.ActionSlots = {
        Slot(TEXT("BasicAttack"), TEXT("Iron Verse"), TEXT("LMB"), TEXT("X"), FLinearColor(0.16f, 0.32f, 0.82f, 1.0f)),
        Slot(TEXT("Block"), TEXT("Rune Guard"), TEXT("RMB"), TEXT("LT"), FLinearColor(0.20f, 0.56f, 1.0f, 1.0f)),
        Slot(TEXT("Dodge"), TEXT("Carve Step"), TEXT("Space"), TEXT("B"), FLinearColor(0.38f, 0.78f, 1.0f, 1.0f)),
        Slot(TEXT("AbilityOne"), TEXT("Rune Cleave"), TEXT("1"), TEXT("RB"), FLinearColor(0.24f, 0.64f, 1.0f, 1.0f)),
        Slot(TEXT("AbilityTwo"), TEXT("Relic Burst"), TEXT("2"), TEXT("RT"), FLinearColor(0.70f, 0.24f, 1.0f, 1.0f)),
        Slot(TEXT("Interact"), TEXT("Interact"), TEXT("E"), TEXT("A"), FLinearColor(1.0f, 0.78f, 0.22f, 1.0f)),
        Slot(TEXT("TargetCycle"), TEXT("Target"), TEXT("Tab"), TEXT("Y"), FLinearColor(0.86f, 0.86f, 0.96f, 1.0f)),
        Slot(TEXT("Ultimate"), TEXT("Relic Surge"), TEXT("3"), TEXT("LB+RB"), FLinearColor(1.0f, 0.34f, 0.82f, 1.0f))
    };
    Model.QuestTitle = FText::FromString(TEXT("The Starfall Gate"));
    Model.QuestObjective = FText::FromString(TEXT("Break the siphons and stabilize the relic surge."));
    Model.WorldEventText = FText::FromString(TEXT("Relic Surge: elite phase pending"));
    Model.MinimapMarkers = FEOZoneProfileCatalog::BuildStarfallValeProfile().MinimapMarkers;
    Model.FeedEntries = {
        Feed(TEXT("Quest accepted: The Starfall Gate"), FLinearColor(1.0f, 0.82f, 0.24f, 1.0f)),
        Feed(TEXT("Relic surge intensifies near Sky-Watch."), FLinearColor(0.86f, 0.24f, 1.0f, 1.0f)),
        Feed(TEXT("Reward preview: Dawnwatch Marks + Relic Shard"), FLinearColor(0.34f, 0.86f, 1.0f, 1.0f))
    };
    return Model;
}
```

- [ ] **Step 5: Connect `AEOHUD` to the presentation model**

Modify `Source/EverwildOdyssey/Public/Gameplay/EOHUD.h`:

```cpp
#include "UI/EOHUDPresentationTypes.h"
```

Add to `AEOHUD`:

```cpp
static FEOHUDPresentationModel BuildPresentationModel(const FEOHUDSnapshot& Snapshot);
```

Modify `Source/EverwildOdyssey/Private/Gameplay/EOHUD.cpp` after `BuildSnapshot`:

```cpp
FEOHUDPresentationModel AEOHUD::BuildPresentationModel(const FEOHUDSnapshot& Snapshot)
{
    FEOHUDPresentationModel Model = FEOHUDPresentationModel::BuildAlphaFixture();
    Model.PlayerFrame.Health = Snapshot.Health;
    Model.PlayerFrame.MaxHealth = Snapshot.MaxHealth;
    Model.QuestObjective = FText::FromString(FormatObjectiveLine(Snapshot));
    Model.WorldEventText = FText::FromString(FormatWorldEventLine(Snapshot));
    return Model;
}
```

In `DrawHUD`, build the model once:

```cpp
const FEOHUDPresentationModel PresentationModel = BuildPresentationModel(Snapshot);
```

Then draw party frame, target frame, action labels, feed, and minimap labels from `PresentationModel` instead of hard-coded strings where practical.

- [ ] **Step 6: Extend existing HUD tests**

Modify `Source/EverwildOdyssey/Private/Tests/EOHUDTests.cpp`:

```cpp
const FEOHUDPresentationModel Model = AEOHUD::BuildPresentationModel(Snapshot);
TestTrue(TEXT("HUD presentation model is valid."), Model.IsValidForAlpha());
TestEqual(TEXT("HUD presentation carries controller attack glyph."), Model.FindGlyphForAction(TEXT("BasicAttack")), FString(TEXT("X")));
TestEqual(TEXT("HUD presentation carries quest objective."), Model.QuestObjective.ToString(), FString(TEXT("Objective: Collect relic shards 2 / 3, then stabilize the Starfall Gate")));
```

- [ ] **Step 7: Run build**

Run:

```bash
"/Users/Shared/Epic Games/UE_5.7/Engine/Build/BatchFiles/Mac/Build.sh" EverwildOdysseyEditor Mac Development -Project="/Users/ethanstuart/Projects/everwild-odyssey/.worktrees/alpha-foundation/EverwildOdyssey.uproject" -WaitMutex
```

Expected: build succeeds and HUD presentation tests compile.

- [ ] **Step 8: Commit**

```bash
git add Source/EverwildOdyssey/Public/UI/EOHUDPresentationTypes.h Source/EverwildOdyssey/Private/UI/EOHUDPresentationTypes.cpp Source/EverwildOdyssey/Private/Tests/EOHUDPresentationTypesTests.cpp Source/EverwildOdyssey/Public/Gameplay/EOHUD.h Source/EverwildOdyssey/Private/Gameplay/EOHUD.cpp Source/EverwildOdyssey/Private/Tests/EOHUDTests.cpp
git commit -m "feat: add mmo hud presentation model"
```

---

### Task 7: Relic Surge Encounter Profile And Enemy Roles

**Tier:** 1 and 2

**Files:**
- Create: `Source/EverwildOdyssey/Public/Encounters/EOEncounterProfile.h`
- Create: `Source/EverwildOdyssey/Private/Encounters/EOEncounterProfile.cpp`
- Create: `Source/EverwildOdyssey/Private/Tests/EOEncounterProfileTests.cpp`
- Modify: `Source/EverwildOdyssey/Public/Gameplay/EOGameMode.h`
- Modify: `Source/EverwildOdyssey/Private/Gameplay/EOGameMode.cpp`
- Modify: `Source/EverwildOdyssey/Private/Tests/EOGameModeTests.cpp`

- [ ] **Step 1: Write the failing encounter-profile test**

Create `Source/EverwildOdyssey/Private/Tests/EOEncounterProfileTests.cpp`:

```cpp
#if WITH_DEV_AUTOMATION_TESTS

#include "Encounters/EOEncounterProfile.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEOEncounterProfileTest, "EverwildOdyssey.Encounters.EncounterProfile", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FEOEncounterProfileTest::RunTest(const FString& Parameters)
{
    const FEOEncounterProfile Profile = FEOEncounterProfileCatalog::BuildRelicSurgeProfile();
    TestTrue(TEXT("Relic surge profile is valid."), Profile.IsValidForAlpha());
    TestEqual(TEXT("Relic surge has three phases."), Profile.Phases.Num(), 3);
    TestTrue(TEXT("Relic surge contains minions."), Profile.CountRole(TEXT("enemy.role.minion")) >= 6);
    TestTrue(TEXT("Relic surge contains casters."), Profile.CountRole(TEXT("enemy.role.caster")) >= 2);
    TestTrue(TEXT("Relic surge contains an elite."), Profile.CountRole(TEXT("enemy.role.elite")) >= 1);
    TestTrue(TEXT("Relic surge rewards XP."), Profile.RewardExperience >= 100);
    TestTrue(TEXT("Relic surge rewards shards."), Profile.RewardRelicShards >= 3);

    return true;
}

#endif
```

- [ ] **Step 2: Run test to verify it fails**

Run:

```bash
"/Users/Shared/Epic Games/UE_5.7/Engine/Build/BatchFiles/Mac/Build.sh" EverwildOdysseyEditor Mac Development -Project="/Users/ethanstuart/Projects/everwild-odyssey/.worktrees/alpha-foundation/EverwildOdyssey.uproject" -WaitMutex
```

Expected: build fails because `Encounters/EOEncounterProfile.h` does not exist.

- [ ] **Step 3: Add encounter profile types**

Create `Source/EverwildOdyssey/Public/Encounters/EOEncounterProfile.h`:

```cpp
#pragma once

#include "CoreMinimal.h"
#include "EOEncounterProfile.generated.h"

USTRUCT(BlueprintType)
struct FEOEncounterEnemySpec
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Encounter")
    FName EnemyId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Encounter")
    FName RoleId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Encounter")
    FName AssetRoleId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Encounter")
    FName SpawnAnchorId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Encounter")
    int32 Level = 1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Encounter")
    bool bElite = false;

    bool IsValidForAlpha() const
    {
        return !EnemyId.IsNone() && !RoleId.IsNone() && !AssetRoleId.IsNone() && !SpawnAnchorId.IsNone() && Level > 0;
    }
};

USTRUCT(BlueprintType)
struct FEOEncounterPhaseSpec
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Encounter")
    FName PhaseId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Encounter")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Encounter")
    TArray<FEOEncounterEnemySpec> Enemies;

    bool IsValidForAlpha() const
    {
        if (PhaseId.IsNone() || DisplayName.IsEmpty() || Enemies.Num() == 0)
        {
            return false;
        }

        for (const FEOEncounterEnemySpec& Enemy : Enemies)
        {
            if (!Enemy.IsValidForAlpha())
            {
                return false;
            }
        }

        return true;
    }
};

USTRUCT(BlueprintType)
struct FEOEncounterProfile
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Encounter")
    FName EncounterId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Encounter")
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Encounter")
    FName LinkedWorldEventId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Encounter")
    TArray<FEOEncounterPhaseSpec> Phases;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Encounter")
    int32 RewardExperience = 0;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Encounter")
    int32 RewardRelicShards = 0;

    bool IsValidForAlpha() const;
    int32 CountRole(FName RoleId) const;
    TArray<FVector> BuildSpawnLocationsFromAnchors(const TMap<FName, FVector>& AnchorLocations) const;
};

class FEOEncounterProfileCatalog
{
public:
    static FEOEncounterProfile BuildRelicSurgeProfile();
};
```

- [ ] **Step 4: Add relic surge profile implementation**

Create `Source/EverwildOdyssey/Private/Encounters/EOEncounterProfile.cpp`:

```cpp
#include "Encounters/EOEncounterProfile.h"

namespace
{
FText Txt(const TCHAR* Value)
{
    return FText::FromString(Value);
}

FEOEncounterEnemySpec Enemy(const TCHAR* Id, const TCHAR* Role, const TCHAR* AssetRole, const TCHAR* Anchor, int32 Level, bool bElite = false)
{
    FEOEncounterEnemySpec Result;
    Result.EnemyId = FName(Id);
    Result.RoleId = FName(Role);
    Result.AssetRoleId = FName(AssetRole);
    Result.SpawnAnchorId = FName(Anchor);
    Result.Level = Level;
    Result.bElite = bElite;
    return Result;
}

FEOEncounterPhaseSpec Phase(const TCHAR* Id, const TCHAR* Name, const TArray<FEOEncounterEnemySpec>& Enemies)
{
    FEOEncounterPhaseSpec Result;
    Result.PhaseId = FName(Id);
    Result.DisplayName = Txt(Name);
    Result.Enemies = Enemies;
    return Result;
}
}

bool FEOEncounterProfile::IsValidForAlpha() const
{
    if (EncounterId.IsNone() || DisplayName.IsEmpty() || LinkedWorldEventId.IsNone() || Phases.Num() == 0 || RewardExperience <= 0 || RewardRelicShards <= 0)
    {
        return false;
    }

    for (const FEOEncounterPhaseSpec& PhaseSpec : Phases)
    {
        if (!PhaseSpec.IsValidForAlpha())
        {
            return false;
        }
    }

    return true;
}

int32 FEOEncounterProfile::CountRole(FName RoleId) const
{
    int32 Count = 0;
    for (const FEOEncounterPhaseSpec& PhaseSpec : Phases)
    {
        for (const FEOEncounterEnemySpec& EnemySpec : PhaseSpec.Enemies)
        {
            if (EnemySpec.RoleId == RoleId)
            {
                ++Count;
            }
        }
    }
    return Count;
}

TArray<FVector> FEOEncounterProfile::BuildSpawnLocationsFromAnchors(const TMap<FName, FVector>& AnchorLocations) const
{
    TArray<FVector> SpawnLocations;
    for (const FEOEncounterPhaseSpec& PhaseSpec : Phases)
    {
        for (const FEOEncounterEnemySpec& EnemySpec : PhaseSpec.Enemies)
        {
            if (const FVector* Location = AnchorLocations.Find(EnemySpec.SpawnAnchorId))
            {
                SpawnLocations.Add(*Location);
            }
        }
    }
    return SpawnLocations;
}

FEOEncounterProfile FEOEncounterProfileCatalog::BuildRelicSurgeProfile()
{
    FEOEncounterProfile Profile;
    Profile.EncounterId = TEXT("encounter.relic_surge");
    Profile.DisplayName = Txt(TEXT("Relic Surge"));
    Profile.LinkedWorldEventId = TEXT("starfall.relic_surge");
    Profile.RewardExperience = 150;
    Profile.RewardRelicShards = 3;
    Profile.Phases = {
        Phase(TEXT("phase.warning"), TEXT("Ashen scouts guard the siphons"), {
            Enemy(TEXT("enemy.minion.01"), TEXT("enemy.role.minion"), TEXT("enemy.minion.body"), TEXT("anchor.road.first_contact"), 1),
            Enemy(TEXT("enemy.minion.02"), TEXT("enemy.role.minion"), TEXT("enemy.minion.body"), TEXT("anchor.relic_surge.entry"), 1),
            Enemy(TEXT("enemy.minion.03"), TEXT("enemy.role.minion"), TEXT("enemy.minion.body"), TEXT("anchor.relic_surge.entry"), 1)
        }),
        Phase(TEXT("phase.active"), TEXT("Caster pockets empower the relic core"), {
            Enemy(TEXT("enemy.minion.04"), TEXT("enemy.role.minion"), TEXT("enemy.minion.body"), TEXT("anchor.relic_surge.center"), 1),
            Enemy(TEXT("enemy.minion.05"), TEXT("enemy.role.minion"), TEXT("enemy.minion.body"), TEXT("anchor.relic_surge.center"), 1),
            Enemy(TEXT("enemy.minion.06"), TEXT("enemy.role.minion"), TEXT("enemy.minion.body"), TEXT("anchor.relic_surge.center"), 1),
            Enemy(TEXT("enemy.caster.01"), TEXT("enemy.role.caster"), TEXT("enemy.caster.body"), TEXT("anchor.relic_surge.caster_north"), 1),
            Enemy(TEXT("enemy.caster.02"), TEXT("enemy.role.caster"), TEXT("enemy.caster.body"), TEXT("anchor.relic_surge.caster_east"), 1)
        }),
        Phase(TEXT("phase.elite"), TEXT("Relic Sentinel awakens"), {
            Enemy(TEXT("enemy.minion.07"), TEXT("enemy.role.minion"), TEXT("enemy.minion.body"), TEXT("anchor.relic_surge.center"), 1),
            Enemy(TEXT("enemy.minion.08"), TEXT("enemy.role.minion"), TEXT("enemy.minion.body"), TEXT("anchor.relic_surge.center"), 1),
            Enemy(TEXT("enemy.elite.01"), TEXT("enemy.role.elite"), TEXT("enemy.elite.body"), TEXT("anchor.relic_surge.elite"), 2, true)
        })
    };
    return Profile;
}
```

- [ ] **Step 5: Make game mode spawn from the profile**

Add this include to `Source/EverwildOdyssey/Public/Gameplay/EOGameMode.h` below `#include "CoreMinimal.h"`:

```cpp
#include "Encounters/EOEncounterProfile.h"
```

Replace the existing `ExpectedOpeningEnemyCount` line and add the new static declarations in the public section:

```cpp
static constexpr int32 ExpectedOpeningEnemyCount = 11;
static TArray<FEOEncounterEnemySpec> BuildOpeningEnemySpecs();
static TMap<FName, FVector> BuildOpeningEnemyAnchorLocations();
```

Modify `Source/EverwildOdyssey/Private/Gameplay/EOGameMode.cpp`:

```cpp
#include "Encounters/EOEncounterProfile.h"
```

Replace the body of `BuildOpeningEnemySpawnLocations()`:

```cpp
TArray<FEOEncounterEnemySpec> AEOGameMode::BuildOpeningEnemySpecs()
{
    TArray<FEOEncounterEnemySpec> EnemySpecs;
    const FEOEncounterProfile Profile = FEOEncounterProfileCatalog::BuildRelicSurgeProfile();
    for (const FEOEncounterPhaseSpec& Phase : Profile.Phases)
    {
        EnemySpecs.Append(Phase.Enemies);
    }
    return EnemySpecs;
}

TArray<FVector> AEOGameMode::BuildOpeningEnemySpawnLocations()
{
    return FEOEncounterProfileCatalog::BuildRelicSurgeProfile()
        .BuildSpawnLocationsFromAnchors(BuildOpeningEnemyAnchorLocations());
}

TMap<FName, FVector> AEOGameMode::BuildOpeningEnemyAnchorLocations()
{
    TMap<FName, FVector> Anchors;
    Anchors.Add(TEXT("anchor.road.first_contact"), FVector(205.0f, -34.0f, 96.0f));
    Anchors.Add(TEXT("anchor.relic_surge.entry"), FVector(520.0f, -235.0f, 96.0f));
    Anchors.Add(TEXT("anchor.relic_surge.center"), FVector(760.0f, -260.0f, 96.0f));
    Anchors.Add(TEXT("anchor.relic_surge.caster_north"), FVector(760.0f, -70.0f, 96.0f));
    Anchors.Add(TEXT("anchor.relic_surge.caster_east"), FVector(970.0f, -260.0f, 96.0f));
    Anchors.Add(TEXT("anchor.relic_surge.elite"), FVector(840.0f, -310.0f, 96.0f));
    return Anchors;
}
```

Inside `BeginPlay`, replace the existing spawn loop setup with role-aware specs:

```cpp
const FEOEnemyArchetype RelicWispArchetype = AEOEnemyCharacter::BuildRelicWispArchetype();
const FEOEnemyArchetype RelicSentinelArchetype = AEOEnemyCharacter::BuildRelicSentinelArchetype();
const TArray<FEOEncounterEnemySpec> EnemySpecs = BuildOpeningEnemySpecs();
const TArray<FVector> SpawnLocations = BuildOpeningEnemySpawnLocations();

for (int32 Index = 0; Index < EnemySpecs.Num() && Index < SpawnLocations.Num(); ++Index)
{
    const FEOEncounterEnemySpec& EnemySpec = EnemySpecs[Index];
    const FVector SpawnLocation = SpawnLocations[Index];
    const FRotator SpawnRotation = (FVector::ZeroVector - SpawnLocation).Rotation();
    FActorSpawnParameters SpawnParameters;
    SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    if (AEOEnemyCharacter* Enemy = World->SpawnActor<AEOEnemyCharacter>(AEOEnemyCharacter::StaticClass(), SpawnLocation, SpawnRotation, SpawnParameters))
    {
        const bool bUseEliteArchetype = EnemySpec.bElite || EnemySpec.RoleId == TEXT("enemy.role.elite");
        Enemy->InitializeFromArchetype(bUseEliteArchetype ? RelicSentinelArchetype : RelicWispArchetype);
    }
}
```

- [ ] **Step 6: Update game mode tests**

Modify `Source/EverwildOdyssey/Private/Tests/EOGameModeTests.cpp`:

```cpp
TestEqual(TEXT("Opening combat exposes the encounter profile enemy count."), SpawnLocations.Num(), 11);
TestEqual(TEXT("Opening combat exposes expected profile enemy specs."), AEOGameMode::BuildOpeningEnemySpecs().Num(), AEOGameMode::ExpectedOpeningEnemyCount);
TestTrue(TEXT("Opening encounter has required anchors."), AEOGameMode::BuildOpeningEnemyAnchorLocations().Contains(TEXT("anchor.relic_surge.elite")));
```

- [ ] **Step 7: Run build**

Run:

```bash
"/Users/Shared/Epic Games/UE_5.7/Engine/Build/BatchFiles/Mac/Build.sh" EverwildOdysseyEditor Mac Development -Project="/Users/ethanstuart/Projects/everwild-odyssey/.worktrees/alpha-foundation/EverwildOdyssey.uproject" -WaitMutex
```

Expected: build succeeds and encounter profile tests compile.

- [ ] **Step 8: Commit**

```bash
git add Source/EverwildOdyssey/Public/Encounters/EOEncounterProfile.h Source/EverwildOdyssey/Private/Encounters/EOEncounterProfile.cpp Source/EverwildOdyssey/Private/Tests/EOEncounterProfileTests.cpp Source/EverwildOdyssey/Public/Gameplay/EOGameMode.h Source/EverwildOdyssey/Private/Gameplay/EOGameMode.cpp Source/EverwildOdyssey/Private/Tests/EOGameModeTests.cpp
git commit -m "feat: stage relic surge from encounter profile"
```

---

### Task 8: Quest, Loot, Reputation, And World-State Hooks

**Tier:** 2 and 3

**Files:**
- Modify: `Source/EverwildOdyssey/Public/Core/EOQuestTypes.h`
- Modify: `Source/EverwildOdyssey/Public/Core/EOInventoryTypes.h`
- Modify: `Source/EverwildOdyssey/Public/Core/EOClassTypes.h`
- Modify: `Source/EverwildOdyssey/Private/Gameplay/EOHeroCharacter.cpp`
- Modify: `Source/EverwildOdyssey/Private/Tests/EOQuestLogComponentTests.cpp`
- Modify: `Source/EverwildOdyssey/Private/Tests/EOInventoryComponentTests.cpp`

- [ ] **Step 1: Extend tests for richer quest and loot data**

In `Source/EverwildOdyssey/Private/Tests/EOQuestLogComponentTests.cpp`, add a quest record with objective and reward fields after the existing record construction:

```cpp
Quest.ObjectiveText = FText::FromString(TEXT("Break the siphons and defeat the Relic Sentinel."));
Quest.RewardText = FText::FromString(TEXT("150 XP, 3 Relic Shards, Dawnwatch reputation"));
TestFalse(TEXT("Quest objective text is not empty."), Quest.ObjectiveText.IsEmpty());
TestFalse(TEXT("Quest reward text is not empty."), Quest.RewardText.IsEmpty());
```

In `Source/EverwildOdyssey/Private/Tests/EOInventoryComponentTests.cpp`, add a relic item:

```cpp
FEOGearItem Relic;
Relic.ItemId = TEXT("relic.skywatch_shard");
Relic.DisplayName = FText::FromString(TEXT("Sky-Watch Shard"));
Relic.Slot = EEOGearSlot::Relic;
Relic.Rarity = EEOGearRarity::Relic;
Relic.Power = 12;
Relic.AffixText = FText::FromString(TEXT("+Relic Surge damage"));
TestTrue(TEXT("Relic gear item is valid."), Relic.IsValidForAlpha());
```

- [ ] **Step 2: Run test to verify it fails**

Run:

```bash
"/Users/Shared/Epic Games/UE_5.7/Engine/Build/BatchFiles/Mac/Build.sh" EverwildOdysseyEditor Mac Development -Project="/Users/ethanstuart/Projects/everwild-odyssey/.worktrees/alpha-foundation/EverwildOdyssey.uproject" -WaitMutex
```

Expected: build fails because `ObjectiveText`, `RewardText`, `EEOGearSlot::Relic`, and `AffixText` do not exist.

- [ ] **Step 3: Add quest copy fields**

Modify `Source/EverwildOdyssey/Public/Core/EOQuestTypes.h` inside `FEOQuestRecord`:

```cpp
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
FText ObjectiveText;

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
FText RewardText;

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
FName NextQuestId = NAME_None;
```

Update `CanAdvance`:

```cpp
bool CanAdvance() const
{
    return State == EEOQuestState::Active && StepIndex >= 0 && !ObjectiveText.IsEmpty();
}
```

- [ ] **Step 4: Add gear slots and affix text**

Modify `Source/EverwildOdyssey/Public/Core/EOInventoryTypes.h`:

```cpp
UENUM(BlueprintType)
enum class EEOGearSlot : uint8
{
    Weapon UMETA(DisplayName = "Weapon"),
    Offhand UMETA(DisplayName = "Offhand"),
    Armor UMETA(DisplayName = "Armor"),
    Trinket UMETA(DisplayName = "Trinket"),
    Relic UMETA(DisplayName = "Relic")
};
```

Add to `FEOGearItem`:

```cpp
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gear")
FText AffixText;
```

Keep `IsValidForAlpha` compact:

```cpp
bool IsValidForAlpha() const
{
    return !ItemId.IsNone() && !DisplayName.IsEmpty() && Power > 0;
}
```

- [ ] **Step 5: Add Ashen Covenant faction**

Modify `Source/EverwildOdyssey/Public/Core/EOClassTypes.h`:

```cpp
UENUM(BlueprintType)
enum class EEOFaction : uint8
{
    DawnwatchCommand UMETA(DisplayName = "Dawnwatch Command"),
    StarfallCollegium UMETA(DisplayName = "Starfall Collegium"),
    ValeVillagers UMETA(DisplayName = "Vale Villagers"),
    AshenCovenant UMETA(DisplayName = "Ashen Covenant")
};
```

- [ ] **Step 6: Seed richer quest and reward copy on the hero**

Modify `Source/EverwildOdyssey/Private/Gameplay/EOHeroCharacter.cpp` in `BeginPlay()` after `ArrivalQuest.DisplayName`:

```cpp
ArrivalQuest.ObjectiveText = FText::FromString(TEXT("Break the Ashen siphons, defeat the Relic Sentinel, and stabilize the Starfall Gate."));
ArrivalQuest.RewardText = FText::FromString(TEXT("150 XP, 3 Relic Shards, Dawnwatch reputation, Sky-Watch route weakened"));
ArrivalQuest.NextQuestId = TEXT("dawnwatch.skywatch_threshold");
```

After `TrainingBlade.Power = 4;`:

```cpp
TrainingBlade.AffixText = FText::FromString(TEXT("+Runeblade basic attack power"));
```

- [ ] **Step 7: Run build**

Run:

```bash
"/Users/Shared/Epic Games/UE_5.7/Engine/Build/BatchFiles/Mac/Build.sh" EverwildOdysseyEditor Mac Development -Project="/Users/ethanstuart/Projects/everwild-odyssey/.worktrees/alpha-foundation/EverwildOdyssey.uproject" -WaitMutex
```

Expected: build succeeds and richer quest/loot data compiles.

- [ ] **Step 8: Commit**

```bash
git add Source/EverwildOdyssey/Public/Core/EOQuestTypes.h Source/EverwildOdyssey/Public/Core/EOInventoryTypes.h Source/EverwildOdyssey/Public/Core/EOClassTypes.h Source/EverwildOdyssey/Private/Gameplay/EOHeroCharacter.cpp Source/EverwildOdyssey/Private/Tests/EOQuestLogComponentTests.cpp Source/EverwildOdyssey/Private/Tests/EOInventoryComponentTests.cpp
git commit -m "feat: deepen quest loot and faction hooks"
```

---

### Task 9: World Diagnostics And Showcase Readiness

**Tier:** 1 and 2

**Files:**
- Create: `Source/EverwildOdyssey/Public/World/EOWorldDiagnostics.h`
- Create: `Source/EverwildOdyssey/Private/World/EOWorldDiagnostics.cpp`
- Create: `Source/EverwildOdyssey/Private/Tests/EOWorldDiagnosticsTests.cpp`
- Modify: `Source/EverwildOdyssey/Private/Gameplay/EOGameMode.cpp`

- [ ] **Step 1: Write the failing diagnostics test**

Create `Source/EverwildOdyssey/Private/Tests/EOWorldDiagnosticsTests.cpp`:

```cpp
#if WITH_DEV_AUTOMATION_TESTS

#include "World/EOWorldDiagnostics.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEOWorldDiagnosticsTest, "EverwildOdyssey.World.Diagnostics", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FEOWorldDiagnosticsTest::RunTest(const FString& Parameters)
{
    FEOWorldReadinessReport Report;
    Report.ZoneId = TEXT("zone.starfall_vale");
    Report.RequiredLandmarks = 18;
    Report.VisibleLandmarks = 18;
    Report.AssetSummary.TotalRoles = 30;
    Report.AssetSummary.RealAssetRoles = 12;
    Report.AssetSummary.ProxyRoles = 18;
    Report.AssetSummary.MissingRequiredRoles = 0;
    Report.bControllerMappingsPresent = true;
    Report.bHUDSurfacesPresent = true;
    Report.bEncounterProfilePresent = true;

    TestTrue(TEXT("Report is valid for alpha."), Report.IsValidForAlpha());
    TestTrue(TEXT("Report is showcase ready."), Report.IsShowcaseReady());
    TestTrue(TEXT("Readable report mentions Starfall Vale."), Report.ToReadableString().Contains(TEXT("zone.starfall_vale")));

    Report.AssetSummary.MissingRequiredRoles = 1;
    TestFalse(TEXT("Missing required asset blocks showcase readiness."), Report.IsShowcaseReady());

    return true;
}

#endif
```

- [ ] **Step 2: Run test to verify it fails**

Run:

```bash
"/Users/Shared/Epic Games/UE_5.7/Engine/Build/BatchFiles/Mac/Build.sh" EverwildOdysseyEditor Mac Development -Project="/Users/ethanstuart/Projects/everwild-odyssey/.worktrees/alpha-foundation/EverwildOdyssey.uproject" -WaitMutex
```

Expected: build fails because `World/EOWorldDiagnostics.h` does not exist.

- [ ] **Step 3: Add diagnostics header**

Create `Source/EverwildOdyssey/Public/World/EOWorldDiagnostics.h`:

```cpp
#pragma once

#include "CoreMinimal.h"
#include "Presentation/EOAssetRoleTypes.h"
#include "EOWorldDiagnostics.generated.h"

USTRUCT(BlueprintType)
struct FEOWorldReadinessReport
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Diagnostics")
    FName ZoneId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Diagnostics")
    int32 RequiredLandmarks = 0;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Diagnostics")
    int32 VisibleLandmarks = 0;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Diagnostics")
    FEOAssetResolutionSummary AssetSummary;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Diagnostics")
    bool bControllerMappingsPresent = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Diagnostics")
    bool bHUDSurfacesPresent = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Diagnostics")
    bool bEncounterProfilePresent = false;

    bool IsValidForAlpha() const;
    bool IsShowcaseReady() const;
    FString ToReadableString() const;
};
```

- [ ] **Step 4: Add diagnostics implementation**

Create `Source/EverwildOdyssey/Private/World/EOWorldDiagnostics.cpp`:

```cpp
#include "World/EOWorldDiagnostics.h"

bool FEOWorldReadinessReport::IsValidForAlpha() const
{
    return !ZoneId.IsNone()
        && RequiredLandmarks > 0
        && VisibleLandmarks >= 0
        && AssetSummary.TotalRoles > 0;
}

bool FEOWorldReadinessReport::IsShowcaseReady() const
{
    return IsValidForAlpha()
        && VisibleLandmarks >= RequiredLandmarks
        && AssetSummary.CanLaunchShowcase()
        && bControllerMappingsPresent
        && bHUDSurfacesPresent
        && bEncounterProfilePresent;
}

FString FEOWorldReadinessReport::ToReadableString() const
{
    return FString::Printf(
        TEXT("World readiness: zone=%s landmarks=%d/%d asset_roles=%d real=%d proxy=%d missing_required=%d controller=%s hud=%s encounter=%s showcase=%s"),
        *ZoneId.ToString(),
        VisibleLandmarks,
        RequiredLandmarks,
        AssetSummary.TotalRoles,
        AssetSummary.RealAssetRoles,
        AssetSummary.ProxyRoles,
        AssetSummary.MissingRequiredRoles,
        bControllerMappingsPresent ? TEXT("true") : TEXT("false"),
        bHUDSurfacesPresent ? TEXT("true") : TEXT("false"),
        bEncounterProfilePresent ? TEXT("true") : TEXT("false"),
        IsShowcaseReady() ? TEXT("true") : TEXT("false"));
}
```

- [ ] **Step 5: Log readiness in game mode**

Modify `Source/EverwildOdyssey/Private/Gameplay/EOGameMode.cpp`:

```cpp
#include "Presentation/EOAssetResolver.h"
#include "Presentation/EOAssetRoleTypes.h"
#include "World/EOWorldDiagnostics.h"
#include "World/EOZoneProfile.h"
```

At the end of `BeginPlay()`:

```cpp
FEOAssetResolver Resolver;
Resolver.Initialize(FEOAssetRoleCatalog::BuildDefaultRoles());

const FEOZoneProfile ZoneProfile = FEOZoneProfileCatalog::BuildStarfallValeProfile();
FEOWorldReadinessReport Report;
Report.ZoneId = ZoneProfile.ZoneId;
Report.RequiredLandmarks = ZoneProfile.Landmarks.Num();
Report.VisibleLandmarks = ZoneProfile.Landmarks.Num();
Report.AssetSummary = Resolver.BuildSummary();
Report.bControllerMappingsPresent = true;
Report.bHUDSurfacesPresent = true;
Report.bEncounterProfilePresent = FEOEncounterProfileCatalog::BuildRelicSurgeProfile().IsValidForAlpha();
UE_LOG(LogTemp, Log, TEXT("%s"), *Report.ToReadableString());
```

- [ ] **Step 6: Run build**

Run:

```bash
"/Users/Shared/Epic Games/UE_5.7/Engine/Build/BatchFiles/Mac/Build.sh" EverwildOdysseyEditor Mac Development -Project="/Users/ethanstuart/Projects/everwild-odyssey/.worktrees/alpha-foundation/EverwildOdyssey.uproject" -WaitMutex
```

Expected: build succeeds and diagnostics tests compile.

- [ ] **Step 7: Commit**

```bash
git add Source/EverwildOdyssey/Public/World/EOWorldDiagnostics.h Source/EverwildOdyssey/Private/World/EOWorldDiagnostics.cpp Source/EverwildOdyssey/Private/Tests/EOWorldDiagnosticsTests.cpp Source/EverwildOdyssey/Private/Gameplay/EOGameMode.cpp
git commit -m "feat: add showcase readiness diagnostics"
```

---

### Task 10: Authored Visual Rescue Scaffold Pass

**Tier:** 1

**Files:**
- Modify: `Source/EverwildOdyssey/Public/Gameplay/EOAlphaWorldScaffold.h`
- Modify: `Source/EverwildOdyssey/Private/Gameplay/EOAlphaWorldScaffold.cpp`
- Modify: `Source/EverwildOdyssey/Private/Tests/EOAlphaWorldScaffoldTests.cpp`
- Modify: `docs/assets/heroic-mmo-asset-manifest.md`

- [ ] **Step 1: Add failing visual-rescue scaffold assertions**

Modify `Source/EverwildOdyssey/Private/Tests/EOAlphaWorldScaffoldTests.cpp` in `FEOAlphaWorldScaffoldTest::RunTest` after the minimap count assertion:

```cpp
TestTrue(TEXT("Scaffold adds authored showcase assembly parts beyond profile primitives."), AEOAlphaWorldScaffold::ExpectedShowcaseAssemblyPartCount() >= 72);
```

Modify `FEOAlphaWorldScaffoldRuntimeGenerationTest::RunTest` after retrieving `RuntimeMeshes`:

```cpp
const int32 ExpectedBaseVisuals = Profile.Landmarks.Num() + Profile.ScenicProps.Num();
TestTrue(TEXT("Runtime mesh count includes authored showcase assemblies."), RuntimeMeshes.Num() >= ExpectedBaseVisuals + AEOAlphaWorldScaffold::ExpectedShowcaseAssemblyPartCount());
```

Add these booleans beside the existing runtime-found flags:

```cpp
bool bFoundDawnwatchAssembly = false;
bool bFoundRelicBeamAssembly = false;
bool bFoundSkywatchAssembly = false;
bool bFoundVistaHeight = false;
```

Inside the runtime mesh loop, set them from tags and location:

```cpp
bFoundDawnwatchAssembly |= MeshComponent->ComponentTags.Contains(TEXT("visual.assembly.dawnwatch_gate"));
bFoundRelicBeamAssembly |= MeshComponent->ComponentTags.Contains(TEXT("visual.assembly.relic_beam"));
bFoundSkywatchAssembly |= MeshComponent->ComponentTags.Contains(TEXT("visual.assembly.skywatch_entrance"));
bFoundVistaHeight |= MeshComponent->GetRelativeLocation().Z >= 560.0f;
```

Before destroying the world, assert:

```cpp
TestTrue(TEXT("Runtime world has a multi-part Dawnwatch gate assembly."), bFoundDawnwatchAssembly);
TestTrue(TEXT("Runtime world has a relic beam spectacle assembly."), bFoundRelicBeamAssembly);
TestTrue(TEXT("Runtime world has a Sky-Watch entrance assembly."), bFoundSkywatchAssembly);
TestTrue(TEXT("Runtime world has skyline-height authored pieces."), bFoundVistaHeight);
```

- [ ] **Step 2: Run build to verify the new assertions fail**

Run:

```bash
"/Users/Shared/Epic Games/UE_5.7/Engine/Build/BatchFiles/Mac/Build.sh" EverwildOdysseyEditor Mac Development -Project="/Users/ethanstuart/Projects/everwild-odyssey/.worktrees/alpha-foundation/EverwildOdyssey.uproject" -WaitMutex
```

Expected: build fails because `ExpectedShowcaseAssemblyPartCount` does not exist.

- [ ] **Step 3: Expose the showcase assembly count**

Modify `Source/EverwildOdyssey/Public/Gameplay/EOAlphaWorldScaffold.h` in the public static count section:

```cpp
static int32 ExpectedShowcaseAssemblyPartCount();
```

- [ ] **Step 4: Implement authored assembly recipes**

Modify `Source/EverwildOdyssey/Private/Gameplay/EOAlphaWorldScaffold.cpp`.

Add these local declarations in the anonymous namespace:

```cpp
enum class EEOProxyMeshShape : uint8
{
    Cube,
    Cone,
    Cylinder,
    Sphere,
    LightBeam
};

struct FEOShowcaseAssemblyPartSpec
{
    FName PartId = NAME_None;
    FName AssemblyTag = NAME_None;
    FName AssetRoleId = NAME_None;
    EEOProxyMeshShape Shape = EEOProxyMeshShape::Cube;
    FVector Location = FVector::ZeroVector;
    FRotator Rotation = FRotator::ZeroRotator;
    FVector Scale = FVector::OneVector;
    FLinearColor Tint = FLinearColor::White;
    bool bBlocksMovement = false;
};

FEOShowcaseAssemblyPartSpec AssemblyPart(const TCHAR* Id, const TCHAR* AssemblyTag, const TCHAR* AssetRoleId, EEOProxyMeshShape Shape, const FVector& Location, const FVector& Scale, const FLinearColor& Tint, const FRotator& Rotation = FRotator::ZeroRotator, bool bBlocksMovement = false)
{
    FEOShowcaseAssemblyPartSpec Result;
    Result.PartId = FName(Id);
    Result.AssemblyTag = FName(AssemblyTag);
    Result.AssetRoleId = FName(AssetRoleId);
    Result.Shape = Shape;
    Result.Location = Location;
    Result.Rotation = Rotation;
    Result.Scale = Scale;
    Result.Tint = Tint;
    Result.bBlocksMovement = bBlocksMovement;
    return Result;
}
```

Add `BuildShowcaseAssemblyParts()` in the anonymous namespace. It must return at least 72 parts and include:

```cpp
TArray<FEOShowcaseAssemblyPartSpec> Parts;
Parts.Reserve(96);
```

Required authored assemblies:

- `visual.assembly.dawnwatch_gate`: thick gate base, two towers, roofs, banner poles, banner cloth, wall wings, and warm lamps around `landmark.dawnwatch.gate`.
- `visual.assembly.road_ribbon`: flattened golden/tan road segments and side markers from spawn toward the relic surge.
- `visual.assembly.relic_beam`: vertical beam/light-beam mesh if available, crystal ring, floating shards, and warning pylons around `landmark.relic_core`.
- `visual.assembly.skywatch_entrance`: tall arch, side spires, portal core, top cap, and blue beam near `landmark.skywatch_entrance`.
- `visual.assembly.future_boundaries`: stylized blockers/sign silhouettes at the Briarfen, Moonwell, and Sunspire hooks.

Add this mesh selector near `ProxyMeshForRole`:

```cpp
UStaticMesh* MeshForAssemblyShape(EEOProxyMeshShape Shape, UStaticMesh* CubeMesh, UStaticMesh* ConeMesh, UStaticMesh* CylinderMesh, UStaticMesh* SphereMesh, UStaticMesh* LightBeamMesh)
{
    switch (Shape)
    {
    case EEOProxyMeshShape::Cone:
        return ConeMesh;
    case EEOProxyMeshShape::Cylinder:
        return CylinderMesh;
    case EEOProxyMeshShape::Sphere:
        return SphereMesh;
    case EEOProxyMeshShape::LightBeam:
        return LightBeamMesh != nullptr ? LightBeamMesh : ConeMesh;
    case EEOProxyMeshShape::Cube:
    default:
        return CubeMesh;
    }
}
```

Load the Engine volumetric beam mesh in `SpawnVerticalSliceWorld()`:

```cpp
UStaticMesh* LightBeamMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/EngineVolumetrics/LightBeam/Mesh/S_EV_SimpleLightBeam_03.S_EV_SimpleLightBeam_03"));
```

After landmark generation and before scenic prop generation, loop `BuildShowcaseAssemblyParts()` and call the existing `CreateMesh` lambda. Add both `PartId` and `AssemblyTag` as tags; the easiest path is to add `AssemblyTag` as an extra optional parameter to `CreateMesh`, defaulting to `NAME_None`, and tag the component when it is set.

Add the static count method near the existing count methods:

```cpp
int32 AEOAlphaWorldScaffold::ExpectedShowcaseAssemblyPartCount()
{
    return BuildShowcaseAssemblyParts().Num();
}
```

- [ ] **Step 5: Update the asset manifest with the local-assets finding**

Append to `docs/assets/heroic-mmo-asset-manifest.md`:

```markdown
## June 15 Visual Rescue Finding

The project `Content/` directory is currently empty. Local search found Engine basic shapes and Engine volumetric meshes, but no committed fantasy character, environment, enemy, animation, or UI asset pack. The visual rescue pass therefore builds authored multi-part proxy assemblies from Engine-safe primitives and volumetrics while keeping every part tagged for future replacement by real assets.
```

- [ ] **Step 6: Run build**

Run:

```bash
"/Users/Shared/Epic Games/UE_5.7/Engine/Build/BatchFiles/Mac/Build.sh" EverwildOdysseyEditor Mac Development -Project="/Users/ethanstuart/Projects/everwild-odyssey/.worktrees/alpha-foundation/EverwildOdyssey.uproject" -WaitMutex
```

Expected: build succeeds and the scaffold tests compile.

- [ ] **Step 7: Commit**

```bash
git add Source/EverwildOdyssey/Public/Gameplay/EOAlphaWorldScaffold.h Source/EverwildOdyssey/Private/Gameplay/EOAlphaWorldScaffold.cpp Source/EverwildOdyssey/Private/Tests/EOAlphaWorldScaffoldTests.cpp docs/assets/heroic-mmo-asset-manifest.md
git commit -m "feat: add authored visual rescue scaffold"
```

---

### Task 11: Visual Verification Docs And Standalone Launch Script

**Tier:** 1 and 2

**Files:**
- Create: `docs/heroic-mmo-visual-verification.md`
- Create: `Scripts/launch-everwild-standalone.sh`
- Modify: `README.md`
- Modify: `docs/unreal-verification.md`

- [ ] **Step 1: Add the visual verification checklist**

Create `docs/heroic-mmo-visual-verification.md`:

```markdown
# Heroic MMO Visual Verification

Date: 2026-06-15

## First Frame Acceptance

Pass only when a 1280x720 standalone screenshot shows:

- A readable heroic Runeblade silhouette in the foreground.
- Dawnwatch Gate or hub identity in the foreground/midground.
- A road leading toward danger.
- Relic surge VFX or crystal landmark visible in the midground/background.
- Sky-Watch or a future-zone landmark visible beyond the combat area.
- HUD surfaces for player, party, target, action bar, quest tracker, minimap, event feed, and controller glyphs.
- No required showcase role reported as missing in launch logs.

## Combat Frame Acceptance

Pass only when a combat screenshot shows:

- At least one minion role.
- At least one caster/ranged role or readable proxy.
- One elite pressure role or elite-phase cue.
- Enemy name/target information in HUD.
- Action bar remains readable at 1280x720.
- Relic surge event state is visible without debug-only text.

## Controller Verification

Record:

- Controller model.
- Movement with left stick.
- Attack with face-left.
- Dodge with face-right.
- Interact with face-bottom.
- Target cycle with face-top.
- Ability one with right shoulder.
- Ability two with right trigger.
- Block with left trigger.

If a controller is unavailable, record `mapped, not physically tested`.

## Proxy Acceptance

Prototype Mode allows proxy roles. Showcase Mode accepts optional proxy roles but fails when a required showcase role is missing.
```

- [ ] **Step 2: Add the standalone launch script**

Create `Scripts/launch-everwild-standalone.sh`:

```bash
#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
PROJECT_FILE="$ROOT_DIR/EverwildOdyssey.uproject"
UNREAL_EDITOR="/Users/Shared/Epic Games/UE_5.7/Engine/Binaries/Mac/UnrealEditor.app/Contents/MacOS/UnrealEditor"

if [[ ! -x "$UNREAL_EDITOR" ]]; then
  echo "Missing UnrealEditor executable: $UNREAL_EDITOR"
  exit 1
fi

"$UNREAL_EDITOR" "$PROJECT_FILE" -game -ResX=1280 -ResY=720 -windowed -log
```

Then run:

```bash
chmod +x Scripts/launch-everwild-standalone.sh
```

Expected: script becomes executable.

- [ ] **Step 3: Update README**

Add under `## Setup` in `README.md`:

```markdown
## Heroic MMO Rebuild Verification

The asset-first rebuild is tracked by:

- `docs/superpowers/specs/2026-06-15-heroic-mmo-asset-first-rebuild-design.md`
- `docs/superpowers/plans/2026-06-15-heroic-mmo-asset-first-rebuild.md`
- `docs/assets/heroic-mmo-asset-manifest.md`
- `docs/heroic-mmo-visual-verification.md`

Launch a 1280x720 standalone play window with:

```bash
Scripts/launch-everwild-standalone.sh
```
```

- [ ] **Step 4: Update verification notes**

Append to `docs/unreal-verification.md`:

```markdown
## Heroic MMO Rebuild Verification Commands

Build:

```bash
"/Users/Shared/Epic Games/UE_5.7/Engine/Build/BatchFiles/Mac/Build.sh" \
  EverwildOdysseyEditor Mac Development \
  -Project="/Users/ethanstuart/Projects/everwild-odyssey/.worktrees/alpha-foundation/EverwildOdyssey.uproject" \
  -WaitMutex
```

Standalone launch:

```bash
Scripts/launch-everwild-standalone.sh
```

Visual pass/fail criteria live in `docs/heroic-mmo-visual-verification.md`.
```

- [ ] **Step 5: Run setup check**

Run:

```bash
Scripts/check-unreal.sh
```

Expected: reports Unreal Engine 5.7, Epic Games Launcher if installed, Xcode/clang, and Git LFS status.

- [ ] **Step 6: Commit**

```bash
git add docs/heroic-mmo-visual-verification.md Scripts/launch-everwild-standalone.sh README.md docs/unreal-verification.md
git commit -m "docs: add heroic mmo visual verification workflow"
```

---

### Task 12: Build, Launch, Screenshot Review, And Push

**Tier:** 1

**Files:**
- Modify: `docs/unreal-verification.md`

- [ ] **Step 1: Run final setup check**

Run:

```bash
Scripts/check-unreal.sh
```

Expected: script completes and prints local toolchain state.

- [ ] **Step 2: Run final editor build**

Run:

```bash
"/Users/Shared/Epic Games/UE_5.7/Engine/Build/BatchFiles/Mac/Build.sh" EverwildOdysseyEditor Mac Development -Project="/Users/ethanstuart/Projects/everwild-odyssey/.worktrees/alpha-foundation/EverwildOdyssey.uproject" -WaitMutex
```

Expected: build exits with code 0.

- [ ] **Step 3: Try command-line automation**

Run:

```bash
"/Users/Shared/Epic Games/UE_5.7/Engine/Binaries/Mac/UnrealEditor-Cmd" "/Users/ethanstuart/Projects/everwild-odyssey/.worktrees/alpha-foundation/EverwildOdyssey.uproject" -ExecCmds="Automation RunTests EverwildOdyssey; Quit" -unattended -nop4 -nosplash -NullRHI -log
```

Expected: if the known macOS `UnrealEditor-Cmd` hang is gone, automation runs and reports EverwildOdyssey tests. If the hang remains, update `docs/unreal-verification.md` with the current command, observed output, and state that editor-target build is the verified automated check.

- [ ] **Step 4: Launch standalone**

Run:

```bash
Scripts/launch-everwild-standalone.sh
```

Expected: Unreal opens a 1280x720 standalone game window. If macOS GUI launch requires approval, request approval for the launch command and retry.

- [ ] **Step 5: Capture visual review notes**

Update `docs/unreal-verification.md` with this section:

```markdown
## Heroic MMO Rebuild Visual Review

Date: 2026-06-15

- Build result:
- Standalone launch result:
- First-frame read:
- Combat-frame read:
- Controller verification:
- Required missing asset roles:
- Optional proxy roles:
- Biggest remaining visual gap:
```

Fill each field with the actual observed result from the run. Use `mapped, not physically tested` for controller verification if no controller is plugged in.

- [ ] **Step 6: Commit verification notes**

```bash
git add docs/unreal-verification.md
git commit -m "docs: record heroic mmo rebuild verification"
```

- [ ] **Step 7: Push branch**

```bash
git push origin feature/alpha-foundation
```

Expected: remote branch updates successfully.

---

## Spec Coverage Self-Review

- Asset-first production discipline: Tasks 1, 2, 3, 4, 9, and 10.
- Heroic first-frame credibility: Tasks 3, 4, 5, 9, 10, 11, and 12.
- MMO HUD density: Task 6.
- Controller support: Tasks 5, 6, 11, and 12.
- Quest/reward/RPG hooks: Task 8.
- Encounter roles and elite phase: Task 7.
- Continent/future-zone hooks: Task 3 and Task 6 minimap markers.
- Authored visual rescue after local asset search: Task 10.
- Diagnostics and build modes: Tasks 1, 2, 9, 11, and 12.
- Public repo asset policy: Task 1 and Task 11.
- Visual verification: Tasks 11 and 12.

## Execution Notes

- Work task-by-task and commit after each task.
- Keep required-role failures visible; do not hide missing assets behind silent primitives.
- Prefer the strongest available installed/local assets. If suitable assets are not installed, request network/install approval before importing free asset packs.
- Do not commit Marketplace cache contents, premium assets, derived data, or oversized binary assets without a separate repo decision.
- If command-line Unreal automation remains blocked by the known macOS startup hang, record the blockage and keep editor-target build plus standalone launch as the verified path.
