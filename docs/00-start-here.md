# Start Here

## What This Repo Is

This repo is the command center for starting DockShield correctly.

It now contains both the planning/control layer and the production Unreal project at `DockShield/DockShield.uproject`. Keep concept, toolchain, budget, scope, agent prompts, handoff materials, and Unreal prototype work together in this repo.

## Immediate Goal

Create a playable vertical slice called **DockShield: Castor Bayou Incident**.

The vertical slice should prove the game's unique feel:

```text
Weather-driven tactical rescue combat
Fishing logic as superhero equipment
Three heroes sharing one target system
```

## Repo Truth

As of the initial GitHub/Unreal setup:

- `DockShield/DockShield.uproject` exists.
- `/Game/DockShield/Maps/M_Test_Targeting` is the first graybox targeting test map.
- No package scripts exist yet.
- No build/test scripts exist yet.
- Git LFS patterns are prepared for Unreal and art assets.
- Unreal Engine 5.7 is installed and detected.
- Blender, OBS Studio, Visual Studio C++ tools, and Windows SDK are installed and detected.
- Unreal C++ builds are blocked by Windows Smart App Control until the security policy is changed or the project is built on another machine.

## Operating Model

Use Codex as the repo operator.

Use other assistants only in controlled roles:

- critique a specific doc
- review a specific implementation branch
- generate alternate wording
- research Unreal implementation details

Do not let multiple agents edit the same files at the same time.

## First Milestone

Milestone 1 is not a full game.

Milestone 1 is:

- one graybox bayou arena
- three playable placeholder heroes
- one ability per hero
- one enemy
- one civilian rescue target
- one toxic/weather hazard
- one mini boss encounter
- packaged Windows build
- short capture video

## Definition of Done for Setup

Setup is done when:

- this repo is pushed to GitHub or another remote
- Unreal Engine 5.7 is installed
- Blender is installed
- Visual Studio C++ tools are installed
- a first Unreal project is created intentionally
- Git LFS is confirmed before committing `.uasset` or `.umap` files
