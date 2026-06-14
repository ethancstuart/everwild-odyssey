# Everwild Odyssey Design Spec

Date: 2026-06-14

## Summary

Everwild Odyssey is an original heroic high-fantasy action RPG built in Unreal Engine as a single-player vertical slice that feels like the opening chapter of a much larger MMO-scale world. The game draws inspiration from expansive fantasy questing, expressive traversal, party-based dungeon play, and readable action-platforming, while using original characters, lore, locations, art direction, systems, and assets.

The first playable alpha is set around Dawnwatch Keep, a fortress-town overlooking Starfall Vale. Generations ago, a celestial relic crashed into the valley and was sealed beneath an ancient watchtower. The seal is failing. Dawnwatch's commanders, relic scholars, local villagers, wandering adventurers, and hostile forces are all reacting to the return of the starborn power.

The project should bias toward grandeur, density, and spectacle whenever a decision can safely go bigger without making the slice unplayable. The first build should feel larger than its actual map footprint through layered vistas, locked gates, distant landmarks, ambient NPC activity, faction chatter, class identity, party UI, gear progression, and dungeon mechanics that imply a broader world.

## Approved Product Direction

- Repository: `everwild-odyssey`
- Visibility: public GitHub repository
- Engine: Unreal Engine, using an installed local version if present; otherwise setup includes installing Unreal Engine.
- Implementation style: C++ first.
- Target platform: PC/computer.
- Input: keyboard/mouse and plugged-in controller.
- Game format: 2.5D/isometric adventure zones using Unreal's 3D foundation.
- Initial mode: single-player vertical slice that feels MMO-scale.
- Long-term direction: development past alpha toward MMO-shaped systems.
- Combat: hybrid real-time action and MMO-style abilities.
- Player fantasy: class-based champion plus party leader.
- Starting classes: Runeblade, Starshaper, Pathwarden, Lightbound Vanguard.
- First slice structure: one town, one explorable region, one dungeon, one boss, one faction story thread.
- MMO-scale feel: living world simulation, progression depth, and group-content feel, all implemented compactly.
- Asset strategy: original repo-safe placeholder assets for core gameplay, with optional local-only Unreal/Marketplace assets documented but not committed.

## Creative Foundation

### World

The first region combines two premises:

- Dawnwatch Keep: a heroic frontier fortress and village that serves as the hub.
- Starfall Vale: a crater valley below the keep, shaped by a celestial relic and ancient ruins.

The region should feel like a playable corner of a much larger kingdom. The player should see unavailable roads, distant towers, blocked passes, unreachable ruins, and faction routes that suggest future zones.

### Tone

The tone is heroic high fantasy. The world should be grand, readable, adventurous, and earnest. It should support noble factions, ancient ruins, class orders, dungeon bosses, party roles, and big fantasy silhouettes.

The tone should not become generic. Starfall Vale gives the setting a distinctive celestial mystery: luminous stone, strange relic weather, ruined sky-temple architecture, distorted beasts, and scholars arguing with soldiers over whether the relic is a weapon, blessing, prison, or warning.

### Originality

The project must not copy protected characters, names, maps, music, art, dialogue, or exact mechanics from other games. Inspirations should be translated into original systems and presentation:

- Expansive MMO feel becomes ambient adventurers, faction hubs, quest chains, dungeon roles, gear tiers, and implied future regions.
- Classic adventure feel becomes exploration, puzzle gates, secret paths, relic tools, and satisfying dungeon progression.
- Platforming joy becomes readable traversal challenges, elevation, hazard timing, controller-friendly movement, and optional side routes.

## Player Classes

Each class should be playable in the first slice, even if ability depth starts compact. Every class needs a distinct silhouette, resource pattern, controller-friendly kit, and party role.

### Runeblade

Melee fighter using sword arcs, rune bursts, guard counters, and short dashes. Intended role: durable striker.

Initial kit:

- Basic combo
- Guard or parry
- Rune cleave
- Short dash strike
- Relic surge ultimate

### Starshaper

Ranged spellcaster using starfire, shields, area control, and ally support. Intended role: ranged damage/support.

Initial kit:

