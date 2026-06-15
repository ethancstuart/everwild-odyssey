# Heroic MMO Asset-First Rebuild Design

Date: 2026-06-15

## Intent

Rebuild the Everwild Odyssey vertical slice around an asset-first visual target: an original heroic fantasy MMO zone with bold proportions, saturated color, readable silhouettes, dense quest presentation, dramatic landmarks, and third-person action readability. The current runtime-authored blockout is no longer an acceptable foundation for the desired experience.

The goal is a private learning project that goes as big as practical on this machine. The result should feel like stepping into a stylized premium MMO-adventure zone, while remaining original: no copied Blizzard assets, names, maps, characters, UI art, music, or exact protected presentation.

## Problem Statement

The current slice compiles and plays, but it reads as a toy prototype because it depends on primitive shapes and code-spawned scenery. It cannot meet the requested visual bar through incremental polish. The project needs a hard presentation reset:

- Use authored or imported art assets as the primary world-building material.
- Treat runtime primitives as temporary debug/proxy geometry only.
- Rebuild the first playable frame around a strong hero, a strong hub/zone vista, a strong hostile camp, and a dense MMO HUD.
- Keep the existing gameplay systems where useful, but re-stage them through a more convincing visual shell.

## Creative Target

The visual language is original "heroic hand-painted MMO fantasy":

- Chunky, exaggerated architecture with large roofs, huge banners, oversized gates, glowing runes, and readable silhouettes.
- Saturated natural color: emerald grass, golden roads, blue-violet magic, warm torchlight, crimson hostile banners, and high-contrast faction colors.
- Big readable character forms: oversized shoulders, large weapons, cape/cloth motion where possible, clear class color language.
- Zone composition that implies scale: hub foreground, winding roads, distant towers, blocked mountain passes, glowing event site, dungeon entrance, and sky/portal landmark.
- MMO UX density: quest tracker, minimap, party frames, target frame, enemy nameplates, action bar, XP/level, loot/event text, and controller prompts.

This target can be inspired by the broad heroic MMO genre, but it must remain Everwild Odyssey: Dawnwatch, Starfall Vale, relic surges, class orders, sky ruin, and original faction identity.

## Asset Strategy

The rebuild should choose the strongest legal asset source available at implementation time:

1. **Installed or local assets first.** Search the project, Epic Vault cache, Unreal feature packs, engine templates, plugin sample content, and any user-installed content. Use real meshes/materials/animations where licenses allow project use.
2. **Free asset acquisition if needed.** If local content is not enough, request permission to use network access and install/import free Unreal/Fab/Quixel assets suitable for stylized fantasy environments, characters, VFX, and UI learning.
3. **Generated support assets where practical.** Use generated placeholder textures/icons/concepts only when they improve the original Everwild identity and do not pretend to be final production art.
4. **Proxy fallback is allowed for mechanics only.** If a required asset is missing, the game may keep a proxy object temporarily, but the visual acceptance check fails until the first frame is asset-led rather than primitive-led.

Repository policy:

- Commit only source files, project config, original lightweight assets, and assets whose license/storage size is appropriate for the public repo.
- Do not commit marketplace cache internals, engine content, derived data, or large proprietary packs without an explicit repo decision.
- Document any local-only asset dependency and provide a fallback or setup note.

## Rebuild Scope

The first rebuild should replace the current Starfall Vale presentation with a compact but dense vertical slice:

- **Dawnwatch Gate Hub:** a heroic town/keep edge with banners, vendors or NPC stand-ins, training props, party staging, and clear roads outward.
- **Starfall Road:** a winding readable route from hub to event site, with props, trees, rocks, fences, lamps, road signs, and ambient movement.
- **Relic Surge Camp:** a hostile field event with colored faction dressing, elite enemies, magic crystals, VFX, nameplates, and combat spacing.
- **Sky-Watch Entrance:** a visible dungeon/portal landmark that becomes the slice's aspirational endpoint.
- **Vista Dressing:** distant mountains, towers, walls, cliffs, portal beams, sky/fog/post-process, and blocked paths suggesting future MMO zones.

The playable loop stays simple and clear:

1. Spawn at Dawnwatch Gate as a Runeblade-style champion.
2. Read the MMO HUD and accept/track the Starfall Gate objective.
3. Move along the road with keyboard/mouse or controller.
4. Fight a staged relic surge group with at least one elite.
5. Gain XP/loot/shards and stabilize the gate or unlock the next route.
6. End with a clear future hook toward the dungeon entrance.

## Gameplay And Camera Direction

The camera should move away from the current toy overhead framing and toward premium third-person MMO readability:

- Third-person trailing camera with adjustable pitch and distance.
- Hero large enough on screen to read silhouette, weapon, shield/cape, and facing.
- Combat framing that keeps the player, target, telegraphs, and nearby enemies readable.
- Controller-friendly movement, target bias/cycling, dodge/block, and ability buttons.
- Optional zoomed-out "MMO exploration" distance, but not so far that the hero becomes a pawn token.

