# Unreal First Implementation Plan

## Start Condition

Begin only after:

- Unreal Engine 5.7.x is installed
- Third Person project named `DockShield` exists
- project opens cleanly once
- Unreal Editor is closed before Codex inspects files
- `git status --short` is reviewed

## Pass 1 - Project Intake

Goal: understand generated project reality.

Steps:

1. Read `.uproject`.
2. Read `Config/`.
3. Inspect `Content/`.
4. Inspect `Source/` if present.
5. Confirm engine version.
6. Confirm enabled plugins.
7. Confirm generated caches are ignored.
8. Report safest next edit.

Do not edit files in this pass.

## Pass 2 - Content Organization

Goal: create the DockShield content structure in Unreal Editor.

Folders:

```text
Content/DockShield/
  Characters/
  Core/
  Data/
  Enemies/
  Environment/
  Maps/
  Missions/
  UI/
  VFX/
  Audio/
```

Verification:

- editor saves cleanly
- `git status --short` shows expected `.uasset` folder assets only
- LFS tracks `.uasset` and `.umap`

## Pass 3 - Targeting Test Map

Goal: create the smallest map where targeting can be tested.

Map:

```text
M_Test_Targeting
```

Objects:

- player start
- one target cube or marker
- one target debug label
- one grapple/civilian placeholder

Verification:

- map opens
- player can move
- target is visible

## Pass 4 - Shared Targetable Interface

Goal: make one target object answer what each hero can do.

Prototype target data:

- target type
- display name
- valid actions
- action prompt
- range
- target state

Initial actions:

- The Reel: Pull
- The Fly: Mark
- Lilly Loch: Bind

Verification:

- debug output shows correct action for selected hero
- no full combat or rescue logic yet

## Pass 5 - The Reel Pull v0

Goal: make the first playable verb real.

Scope:

- one line trace or overlap target selection
- one valid target
- one cast/hold/release Reel input loop
- one movement or target reaction
- one UI/debug prompt

Out of scope:

- The Fly implementation
- Lilly implementation
- full animation polish
- boss combat
- command mode

Verification:

- player can trigger Pull repeatedly
- invalid targets are ignored
- no crash when target is missing

## Pass 6 - Save Point

Goal: stop and review before expanding.

Deliverables:

- change summary
- verification output
- remaining blockers
- next recommended pass
