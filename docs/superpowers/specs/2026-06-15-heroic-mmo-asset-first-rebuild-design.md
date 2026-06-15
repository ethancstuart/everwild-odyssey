# Heroic MMO Asset-First Rebuild Design

Date: 2026-06-15

## Intent

Rebuild the Everwild Odyssey vertical slice around an asset-first visual target: an original heroic fantasy MMO zone with bold proportions, saturated color, readable silhouettes, dense quest presentation, dramatic landmarks, and third-person action readability. The current runtime-authored blockout is no longer an acceptable foundation for the desired experience.

The goal is a private learning project that goes as big as practical on this machine. The result should feel like stepping into a stylized premium MMO-adventure zone with real art direction, authored composition, systems depth, combat readability, and a production-minded content pipeline, while remaining original: no copied Blizzard assets, names, maps, characters, UI art, music, or exact protected presentation.

## Problem Statement

The current slice compiles and plays, but it reads as a toy prototype because it depends on primitive shapes and code-spawned scenery. It cannot meet the requested visual bar through incremental polish. The project needs a hard presentation reset:

- Use authored or imported art assets as the primary world-building material.
- Treat runtime primitives as temporary debug/proxy geometry only.
- Rebuild the first playable frame around a strong hero, a strong hub/zone vista, a strong hostile camp, and a dense MMO HUD.
- Keep the existing gameplay systems where useful, but re-stage them through a more convincing visual shell.

The prior approach optimized for code completeness. The new approach optimizes for the whole player impression: composition, scale, animation, material language, UI density, input feel, combat responsiveness, and the fantasy that a larger world exists beyond the current playable boundary.

## Production Pillars

The rebuild is guided by eight pillars. Every design and implementation decision should serve at least one of them:

1. **First-frame credibility.** Within three seconds, the player should see a real fantasy champion, an authored zone, a clear road into danger, a dramatic landmark, and an MMO-style interface.
2. **Heroic exaggeration.** Shapes should be bold: broad shoulders, oversized weapons, thick gates, large banners, chunky props, strong color coding, and readable enemy silhouettes.
3. **Dense but legible MMO information.** The HUD should feel rich without becoming noise: party frames, target frame, action bar, quest tracker, minimap, nameplates, loot feed, XP, cooldowns, and world-event state.
4. **Systems imply scale.** Even in single-player, the player should feel factions, reputation, public events, patrols, elites, dungeon roles, loot tiers, vendors, and future-zone hooks.
5. **Asset-led production discipline.** The game must stop relying on one giant procedural scaffold. The world, HUD, characters, encounters, and VFX should be data-driven enough to replace assets without rewriting gameplay.
6. **Aspirational systems, staged delivery.** The architecture should point toward classes, talents, dungeons, factions, crafting, events, and future multiplayer, while the first build lands a smaller playable spine.
7. **Readable spectacle.** Big effects, big silhouettes, and big UI moments are encouraged only when they preserve player understanding.
8. **Designer-operable content.** Quests, encounters, loot, events, and zone composition should move toward data profiles and editor-friendly surfaces instead of requiring code edits for every content change.

## North-Star Game Vision

The long-term fantasy is an original single-player-first MMO-shaped action RPG that can grow toward online systems later. The player is a champion of Dawnwatch exploring a continent fractured by falling star relics, faction conflicts, sealed sky ruins, corrupted wildlife, and rival adventuring orders.

The full game vision has five layers:

1. **Continent-scale world:** multiple stylized zones, each with a hub, quest chains, public events, dungeon entrances, faction conflicts, and skyline landmarks.
2. **Class identity:** distinct playable classes with silhouettes, resources, talent paths, ability upgrades, gear preferences, and role fantasy.
3. **Living zone systems:** patrols, public events, faction reputation, rare elites, treasure pockets, vendors, gathering nodes, and world-state changes after quest progress.
4. **Group-content illusion:** AI companions, role frames, dungeon mechanics, threat/targeting, boss phases, party barks, and reward pacing that evoke MMO dungeon play without requiring multiplayer.
5. **Production pipeline:** asset packs, style normalization, data-driven zone profiles, encounter profiles, quest profiles, HUD profiles, visual diagnostics, performance budgets, and repeatable build/verification scripts.

The vertical slice is not the whole game. It is the foundation slice that proves those layers can exist together.

## Scope Tiers

To go big without losing the build, the spec separates ambition into tiers:

### Tier 1: Spectacular Playable Slice

This is the immediate implementation target. It must deliver one hero, one dense micro-zone, one public event, one elite phase, one strong HUD, one reward loop, and one visually convincing first frame.

### Tier 2: Systems Bedrock

This is built only where it directly supports Tier 1, but the shape should be future-safe: data-driven assets, encounters, quests, UI state, loot, faction state, and world-event phases.

### Tier 3: Expansion Hooks

These are visible in the slice but not fully built: class trainers, locked roads, dungeon entrance, future-zone map markers, unused gear slots, faction reputation, party roles, and talent preview stubs.

### Tier 4: Long-Term MMO Direction

This remains a design horizon, not an immediate promise: networking, real multiplayer parties, persistence servers, auction/economy systems, large zone streaming, guilds, raids, and live service operations.

Every implementation plan should label work as Tier 1, Tier 2, Tier 3, or Tier 4 so ambition does not blur into uncontrolled scope.

## Creative Target

The visual language is original "heroic hand-painted MMO fantasy":

