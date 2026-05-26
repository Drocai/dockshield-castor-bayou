# Producer Dashboard

## Current Phase

Phase 2 - Shared targeting v0 and prototype HUD/reticle pass.

## Current Repo State

- Git repo exists.
- Git LFS is configured.
- Planning docs exist.
- Private GitHub repo exists: https://github.com/Drocai/dockshield-castor-bayou
- Unreal Engine 5.7 is installed.
- Production Unreal project exists at `DockShield/DockShield.uproject`.
- First targeting test map exists at `/Game/DockShield/Maps/M_Test_Targeting`.
- Shared targeting v0 C++ module builds.
- The Reel prototype pawn and Pull input are implemented for manual smoke testing.
- Private gameplay UI reference set is stored in `docs/reference/gameplay-ui/`.

## Current Blockers

No repo or toolchain blocker is currently stopping the next implementation pass.

## Current Playtest Debt

| Debt | Owner | Check |
| --- | --- | --- |
| Manual editor smoke test for The Reel Pull v0 | D RoC or Codex with editor session | Open `M_Test_Targeting`, press Play, face target, press `E` |

## Current Agent Lane

Codex can continue preparing:

- focused Unreal automation scripts
- Blueprint/C++ implementation passes after repo inspection
- docs, backlog, prompts, asset registers, and handoff briefs
- code review criteria and validation commands

Codex should only create or modify Unreal-generated files through Unreal Editor, Unreal commandlets, or reviewed automation scripts.

## Next Human Actions

1. Optional: open `DockShield/DockShield.uproject` in Unreal.
2. Optional: press Play in `M_Test_Targeting`, face the grapple/civilian placeholders, and press `E`.

## Next Agent Actions

1. Keep scope locked to the first vertical slice.
2. Keep The Reel Pull scope to one grapple target and one civilian target.
3. Add a prototype Canvas HUD/reticle for target readability.
4. Record manual smoke-test status, but do not add The Fly, Lilly, weather, or command mode before The Reel loop is verified.

## Current Implementation Plan

The first Unreal pass sequence is defined in `docs/unreal-first-implementation-plan.md`.
