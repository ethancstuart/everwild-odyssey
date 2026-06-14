#include "Core/EOHeroClassDefinition.h"

namespace
{
FEOAbilityDefinition Ability(FName Id, const TCHAR* Name, const TCHAR* Description, float Cooldown, float Cost, bool bUltimate = false)
{
    FEOAbilityDefinition Result;
    Result.AbilityId = Id;
    Result.DisplayName = FText::FromString(Name);
    Result.Description = FText::FromString(Description);
    Result.CooldownSeconds = Cooldown;
    Result.ResourceCost = Cost;
    Result.bIsUltimate = bUltimate;
    return Result;
}
}

TArray<FEOHeroClassDefinition> FEOWildClassCatalog::BuildStartingClasses()
{
    TArray<FEOHeroClassDefinition> Classes;
    Classes.Reserve(4);

    FEOHeroClassDefinition Runeblade;
    Runeblade.HeroClass = EEOHeroClass::Runeblade;
    Runeblade.DisplayName = FText::FromString(TEXT("Runeblade"));
    Runeblade.FantasySummary = FText::FromString(TEXT("A durable melee striker who carves runes into the battlefield."));
    Runeblade.PrimaryRole = EEOPartyRole::Striker;
    Runeblade.ResourceType = EEOResourceType::Stamina;
    Runeblade.BaseStats = { 135.0f, 100.0f, 16.0f, 8.0f, 8.0f, 610.0f };
    Runeblade.StartingAbilities = {
        Ability(TEXT("runeblade.basic_combo"), TEXT("Iron Verse"), TEXT("A three-hit sword chain that builds rune charge."), 0.0f, 0.0f),
        Ability(TEXT("runeblade.guard"), TEXT("Rune Guard"), TEXT("Raise a warding blade to reduce frontal damage."), 4.0f, 15.0f),
        Ability(TEXT("runeblade.cleave"), TEXT("Rune Cleave"), TEXT("Release a crescent slash that damages enemies in an arc."), 6.0f, 25.0f),
        Ability(TEXT("runeblade.dash"), TEXT("Carve Step"), TEXT("Dash through danger and strike the nearest marked foe."), 8.0f, 20.0f),
        Ability(TEXT("runeblade.ultimate"), TEXT("Relic Surge"), TEXT("Overload the blade with starfire for a devastating finisher."), 45.0f, 100.0f, true)
    };
    Classes.Add(Runeblade);

    FEOHeroClassDefinition Starshaper;
    Starshaper.HeroClass = EEOHeroClass::Starshaper;
    Starshaper.DisplayName = FText::FromString(TEXT("Starshaper"));
    Starshaper.FantasySummary = FText::FromString(TEXT("A ranged spellcaster who bends fallen starlight into shields and lances."));
    Starshaper.PrimaryRole = EEOPartyRole::Support;
    Starshaper.ResourceType = EEOResourceType::Mana;
    Starshaper.BaseStats = { 95.0f, 130.0f, 6.0f, 19.0f, 3.0f, 590.0f };
    Starshaper.StartingAbilities = {
        Ability(TEXT("starshaper.bolt"), TEXT("Star Bolt"), TEXT("Fire a clean bolt of celestial energy."), 0.0f, 0.0f),
        Ability(TEXT("starshaper.field"), TEXT("Arcane Field"), TEXT("Create a zone that slows foes and empowers allies."), 9.0f, 30.0f),
        Ability(TEXT("starshaper.barrier"), TEXT("Barrier Pulse"), TEXT("Shield nearby party members from the next burst of damage."), 12.0f, 35.0f),
        Ability(TEXT("starshaper.lance"), TEXT("Celestial Lance"), TEXT("Pierce a line of enemies with concentrated starlight."), 7.0f, 28.0f),
        Ability(TEXT("starshaper.ultimate"), TEXT("Astral Nova"), TEXT("Collapse gathered starfire into a brilliant area detonation."), 50.0f, 100.0f, true)
    };
    Classes.Add(Starshaper);

    FEOHeroClassDefinition Pathwarden;
    Pathwarden.HeroClass = EEOHeroClass::Pathwarden;
    Pathwarden.DisplayName = FText::FromString(TEXT("Pathwarden"));
    Pathwarden.FantasySummary = FText::FromString(TEXT("A mobile explorer who controls the field with marks, traps, and precision."));
    Pathwarden.PrimaryRole = EEOPartyRole::Controller;
    Pathwarden.ResourceType = EEOResourceType::Focus;
    Pathwarden.BaseStats = { 110.0f, 110.0f, 14.0f, 9.0f, 5.0f, 640.0f };
    Pathwarden.StartingAbilities = {
        Ability(TEXT("pathwarden.shot"), TEXT("Warden Shot"), TEXT("Loose a quick shot or close spear jab based on range."), 0.0f, 0.0f),
        Ability(TEXT("pathwarden.roll_shot"), TEXT("Roll Shot"), TEXT("Evade and fire at the nearest threat while moving."), 5.0f, 18.0f),
        Ability(TEXT("pathwarden.snare"), TEXT("Snare Trap"), TEXT("Place a trap that roots the first enemy to cross it."), 10.0f, 25.0f),
        Ability(TEXT("pathwarden.mark"), TEXT("Hunter's Mark"), TEXT("Mark a priority target for increased party damage."), 8.0f, 20.0f),
        Ability(TEXT("pathwarden.ultimate"), TEXT("Wild Hunt"), TEXT("Call a spectral volley against marked enemies."), 45.0f, 100.0f, true)
    };
    Classes.Add(Pathwarden);

    FEOHeroClassDefinition Vanguard;
    Vanguard.HeroClass = EEOHeroClass::LightboundVanguard;
    Vanguard.DisplayName = FText::FromString(TEXT("Lightbound Vanguard"));
    Vanguard.FantasySummary = FText::FromString(TEXT("A radiant defender who anchors the party with shields, heals, and challenge."));
    Vanguard.PrimaryRole = EEOPartyRole::Defender;
    Vanguard.ResourceType = EEOResourceType::Valor;
    Vanguard.BaseStats = { 155.0f, 100.0f, 12.0f, 12.0f, 12.0f, 575.0f };
    Vanguard.StartingAbilities = {
        Ability(TEXT("vanguard.strike"), TEXT("Shield Strike"), TEXT("Bash a foe and generate valor."), 0.0f, 0.0f),
        Ability(TEXT("vanguard.block"), TEXT("Dawnwall"), TEXT("Brace behind a radiant shield and reduce incoming damage."), 5.0f, 15.0f),
        Ability(TEXT("vanguard.heal"), TEXT("Radiant Mend"), TEXT("Restore health to yourself or the weakest companion."), 12.0f, 35.0f),
        Ability(TEXT("vanguard.challenge"), TEXT("Sunlit Challenge"), TEXT("Force nearby enemies to focus on you briefly."), 10.0f, 25.0f),
        Ability(TEXT("vanguard.ultimate"), TEXT("Dawn Banner"), TEXT("Plant a banner that shields allies and burns enemies."), 55.0f, 100.0f, true)
    };
    Classes.Add(Vanguard);

    return Classes;
}

bool FEOWildClassCatalog::TryGetClassDefinition(EEOHeroClass HeroClass, FEOHeroClassDefinition& OutDefinition)
{
    const TArray<FEOHeroClassDefinition> Classes = BuildStartingClasses();
    for (const FEOHeroClassDefinition& Definition : Classes)
    {
        if (Definition.HeroClass == HeroClass)
        {
            OutDefinition = Definition;
            return true;
        }
    }

    return false;
}