- Star bolt
- Arcane field
- Barrier pulse
- Celestial lance
- Astral nova ultimate

### Pathwarden

Bow/spear explorer using traps, mobility, marks, and survival tools. Intended role: mobile controller/skirmisher.

Initial kit:

- Bow shot or spear jab
- Roll shot
- Snare trap
- Marked target
- Wild hunt ultimate

### Lightbound Vanguard

Paladin-style frontline class using shields, heals, radiant strikes, and protective auras. Intended role: tank/support.

Initial kit:

- Shield strike
- Block stance
- Radiant heal
- Taunt or challenge
- Dawn banner ultimate

## Companions And Party Feel

The player controls one hero and travels with two AI companions during the slice. Companions should create the fantasy of group content without requiring multiplayer.

Initial companion roles:

- Defender companion: holds enemy attention, blocks lanes, uses taunt or guard abilities.
- Mystic companion: heals, shields, comments on relic lore, and supports boss mechanics.

Companions should have:

- Follow behavior
- Combat engagement
- Basic ability cooldowns
- Downed or low-health state
- Party UI frames
- Contextual barks during quests, traversal, and boss phases

Companions do not need deep tactical commands in the first alpha. The first target is believability, readability, and party identity.

## Core Gameplay Loop

The vertical slice loop:

1. Choose a class at the title or camp screen.
2. Enter Dawnwatch Keep.
3. Meet faction NPCs and companions.
4. Accept the main Starfall disturbance quest.
5. Explore the valley, completing optional events and gathering loot.
6. Unlock the old watchtower ruin.
7. Enter the dungeon with companions.
8. Solve traversal/combat gates.
9. Defeat the relic-corrupted guardian boss.
10. Return to Dawnwatch for rewards, world-state changes, and future-zone hooks.

The slice should support short-session fun and longer exploration. The critical path should be completable, but side activities should make the zone feel broad.

## World Structure

### Dawnwatch Keep

Purpose: town hub, onboarding, vendors, class flavor, faction tension, quest turn-ins.

Key spaces:

- Main gate and training yard
- Command hall
- Scholar's observatory
- Village market
- Companion camp
- Locked roads to future regions

NPC types:

- Keep commander
- Relic scholar
- Village elder
- Gear vendor
- Class trainers or representatives
- Ambient adventurers
- Guards and patrols

### Starfall Vale

Purpose: explorable open region that suggests MMO scale while remaining compact.

Key spaces:

- Crater overlook
- Broken caravan route
- Luminous crystal fields
- Patrol road
- Beast den
- Ancient lift or bridge puzzle
- Watchtower entrance
- Distant blocked landmarks

Events:

- Patrol ambush
- Relic surge spawning enemies
- NPC adventurer rescue
- Elite miniboss roaming route
- Resource node discovery

### Sky-Watch Ruin

Purpose: dungeon beneath the old watchtower and celestial ruin.

Dungeon beats:

- Entry chamber with relic seal
- Combat room teaching party roles
- Traversal/puzzle gate using elevation and timing
- Mid-dungeon elite encounter
- Boss arena with phase changes

Boss: the Starbound Warden, a guardian corrupted by the failing relic seal.

Boss mechanics:

- Telegraph attacks
- Add waves
- Shield phase requiring companion support
- Ground hazards
- Final relic surge phase

## Combat Design

Combat should be hybrid action/MMO:

- Real-time movement
- Soft targeting
- Dodge and block
- Basic attacks
- Small ability bar
- Cooldowns
- Class resource or charge meter
- Enemy telegraphs
- Controller-friendly input

The first alpha should prioritize feel over system breadth. Each class needs a working core loop, but numbers and upgrades can be simple.

Controller mapping target:

- Left stick: move
- Right stick: camera or target bias
- Face buttons: basic attack, dodge, interact, ability modifier
- Shoulder buttons/triggers: block, primary ability, secondary ability, target cycle
- D-pad: consumables or party/quest shortcuts
- Start/menu: pause and settings

Keyboard/mouse should remain fully playable.

## Progression

Progression should create MMO-scale flavor without requiring large data volume.

Initial systems:

