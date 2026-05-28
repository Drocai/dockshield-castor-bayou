# Producer Dashboard

## Current Phase

Phase 2 - Shared targeting v0, continuous Reel feedback, water pressure, water/boat prototype foundations, first Fly recon foundations, first Lilly bind foundations, three-hero switching v0, and visual style pass v1.

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
| Candidate 3D asset production gap | Codex / art pipeline | Copilot3D GLB/STL files are useful as static visual proxies or sculpt references, but they have no armatures/actions and need license confirmation, cleanup, retopology, and rigging before playable use |

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
3. Optional: judge controls, targeting clarity, tension readability, flood/current readability, feedback timing, and whether the new storm/fog/toxic/wet-dock pass is moving toward the references. Final cinematic Castor Bayou assets are not in this test map yet.

## Next Agent Actions

1. Keep scope locked to the first vertical slice.
2. Keep The Reel continuous line scope to one grapple target, one civilian target, one invalid hazard target, and one prototype boat target.
3. Validate prototype runtime wiring and Reel action behavior with `scripts\unreal\validate-prototype.ps1`.
4. Next visual pass should either generate a riggable Reel A-pose source from the prompt pack or import the best current Reel GLB as a static scale/lighting proxy only after license confirmation.
5. Do not replace the playable mannequin until the Reel source has a rigging route and animation-compatible topology.

## Current Implementation Plan

The first Unreal pass sequence is defined in `docs/unreal-first-implementation-plan.md`.
