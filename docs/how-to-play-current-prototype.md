# How To Play The Current Prototype

## What This Is

This is a styled mechanics test for the shared hero loop, not the full DockShield look.

You should expect:

- styled graybox test arena with prototype bayou materials, storm lighting/fog, toxic glow, wet dock planks, reeds, and backdrop dressing
- placeholder Unreal mannequin
- simple HUD and reticle
- target lock, distance, line tension state, snap count, and action counters
- debug target visuals
- temporary debug Reel cast/reel feedback
- first-pass prototype visual identity kits for The Reel and The Fly
- first-pass prototype visual identity kit for Lilly Loch
- a private static Reel proxy placed in the map for scale, silhouette, and storm-lighting review
- one grapple target
- one civilian rescue target
- one toxic hazard placeholder
- C++ foundation for water depth, current/flood pressure, prototype boat drifting/towing, boat boarding, and boat piloting
- C++ foundation for The Fly sonar pulse and mark state
- C++ foundation for Lilly swamp pulse and bind state
- prototype beta economy, achievements, and settings HUD panels
- prototype Deep Dock boss arena dressing with a toxic core and weak-point targets
- prototype Hook, Line & Sinker weak-point combo damage: Reel expose, Fly mark, Lilly bind

You should not expect:

- the final canon Castor Bayou visuals yet
- final character models
- final storm/rain/water art
- command mode, boss combat, or full mission flow
- real boss AI, timed damage windows, cinematic finisher VFX, or win/fail encounter rules
- final rescue raft art, water simulation, or finished boat handling
- final MetaHuman/custom hero models or photoreal bayou production art
- the static Reel proxy to be playable, animated, licensed for public use, or final

## Launch

1. Open `DockShield/DockShield.uproject`.
2. Open `/Game/DockShield/Maps/M_Test_Targeting`.
3. Press Play.

## Controls

- Move: `WASD`
- Look: mouse
- Jump: `Space`
- Aim lock: hold right mouse button
- Switch to The Reel: `1`
- Switch to The Fly: `2`
- Switch to Lilly Loch: `3`
- Cycle hero: `Tab`
- Cast / detach Reel line: left mouse button or `E`
- Reel attached line: hold `R`
- Ease line tension: release `R`
- Board / Exit prototype boat: `F` or `B`
- Pilot boarded prototype boat: `WASD`
- Open/close prototype settings panel: `P` or `Esc`
- HUD scale: `[` and `]`
- Prototype gamma value: `-` and `=`
- Cycle prototype visual quality label: `O`

The Fly controls after pressing `2` or `Tab`:

- Sonar pulse: `Q`
- Mark recon target: left mouse button or `E`
- Aim recon cone: hold right mouse button

Lilly controls after pressing `3` or `Tab`:

- Swamp pulse: `Q`
- Bind target: left mouse button or `E`
- Aim root focus: hold right mouse button

## Test

1. Face `Target_GrapplePull_Debug`.
2. Confirm the reticle/prompt changes.
3. Confirm `TARGET LOCK`, distance, and `LINE TENSION` appear in the lower-left HUD.
4. Hold right mouse button to aim, then press left mouse button or `E` to cast.
5. Hold `R` to reel toward the target, then release `R` to ease tension.
6. Face `Target_CivilianRescue_Debug`.
7. Confirm the reticle/prompt changes.
8. Press left mouse button or `E` to cast, then hold `R` until the rescue completes.
9. Repeat both interactions and watch for crashes, stuck movement, bad prompts, or missing HUD updates.
10. Face `DS_Boat_Prototype_Tow` near the water test area.
11. Press left mouse button or `E` to cast, then hold `R` to tow.
12. Confirm the boat moves toward you, the Reel feedback line appears, and the boat tow count increases.
13. Watch the lower-left HUD for `PRESSURE` and `CURRENT` while standing in water or aiming at the boat.
14. Move close to `DS_Boat_Prototype_Tow`, then press `F` or `B`.
15. Confirm the HUD switches to boat control, then use `WASD` to pilot the boat.
16. Press `F` or `B` again and confirm the player exits back to foot movement.
17. Press `2` or `Tab` to switch to The Fly.
18. Press `Q` and confirm sonar debug feedback appears.
19. Face a target and press left mouse button or `E` to mark it.
20. Press `3` or `Tab` to switch to Lilly.
21. Press `Q` and confirm swamp pulse debug feedback appears.
22. Face a bindable target and press left mouse button or `E` to bind it.
23. Press `1` or `Tab` to return to The Reel.
24. Find `DS_Reel_Proxy_Copilot3D_Static` in the proxy review area.
25. Compare the static proxy against the playable pawn for scale, silhouette, wet material readability, toxic accent visibility, and storm/fog contrast.
26. Move toward the `DS_DeepDock_*` arena area and confirm the toxic core, red warning lights, and three weak-point targets are readable.
27. Complete one rescue, one Fly mark, and one Lilly bind; confirm credits, XP, samples, and achievement text update in the HUD.
28. Press `P` or `Esc` and confirm the settings panel opens; use `[`/`]`, `-`/`=`, and `O` to review readability controls.
29. At a Deep Dock weak point, use The Reel to expose it.
30. Switch to The Fly and mark the same weak point.
31. Switch to Lilly and bind the same weak point.
32. Confirm the boss HUD status advances and the combo reward appears.
33. Repeat the combo on all three weak points to push the boss arena into defeated state.