- Chunky, exaggerated architecture with large roofs, huge banners, oversized gates, glowing runes, and readable silhouettes.
- Saturated natural color: emerald grass, golden roads, blue-violet magic, warm torchlight, crimson hostile banners, and high-contrast faction colors.
- Big readable character forms: oversized shoulders, large weapons, cape/cloth motion where possible, clear class color language.
- Zone composition that implies scale: hub foreground, winding roads, distant towers, blocked mountain passes, glowing event site, dungeon entrance, and sky/portal landmark.
- MMO UX density: quest tracker, minimap, party frames, target frame, enemy nameplates, action bar, XP/level, loot/event text, and controller prompts.

This target can be inspired by the broad heroic MMO genre, but it must remain Everwild Odyssey: Dawnwatch, Starfall Vale, relic surges, class orders, sky ruin, and original faction identity.

## Art Bible

The rebuild needs an explicit art bible so the result does not become a random mix of free assets.

### Shape Language

- Dawnwatch: sturdy heroic geometry, blue-and-gold banners, arched gates, thick stone, warm lamps, broad rooflines, guard towers, and clean readable roads.
- Starfall relics: violet-blue crystalline forms, floating shards, curved ancient stone, star-map engravings, soft bloom, and vertical beams that pull the eye across the zone.
- Hostile faction: jagged silhouettes, red-black banners, corrupted crystals, spiked barricades, harsh torchlight, and enemy units with aggressive asymmetry.
- Natural world: saturated greens, strong tree silhouettes, chunky rocks, flower/grass clusters in clumps, and readable paths rather than noisy scatter.

### Color Script

- Hub safety: blue, gold, warm orange torchlight, green grass.
- Road adventure: green, tan, white stone, moving shadows.
- Relic mystery: cyan, violet, magenta, deep blue fog.
- Combat danger: crimson, hot orange telegraphs, blackened stone, hostile banners.
- Dungeon invitation: cool shadow, moonlit stone, vertical portal beam, distant gold reward light.

### Material Direction

- Prefer stylized, hand-painted, or clean PBR assets over photorealistic realism.
- If asset styles clash, use color grading, shared material instances, outline/rim lighting, and scale normalization to pull them into one world.
- Avoid tiny realistic detail that collapses at third-person distance. Big forms matter more than micro texture fidelity.

### Visual Density Rules

- The first playable frame must include foreground hero detail, midground road/event information, and background landmark/vista.
- Every sub-area needs one landmark silhouette visible from at least two other areas.
- Props should be clustered around story purpose: market, guard post, ritual site, barricade, camp, shrine, bridge, or tower.
- Empty fields are allowed only as readable movement/combat space, not as unfinished terrain.

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

## Asset Acquisition Targets

The implementation plan should actively look for these categories, in this order of importance:

1. **Hero character and animation set:** third-person fantasy humanoid, idle/run/strafe/attack/block/dodge/hit/death, weapon sockets, cape or cloak if available.
2. **Enemy character set:** at least three readable enemy roles: minion, caster/ranged, elite/brute.
3. **Stylized environment kit:** walls, gates, towers, roofs, carts, crates, banners, lamps, fences, bridges, rocks, ruins, roads, and village props.
4. **Vegetation and terrain dressing:** stylized trees, shrubs, grass clusters, flowers, cliff rocks, boulders, and water.
5. **VFX:** attack trails, impact sparks, healing/shield glow, relic crystals, portal beam, elite telegraphs, ambient particles.
6. **UI icons and frames:** original or license-safe icon set, action bar slots, quest markers, minimap icons, role markers, loot rarity frames.
7. **Audio if available:** footsteps, weapon hits, UI clicks, magic cast, ambient wind, campfire, portal hum, combat stingers.

If a category cannot be sourced, the implementation plan must call that out explicitly and choose either a generated/original replacement, a temporary proxy, or a smaller milestone.

## Asset Curation And Style Normalization

A larger asset library is useful only if it becomes cohesive. The implementation plan should include a curation pass with these rules:

- Pick one dominant environment style and force secondary assets to support it.
- Normalize scale so doors, stairs, weapons, trees, rocks, and walls fit heroic proportions.
- Create shared material instances or post-process/color grading to reduce pack mismatch.
- Prefer 20 high-impact curated assets over 200 uncurated imports.
- Tag assets by role, faction, biome, scale, collision, and visual importance.
- Maintain an asset manifest documenting source, license, commit policy, local-only status, and fallback role.
- Use proxy colors that communicate missing status: debug magenta for missing required assets, neutral grey for optional proxies, faction tint for intentional stylized primitives.

The visual target is a unified Everwild art direction, not a museum of downloaded packs.

## Originality And Inspiration Boundary

The private learning context allows broad inspiration study, but the project should still train good creative discipline:

- Study the broad genre traits: heroic scale, saturated zones, readable silhouettes, action bars, quest density, and faction-driven zones.
- Translate those traits into original Dawnwatch, Starfall, Scholarium, and Ashen Covenant language.
- Do not import, trace, recreate, or imitate exact proprietary maps, UI frames, icons, names, faction marks, class names, characters, armor sets, spell names, cinematics, audio, or music.
- If a design feels too close to a specific protected source, rename, recolor, reshape, and reframe it until it reads as Everwild.
- Use "what role does this serve?" as the test. Copying an expression is out; building an original system to serve the same player-facing role is in.

## Rebuild Scope

