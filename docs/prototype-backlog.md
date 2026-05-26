# Prototype Backlog

## Phase 0 - Setup

- [x] Create planning/control repo
- [x] Add repo-level agent rules
- [x] Add Git ignore rules
- [x] Add Git LFS patterns
- [x] Add roadmap, risk register, decision log, and handoff docs
- [x] Add GitHub issue templates
- [x] Add asset register
- [x] Install Unreal Engine 5.7.x
- [x] Install Blender
- [x] Install OBS Studio
- [x] Detect Visual Studio Build Tools
- [x] Install Visual Studio C++ tools
- [x] Install Windows SDK
- [x] Resolve Windows Smart App Control block on Unreal C++ build-rules DLLs
- [x] Create GitHub remote
- [x] Push setup repo
- [x] Create Unreal project after install
- [x] Add private canon visual target pack

## Phase 1 - Unreal Project Foundation

- [x] Create Third Person Unreal project named `DockShield`
- [x] Confirm `.uproject` passes headless DataValidation
- [ ] Enable source control awareness
- [x] Create base folders in Unreal Content Browser
- [x] Create `M_Test_Targeting` graybox test map
- [x] Commit only intentional generated files

Suggested Unreal folders:

```text
Content/DockShield/
  Characters/
    Reel/
    Fly/
    Lilly/
  Core/
    Abilities/
    Targeting/
    Weather/
    CommandMode/
  Enemies/
  Environment/
    Bayou/
    Docks/
    Hazards/
  UI/
  Missions/
  Audio/
  VFX/
  Data/
```

## Phase 2 - Shared Targeting

- [x] Define target categories
- [x] Create shared target interface
- [x] Create prototype target reticle/HUD v0
- [x] Add interact prompt
- [x] Add target debug overlay
- [x] Add target lock, distance, and line tension HUD v0
- [x] Add initial grapple, civilian, and hazard placeholder targets
- [x] Add one-command prototype validation runner
- [ ] Verify one target works for all three heroes

Current note: shared targeting v0 is implemented in C++ and validated against the targeting test map. Full three-hero verification waits for The Fly and Lilly.

## Phase 3 - Heroes

- [x] The Reel placeholder character
- [x] The Reel pull/grapple ability v0
- [x] The Reel aim mode v0
- [x] The Reel headless interaction smoke test
- [x] The Reel prototype boat tow action v0
- [ ] The Fly placeholder character
- [ ] The Fly mark ability
- [ ] Lilly placeholder character
- [ ] Lilly bind ability
- [ ] Character switching

Current note: The Reel Pull v0 is implemented in C++, builds, and passes a headless commandlet smoke test. It still needs a manual in-editor feel test before closing the gameplay task.

## Phase 4 - Mission Objects

- [ ] Civilian rescue target
- [ ] Toxic hazard
- [ ] Movable object
- [ ] Grapple point
- [ ] Enemy weak point
- [ ] Extraction point
- [x] Prototype water depth zone
- [x] Prototype boat target
- [x] Prototype boat boarding/tow logic
- [x] Place water/boat prototype actors in `M_Test_Targeting`

## Phase 5 - Encounter

- [ ] One mutation enemy
- [ ] One mini boss
- [ ] Boss weak-point phase
- [ ] Hook, Line & Sinker finisher beat
- [ ] Win/fail conditions

## Phase 6 - Presentation

- [x] Private gameplay UI reference set
- [x] Basic prototype HUD
- [x] Current prototype audit and play guide
- [x] Test-map lighting/fog pass v0
- [ ] Basic SFX
- [ ] Basic VFX
- [x] Prototype Reel feedback VFX v0
- [x] Water depth HUD metrics v0
- [x] Placed water/boat debug visualization v0
- [ ] Weather state v0
- [ ] Packaged Windows build
- [ ] OBS capture
- [ ] 60-90 second prototype video

Current note: the image pack is the private canon visual target for the intended movie/game look. The current Unreal build is still a graybox mechanics prototype and does not recreate the full cinematic look yet.
