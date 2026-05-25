# Producer Dashboard

## Current Phase

Phase 0 - setup and design control.

## Current Repo State

- Git repo exists.
- Git LFS is configured.
- Planning docs exist.
- No Unreal `.uproject` exists yet.
- No Unreal assets exist yet.
- No generated project files exist yet.

## Current Blockers

| Blocker | Owner | Unblock Step |
| --- | --- | --- |
| Unreal Engine not installed | D RoC | Install Unreal Engine 5.7.x from Epic Games Launcher |
| Blender not installed | D RoC or agent with install permission | Install Blender |
| OBS not installed | D RoC or agent with install permission | Install OBS Studio |
| No remote repo | D RoC | Create private GitHub repo and push local commits |

## Current Agent Lane

Codex can continue preparing:

- docs
- backlog
- prompts
- asset registers
- issue templates
- implementation plans
- handoff briefs
- code review criteria

Codex should not create Unreal-generated files until the Unreal project exists.

## Next Human Actions

1. Install Unreal Engine 5.7.x.
2. Install Blender.
3. Install OBS Studio.
4. Create a private GitHub repo.
5. Push this repo.

## Next Agent Actions

1. Keep scope locked to the first vertical slice.
2. Prepare first Unreal implementation plan.
3. Prepare asset sourcing checklist.
4. Prepare developer/art contractor briefs.
5. Inspect `.uproject` after Unreal project creation.

## Current Implementation Plan

The first Unreal pass sequence is defined in `docs/unreal-first-implementation-plan.md`.
