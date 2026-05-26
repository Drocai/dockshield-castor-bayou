# How To Play The Current Prototype

## What This Is

This is a mechanics test for The Reel, not the full DockShield look.

You should expect:

- graybox test arena
- placeholder Unreal mannequin
- simple HUD and reticle
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
3. Hold right mouse button to aim, then press left mouse button or `E`.
4. Face `Target_CivilianRescue_Debug`.
5. Confirm the reticle/prompt changes.
6. Hold right mouse button to aim, then press left mouse button or `E`.
7. Repeat both interactions and watch for crashes, stuck movement, bad prompts, or missing HUD updates.

## Pass Criteria

- The map opens.
- The player can move and look.
- The HUD appears.
- Valid target reticle/prompt appears.
- `E` triggers grapple/rescue behavior.
- Repeating interactions does not crash or break the map.
