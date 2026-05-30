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
- [x] Verify one target works for all three heroes

Current note: shared targeting v0 is implemented in C++ and validated against the targeting test map. The Reel, The Fly, and Lilly now use the shared target system through normal switching.

## Phase 3 - Heroes

- [x] The Reel placeholder character
- [x] The Reel pull/grapple ability v0
- [x] The Reel aim mode v0
- [x] The Reel cast/hold/release line tension loop v0
- [x] The Reel headless interaction smoke test
- [x] The Reel prototype boat tow action v0
- [x] The Reel prototype boat board/exit controls v0
- [x] The Reel prototype boat pilot controls v0
- [x] The Fly placeholder character
- [x] The Fly mark ability
- [x] Lilly placeholder character
- [x] Lilly bind ability
- [x] Character switching

Current note: The Reel continuous line v0 is implemented in C++, builds, and has headless commandlet coverage for one-shot and sustained tension behavior. The Fly sonar/mark v0 and Lilly swamp pulse/bind v0 are implemented as C++ pawns and can now be reached in normal play through `1`/`2`/`3`/`Tab` switching.

## Phase 4 - Mission Objects

- [ ] Civilian rescue target
- [ ] Toxic hazard
- [ ] Movable object
- [ ] Grapple point
- [ ] Enemy weak point
- [ ] Extraction point
- [x] Prototype water depth zone
- [x] Prototype water current/flood pressure logic
- [x] Prototype boat target
- [x] Prototype boat current drift logic
- [x] Prototype boat boarding/tow logic
- [x] Prototype boat piloting logic
- [x] Place water/boat prototype actors in `M_Test_Targeting`

## Phase 5 - Encounter

- [x] One mutation enemy
- [x] Deep Dock boss arena state actor v0
- [x] Deep Dock boss arena map dressing v0
- [x] Boss weak-point target placeholders v0
- [x] Boss weak-point phase with damage/state changes v0
- [x] Hook, Line & Sinker combo trigger v0
- [ ] One mini boss
- [ ] Hook, Line & Sinker finisher beat with readable VFX/SFX
- [ ] Win/fail conditions

## Phase 6 - Presentation

- [x] Private gameplay UI reference set
- [x] Private Copilot3D candidate asset audit
- [x] Reel-first character asset research and generation prompt pack
- [x] Basic prototype HUD
- [x] Current prototype audit and play guide
- [x] Test-map lighting/fog pass v0
- [x] Test-map visual style pass v1
- [x] Generated prototype bayou material set
- [ ] Basic SFX
- [ ] Basic VFX
- [x] Prototype Reel feedback VFX v0
- [x] Water depth HUD metrics v0
- [x] Flood pressure/current HUD metrics v0
- [x] Placed water/boat debug visualization v0
- [x] Prototype boat control HUD state v0
- [x] Fly sonar/mark prototype HUD path v0
- [x] Lilly bind prototype HUD path v0
- [x] Reel/Fly/Lilly switching HUD prompt v0
- [x] Reel/Fly/Lilly prototype visual identity kit v0
- [x] Import best Reel GLB as private static scale/lighting proxy
- [x] Add Reel proxy source checksum/size validation
- [x] Add current 20-point prototype audit
- [x] Add optional clean Git validation gate
- [x] Add prototype beta economy HUD v0
- [x] Add prototype achievements HUD v0
- [x] Add prototype settings panel v0
- [x] Add Deep Dock boss arena validation pass
- [x] Add Hook, Line & Sinker boss combo smoke test
- [x] Add Duct legendary encounter design note
- [x] Add Duct uncatchable target/state actor v0
- [x] Add Duct Trophy Case HUD evidence v0
- [x] Add Duct map pass and smoke validation
- [x] Add Bayou Mutation enemy combat actor, map pass, HUD status, and smoke validation
- [x] Add mutation combat feedback HUD/light/debug pulse hooks v0
- [ ] Review static Reel proxy in editor at gameplay camera distance
- [ ] Review Deep Dock boss arena lighting/readability in editor at gameplay camera distance
- [ ] Generate or source riggable The Reel A-pose body
- [ ] Generate or source separate Reel gear prop pack
- [ ] Decide Reel rigging route: MetaHuman, AccuRIG/Mixamo, or custom Blender rig
- [ ] Add Blender manifest for next Reel source package
- [ ] Add in-map screenshot/review camera for visual evidence
- [ ] Weather state v0
- [ ] Packaged Windows build
- [ ] OBS capture
- [ ] 60-90 second prototype video

Current note: the image pack is the private canon visual target for the intended movie/game look. The current Unreal build now has first-pass hero identity meshes plus generated bayou prototype materials, storm lighting/fog, toxic glow, wet dock planks, reed silhouettes, backdrop dressing, a private static Copilot3D Reel proxy for scale and lighting review, Bayou Mutation enemy combat v0, and a Duct legendary near-catch debug encounter. It still needs real MetaHuman/custom character assets and production environment art before it matches the cinematic reference.

## Phase 7 - Optimization And Upgrade Queue

- [ ] Add automated screenshot/review camera for the Reel proxy area
- [ ] Add weather state v0 that changes target visibility or interaction range
- [ ] Add basic cast/latch/reel/rescue SFX event hooks
- [ ] Add basic sonar pulse and root bind VFX placeholders
- [x] Add one mutation enemy with shared target weak-point support
- [x] Add mutation stagger/combo impact feedback hooks v0
- [ ] Replace mutation feedback hooks with first-pass Niagara/SFX assets and camera feedback
- [ ] Add extraction marker and success/fail counter
- [ ] Add objective-state HUD card for the current mission step
- [ ] Convert beta economy rewards into data-driven mission reward rows
- [ ] Add achievement save/load only after a real profile/save slot exists
- [ ] Wire settings panel to actual scalability/post-process settings after visual baseline is stable
- [x] Add boss arena state machine: dormant, exposed, enraged, defeated
- [x] Add boss weak-point damage and Hook, Line & Sinker combo trigger
- [ ] Add timed boss weak-point damage windows
- [ ] Add combo impact VFX/SFX and camera feedback
- [ ] Add rare Duct spawn manager with weather/mission weighting
- [ ] Add bait economy rows for Duct attempts without allowing capture
- [ ] Add Duct VFX/SFX: golden wake, squeak ping, slip shimmer, tape trace
- [ ] Add packaged Windows build validation script
- [ ] Add OBS capture checklist for a 60-90 second prototype reel
- [ ] Add asset-performance budget targets for triangles, materials, textures, and draw calls
