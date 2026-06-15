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