The first rebuild should replace the current Starfall Vale presentation with a compact but dense vertical slice that feels like the opening edge of a much larger continent. It should be designed as a "micro-zone" with the density of a showcase demo rather than a sparse open field.

- **Dawnwatch Gate Hub:** a heroic town/keep edge with banners, vendors or NPC stand-ins, training props, party staging, and clear roads outward.
- **Starfall Road:** a winding readable route from hub to event site, with props, trees, rocks, fences, lamps, road signs, and ambient movement.
- **Relic Surge Camp:** a hostile field event with colored faction dressing, elite enemies, magic crystals, VFX, nameplates, and combat spacing.
- **Sky-Watch Entrance:** a visible dungeon/portal landmark that becomes the slice's aspirational endpoint.
- **Vista Dressing:** distant mountains, towers, walls, cliffs, portal beams, sky/fog/post-process, and blocked paths suggesting future MMO zones.
- **Side Spaces:** at least two optional pockets such as a ruined shrine, wounded adventurer camp, treasure alcove, elite patrol loop, broken bridge, or resource node cluster.
- **Layered Boundaries:** blocked gates, broken bridges, mountain passes, fog walls, patrol barricades, or locked portals that imply content beyond the slice.

The playable loop stays simple and clear:

1. Spawn at Dawnwatch Gate as a Runeblade-style champion.
2. Read the MMO HUD and accept/track the Starfall Gate objective.
3. Move along the road with keyboard/mouse or controller.
4. Fight a staged relic surge group with at least one elite.
5. Gain XP/loot/shards and stabilize the gate or unlock the next route.
6. End with a clear future hook toward the dungeon entrance.

## Zone Macro-Design

The zone should be arranged as a readable loop, not a flat arena:

1. **Spawn Vista:** the camera starts behind the hero at Dawnwatch Gate, looking down the road toward the relic beam and Sky-Watch silhouette.
2. **Hub Ring:** compact safe area with NPCs, banners, class flavor props, vendors, and companion staging.
3. **Road Spine:** primary navigational path, shaped with fences, lamps, cliffs, tree lines, and road curves.
4. **Event Bowl:** open combat space with hostile camp dressing, raised elite position, caster pockets, cover/rocks, and VFX center.
5. **Reward/Reveal Path:** after event progress, a gate or bridge visually points toward the dungeon entrance.
6. **Future-Zone Edges:** visible but blocked off roads to mountain, forest, coast, or city zones.

The zone should support a clean 5-8 minute critical path while looking like a 20-30 minute MMO zone teaser.

## Continent And Zone Roadmap

The vertical slice should hint at a larger world with named future zones. These do not need implementation now, but they give art direction and world-state hooks purpose:

- **Starfall Vale:** starter frontier, Dawnwatch hub, relic surge, sky ruin dungeon.
- **Briarfen March:** wetland/forest zone with Pathwarden fantasy, beast factions, ruins under roots, gathering/crafting hooks.
- **Sunspire Expanse:** bright desert/mesa zone with ancient titan roads, caravan hubs, elite patrols, and long sightlines.
- **Frostwake Reach:** snowy mountain zone with fortress passes, frozen relic storms, and defensive faction warfare.
- **Emberdeep Foundry:** volcanic dungeon/industrial zone with crafting, siege engines, and heavy elite enemies.
- **Moonwell Archive:** magical city/academy zone for Starshaper identity, puzzles, portals, and lore systems.

Starfall Vale should include visual teasers for at least two future zones through road signs, locked gates, distant landmarks, NPC dialogue, or minimap edge labels.

## Dawnwatch Gate Hub Detail

The hub should feel like a compressed MMO town:

- Entrance gate with readable silhouette and banner identity.
- Training yard with target dummies, weapon racks, class props, and combat tutorial affordances.
- Command post with quest giver, map table, guards, and future-zone notices.
- Scholar station with relic tools, glowing charts, crystals, and lore barks.
- Vendor corner with armor/weapons crates, rarity color hints, and shop marker.
- Companion camp with two companions, bedrolls, food/fire props, party setup cue, and barks.
- Locked exits: one bridge, one gate, one mountain road, or one portal that visibly implies expansion.

The hub must be small enough to walk through quickly but dense enough to read as a real place.

## Starfall Road Detail

The road should teach the world without UI exposition:

- Safe zone transitions from blue/gold Dawnwatch dressing into violet relic contamination.
- Environmental storytelling: broken cart, fallen banner, wounded scout, corrupted wildlife corpse, warning signs, abandoned tools.
- Navigation affordances: lamps, fences, terrain edges, road curves, signposts, and camera-facing landmark.
- Optional side pocket: a rescue, chest, shrine, or elite warning route.
- Combat spacing: enough room for dodge/block/ability testing before the event bowl.

## Relic Surge Camp Detail

The event area is the slice's main combat showcase:

- Three-layer staging: outer minion guards, caster/ranged pockets, elite center.
- Distinct faction props: Ashen Covenant banners, barricades, crates, relic siphons, corrupted crystals.
- Strong event center: floating crystal, broken altar, portal rift, or sky beam.
- Mechanics readability: red/orange danger telegraphs, blue/gold stabilization VFX, magenta corruption VFX.
- Reward reveal: after victory, the center changes state and points the player toward Sky-Watch.

## Sky-Watch Entrance Detail

The dungeon entrance should feel like a promise:

