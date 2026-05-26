# Producer Dashboard

## Current Phase

Phase 2 - Shared targeting v0.

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

## Current Blockers

| Blocker | Owner | Unblock Step |
| --- | --- | --- |
| Manual editor smoke test pending | D RoC or Codex with editor session | Open `M_Test_Targeting`, press Play, face target, press `E` |

## Current Agent Lane

Codex can continue preparing:

- focused Unreal automation scripts
- Blueprint/C++ implementation passes after repo inspection
- docs, backlog, prompts, asset registers, and handoff briefs
- code review criteria and validation commands

Codex should only create or modify Unreal-generated files through Unreal Editor, Unreal commandlets, or reviewed automation scripts.

## Next Human Actions

1. Open `DockShield/DockShield.uproject` in Unreal.
2. Confirm `M_Test_Targeting` opens by default.
3. Press Play, face the grapple/civilian placeholders, and press `E`.

## Next Agent Actions

1. Keep scope locked to the first vertical slice.
2. Keep The Reel Pull scope to one grapple target and one civilian target.
3. Close the manual smoke-test gap before The Fly, Lilly, weather, or command mode.

## Current Implementation Plan

The first Unreal pass sequence is defined in `docs/unreal-first-implementation-plan.md`.
