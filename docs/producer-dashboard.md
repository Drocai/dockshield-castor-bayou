# Producer Dashboard

## Current Phase

Phase 1 - Unreal project foundation.

## Current Repo State

- Git repo exists.
- Git LFS is configured.
- Planning docs exist.
- Private GitHub repo exists: https://github.com/Drocai/dockshield-castor-bayou
- Unreal Engine 5.7 is installed.
- Production Unreal project exists at `DockShield/DockShield.uproject`.
- First targeting test map exists at `/Game/DockShield/Maps/M_Test_Targeting`.

## Current Blockers

| Blocker | Owner | Unblock Step |
| --- | --- | --- |
| Blender not installed | D RoC or agent with install permission | Install Blender |
| OBS not installed | D RoC or agent with install permission | Install OBS Studio |

## Current Agent Lane

Codex can continue preparing:

- focused Unreal automation scripts
- Blueprint/C++ implementation passes after repo inspection
- docs, backlog, prompts, asset registers, and handoff briefs
- code review criteria and validation commands

Codex should only create or modify Unreal-generated files through Unreal Editor, Unreal commandlets, or reviewed automation scripts.

## Next Human Actions

1. Install Blender.
2. Install OBS Studio.
3. Open `DockShield/DockShield.uproject` in Unreal and confirm `M_Test_Targeting` opens and the Third Person pawn can move.

## Next Agent Actions

1. Keep scope locked to the first vertical slice.
2. Commit verified targeting map scaffold.
3. Implement shared targetable interface v0.
4. Implement The Reel Pull v0.
5. Stop before The Fly, Lilly, weather, or command mode.

## Current Implementation Plan

The first Unreal pass sequence is defined in `docs/unreal-first-implementation-plan.md`.
