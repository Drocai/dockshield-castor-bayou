# Phase 01 Unreal Foundation Tasks

Start only after Unreal Engine 5.7.x is installed.

## Intake

- [ ] Open repo in Codex
- [ ] Run `git status --short`
- [ ] Run `scripts/check-toolchain.ps1`
- [ ] Confirm Unreal 5.7.x is installed
- [ ] Create Third Person project named `DockShield`
- [ ] Close Unreal Editor before first git review
- [ ] Inspect generated files

## First Commit

- [ ] Confirm `.uproject` exists
- [ ] Confirm `Config/` exists
- [ ] Confirm generated caches are ignored
- [ ] Confirm `.uasset` and `.umap` files are tracked by LFS
- [ ] Commit initial Unreal project

## Content Organization

- [ ] Create `Content/DockShield/` folders in Unreal Editor
- [ ] Save and close editor
- [ ] Review `git status --short`
- [ ] Commit folder organization if real assets were generated

## First Test Map

- [ ] Create `M_Test_Targeting`
- [ ] Add third-person player start
- [ ] Add one target object
- [ ] Add one debug prompt
- [ ] Save and commit

## First Implementation Passes

Use `docs/unreal-first-implementation-plan.md` as the pass sequence.

- [ ] Pass 1 - Project Intake
- [ ] Pass 2 - Content Organization
- [ ] Pass 3 - Targeting Test Map
- [ ] Pass 4 - Shared Targetable Interface
- [ ] Pass 5 - The Reel Pull v0
- [ ] Pass 6 - Save Point
