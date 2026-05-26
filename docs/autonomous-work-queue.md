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
- run the manual editor smoke test for The Reel Pull v0
- approve paid asset purchases or contractors

## Needs Unreal Installed

- create Content Browser folders
- create test maps
- implement Blueprints
- validate in editor
- package Windows build

## Suggested Parallel Flow

1. Codex commits the shared targetable interface and The Reel Pull v0 implementation after validation.
2. D RoC opens the map in Unreal for a manual editor smoke test.
3. Codex fixes any smoke-test issues before adding The Fly, Lilly, weather, or command mode.