- Visible from spawn and event bowl.
- Locked or partially opened after event stabilization.
- Different material language than Dawnwatch: ancient curved stone, star-map engravings, cool moonlit color, floating relic pieces.
- UI hook: quest tracker updates to "Sky-Watch seal weakened" or equivalent.
- Optional camera pan or beacon pulse after event completion if feasible.

## World Simulation Layer

The slice should feel alive through lightweight simulation:

- Ambient NPCs: guards, vendors, scholars, injured adventurers, patrols, or training dummies with idle loops.
- Patrol routes: friendly guard path in the hub, hostile patrol near the event, and one elite route around the camp.
- World event state: calm, warning, active surge, elite phase, stabilized.
- Faction presence: Dawnwatch colors in safe areas; hostile corruption colors in event areas.
- Interaction markers: quest giver, vendor, dungeon gate, loot, wounded NPC, and relic node.
- Barks/toasts: short original lines for quest start, event escalation, elite spawn, victory, and future hook.

This layer should be deterministic and local. It does not need networking, persistence, or complex schedules yet.

## Quest And Narrative Systems

The slice should move beyond a single objective string toward a quest profile model:

- Quest id, title, giver, faction, intro text, objectives, rewards, completion text, and next hook.
- Objective types: talk, travel, defeat, collect, activate, escort/rescue, discover, survive event.
- Quest tracker formatting separate from quest data.
- World barks tied to quest phase.
- Companion barks tied to landmark entry, event escalation, low health, victory, and future hooks.
- Quest rewards: XP, item, reputation, shard/currency, and unlock state.

Initial quest chain:

1. **Hold the Gate:** speak to Dawnwatch commander, learn the road is unsafe.
2. **Relic Surge Warning:** travel to the event site and assess corruption.
3. **Break the Siphons:** defeat enemies and disable relic focus points.
4. **Sentinel Awakens:** elite phase and survival/defeat objective.
5. **Seal Stabilized:** return or open route toward Sky-Watch entrance.

The first implementation may compress this into one quest, but the data model should support the chain.

## Narrative And Lore Bible

The world needs enough lore depth to steer art and systems:

- Starfall relics are not generic crystals. They are fragments of an ancient sky machine, half magical, half unknowable.
- Dawnwatch believes containment protects the valley.
- The Scholarium believes study can unlock travel, healing, and power.
- The Ashen Covenant believes the relics are keys to breaking old divine prisons.
- Companions should disagree lightly: defender favors safety, mystic favors understanding.
- Enemy elites are not random monsters; they are people or constructs reshaped by bad relic rituals.

Lore should appear in environmental props, barks, quest copy, faction icons, dungeon names, and item names.

## Gameplay And Camera Direction

The camera should move away from the current toy overhead framing and toward premium third-person MMO readability:

- Third-person trailing camera with adjustable pitch and distance.
- Hero large enough on screen to read silhouette, weapon, shield/cape, and facing.
- Combat framing that keeps the player, target, telegraphs, and nearby enemies readable.
- Controller-friendly movement, target bias/cycling, dodge/block, and ability buttons.
- Optional zoomed-out "MMO exploration" distance, but not so far that the hero becomes a pawn token.
- Cinematic spawn framing: the initial camera angle should be authored enough to sell the vista before player movement.
- Collision-safe camera behavior: avoid clipping through cliffs, gates, and large props.
- Contextual combat distance: slightly closer for melee impact, slightly wider when multiple enemies are engaged.

Combat should emphasize feel over breadth:

- One polished hero kit is better than four shallow classes for this reset.
- Basic attack, block/dodge, two abilities, and one ultimate/surge are enough if the animations, VFX, hit feedback, cooldowns, and enemy reactions read well.
- Enemies need silhouettes and behaviors: small minions, caster or ranged unit, and a larger elite sentinel.
- Add readable target frame, nameplates, health bars, damage numbers or hit text, and telegraph color.

## Combat Depth Target

The first hero should support a compact but expressive MMO-action kit:

- **Basic chain:** three-hit melee rhythm with clear animation or VFX stages.
- **Block or guard:** reduces incoming damage, visually raises shield/weapon, can trigger a counter window if feasible.
- **Dodge:** short movement burst with cooldown or stamina/resource cost.
- **Ability 1: Rune Cleave.** Frontal cone or arc, strong VFX trail, good against minions.
- **Ability 2: Starfall Lunge.** Gap closer or targeted strike, useful for caster enemies.
- **Ultimate: Relic Surge.** Charged by combat, large readable effect, knocks back or heavily damages enemies.
- **Resource:** simple energy/rage/rune meter displayed in the HUD.

Enemy roles should create simple tactical pressure:

- Minion: approaches and attacks in groups.
- Caster/ranged: stays back, uses telegraphed projectile or ground hazard.
- Elite: larger health pool, red nameplate, wind-up slam, add-call or shield phase.

Combat acceptance is not raw complexity. It is readable input-response: press button, see hero action, see VFX, see enemy reaction, see UI feedback.

## Class Roadmap

The first rebuild should polish one hero, but the systems should anticipate four original classes:

### Runeblade

Role: durable melee striker. Resource: rune charge built by attacking/blocking. Signature: sword arcs, guard counters, relic surge.

### Starshaper

Role: ranged caster/support. Resource: astral focus. Signature: star bolts, shields, area fields, celestial burst.

### Pathwarden

Role: mobile skirmisher/controller. Resource: momentum or marks. Signature: bow/spear hybrid, traps, dodge shots, beast/nature motifs.

