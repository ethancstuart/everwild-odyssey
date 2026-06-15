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
