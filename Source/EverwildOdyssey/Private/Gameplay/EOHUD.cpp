#include "Gameplay/EOHUD.h"

#include "Engine/Canvas.h"
#include "EngineUtils.h"
#include "Gameplay/EOCombatStatsComponent.h"
#include "Gameplay/EOEnemyCharacter.h"
#include "Gameplay/EOHeroCharacter.h"
#include "Gameplay/EOHeroProgressionComponent.h"
#include "Gameplay/EOQuestLogComponent.h"

namespace
{
void DrawPanel(AHUD* HUD, float X, float Y, float W, float H, const FLinearColor& Fill, const FLinearColor& Border)
{
    HUD->DrawRect(Fill, X, Y, W, H);
    HUD->DrawRect(Border, X, Y, W, 2.0f);
    HUD->DrawRect(Border, X, Y + H - 2.0f, W, 2.0f);
    HUD->DrawRect(Border, X, Y, 2.0f, H);
    HUD->DrawRect(Border, X + W - 2.0f, Y, 2.0f, H);
}

void DrawBar(AHUD* HUD, float X, float Y, float W, float H, float Value, float MaxValue, const FLinearColor& Fill, const FLinearColor& Backing)
{
    HUD->DrawRect(Backing, X, Y, W, H);
    const float Percent = MaxValue > 0.0f ? FMath::Clamp(Value / MaxValue, 0.0f, 1.0f) : 0.0f;
    HUD->DrawRect(Fill, X, Y, W * Percent, H);
}
}

void AEOHUD::DrawHUD()
{
    Super::DrawHUD();

    const AEOHeroCharacter* Hero = Cast<AEOHeroCharacter>(GetOwningPawn());
    const FEOHUDSnapshot Snapshot = BuildSnapshot(Hero);
    if (!Snapshot.IsValidForAlpha())
    {
        return;
    }

    const float ViewW = Canvas != nullptr ? Canvas->SizeX : 1280.0f;
    const float ViewH = Canvas != nullptr ? Canvas->SizeY : 720.0f;

    constexpr float Left = 28.0f;
    constexpr float Top = 26.0f;
    DrawPanel(this, Left, Top, 410.0f, 132.0f, FLinearColor(0.015f, 0.022f, 0.035f, 0.78f), FLinearColor(1.0f, 0.74f, 0.22f, 0.95f));
    DrawText(TEXT("EVERWILD ODYSSEY"), FLinearColor(1.0f, 0.88f, 0.42f, 1.0f), Left + 18.0f, Top + 12.0f);
    DrawText(TEXT("Starfall Vale - Alpha Vertical Slice"), FLinearColor(0.70f, 0.92f, 1.0f, 1.0f), Left + 18.0f, Top + 36.0f);
    DrawBar(this, Left + 18.0f, Top + 70.0f, 360.0f, 14.0f, Snapshot.Health, Snapshot.MaxHealth, FLinearColor(0.18f, 0.86f, 0.38f, 1.0f), FLinearColor(0.10f, 0.18f, 0.12f, 0.85f));
    DrawBar(this, Left + 18.0f, Top + 93.0f, 360.0f, 12.0f, Snapshot.Resource, Snapshot.MaxResource, FLinearColor(0.25f, 0.62f, 1.0f, 1.0f), FLinearColor(0.08f, 0.12f, 0.20f, 0.85f));
    DrawText(FormatVitalsLine(Snapshot), FLinearColor(0.84f, 1.0f, 0.86f, 1.0f), Left + 18.0f, Top + 108.0f);

    DrawPanel(this, ViewW - 452.0f, 28.0f, 424.0f, 176.0f, FLinearColor(0.018f, 0.019f, 0.028f, 0.82f), FLinearColor(0.28f, 0.70f, 1.0f, 0.95f));
    DrawText(TEXT("QUEST TRACKER"), FLinearColor(0.70f, 0.92f, 1.0f, 1.0f), ViewW - 432.0f, 44.0f);
    DrawText(FormatObjectiveLine(Snapshot), FLinearColor(1.0f, 0.94f, 0.70f, 1.0f), ViewW - 432.0f, 74.0f);
    DrawText(FormatWorldEventLine(Snapshot), FLinearColor(1.0f, 0.54f, 0.94f, 1.0f), ViewW - 432.0f, 102.0f);
    DrawText(FormatProgressionLine(Snapshot), FLinearColor(0.82f, 0.94f, 1.0f, 1.0f), ViewW - 432.0f, 132.0f);
    DrawText(TEXT("Party: Runeblade + Mira + Tor"), FLinearColor(0.78f, 1.0f, 0.78f, 1.0f), ViewW - 432.0f, 158.0f);

    const float MiniX = ViewW - 218.0f;
    const float MiniY = ViewH - 214.0f;
    DrawPanel(this, MiniX, MiniY, 190.0f, 186.0f, FLinearColor(0.012f, 0.018f, 0.016f, 0.74f), FLinearColor(0.20f, 0.84f, 0.54f, 0.85f));
    DrawText(TEXT("STARFALL VALE"), FLinearColor(0.78f, 1.0f, 0.78f, 1.0f), MiniX + 18.0f, MiniY + 14.0f);
    DrawRect(FLinearColor(0.12f, 0.44f, 0.18f, 0.95f), MiniX + 34.0f, MiniY + 48.0f, 124.0f, 90.0f);
    DrawRect(FLinearColor(0.13f, 0.38f, 0.92f, 0.95f), MiniX + 92.0f, MiniY + 106.0f, 54.0f, 28.0f);
    DrawRect(FLinearColor(0.34f, 0.22f, 0.10f, 0.95f), MiniX + 36.0f, MiniY + 88.0f, 118.0f, 8.0f);
    DrawRect(FLinearColor(1.0f, 0.76f, 0.22f, 1.0f), MiniX + 118.0f, MiniY + 72.0f, 10.0f, 10.0f);
    DrawRect(FLinearColor(0.82f, 0.18f, 1.0f, 1.0f), MiniX + 132.0f, MiniY + 96.0f, 12.0f, 12.0f);
    DrawRect(FLinearColor(0.16f, 0.56f, 1.0f, 1.0f), MiniX + 76.0f, MiniY + 90.0f, 9.0f, 9.0f);
    DrawText(TEXT("Keep   Gate   Surge"), FLinearColor(0.86f, 0.94f, 0.82f, 1.0f), MiniX + 24.0f, MiniY + 150.0f);

    DrawPanel(this, 28.0f, ViewH - 82.0f, 710.0f, 54.0f, FLinearColor(0.012f, 0.012f, 0.018f, 0.76f), FLinearColor(0.50f, 0.58f, 0.72f, 0.9f));
    DrawText(TEXT("Move: WASD / Left Stick   Attack: Mouse1 / X   Dodge: Space / B   Abilities: 1-2 / RB-RT   Interact: E / A"), FLinearColor(1.0f, 0.92f, 0.65f, 1.0f), 46.0f, ViewH - 64.0f);
}

