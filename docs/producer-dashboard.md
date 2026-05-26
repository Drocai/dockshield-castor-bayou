# Producer Dashboard

## Current Phase

Phase 2 - Shared targeting v0, Reel feedback, and water/boat prototype foundations.

## Current Repo State

- Git repo exists.
- Git LFS is configured.
- Planning docs exist.
- Private GitHub repo exists: https://github.com/Drocai/dockshield-castor-bayou
- Unreal Engine 5.7 is installed.
- Production Unreal project exists at `DockShield/DockShield.uproject`.
- First targeting test map exists at `/Game/DockShield/Maps/M_Test_Targeting`.
- Shared targeting v0 C++ module builds.
- The Reel prototype pawn, aim mode, Pull input, target lock HUD, line tension HUD, and action counters are implemented.
- The Reel prototype has temporary debug-rendered feedback for valid pulls, rescues, and invalid fire attempts.
- The Reel core action now has a headless commandlet smoke test for invalid target, grapple, and civilian rescue behavior.
- Prototype water-depth zones and prototype boat towing/boarding/piloting actors are implemented in C++ and covered by a headless smoke test.
- `M_Test_Targeting` now contains `DS_Water_Depth_Test` and `DS_Boat_Prototype_Tow` for the first boat tow pass.
- Private canon visual target pack is stored in `docs/reference/gameplay-ui/`.
- Current playable graphics are graybox prototype quality with a first lighting/fog pass, not the canon Castor Bayou look yet.

## Current Blockers

No repo or toolchain blocker is currently stopping the next implementation pass. The Visual Studio compiler/SDK warnings are tracked as cleanup work, not build blockers.

## Current Playtest Debt

| Debt | Owner | Check |
| --- | --- | --- |
| Manual editor feel test for The Reel Pull v0 | D RoC or Codex with editor session | Open `M_Test_Targeting`, press Play, confirm target lock/tension HUD, hold RMB to aim, press LMB or `E` |
| Manual editor feel test for water/boat v0 | D RoC or Codex with editor session | Face `DS_Boat_Prototype_Tow`, hold RMB, press LMB or `E`, confirm boat tow feedback and HUD boat count, then press `F` or `B` to board/exit and use `WASD` to pilot |
| Visual expectation gap | Codex / art pipeline | Current build uses placeholder geometry, debug feedback, and mannequin; do not judge final graphics from this test map |

## Current Agent Lane

Codex can continue preparing:

- focused Unreal automation scripts
- Blueprint/C++ implementation passes after repo inspection
- docs, backlog, prompts, asset registers, and handoff briefs
- code review criteria and validation commands

Codex should only create or modify Unreal-generated files through Unreal Editor, Unreal commandlets, or reviewed automation scripts.

## Next Human Actions

1. Optional: open `DockShield/DockShield.uproject` in Unreal.
2. Optional: press Play in `M_Test_Targeting`, face the grapple/civilian placeholders, hold right mouse button to aim, then press left mouse button or `E`.
3. Optional: judge only controls, targeting clarity, and feedback timing. The cinematic Castor Bayou graphics are not in this graybox map yet.

## Next Agent Actions

1. Keep scope locked to the first vertical slice.
2. Keep The Reel Pull scope to one grapple target, one civilian target, one invalid hazard target, and one prototype boat target.
3. Validate prototype runtime wiring and Reel action behavior with `scripts\unreal\validate-prototype.ps1`.
4. Next pass should improve visual readability of rescue raft/boat feedback, then move toward basic water current/boating hazard behavior.

## Current Implementation Plan

The first Unreal pass sequence is defined in `docs/unreal-first-implementation-plan.md`.
