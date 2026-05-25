# Autonomous Work Queue

## Purpose

This file separates what Codex can do without waiting from what requires D RoC, Unreal Editor, accounts, or external credentials.

## Codex Can Do Now

- refine docs and scope
- break backlog into implementation passes
- write prompts for specific Codex/Claude/Gemini runs
- create GitHub issue templates
- maintain decision log and risk register
- prepare contractor briefs
- prepare asset intake templates
- research implementation options
- inspect generated Unreal files
- create focused Unreal automation scripts
- create or validate graybox test maps through Unreal commandlets
- review diffs and catch scope creep

## Codex Should Not Do Yet

- create fake `.uproject` files
- hand-create `.uasset` or `.umap` files
- claim Unreal build success without Unreal command output
- spend budget or purchase assets
- open external accounts
- publish public pages
- install large software silently without explicit approval

## Needs D RoC

- sign in to Epic Games Launcher
- confirm whether Blender/OBS can be installed by agent
- approve paid asset purchases or contractors

## Needs Unreal Installed

- create Content Browser folders
- create test maps
- implement Blueprints
- validate in editor
- package Windows build

## Suggested Parallel Flow

1. D RoC installs Blender and OBS.
2. Codex commits the verified targeting test map.
3. Codex implements shared targetable interface v0.
4. D RoC opens the map in Unreal for a manual editor smoke test.
5. Codex implements The Reel Pull v0 after the target interface is verified.
