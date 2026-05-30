# Producer Dashboard

## Current Phase

Phase 2 - Shared targeting v0, continuous Reel feedback, water pressure, water/boat prototype foundations, first Fly recon foundations, first Lilly bind foundations, three-hero switching v0, visual style pass v1, Reel static proxy review, beta economy/achievements/settings HUD v0, Bayou Mutation enemy combat v0, Deep Dock boss arena foundation, Hook, Line & Sinker weak-point combo damage v0, and Duct legendary encounter v0.

## Current Repo State

- Git repo exists.
- Git LFS is configured.
- Planning docs exist.
- Private GitHub repo exists: https://github.com/Drocai/dockshield-castor-bayou
- Unreal Engine 5.7 is installed.
- Production Unreal project exists at `DockShield/DockShield.uproject`.
- First targeting test map exists at `/Game/DockShield/Maps/M_Test_Targeting`.
- Shared targeting v0 C++ module builds.
- The Reel prototype pawn, aim mode, cast/hold/release Reel input, target lock HUD, line tension state HUD, and action counters are implemented.
- The Reel prototype has temporary debug-rendered feedback for valid casts, rescues, boat towing, and invalid cast attempts.
- The Reel core action now has a headless commandlet smoke test for invalid target, grapple, and civilian rescue behavior.
- Prototype water-depth/current/flood-pressure zones and prototype boat drifting/towing/boarding/piloting actors are implemented in C++ and covered by a headless smoke test.
- `M_Test_Targeting` now contains `DS_Water_Depth_Test` and `DS_Boat_Prototype_Tow` for the first boat tow pass.
- Private canon visual target pack is stored in `docs/reference/gameplay-ui/`.
- The Fly placeholder C++ pawn now supports sonar pulse, recon target selection, shared target mark state, and a Fly-specific prototype HUD path.
- Lilly Loch placeholder C++ pawn now supports swamp pulse, bind target selection, shared target bind state, and a Lilly-specific prototype HUD path.
- `DSPrototypePlayerController` supports `1` for The Reel, `2` for The Fly, `3` for Lilly Loch, and `Tab` to cycle through all three in normal play.
- The Reel, The Fly, and Lilly Loch have first-pass in-engine prototype visual kits so they no longer read as identical mannequins.
- `M_Test_Targeting` now has generated prototype bayou materials, wet dock planks, reed silhouettes, toxic water/glow planes, storm moon key light, amber rescue light, red neon sign light, fog, and a storm-grade post-process marker.
- User-provided Copilot3D character mesh candidates have been inspected in Blender and documented at `docs/reference/candidate-3d-assets/copilot3d-2026-05-27/`.
- Reel-first character production research and generation prompts are documented in `docs/research/character-asset-pipeline-research-2026-05-27.md` and `docs/prompts/the-reel-asset-generation-prompts.md`.
- The best current Copilot3D Reel GLB is copied to `SourceAssets/Private/Copilot3D/SM_Reel_Copilot3D_Proxy.glb`, imported as a private static proxy under `/Game/DockShield/Characters/Reel/Proxy/Copilot3D`, and placed in `M_Test_Targeting` for scale, silhouette, and lighting review.
- The Reel proxy source is now guarded by file-size and SHA256 validation in the apply/validate scripts.
- Prototype beta economy state is implemented in `DSPrototypePlayerController`: credits, XP, Depth samples, one-time achievements, and HUD display.
- Prototype settings state is implemented in `DSPrototypePlayerController`: `P`/`Esc` settings panel, HUD scale, gamma value, and visual-quality mode selection for readability review.
- `DSMutationEnemyActor` implements the first Bayou Mutation enemy: The Reel can stagger/pull it, The Fly can mark it, Lilly can bind it, and one shared Hook, Line & Sinker combo applies damage through the same target state used by boss weak points.
- `apply_mutation_enemy_pass.py` places `DS_MutationEnemy_BayouBruiser_Debug` plus toxic glow, warning light, core glow, and wake markers in `M_Test_Targeting`.
- `validate_mutation_enemy_smoke.py` verifies Reel impact, Fly pressure, Lilly pressure, combo damage, and defeat headlessly.
- Deep Dock boss arena state actor and commandlet map pass are implemented for `M_Test_Targeting`; the pass places a toxic core, weak-point targets, warning pylons, service decks, and boss lighting.
- Deep Dock weak points now track Reel exposure, Fly marks, and Lilly binds. When all three are present on one weak point, `DSDeepDockBossArenaActor` applies Hook, Line & Sinker combo damage and advances boss phase from dormant to exposed, enraged, then defeated.
- `validate_boss_combo_smoke.py` verifies all three weak-point combos and boss defeat headlessly.
- `DSDuctLegendaryEncounterActor` implements Duct as an uncatchable legendary sighting with sighted, latched, near-catch, and slipped-away states.
- The Reel can latch Duct and build near-catch progress with held `R`; Duct always escapes and leaves Trophy Case evidence instead of becoming inventory loot.
- `validate_duct_legendary_smoke.py` verifies the Duct sighting, bait, latch, near-catch, tape trace, and uncatchable outcome headlessly.
- Current playable graphics are styled prototype quality, not final canon Castor Bayou production art yet.

## Current Blockers

No repo or toolchain blocker is currently stopping the next implementation pass. The Visual Studio compiler/SDK warnings are tracked as cleanup work, not build blockers.

## Current Playtest Debt