## Pass Criteria

- The map opens.
- The player can move and look.
- The HUD appears.
- Valid target reticle/prompt appears.
- Left mouse button or `E` casts and detaches the Reel line.
- Holding `R` pulls over time and releasing `R` eases line tension.
- Lower-left HUD updates line state, tension, snap count, last result, pull count, and rescue count.
- Lower-left HUD includes water depth, movement scale, and boat tow count when water/boat actors are present.
- Lower-left HUD reports flood pressure and current speed.
- Boat HUD state appears when the player targets, boards, pilots, and exits the prototype boat.
- A floating prototype boat can drift under water current and is harder to tow/pilot under higher flood pressure.
- `1`, `2`, `3`, and `Tab` switch between The Reel, The Fly, and Lilly Loch.
- The HUD changes to the active hero.
- The Reel, The Fly, and Lilly Loch have visibly different prototype silhouettes.
- The static Reel proxy is visible in-map but does not replace the playable pawn.
- The test map has visible wet dock surfaces, toxic green water/glow, reed silhouettes, storm/fog lighting, and neon/backdrop markers.
- Beta economy, achievement, boss arena, and settings HUD panels appear without hiding the core targeting prompt.
- Deep Dock boss core and weak-point targets are present in the map and remain targetable through the shared target system.
- Hook, Line & Sinker damage applies once per weak point after Reel exposure, Fly mark, and Lilly bind are all active.
- The boss arena advances through exposed/enraged/defeated states after repeated weak-point combos.
- Repeating interactions does not crash or break the map.

## Agent Validation

Codex can validate the current prototype without manual PIE:

```powershell
powershell -ExecutionPolicy Bypass -File scripts\unreal\validate-prototype.ps1
```

After a checkpoint is committed, require a clean Git worktree too:

```powershell
powershell -ExecutionPolicy Bypass -File scripts\unreal\validate-prototype.ps1 -RequireCleanGit
```

To reapply the current visual style pass:

```powershell
& 'C:\Program Files\Epic Games\UE_5.7\Engine\Binaries\Win64\UnrealEditor-Cmd.exe' 'C:\Users\djmc1\Documents\Codex\2026-05-25\hello-i-have-this-new-project\DockShield\DockShield.uproject' -run=pythonscript -script='C:\Users\djmc1\Documents\Codex\2026-05-25\hello-i-have-this-new-project\scripts\unreal\apply_visual_style_pass.py' -unattended -nop4 -nosplash
```

To reapply the Deep Dock boss arena pass:

```powershell
& 'C:\Program Files\Epic Games\UE_5.7\Engine\Binaries\Win64\UnrealEditor-Cmd.exe' 'C:\Users\djmc1\Documents\Codex\2026-05-25\hello-i-have-this-new-project\DockShield\DockShield.uproject' -run=pythonscript -script='C:\Users\djmc1\Documents\Codex\2026-05-25\hello-i-have-this-new-project\scripts\unreal\apply_deep_dock_boss_arena_pass.py' -unattended -nop4 -nosplash
```

For the narrow Reel action smoke test only:

```powershell
& 'C:\Program Files\Epic Games\UE_5.7\Engine\Binaries\Win64\UnrealEditor-Cmd.exe' 'C:\Users\djmc1\Documents\Codex\2026-05-25\hello-i-have-this-new-project\DockShield\DockShield.uproject' -run=pythonscript -script='C:\Users\djmc1\Documents\Codex\2026-05-25\hello-i-have-this-new-project\scripts\unreal\validate_reel_interaction_smoke.py' -unattended -nop4 -nosplash
```

For the narrow Fly recon smoke test only:

```powershell
& 'C:\Program Files\Epic Games\UE_5.7\Engine\Binaries\Win64\UnrealEditor-Cmd.exe' 'C:\Users\djmc1\Documents\Codex\2026-05-25\hello-i-have-this-new-project\DockShield\DockShield.uproject' -run=pythonscript -script='C:\Users\djmc1\Documents\Codex\2026-05-25\hello-i-have-this-new-project\scripts\unreal\validate_fly_recon_smoke.py' -unattended -nop4 -nosplash
```

For the narrow Lilly bind smoke test only:

```powershell
& 'C:\Program Files\Epic Games\UE_5.7\Engine\Binaries\Win64\UnrealEditor-Cmd.exe' 'C:\Users\djmc1\Documents\Codex\2026-05-25\hello-i-have-this-new-project\DockShield\DockShield.uproject' -run=pythonscript -script='C:\Users\djmc1\Documents\Codex\2026-05-25\hello-i-have-this-new-project\scripts\unreal\validate_lilly_bind_smoke.py' -unattended -nop4 -nosplash
```
