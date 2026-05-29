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

As of the current Unreal prototype setup:

- `DockShield/DockShield.uproject` exists.
- `/Game/DockShield/Maps/M_Test_Targeting` is the first graybox targeting test map.
- No package scripts exist yet.
- Unreal validation scripts exist under `scripts/unreal/`.
- The current one-command validation runner is `scripts/unreal/validate-prototype.ps1`.
- Git LFS patterns are prepared for Unreal and art assets.
- Unreal Engine 5.7 is installed and detected.
- Blender, OBS Studio, Visual Studio C++ tools, and Windows SDK are installed and detected.
- Unreal C++ builds are currently unblocked after D RoC turned off Smart App Control.
- The Reel placeholder character, target lock HUD, aim mode, and headless Reel interaction smoke test are implemented.
- Prototype water depth/current/flood pressure and boat towing/drift foundations are implemented in C++ and validated by commandlet smoke tests.
- The Fly sonar/mark foundation, Lilly swamp pulse/bind foundation, and three-hero switching are implemented in C++ and validated by commandlet smoke tests.
- A repeatable visual style pass now generates prototype bayou materials and applies wet dock, toxic water, reed, backdrop, storm lighting, fog, and post-process markers to `M_Test_Targeting`.
- A private static Reel proxy is imported and placed in `M_Test_Targeting` for scale, silhouette, and lighting review; it is not playable or public-ready.
- Prototype beta economy, achievements, settings HUD, and Deep Dock boss arena foundations are implemented in C++ and Unreal commandlet automation.

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
