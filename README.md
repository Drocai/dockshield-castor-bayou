# DockShield: Castor Bayou Incident

This is the planning/control repo for the first playable DockShield prototype.

DockShield is a tactical supernatural rescue-action game platform where mutated heroes defend contaminated waterways, storm zones, marinas, lakes, docks, bayous, and hidden spill sites.

## Current Status

This repo is in **Phase 2: shared targeting v0, continuous Reel feedback, and water/boat prototype foundations**.

The planning/control layer exists, the production Unreal project is at `DockShield/DockShield.uproject`, and the first targeting test map exists at `/Game/DockShield/Maps/M_Test_Targeting`. The Reel placeholder character, target lock HUD, aim mode, cast/hold/release Reel line loop, headless Reel interaction smoke test, prototype water-depth zone, boat towing, boat boarding, and prototype boat piloting foundations are implemented.

## Working Title Structure

```text
DockShield
  Campaign 1: The Depth
    Vertical Slice: Castor Bayou Incident
    Mission Arc: Hook, Line & Sinker
```

## First Playable Target

Build an 8-12 minute vertical slice that proves:

- shared target interaction
- three hero verbs: Pull, Mark, Bind
- weather-driven visibility or hazard pressure
- civilian rescue
- one mutation enemy
- one scripted mini boss
- slow-time command mode v0

## Start Here

Read these in order:

1. [docs/00-start-here.md](docs/00-start-here.md)
2. [docs/toolchain-setup.md](docs/toolchain-setup.md)
3. [docs/core-gameplay-bible.md](docs/core-gameplay-bible.md)
4. [docs/castor-bayou-vertical-slice.md](docs/castor-bayou-vertical-slice.md)
5. [docs/prototype-backlog.md](docs/prototype-backlog.md)
6. [docs/milestone-roadmap.md](docs/milestone-roadmap.md)
7. [docs/unreal-architecture-plan.md](docs/unreal-architecture-plan.md)
8. [docs/handoff-package.md](docs/handoff-package.md)

## Repo Rules

- Do not commit secrets.
- Track large binary assets through Git LFS.
- Do not create fake Unreal files by hand.
- Do not add paid dependencies unless the budget doc is updated.
- Keep the first slice small enough to finish.

## Toolchain Target

- Unreal Engine 5.7
- Blender
- Git + Git LFS
- Visual Studio with Game development with C++ workload
- Optional later: Perforce P4 if binary assets outgrow Git/LFS

## Prototype Validation

Run the current one-command validation pass from the repo root:

```powershell
powershell -ExecutionPolicy Bypass -File scripts\unreal\validate-prototype.ps1
```

This runs the toolchain check, C++ editor build, Unreal commandlet smoke tests, DataValidation, Git status, and Git LFS status.

## Remote

Private GitHub repo:

https://github.com/Drocai/dockshield-castor-bayou