### Lightbound Vanguard

Role: tank/support. Resource: conviction. Signature: shield, taunt/challenge, radiant heal, banners, protective aura.

The immediate slice may only implement Runeblade visuals and gameplay, but the UI, asset roles, input contract, and progression data should not prevent later class selection.

## Talent And Ability Growth

Do not implement a full talent UI immediately, but design for it:

- Each class eventually has three 3-5 node talent paths.
- Talents modify existing abilities before adding many new abilities.
- Early examples: Rune Cleave wider arc, block counter window, dodge cooldown reduction, ultimate leaves stabilizing field.
- Talent data should be separable from ability execution.
- The HUD should reserve room for ability ranks or passive icons later.

## Itemization Model

Loot should feel MMO-like even when shallow:

- Slots: weapon, offhand/shield, armor, trinket, relic.
- Rarities: common, uncommon, rare, epic/relic.
- Stats: attack, spell, guard, vitality, haste/cooldown, relic power.
- Affixes: short readable text such as "+Rune Cleave damage" or "+Dodge resource recovery."
- Drops: minions drop currency/shards rarely, caster drops focus item, elite drops a named weapon/trinket.
- Reward presentation: loot toast, rarity color, comparison stub, and inventory data entry even if full inventory UI waits.

The item system should remain compact, but data should be shaped so gear can become meaningful later.

## Encounter Design

The relic surge event should behave like a miniature public event:

1. Warning state: quest tracker and map marker identify the surge site.
2. First wave: minions already occupying the camp.
3. Escalation: defeating enough enemies spawns caster/ranged support or activates crystals.
4. Elite phase: larger sentinel enters or awakens from the event center.
5. Stabilization: player destroys/activates relic focus, VFX color shifts from hostile magenta to Dawnwatch blue-gold.
6. Reward: XP, loot toast, shard progress, future dungeon hook.

The event should be replayable in code, even if the first version triggers once.

## Dungeon And Boss North Star

The Sky-Watch dungeon is a future milestone but should influence the first slice:

- Three-room dungeon: seal chamber, traversal/puzzle chamber, boss arena.
- Boss: Starbound Warden, a guardian construct or corrupted champion tied to the relic.
- Boss phases:
  1. Melee telegraphs and shield testing.
  2. Relic crystals empower the boss until destroyed.
  3. Add wave pressures companions/party frames.
  4. Final surge with arena hazards and ultimate opportunity.
- Dungeon rewards: named relic item, faction reputation, unlock of future zone clue.

The first rebuild should not fully build the dungeon unless the asset and time budget supports it. It should visually promise it and architect event/boss systems in that direction.

## Encounter AI Director

The event should eventually use a small AI director rather than static spawn-only logic:

- Tracks player health, enemy count, event phase, and time in combat.
- Spawns reinforcements when phase thresholds are met.
- Avoids overwhelming the player if health is low.
- Announces elite phase through UI/VFX/audio.
- Can reset or calm the event if the player leaves.
- Emits telemetry-style debug logs for tuning.

The first implementation can be deterministic, but its structure should make the director easy to add.

## Progression And RPG Depth

The reset should add enough RPG structure to feel MMO-shaped:

- Player level and XP bar.
- Gear slots represented in data: weapon, shield/offhand, chest, trinket.
- Loot rarity colors: common, uncommon, rare, relic.
- Item drops from the elite or event completion.
- Quest reward panel or toast.
- Faction reputation stub: Dawnwatch standing increases after event completion.
- Ability cooldowns and resource costs surfaced in the action bar.

Do not build a full inventory UI before the first visual reset succeeds. The goal is enough progression feedback to make combat and event completion feel like part of a larger RPG.

## Economy, Crafting, And Reputation Horizon

The slice should include hooks that imply deeper MMO systems:

- Currency: Dawnwatch marks or star shards.
- Gathering nodes: relic shard, herb, ore, or salvage.
- Crafting station prop in hub, even if not functional yet.
- Vendor UI stub or interaction marker.
- Reputation: Dawnwatch standing and Scholarium interest values.
- Rewards can reference future vendors, recipes, or class trainers.

These should be present as data and world dressing first. Full economy UI is a later milestone.

## Factions And World Identity

The first slice should introduce three original identity layers:

- **Dawnwatch Compact:** keep guards, blue/gold banners, responsible for defending the road and sealing the relic.
- **Starfall Scholarium:** scholars, cyan/violet sigils, fascinated by the relic and willing to take risks.
- **Ashen Covenant:** hostile scavenger/cult force, red/black corruption palette, attempting to weaponize the surge.

These factions should appear in UI, props, banners, barks, quest text, and encounter dressing. Even if their gameplay is shallow at first, their visual language should be strong.

## Companion And Party Sophistication

Companions should sell the "single-player MMO party" fantasy:

- Defender companion: shield/taunt visual role, stands near enemies, can draw attention or block a lane.
- Mystic companion: support/heal/shield visual role, stays at range, comments on relic state.
- Party frames show health, role icon, and low-health state.
- Companions use simple cooldowns and readable VFX.
- Companion barks should trigger at hub departure, road danger, event start, elite spawn, low health, victory, and Sky-Watch reveal.
- Companions should not need deep commands in the first pass, but their presentation must be more than floating colored shapes.

Long-term, companions can become party members with role builds, gear, trust, and quest arcs.

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

