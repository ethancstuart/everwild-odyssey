#pragma once

#include "CoreMinimal.h"
#include "Core/EOInventoryTypes.h"
#include "Core/EOQuestTypes.h"
#include "Core/EOWorldEventTypes.h"

namespace EOTestData
{
    inline FEOGearItem MakeRelicWeapon()
    {
        FEOGearItem Item;
        Item.ItemId = TEXT("relic.starforged_blade");
        Item.DisplayName = FText::FromString(TEXT("Starforged Blade"));
        Item.Slot = EEOGearSlot::Weapon;
        Item.Rarity = EEOGearRarity::Relic;
        Item.Power = 12;
        return Item;
    }

    inline FEOQuestRecord MakeActiveMainQuest()
    {
        FEOQuestRecord Quest;
        Quest.QuestId = TEXT("main.starfall_disturbance");
        Quest.DisplayName = FText::FromString(TEXT("The Starfall Disturbance"));
        Quest.State = EEOQuestState::Active;
        Quest.StepIndex = 2;
        Quest.ReportedToFaction = EEOFaction::StarfallCollegium;
        return Quest;
    }

    inline FEOWorldEventRecord MakeResolvedRelicSurge()
    {
        FEOWorldEventRecord Event;
        Event.EventId = TEXT("event.relic_surge");
        Event.DisplayName = FText::FromString(TEXT("Relic Surge"));
        Event.State = EEOWorldEventState::Resolved;
        Event.RespawnSeconds = 180.0f;
        return Event;
    }
}