- XP and level display
- Class level or hero level
- Gear slots
- Gear rarity colors
- Simple item stats
- Quest rewards
- Ability unlocks or upgrades
- Save/load profile

Gear slots for alpha:

- Weapon
- Armor
- Trinket

Rarities:

- Common
- Uncommon
- Rare
- Relic

The first slice should include a few class-flavored rewards and one boss drop that feels special.

## Living World Simulation

The world should feel populated even though it is single-player.

Initial simulation features:

- Ambient NPC schedules or loops
- Guard patrols
- Wandering adventurer NPCs
- Respawning enemy camps
- Timed relic surge events
- Vendors and trainers
- World-state changes after major quest steps

NPCs can use lightweight behavior trees or C++ AI controllers. The goal is not deep simulation; it is a credible illusion of a world that continues beyond the player.

## Quest And Faction Design

The first faction thread should involve three pressures:

- Dawnwatch Command wants to secure the valley and protect the keep.
- The Starfall Collegium wants to study and possibly use the relic.
- Local villagers want safety, answers, and compensation for danger.

Main quest chain:

1. Report to Dawnwatch.
2. Investigate Starfall activity.
3. Rescue or assist a patrol.
4. Recover a relic fragment.
5. Choose which faction receives the first report.
6. Open the watchtower seal.
7. Defeat the Starbound Warden.
8. Return to Dawnwatch and see the faction reaction.

The faction choice can be lightweight in alpha: different dialogue, reward flavor, and hub world-state marker. It should be implemented in a way that can expand later.

## Technical Architecture

The project should be Unreal Engine C++ first. Blueprints may exist for content tuning, but core gameplay systems should be implemented in C++.

Core modules/classes should be separated by responsibility:

- Game mode and game instance
- Player character and class definitions
- Enhanced Input mapping
- Ability system or lightweight custom ability framework
- Combat components
- Health/resource components
- Inventory and gear components
- Quest state component/subsystem
- Companion AI controller and behavior logic
- Enemy AI controller
- Interaction system
- Save game system
- UI view models/widgets
- World event manager

If Unreal's Gameplay Ability System is practical in the installed setup, evaluate it for long-term suitability. If it adds too much ceremony for the first alpha, implement a smaller C++ ability framework that can later migrate or interoperate.

## Content And Asset Strategy

The public repo should use repo-safe original placeholders:

- Simple custom meshes
- Basic materials
- Primitive environment pieces
- Placeholder VFX
- Placeholder UI art
- Generated or simple audio only if license-safe

Optional high-detail assets may be used locally but must not be committed unless their license explicitly permits public redistribution.

Large generated files, derived build artifacts, local Marketplace assets, and installed engine content should stay out of git. The repo should document how to add optional local content.

## Testing And Verification

Testing should scale with the project:

- Compile/build checks after C++ changes
- Unreal automation tests for pure systems where feasible
- Manual playtest checklist for controller, keyboard/mouse, class selection, combat, quest progression, dungeon completion, boss defeat, save/load
- Regression checklist before claiming the slice playable

Minimum first-alpha acceptance criteria:

- Project opens in Unreal.
- C++ build succeeds.
- Player can select all four classes.
- Controller and keyboard/mouse movement work.
- Player can enter Dawnwatch, accept the main quest, explore Starfall Vale, enter the dungeon, defeat the boss, receive a reward, and save progress.
- Companions follow and participate in combat.
- At least one living-world event runs in the open zone.
- The game communicates future MMO scale through UI, NPCs, locked roads, faction hooks, and world presentation.

## Implementation Posture

During implementation, make decisions autonomously within this spec. When a choice arises, prefer the option that makes the game feel:

- Larger
- More polished
- More heroic
- More systemic
- More expandable past alpha

Do not let grandeur break playability. The first slice must remain buildable, runnable, and testable.

## Non-Goals For First Alpha

- Real multiplayer networking
- User accounts
- Online servers
- Full MMO persistence
- Large-scale procedural world generation
- Complete class talent trees
- Full economy or crafting depth
- Final art, animation, or audio polish
- Marketplace asset dependency inside the public repo

These may be future directions, but the first alpha should establish the playable foundation.
