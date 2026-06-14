#pragma once

#include "CoreMinimal.h"
#include "EOClassTypes.generated.h"

UENUM(BlueprintType)
enum class EEOHeroClass : uint8
{
    Runeblade UMETA(DisplayName = "Runeblade"),
    Starshaper UMETA(DisplayName = "Starshaper"),
    Pathwarden UMETA(DisplayName = "Pathwarden"),
    LightboundVanguard UMETA(DisplayName = "Lightbound Vanguard")
};

UENUM(BlueprintType)
enum class EEOPartyRole : uint8
{
    Striker UMETA(DisplayName = "Striker"),
    Defender UMETA(DisplayName = "Defender"),
    Support UMETA(DisplayName = "Support"),
    Controller UMETA(DisplayName = "Controller")
};

UENUM(BlueprintType)
enum class EEOResourceType : uint8
{
    Stamina UMETA(DisplayName = "Stamina"),
    Mana UMETA(DisplayName = "Mana"),
    Focus UMETA(DisplayName = "Focus"),
    Valor UMETA(DisplayName = "Valor")
};

UENUM(BlueprintType)
enum class EEOGearRarity : uint8
{
    Common UMETA(DisplayName = "Common"),
    Uncommon UMETA(DisplayName = "Uncommon"),
    Rare UMETA(DisplayName = "Rare"),
    Relic UMETA(DisplayName = "Relic")
};

UENUM(BlueprintType)
enum class EEOFaction : uint8
{
    DawnwatchCommand UMETA(DisplayName = "Dawnwatch Command"),
    StarfallCollegium UMETA(DisplayName = "Starfall Collegium"),
    ValeVillagers UMETA(DisplayName = "Vale Villagers")
};
