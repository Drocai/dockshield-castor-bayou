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
| Windows Smart App Control blocks Unreal C++ build-rules DLLs | D RoC | Decide whether to turn Smart App Control off, use Blueprint-only prototyping for now, or build C++ on another machine |

## Current Agent Lane

Codex can continue preparing:

- focused Unreal automation scripts
- Blueprint/C++ implementation passes after repo inspection
- docs, backlog, prompts, asset registers, and handoff briefs
- code review criteria and validation commands

Codex should only create or modify Unreal-generated files through Unreal Editor, Unreal commandlets, or reviewed automation scripts.

## Next Human Actions

1. Decide how to handle Smart App Control for Unreal C++ builds.
2. Open `DockShield/DockShield.uproject` in Unreal and confirm `M_Test_Targeting` opens.
3. If Smart App Control stays on, keep the next implementation pass Blueprint-only or use another dev machine for C++.

## Next Agent Actions

1. Keep scope locked to the first vertical slice.
2. Keep the C++ shared targeting/Reel Pull WIP in `stash@{0}` until the build policy is resolved.
3. Prepare a Blueprint-only fallback plan if Smart App Control stays on.
4. Stop before The Fly, Lilly, weather, or command mode.

## Current Implementation Plan

The first Unreal pass sequence is defined in `docs/unreal-first-implementation-plan.md`.
