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
- [ ] Install Blender
- [x] Detect Visual Studio Build Tools
- [x] Create GitHub remote
- [x] Push setup repo
- [x] Create Unreal project after install

## Phase 1 - Unreal Project Foundation

- [ ] Create Third Person Unreal project named `DockShield`
- [x] Create Third Person Unreal project named `DockShield`
- [x] Confirm `.uproject` passes headless DataValidation
- [ ] Enable source control awareness
- [ ] Create base folders in Unreal Content Browser
- [ ] Commit only intentional generated files

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

- [ ] Define target categories
- [ ] Create shared target interface
- [ ] Create target reticle
- [ ] Add interact prompt
- [ ] Add target debug overlay
- [ ] Verify one target works for all three heroes

## Phase 3 - Heroes

- [ ] The Reel placeholder character
- [ ] The Reel pull/grapple ability
- [ ] The Fly placeholder character
- [ ] The Fly mark ability
- [ ] Lilly placeholder character
- [ ] Lilly bind ability
- [ ] Character switching

## Phase 4 - Mission Objects

- [ ] Civilian rescue target
- [ ] Toxic hazard
- [ ] Movable object
- [ ] Grapple point
- [ ] Enemy weak point
- [ ] Extraction point

## Phase 5 - Encounter

- [ ] One mutation enemy
- [ ] One mini boss
- [ ] Boss weak-point phase
- [ ] Hook, Line & Sinker finisher beat
- [ ] Win/fail conditions

## Phase 6 - Presentation

- [ ] Basic HUD
- [ ] Basic SFX
- [ ] Basic VFX
- [ ] Weather state v0
- [ ] Packaged Windows build
- [ ] OBS capture
- [ ] 60-90 second prototype video
