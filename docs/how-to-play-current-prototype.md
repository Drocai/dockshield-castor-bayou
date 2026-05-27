# How To Play The Current Prototype

## What This Is

This is a mechanics test for The Reel, not the full DockShield look.

You should expect:

- graybox test arena
- placeholder Unreal mannequin
- simple HUD and reticle
- target lock, distance, line tension state, snap count, and action counters
- debug target visuals
- temporary debug Reel cast/reel feedback
- first-pass prototype visual identity kits for The Reel and The Fly
- one grapple target
- one civilian rescue target
- one toxic hazard placeholder
- C++ foundation for water depth, current/flood pressure, prototype boat drifting/towing, boat boarding, and boat piloting
- C++ foundation for The Fly sonar pulse and mark state

You should not expect:

- the canon Castor Bayou visuals yet
- final character models
- storm/rain/water art
- Lilly, command mode, boss combat, or full mission flow
- final rescue raft art, water simulation, or finished boat handling
- final MetaHuman/custom hero models or photoreal bayou production art

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
- Cycle hero: `Tab`
- Cast / detach Reel line: left mouse button or `E`
- Reel attached line: hold `R`
- Ease line tension: release `R`
- Board / Exit prototype boat: `F` or `B`
- Pilot boarded prototype boat: `WASD`

The Fly controls after pressing `2` or `Tab`:

- Sonar pulse: `Q`
- Mark recon target: left mouse button or `E`
- Aim recon cone: hold right mouse button

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
20. Press `1` or `Tab` to return to The Reel.

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
- `1`, `2`, and `Tab` switch between The Reel and The Fly.
- The HUD changes to the active hero.
- The Reel and The Fly have visibly different prototype silhouettes.
- Repeating interactions does not crash or break the map.

## Agent Validation

Codex can validate the current prototype without manual PIE:

```powershell
powershell -ExecutionPolicy Bypass -File scripts\unreal\validate-prototype.ps1
```

For the narrow Reel action smoke test only:

```powershell
& 'C:\Program Files\Epic Games\UE_5.7\Engine\Binaries\Win64\UnrealEditor-Cmd.exe' 'C:\Users\djmc1\Documents\Codex\2026-05-25\hello-i-have-this-new-project\DockShield\DockShield.uproject' -run=pythonscript -script='C:\Users\djmc1\Documents\Codex\2026-05-25\hello-i-have-this-new-project\scripts\unreal\validate_reel_interaction_smoke.py' -unattended -nop4 -nosplash
```

For the narrow Fly recon smoke test only:

```powershell
& 'C:\Program Files\Epic Games\UE_5.7\Engine\Binaries\Win64\UnrealEditor-Cmd.exe' 'C:\Users\djmc1\Documents\Codex\2026-05-25\hello-i-have-this-new-project\DockShield\DockShield.uproject' -run=pythonscript -script='C:\Users\djmc1\Documents\Codex\2026-05-25\hello-i-have-this-new-project\scripts\unreal\validate_fly_recon_smoke.py' -unattended -nop4 -nosplash
```