## UI Sophistication Target

The UI should be treated as a first-class part of the fantasy, not debug overlay text:

- **Player frame:** portrait/silhouette, health, resource, level, class name.
- **Party frames:** two companion frames with health and role icons.
- **Target frame:** name, role, health, elite marker, cast bar for ranged/elite attacks.
- **Action bar:** 5-8 slots with cooldown radial/fill, keybinds, controller binds, disabled states, and resource cost hints.
- **Quest tracker:** current objective, progress counters, next location, event state.
- **Minimap:** stylized zone map with player arrow, hub, road, event, dungeon, and future-road markers.
- **Nameplates:** enemy health, elite/ranged tags, threat color, interactable markers.
- **Feedback feed:** XP, loot, reputation, world event updates, companion barks.

Implementation may use Canvas HUD first if needed, but the design should aim toward UMG/CommonUI widgets or a structured HUD presentation layer so it can grow without turning into a single draw function.

## UX Modes And Accessibility

The UI should support both keyboard/mouse and controller without feeling like an afterthought:

- Keyboard/mouse action bar labels and controller glyph labels.
- Gamepad navigation for menus, quest panel, and pause/settings.
- Optional target lock or target cycle for controller combat.
- Scalable HUD size for 1280x720 readability.
- Color choices should not rely only on red/green distinction; use icons, shapes, and labels for danger/ally/quest.
- Combat text and cooldown indicators should be readable without covering the hero.
- Pause/settings should expose camera distance, sensitivity, audio volume, and control reference.

These features can arrive in stages, but the HUD architecture should not block them.

## Cinematic And Audio Direction

The slice should have a simple cinematic language:

- Opening camera gives a heroic view of the champion and the relic beam beyond the road.
- Event escalation uses camera shake, sound cue, VFX pulse, or UI warning.
- Elite phase has a distinct visual/audio cue.
- Victory shifts lighting/VFX color and plays a clear completion toast.

Audio should be opportunistic but intentional:

- Ambient hub bed: wind, torches, distant work/market sound if assets exist.
- Road bed: wind, birds/insects if available, subtle relic hum growing near event.
- Combat: weapon swing, impact, block, dodge, spell cast, enemy hit/death.
- UI: click, quest accepted, loot, level/XP, event warning.

If audio assets are unavailable, document the gap and keep hooks ready for later.

## Animation And VFX Pipeline

The game will not feel premium without animation and feedback:

- Use a humanoid animation blueprint or simple state machine for idle, locomotion, attack, block, dodge, hit, death.
- Add montage or timed windows for attacks where feasible.
- Use sockets for weapon/shield placement and VFX origins.
- VFX roles: weapon trail, hit impact, block spark, dodge streak, ability burst, enemy telegraph, elite slam, relic pulse, portal beam, completion cleanse.
- VFX should have color language: Dawnwatch blue/gold, relic cyan/violet, hostile crimson/magenta.
- Camera shake and hit stop should be subtle but present for heavy attacks.

If proper animation assets are unavailable, prioritize importing/installing them before building more static environment content.

## Music And Soundscape Horizon

The first pass can be light on audio, but the design target is:

- Hub theme: warm heroic ambient loop.
- Road exploration: lighter travel ambience with relic hum.
- Event combat: percussion or stinger layer.
- Elite phase: sharper warning cue.
- Victory: short completion motif.

Audio hooks should be implemented as semantic events even if placeholder sounds are used first.

## Technical Design

Introduce a clear separation between game systems and presentation assets:

- **Zone Presentation Layer:** owns placed/imported environment pieces, landmark transforms, lighting, fog, post-process, and vista staging.
- **Asset Resolver:** maps semantic asset roles such as `HeroBody`, `DawnwatchWall`, `RoadLamp`, `RelicCrystal`, `EnemyElite`, and `ActionIconBasic` to available assets or proxies.
- **Encounter Stager:** spawns enemies and event props from authored encounter points rather than arbitrary hard-coded positions.
- **HUD Presentation Model:** gathers player, party, target, quest, event, loot, and control state into stable draw/widget data.
- **Input Contract:** preserves keyboard/mouse and controller mappings and makes them visible in the action bar.
- **World State Model:** stores zone phase, event phase, faction/reputation stubs, discovered landmarks, and future hook state.
- **Presentation Diagnostics:** reports proxy asset counts, missing critical roles, visible landmark counts, and first-frame acceptance flags.

Implementation can remain C++ first, but visual assembly may use Blueprint-friendly data assets or config structs if they speed iteration. The project should not keep growing one monolithic scaffold file for all world content.

## Recommended Unreal Architecture

The implementation plan should evaluate these Unreal systems:

- **Enhanced Input:** preserve and deepen keyboard/controller mappings.
- **CommonUI or UMG:** move toward structured widgets for MMO HUD and controller navigation.
- **Gameplay Tags:** tag factions, enemy roles, item rarity, quest states, event phases, and asset roles.
- **Data Assets or data structs:** zone, asset roles, encounters, quests, loot tables, abilities, UI theme.
- **Niagara:** VFX for attacks, relic events, portal beams, telegraphs, and ambient particles.
- **Level/World Partition or staged sublevels:** future option for larger zones; first slice may remain one map/profile.
- **Gameplay Ability System:** evaluate later for multi-class/talent depth, but do not block the immediate hero-read milestone on adopting it.
- **Mass/Smart Objects:** future option for ambient NPC density, patrols, and interaction points.