| Debt | Owner | Check |
| --- | --- | --- |
| Manual editor feel test for The Reel continuous line v0 | D RoC or Codex with editor session | Open `M_Test_Targeting`, press Play, confirm target lock/tension HUD, hold RMB to aim, press LMB or `E` to cast, hold `R` to reel, release `R` to ease |
| Manual editor feel test for water/boat pressure v0 | D RoC or Codex with editor session | Face `DS_Boat_Prototype_Tow`, confirm HUD pressure/current readout, cast with LMB or `E`, hold `R` to tow, confirm boat feedback and HUD boat count, then press `F` or `B` to board/exit and use `WASD` to pilot |
| Manual editor feel test for The Fly sonar/mark v0 | D RoC or Codex with editor session | Press `2` or `Tab` to switch to The Fly, press `Q` for sonar, aim with RMB, press LMB or `E` to mark a target, confirm teal HUD and prompt state |
| Manual editor feel test for Lilly bind v0 | D RoC or Codex with editor session | Press `3` or `Tab` to switch to Lilly, press `Q` for swamp pulse, aim with RMB, press LMB or `E` to bind a target, confirm swamp HUD and prompt state |
| Manual editor feel test for switching v0 | D RoC or Codex with editor session | Press `1`, `2`, `3`, and `Tab`; confirm possession swaps, HUD changes, and each hero keeps their own ability controls |
| Visual expectation gap | Codex / art pipeline | Current build now has styled prototype materials/lighting, but still uses placeholder geometry, debug feedback, and mannequin bodies; do not judge final character fidelity from this test map |
| Candidate 3D asset production gap | Codex / art pipeline | One Copilot3D Reel GLB is now in-engine as a private static proxy, but it has no armature/actions and still needs license confirmation, cleanup, retopology, and rigging before playable use |
| Reel proxy visual review | D RoC or Codex with editor session | Open `M_Test_Targeting`, compare `DS_Reel_Proxy_Copilot3D_Static` against the playable pawn under storm fog, toxic glow, amber rescue light, and wet dock lighting |
| Riggable Reel source gap | Codex / art pipeline | Generate or source the next A-pose/T-pose Reel body and separate gear props; inspect in Blender before any playable replacement work |
| Deep Dock boss arena feel test | D RoC or Codex with editor session | Open `M_Test_Targeting`, approach the Deep Dock area, confirm boss core/weak-point targets read clearly under toxic green and warning red lighting |
| Deep Dock boss combo feel test | D RoC or Codex with editor session | Expose a Deep Dock weak point with The Reel, switch to The Fly and mark it, switch to Lilly and bind it, then confirm HUD boss status advances and economy rewards trigger |
| Bayou Mutation combat feel test | D RoC or Codex with editor session | Find `DS_MutationEnemy_BayouBruiser_Debug`, stagger it with The Reel, switch to The Fly and mark it, switch to Lilly and bind it, then confirm the mutation HUD/combo reward/defeat state advances |
| Beta economy/settings feel test | D RoC or Codex with editor session | Press `P` or `Esc`, adjust HUD scale with `[`/`]`, adjust gamma value with `-`/`=`, cycle quality with `O`, then complete one rescue/mark/bind to confirm HUD feedback |
| Duct legendary encounter feel test | D RoC or Codex with editor session | Find `DS_Duct_Legendary_Sighting_Debug`, latch with LMB/E, hold `R`, confirm near-catch pressure builds, Duct slips away, and Trophy Case evidence shows `UNCAUGHT` |

## Current Agent Lane

Codex can continue preparing:

- focused Unreal automation scripts
- Blueprint/C++ implementation passes after repo inspection
- docs, backlog, prompts, asset registers, and handoff briefs
- code review criteria and validation commands

Codex should only create or modify Unreal-generated files through Unreal Editor, Unreal commandlets, or reviewed automation scripts.

## Next Human Actions

1. Optional: open `DockShield/DockShield.uproject` in Unreal.
2. Optional: press Play in `M_Test_Targeting`, face the grapple/civilian placeholders, hold right mouse button to aim, press left mouse button or `E` to cast, then hold `R` to reel.
3. Optional: review the static Reel proxy beside the playable pawn for silhouette, size, material read, and camera-distance clarity. Final cinematic Castor Bayou assets are not in this test map yet.
4. Optional: review the Deep Dock prototype arena in `M_Test_Targeting` for boss-core readability, weak-point clarity, lighting contrast, and the Reel/Fly/Lilly combo loop.

## Next Agent Actions

1. Keep scope locked to the first vertical slice.
2. Keep The Reel continuous line scope to one grapple target, one civilian target, one invalid hazard target, and one prototype boat target.
3. Validate prototype runtime wiring and Reel action behavior with `scripts\unreal\validate-prototype.ps1`.
4. Add readable in-world VFX/SFX hooks for Reel exposure, Fly mark, Lilly bind, mutation stagger, and combo impact.
5. Keep beta economy/settings/achievement work prototype-only until the first encounter loop has real win/fail conditions.
6. Keep Duct uncatchable in this version; expand only sightings, bait, evidence, VFX/SFX, and rarity logic until lore changes.
7. Next visual pass should generate or source a riggable Reel A-pose source from the prompt pack; the current in-engine Reel GLB is static proxy evidence only.
8. Do not replace the playable mannequin until the Reel source has a rigging route and animation-compatible topology.
9. Use `scripts\unreal\validate-prototype.ps1 -RequireCleanGit` after committed checkpoints when a clean-release handoff is needed.

## Current Implementation Plan

The first Unreal pass sequence is defined in `docs/unreal-first-implementation-plan.md`.
