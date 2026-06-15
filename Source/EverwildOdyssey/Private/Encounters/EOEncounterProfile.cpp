#include "Encounters/EOEncounterProfile.h"

namespace
{
FText Txt(const TCHAR* Value)
{
    return FText::FromString(Value);
}

FEOEncounterEnemySpec Enemy(const TCHAR* EnemyId, const TCHAR* RoleId, const TCHAR* AssetRoleId, const TCHAR* SpawnAnchorId, int32 Level = 1, bool bElite = false)
{
    FEOEncounterEnemySpec Result;
    Result.EnemyId = FName(EnemyId);
    Result.RoleId = FName(RoleId);
    Result.AssetRoleId = FName(AssetRoleId);
    Result.SpawnAnchorId = FName(SpawnAnchorId);
    Result.Level = Level;
    Result.bElite = bElite;
    return Result;
}

FEOEncounterPhaseSpec Phase(const TCHAR* PhaseId, const TCHAR* DisplayName, TArray<FEOEncounterEnemySpec>&& Enemies)
{
    FEOEncounterPhaseSpec Result;
    Result.PhaseId = FName(PhaseId);
    Result.DisplayName = Txt(DisplayName);
    Result.Enemies = MoveTemp(Enemies);
    return Result;
}
}

bool FEOEncounterProfile::IsValidForAlpha() const
{
    if (EncounterId.IsNone() || DisplayName.IsEmpty() || LinkedWorldEventId.IsNone() || Phases.IsEmpty() || RewardExperience <= 0 || RewardRelicShards <= 0)
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
            if (const FVector* SpawnLocation = AnchorLocations.Find(EnemySpec.SpawnAnchorId))
            {
                SpawnLocations.Add(*SpawnLocation);
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
        Phase(
            TEXT("phase.warning"),
            TEXT("Warning"),
            {
                Enemy(TEXT("enemy.relic_surge.warning.minion_01"), TEXT("enemy.role.minion"), TEXT("enemy.minion.body"), TEXT("anchor.road.first_contact")),
                Enemy(TEXT("enemy.relic_surge.warning.minion_02"), TEXT("enemy.role.minion"), TEXT("enemy.minion.body"), TEXT("anchor.relic_surge.entry")),
                Enemy(TEXT("enemy.relic_surge.warning.minion_03"), TEXT("enemy.role.minion"), TEXT("enemy.minion.body"), TEXT("anchor.relic_surge.entry"))
            }),
        Phase(
            TEXT("phase.active"),
            TEXT("Active Surge"),
            {
                Enemy(TEXT("enemy.relic_surge.active.minion_01"), TEXT("enemy.role.minion"), TEXT("enemy.minion.body"), TEXT("anchor.relic_surge.center")),
                Enemy(TEXT("enemy.relic_surge.active.minion_02"), TEXT("enemy.role.minion"), TEXT("enemy.minion.body"), TEXT("anchor.relic_surge.center")),
                Enemy(TEXT("enemy.relic_surge.active.minion_03"), TEXT("enemy.role.minion"), TEXT("enemy.minion.body"), TEXT("anchor.relic_surge.center")),
                Enemy(TEXT("enemy.relic_surge.active.caster_01"), TEXT("enemy.role.caster"), TEXT("enemy.caster.body"), TEXT("anchor.relic_surge.caster_north")),
                Enemy(TEXT("enemy.relic_surge.active.caster_02"), TEXT("enemy.role.caster"), TEXT("enemy.caster.body"), TEXT("anchor.relic_surge.caster_east"))
            }),
        Phase(
            TEXT("phase.elite"),
            TEXT("Elite Surge"),
            {
                Enemy(TEXT("enemy.relic_surge.elite.minion_01"), TEXT("enemy.role.minion"), TEXT("enemy.minion.body"), TEXT("anchor.relic_surge.center")),
                Enemy(TEXT("enemy.relic_surge.elite.minion_02"), TEXT("enemy.role.minion"), TEXT("enemy.minion.body"), TEXT("anchor.relic_surge.center")),
                Enemy(TEXT("enemy.relic_surge.elite.sentinel"), TEXT("enemy.role.elite"), TEXT("enemy.elite.body"), TEXT("anchor.relic_surge.elite"), 2, true)
            })
    };

    return Profile;
}
