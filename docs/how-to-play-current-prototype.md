# How To Play The Current Prototype

## What This Is

This is a mechanics test for The Reel, not the full DockShield look.

You should expect:

- graybox test arena
- placeholder Unreal mannequin
- simple HUD and reticle
- target lock, distance, line tension, and action counters
- debug target visuals
- one grapple target
- one civilian rescue target
- one toxic hazard placeholder

You should not expect:

- the canon Castor Bayou visuals yet
- final character models
- storm/rain/water art
- The Fly, Lilly, command mode, boss combat, or full mission flow

## Launch

1. Open `DockShield/DockShield.uproject`.
2. Open `/Game/DockShield/Maps/M_Test_Targeting`.
3. Press Play.

## Controls

- Move: `WASD`
- Look: mouse
- Jump: `Space`
- Aim lock: hold right mouse button
- Reel Pull / Rescue: left mouse button or `E`

## Test

1. Face `Target_GrapplePull_Debug`.
2. Confirm the reticle/prompt changes.
3. Confirm `TARGET LOCK`, distance, and `LINE TENSION` appear in the lower-left HUD.
4. Hold right mouse button to aim, then press left mouse button or `E`.
5. Face `Target_CivilianRescue_Debug`.
6. Confirm the reticle/prompt changes.
7. Hold right mouse button to aim, then press left mouse button or `E`.
8. Repeat both interactions and watch for crashes, stuck movement, bad prompts, or missing HUD updates.

## Pass Criteria

- The map opens.
- The player can move and look.
- The HUD appears.
- Valid target reticle/prompt appears.
- Left mouse button or `E` triggers grapple/rescue behavior.
- Lower-left HUD updates line tension, last result, pull count, and rescue count.
- Repeating interactions does not crash or break the map.

## Agent Smoke Test

Codex can validate the core Reel action without manual PIE:

```powershell
& 'C:\Program Files\Epic Games\UE_5.7\Engine\Binaries\Win64\UnrealEditor-Cmd.exe' 'C:\Users\djmc1\Documents\Codex\2026-05-25\hello-i-have-this-new-project\DockShield\DockShield.uproject' -run=pythonscript -script='C:\Users\djmc1\Documents\Codex\2026-05-25\hello-i-have-this-new-project\scripts\unreal\validate_reel_interaction_smoke.py' -unattended -nop4 -nosplash
```