FEOHUDSnapshot AEOHUD::BuildSnapshot(const AEOHeroCharacter* Hero)
{
    FEOHUDSnapshot Snapshot;
    if (Hero == nullptr || Hero->GetCombatStatsComponent() == nullptr || Hero->GetProgressionComponent() == nullptr)
    {
        return Snapshot;
    }

    const UEOCombatStatsComponent* CombatStats = Hero->GetCombatStatsComponent();
    const UEOHeroProgressionComponent* Progression = Hero->GetProgressionComponent();

    Snapshot.Health = CombatStats->GetCurrentHealth();
    Snapshot.MaxHealth = CombatStats->GetStats().MaxHealth;
    Snapshot.Resource = CombatStats->GetCurrentResource();
    Snapshot.MaxResource = CombatStats->GetStats().MaxResource;
    Snapshot.Level = Progression->GetHeroLevel();
    Snapshot.Experience = Progression->GetExperience();
    Snapshot.ExperienceForNextLevel = Progression->GetExperienceForNextLevel();
    Snapshot.RelicShards = Hero->GetRelicShardCount();
    Snapshot.RequiredRelicShards = Hero->GetRequiredRelicShardCount();
    Snapshot.bQuestAccepted = Hero->GetQuestLogComponent() != nullptr
        && Hero->GetQuestLogComponent()->GetQuestState(TEXT("dawnwatch.starfall_arrival")) != EEOQuestState::Available
        && Hero->GetQuestLogComponent()->GetQuestState(TEXT("dawnwatch.starfall_arrival")) != EEOQuestState::Locked;
    Snapshot.bGateStabilized = Hero->IsStarfallGateStabilized();

    int32 ActiveEnemyCount = 0;
    if (UWorld* World = Hero->GetWorld())
    {
        for (TActorIterator<AEOEnemyCharacter> EnemyIt(World); EnemyIt; ++EnemyIt)
        {
            const AEOEnemyCharacter* Enemy = *EnemyIt;
            if (Enemy != nullptr && Enemy->GetCombatStatsComponent() != nullptr && Enemy->GetCombatStatsComponent()->IsAlive())
            {
                ++ActiveEnemyCount;
            }
        }
    }
    Snapshot.ActiveEnemyCount = ActiveEnemyCount;
    return Snapshot;
}

FString AEOHUD::FormatVitalsLine(const FEOHUDSnapshot& Snapshot)
{
    return FString::Printf(
        TEXT("HP %.0f / %.0f  Resource %.0f / %.0f"),
        Snapshot.Health,
        Snapshot.MaxHealth,
        Snapshot.Resource,
        Snapshot.MaxResource);
}

FString AEOHUD::FormatProgressionLine(const FEOHUDSnapshot& Snapshot)
{
    return FString::Printf(
        TEXT("Level %d  XP %d / %d"),
        Snapshot.Level,
        Snapshot.Experience,
        Snapshot.ExperienceForNextLevel);
}

FString AEOHUD::FormatObjectiveLine(const FEOHUDSnapshot& Snapshot)
{
    if (Snapshot.bGateStabilized)
    {
        return TEXT("Objective: Starfall Gate stabilized - return to Dawnwatch Keep");
    }

    if (!Snapshot.bQuestAccepted)
    {
        return TEXT("Objective: Press E / A to accept The Starfall Gate");
    }

    return FString::Printf(
        TEXT("Objective: Collect relic shards %d / %d, then stabilize the Starfall Gate"),
        Snapshot.RelicShards,
        Snapshot.RequiredRelicShards);
}

FString AEOHUD::FormatWorldEventLine(const FEOHUDSnapshot& Snapshot)
{
    if (Snapshot.bGateStabilized)
    {
        return TEXT("World Event: Relic Surge resolved - valley stabilized");
    }

    if (!Snapshot.bQuestAccepted)
    {
        return FString::Printf(TEXT("World Event: Relic Surge warning - %d hostiles sighted"), Snapshot.ActiveEnemyCount);
    }

    return FString::Printf(TEXT("World Event: Relic Surge active - %d hostiles remain"), Snapshot.ActiveEnemyCount);
}
