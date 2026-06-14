#include "Gameplay/EOHUD.h"

#include "Gameplay/EOCombatStatsComponent.h"
#include "Gameplay/EOHeroCharacter.h"
#include "Gameplay/EOHeroProgressionComponent.h"

void AEOHUD::DrawHUD()
{
    Super::DrawHUD();

    const AEOHeroCharacter* Hero = Cast<AEOHeroCharacter>(GetOwningPawn());
    const FEOHUDSnapshot Snapshot = BuildSnapshot(Hero);
    if (!Snapshot.IsValidForAlpha())
    {
        return;
    }

    constexpr float Left = 32.0f;
    float Top = 32.0f;
    constexpr float LineHeight = 24.0f;

    DrawText(TEXT("Everwild Odyssey - Alpha Valley"), FLinearColor::White, Left, Top);
    Top += LineHeight;
    DrawText(FormatVitalsLine(Snapshot), FLinearColor(0.7f, 1.0f, 0.8f, 1.0f), Left, Top);
    Top += LineHeight;
    DrawText(FormatProgressionLine(Snapshot), FLinearColor(0.8f, 0.9f, 1.0f, 1.0f), Left, Top);
    Top += LineHeight;
    DrawText(TEXT("Move: WASD / Left Stick   Attack: Mouse1 / X   Abilities: 1-2 / RB-RT   Interact: E / A"), FLinearColor(1.0f, 0.92f, 0.65f, 1.0f), Left, Top);
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