Combat should emphasize feel over breadth:

- One polished hero kit is better than four shallow classes for this reset.
- Basic attack, block/dodge, two abilities, and one ultimate/surge are enough if the animations, VFX, hit feedback, cooldowns, and enemy reactions read well.
- Enemies need silhouettes and behaviors: small minions, caster or ranged unit, and a larger elite sentinel.
- Add readable target frame, nameplates, health bars, damage numbers or hit text, and telegraph color.

## UI Direction

The HUD should intentionally evoke MMO information density while using original art:

- Top-left player and party frames.
- Bottom-center action bar with keyboard and controller bindings.
- Right-side quest tracker with objective progress and world event state.
- Top-center target frame when an enemy is selected or damaged.
- Minimap with hub, road, surge, and gate markers.
- Floating nameplates over enemies and interactables.
- Loot/XP/event toast feed.

The UI should use chunky frames, parchment/dark translucent panels, gold/blue accents, readable type sizes, and icon slots. It must avoid copying WoW's exact UI art, icon art, layout proportions, names, or fonts.

## Technical Design

Introduce a clear separation between game systems and presentation assets:

- **Zone Presentation Layer:** owns placed/imported environment pieces, landmark transforms, lighting, fog, post-process, and vista staging.
- **Asset Resolver:** maps semantic asset roles such as `HeroBody`, `DawnwatchWall`, `RoadLamp`, `RelicCrystal`, `EnemyElite`, and `ActionIconBasic` to available assets or proxies.
- **Encounter Stager:** spawns enemies and event props from authored encounter points rather than arbitrary hard-coded positions.
- **HUD Presentation Model:** gathers player, party, target, quest, event, loot, and control state into stable draw/widget data.
- **Input Contract:** preserves keyboard/mouse and controller mappings and makes them visible in the action bar.

Implementation can remain C++ first, but visual assembly may use Blueprint-friendly data assets or config structs if they speed iteration. The project should not keep growing one monolithic scaffold file for all world content.

## Data Flow

1. Game mode loads the selected zone profile.
2. Zone profile provides asset-role names, landmark anchors, encounter anchors, and HUD theme settings.
3. Asset resolver attempts to load preferred assets and records any proxy fallbacks.
4. Zone presentation layer places hub, road, vista, dungeon entrance, event dressing, lights, and post-process.
5. Encounter stager spawns enemies and event props at named anchors.
6. Player/companion/enemy systems emit gameplay state.
7. HUD presentation model formats the state into player frames, target frame, action bar, quest tracker, minimap, and event toasts.

## Error Handling And Fallbacks

- Missing optional assets should log a readable warning and fall back to an obvious proxy with debug naming.
- Missing required hero, ground, enemy, or HUD assets should fail the visual acceptance checklist even if the build compiles.
- Asset resolver should expose a count/list of proxy fallbacks so tests and launch logs can identify when the game is still under-arted.
- If downloaded/local assets cannot be committed, setup docs must explain where they come from and how to restore them.

## Testing And Verification

Automated tests should cover:

- Asset roles resolve to either real assets or explicit proxy fallbacks.
- Zone profile exposes required landmarks, encounter anchors, and minimap markers.
- Opening encounter has minion, caster/ranged, and elite roles.
- HUD presentation model includes party frames, target frame, action bar, quest tracker, minimap, and event feed data.
- Controller and keyboard mappings remain present.

Manual verification must include:

- Fresh Unreal build succeeds.
- Project automation tests pass.
- Standalone game launches.
- First frame screenshot shows an asset-led heroic fantasy zone, not primitives.
- Hero silhouette, HUD, event site, road, and dungeon/vista landmark are readable at 1280x720.
- Keyboard/mouse movement and combat work.
- Controller input is tested physically when a controller is available.

## Acceptance Criteria

This rebuild is accepted only when:

- The first screen no longer reads as primitive blockout.
- The hero is visibly a fantasy champion, not a basic capsule/shape stack.
- The zone has an authored MMO-adventure composition: hub, road, hostile event, landmark, vista.
- The UI reads like an MMO action RPG interface while remaining original.
- The opening loop is playable and understandable without debug text.
- The branch builds, tests pass, launches standalone, and is pushed to GitHub.

## Explicit Non-Goals

- Do not copy World of Warcraft assets, names, maps, UI art, exact iconography, music, characters, factions, or protected story elements.
- Do not chase full MMO networking in this visual reset.
- Do not build all four classes before one hero presentation feels good.
- Do not continue expanding runtime primitive scenery as the main production path.
- Do not claim AAA quality; aim for the strongest private-learning vertical slice possible with available assets and time.
