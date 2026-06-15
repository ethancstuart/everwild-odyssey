#include "UI/EOHUDPresentationTypes.h"

namespace
{
FText HudText(const TCHAR* Value)
{
    return FText::FromString(Value);
}

FEOHUDUnitFrame UnitFrame(
    const TCHAR* DisplayName,
    const TCHAR* RoleId,
    const TCHAR* TargetRole,
    float Health,
    float MaxHealth,
    const FLinearColor& RoleColor)
{
    FEOHUDUnitFrame Result;
    Result.DisplayName = HudText(DisplayName);
    Result.RoleId = FName(RoleId);
    Result.TargetRole = TargetRole;
    Result.Health = Health;
    Result.MaxHealth = MaxHealth;
    Result.RoleColor = RoleColor;
    return Result;
}

FEOHUDActionSlot ActionSlot(
    const TCHAR* ActionId,
    const TCHAR* DisplayName,
    const TCHAR* KeyboardGlyph,
    const TCHAR* ControllerGlyph,
    float CooldownRemaining,
    const FLinearColor& SlotColor)
{
    FEOHUDActionSlot Result;
    Result.ActionId = FName(ActionId);
    Result.DisplayName = HudText(DisplayName);
    Result.KeyboardGlyph = KeyboardGlyph;
    Result.ControllerGlyph = ControllerGlyph;
    Result.CooldownRemaining = CooldownRemaining;
    Result.SlotColor = SlotColor;
    return Result;
}

FEOHUDEventFeedEntry FeedEntry(const TCHAR* Message, const FLinearColor& Color)
{
    FEOHUDEventFeedEntry Result;
    Result.Message = HudText(Message);
    Result.Color = Color;
    return Result;
}
}

bool FEOHUDUnitFrame::IsValidForAlpha() const
{
    return !DisplayName.IsEmpty()
        && !RoleId.IsNone()
        && !TargetRole.IsEmpty()
        && MaxHealth > 0.0f
        && Health >= 0.0f
        && Health <= MaxHealth
        && RoleColor.A > 0.0f;
}

bool FEOHUDActionSlot::IsValidForAlpha() const
{
    return !ActionId.IsNone()
        && !DisplayName.IsEmpty()
        && !KeyboardGlyph.IsEmpty()
        && !ControllerGlyph.IsEmpty()
        && CooldownRemaining >= 0.0f
        && SlotColor.A > 0.0f;
}

bool FEOHUDEventFeedEntry::IsValidForAlpha() const
{
    return !Message.IsEmpty() && Color.A > 0.0f;
}

bool FEOHUDPresentationModel::IsValidForAlpha() const
{
    if (!PlayerFrame.IsValidForAlpha()
        || !TargetFrame.IsValidForAlpha()
        || QuestTitle.IsEmpty()
        || QuestObjective.IsEmpty()
        || WorldEventText.IsEmpty()
        || PartyFrames.Num() < 2
        || ActionSlots.Num() < 8
        || MinimapMarkers.Num() < 8
        || FeedEntries.Num() < 3)
    {
        return false;
    }

    for (const FEOHUDUnitFrame& PartyFrame : PartyFrames)
    {
        if (!PartyFrame.IsValidForAlpha())
        {
            return false;
        }
    }

    for (const FEOHUDActionSlot& ActionSlotSpec : ActionSlots)
    {
        if (!ActionSlotSpec.IsValidForAlpha())
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

    return true;
}

FString FEOHUDPresentationModel::FindGlyphForAction(FName ActionId) const
{
    for (const FEOHUDActionSlot& ActionSlotSpec : ActionSlots)
    {
        if (ActionSlotSpec.ActionId == ActionId)
        {
            return ActionSlotSpec.ControllerGlyph;
        }
    }

    return FString();
}

FEOHUDPresentationModel FEOHUDPresentationModel::BuildAlphaFixture()
{
    FEOHUDPresentationModel Model;
    Model.PlayerFrame = UnitFrame(TEXT("Runeblade"), TEXT("hero.runeblade"), TEXT("Player"), 120.0f, 135.0f, FLinearColor(0.18f, 0.62f, 1.0f, 1.0f));
    Model.PartyFrames = {
        UnitFrame(TEXT("Mira"), TEXT("companion.mira"), TEXT("Support"), 88.0f, 100.0f, FLinearColor(0.32f, 0.92f, 0.72f, 1.0f)),
        UnitFrame(TEXT("Tor"), TEXT("companion.tor"), TEXT("Defender"), 142.0f, 160.0f, FLinearColor(1.0f, 0.62f, 0.18f, 1.0f))
    };
    Model.TargetFrame = UnitFrame(TEXT("Relic Sentinel"), TEXT("enemy.relic_sentinel"), TEXT("Elite"), 310.0f, 360.0f, FLinearColor(0.82f, 0.20f, 1.0f, 1.0f));

    Model.ActionSlots = {
        ActionSlot(TEXT("BasicAttack"), TEXT("Strike"), TEXT("Mouse1"), TEXT("X"), 0.0f, FLinearColor(1.0f, 0.76f, 0.24f, 1.0f)),
        ActionSlot(TEXT("Dodge"), TEXT("Dodge"), TEXT("Space"), TEXT("B"), 0.0f, FLinearColor(0.28f, 0.70f, 1.0f, 1.0f)),
        ActionSlot(TEXT("RelicSlash"), TEXT("Relic Slash"), TEXT("1"), TEXT("RB"), 2.4f, FLinearColor(0.72f, 0.20f, 1.0f, 1.0f)),
        ActionSlot(TEXT("ShieldPulse"), TEXT("Shield Pulse"), TEXT("2"), TEXT("RT"), 6.0f, FLinearColor(0.22f, 0.86f, 0.54f, 1.0f)),
        ActionSlot(TEXT("Interact"), TEXT("Interact"), TEXT("E"), TEXT("A"), 0.0f, FLinearColor(1.0f, 0.94f, 0.70f, 1.0f)),
        ActionSlot(TEXT("TargetCycle"), TEXT("Target"), TEXT("Tab"), TEXT("Y"), 0.0f, FLinearColor(1.0f, 0.54f, 0.94f, 1.0f)),
        ActionSlot(TEXT("Potion"), TEXT("Tonic"), TEXT("Q"), TEXT("DPad Up"), 12.0f, FLinearColor(0.18f, 0.86f, 0.38f, 1.0f)),
        ActionSlot(TEXT("Ultimate"), TEXT("Starfall Break"), TEXT("R"), TEXT("LB+RB"), 45.0f, FLinearColor(1.0f, 0.42f, 0.20f, 1.0f))
    };

    Model.QuestTitle = HudText(TEXT("The Starfall Gate"));
    Model.QuestObjective = HudText(TEXT("Break the siphons and stabilize the relic surge."));
    Model.WorldEventText = HudText(TEXT("Relic Surge: elite phase pending"));
    Model.MinimapMarkers = FEOZoneProfileCatalog::BuildStarfallValeProfile().MinimapMarkers;
    Model.FeedEntries = {
        FeedEntry(TEXT("Mira: Barrier ready for the next surge."), FLinearColor(0.32f, 0.92f, 0.72f, 1.0f)),
        FeedEntry(TEXT("Tor: Sentinel attention locked."), FLinearColor(1.0f, 0.62f, 0.18f, 1.0f)),
        FeedEntry(TEXT("Relic siphon destabilized: elite phase pending."), FLinearColor(1.0f, 0.54f, 0.94f, 1.0f))
    };
    return Model;
}