The first implementation should not adopt every system at once. The plan should choose the smallest set that unlocks asset-led presentation and future growth.

## Module Boundaries

The codebase should evolve away from one gameplay folder with large mixed-responsibility classes:

- `Core`: data types, tags, class/item/quest/event definitions.
- `Presentation`: asset resolver, zone profile, visual diagnostics, materials/style helpers.
- `World`: zone actor, landmarks, ambient actors, interactions, world state.
- `Combat`: hero abilities, enemy roles, damage, target selection, combat feedback.
- `Encounters`: event director, wave definitions, encounter rewards, boss phase data.
- `UI`: HUD presentation model, widgets/draw surfaces, minimap, nameplates, action bar.
- `Input`: input mapping and controller affordances.
- `Tests`: automation coverage for the above.

This structure can be introduced gradually, but every new system should have a clear home.

## Content Pipeline

The implementation plan should introduce small content authoring surfaces instead of hard-coding the world:

- Zone profile data: zone name, theme colors, landmarks, spawn points, minimap bounds, event anchors, and future-road markers.
- Asset role table: semantic role to preferred asset path, fallback role, scale normalization, tint/material overrides, collision profile.
- Encounter profile: waves, enemy roles, spawn anchors, event props, rewards, and completion triggers.
- HUD theme profile: colors, panel style, icon roles, action labels, keybind display.
- Visual verification checklist: generated from data so the build can report "asset-led" or "proxy-heavy."

Data can start as C++ structs and arrays, but should be shaped so it can later become DataAssets, JSON, CSV, or editor-authored assets.

## Save And World-State Horizon

The slice should design for saveable local state even if the first pass uses in-memory state:

- Quest phase.
- Event phase or completion.
- Player XP/level.
- Loot acquired.
- Reputation values.
- Discovered landmarks.
- Gate/dungeon unlock state.
- Settings/input preferences.

Persistent save is not required for the first visual reset, but the data model should not make it hard.

## Diagnostics And Telemetry For Development

A sophisticated private project still needs feedback loops:

- On launch, log project version, zone profile, asset role resolution summary, proxy count, missing required assets, input mode, and enabled plugins.
- In-game developer overlay or console command can show event phase, enemy count, target, player stats, and asset-proxy status.
- Screenshot verification should be repeatable at a known resolution.
- Automation tests should assert not only "counts exist" but "critical systems have meaningful data."
- The final report after each milestone should include build/test result, screenshot status, known proxy gaps, and next risk.

## Tooling And Editor Workflow

The rebuild should create tools/scripts that make iteration less painful:

- A launch script for standalone at known resolution.
- A test command for project automation tests.
- A visual verification checklist in docs.
- Asset dependency notes listing which local/downloaded packs are expected.
- Optional screenshot capture path for before/after comparisons.
- A lightweight asset audit command or log that lists missing/proxy asset roles.

This matters because the project is now asset-heavy. Without an audit trail, it will be too easy to accidentally regress into primitives or local-only broken references.

## Build Modes

Define build/run modes for development:

- **Prototype Mode:** proxies allowed, fast iteration, debug overlays visible.
- **Showcase Mode:** proxies for required visual roles fail acceptance, debug overlays hidden, first-frame composition verified.
- **Combat Tuning Mode:** event and combat diagnostics visible, enemy roles/waves tunable.
- **Controller Check Mode:** displays live input glyph/state and verifies bindings.

These modes can start as console flags, config values, or docs conventions, but the team should use them deliberately.

## Data Flow

1. Game mode loads the selected zone profile.
2. Zone profile provides asset-role names, landmark anchors, encounter anchors, minimap markers, faction colors, and HUD theme settings.
3. Asset resolver attempts to load preferred assets, applies scale/material normalization, and records any proxy fallbacks.
4. Zone presentation layer places hub, road, vista, dungeon entrance, event dressing, lights, fog, post-process, and ambient actors.
5. Encounter stager spawns enemies and event props at named anchors, then advances the event phase as combat progresses.
6. Player, companion, enemy, quest, loot, and world-event systems emit gameplay state.
7. HUD presentation model formats the state into player frames, party frames, target frame, action bar, quest tracker, minimap, nameplates, loot feed, and event toasts.
8. Presentation diagnostics records whether critical asset roles are real, proxy, or missing.

## Error Handling And Fallbacks

- Missing optional assets should log a readable warning and fall back to an obvious proxy with debug naming.
- Missing required hero, ground, enemy, or HUD assets should fail the visual acceptance checklist even if the build compiles.
- Asset resolver should expose a count/list of proxy fallbacks so tests and launch logs can identify when the game is still under-arted.
- If downloaded/local assets cannot be committed, setup docs must explain where they come from and how to restore them.
- If visual style clashes across asset packs, prefer a smaller cohesive set over a larger inconsistent set.
- If no suitable character assets are available, prioritize installing/importing character assets before expanding the environment.
- If controller cannot be physically verified, the final report must distinguish "mapped" from "physically tested."

## Performance And Scope Constraints

Going bigger should not mean making the project unusable:

- Target smooth standalone play at 1280x720 on the current Mac.
- Keep the first micro-zone compact and dense rather than physically enormous.
- Prefer a controlled number of high-impact assets over hundreds of uncurated props.
- Avoid expensive dynamic systems until the visual baseline is established.
- Use LODs, culling, instancing, or PCG/foliage only where they help real visual density.
- Keep code modules small enough to test and reason about; do not put all rebuild logic into `EOAlphaWorldScaffold`.

## Milestone Strategy

The implementation plan should break the rebuild into visible milestones:

1. **Asset Audit Milestone:** identify usable local assets and required downloads; produce asset role table and proxy report.
2. **Hero Read Milestone:** replace toy hero with a champion silhouette, third-person camera, movement, and action bar.
3. **Zone Vista Milestone:** build Dawnwatch Gate, road, relic beam, dungeon entrance, lighting, fog, and first-frame composition.
4. **Event Combat Milestone:** stage relic surge event with enemy roles, nameplates, target frame, VFX, and reward feedback.
5. **MMO HUD Milestone:** replace debug-like HUD with structured player/party/target/action/quest/minimap/feed presentation.
6. **Polish And Verification Milestone:** build, tests, standalone launch, screenshot review, controller check if available, git push.

Each milestone should be playable, inspectable, and visually better than the previous one.

## Expanded Roadmap

After the first six milestones, the next horizon should be:

7. **Dungeon Promise Milestone:** playable Sky-Watch entrance room or locked cinematic reveal with one puzzle/combat gate.
8. **Class Preview Milestone:** non-playable class representatives or simple class select preview for Starshaper, Pathwarden, and Lightbound Vanguard.
9. **Loot/Inventory Milestone:** lightweight inventory and gear comparison UI.
10. **Quest Chain Milestone:** break the single objective into the five-step Starfall chain.
11. **Companion Depth Milestone:** companion ability cooldowns, barks, and low-health/downed states.
12. **Style Unification Milestone:** material/theme pass, post-process, UI art pass, audio pass, and screenshot review.

The implementation plan should start with milestones 1-6 but avoid choices that make 7-12 painful.

## Risk Register

Major risks and mitigation:

- **No suitable assets available:** request network/install permission early; prioritize hero/enemy/environment in that order.
- **Asset style mismatch:** reduce asset set, normalize materials/colors, prefer cohesion over quantity.
- **Unreal import friction:** start with one small asset pack and prove build/launch before importing more.
- **HUD complexity explosion:** use a presentation model and implement surfaces incrementally.
- **Combat feels bad despite good visuals:** prioritize animation/VFX/input response before adding more systems.
- **Scope keeps expanding:** use tier labels and milestone acceptance gates.
- **Public repo asset licensing:** document dependencies and avoid committing questionable or oversized assets.
- **Controller verification unavailable:** keep mappings visible and report physical test status honestly.

## Definition Of "Bigger"

"Bigger" means:

- More authored visual density in a smaller space.
- More layers of readable systems behind the first loop.
- More production discipline in assets, data, diagnostics, and verification.
- More future-world implication through locked paths, factions, reputation, dungeon hooks, and class previews.

"Bigger" does not mean:

- A giant empty map.
- Copying protected IP.
- Implementing multiplayer before the single-player slice feels good.
- Adding 20 shallow features before one hero, one zone, and one event feel impressive.

## Testing And Verification

Automated tests should cover:

- Asset roles resolve to either real assets or explicit proxy fallbacks.
- Zone profile exposes required landmarks, encounter anchors, and minimap markers.
- Opening encounter has minion, caster/ranged, and elite roles.
- HUD presentation model includes party frames, target frame, action bar, quest tracker, minimap, and event feed data.
- Controller and keyboard mappings remain present.
- Event state advances through warning, active, elite, stabilized, and reward states.
- Loot/reputation/XP feedback can be emitted after event completion.
- Visual diagnostics can distinguish required real assets from temporary proxies.

Manual verification must include:

- Fresh Unreal build succeeds.
- Project automation tests pass.
- Standalone game launches.
- First frame screenshot shows an asset-led heroic fantasy zone, not primitives.
- Hero silhouette, HUD, event site, road, and dungeon/vista landmark are readable at 1280x720.
- Keyboard/mouse movement and combat work.
- Controller input is tested physically when a controller is available.
- The first frame has foreground, midground, and background read.
- At least one screenshot before combat and one during combat are reviewed.
- HUD readability is checked on a 1280x720 window, not only at large desktop size.

## Acceptance Criteria

This rebuild is accepted only when:

- The first screen no longer reads as primitive blockout.
- The hero is visibly a fantasy champion, not a basic capsule/shape stack.
- The zone has an authored MMO-adventure composition: hub, road, hostile event, landmark, vista.
- The UI reads like an MMO action RPG interface while remaining original.
- The opening loop is playable and understandable without debug text.
- The zone includes at least one side pocket, one future-zone boundary, and one dungeon/vista hook.
- The event has at least three enemy roles or phases: minion pressure, ranged/caster pressure, and elite pressure.
- The HUD includes player, party, target, action bar, quest tracker, minimap, nameplate, and reward feedback surfaces.
- The asset resolver/diagnostics can report whether the build is still proxy-heavy.
- The branch builds, tests pass, launches standalone, and is pushed to GitHub.

## Explicit Non-Goals

- Do not copy World of Warcraft assets, names, maps, UI art, exact iconography, music, characters, factions, or protected story elements.
- Do not chase full MMO networking in this visual reset.
- Do not build all four classes before one hero presentation feels good.
- Do not continue expanding runtime primitive scenery as the main production path.
- Do not claim AAA quality; aim for the strongest private-learning vertical slice possible with available assets and time.
