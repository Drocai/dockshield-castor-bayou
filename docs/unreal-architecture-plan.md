# Unreal Architecture Plan

## Purpose

This is the first-pass architecture target for the Unreal project after it exists. It is not generated code.

## Engine Target

Unreal Engine 5.7.x.

## Project Creation

Use:

- Third Person template
- Desktop target
- Blueprint-first
- C++ enabled only when needed
- Starter Content enabled if available

## Content Folder Target

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
  Data/
  Enemies/
  Environment/
    Bayou/
    Docks/
    Hazards/
  Maps/
  Missions/
  UI/
  VFX/
  Audio/
```

## First Blueprint/Class Names

Use these names only after the project exists:

| Type | Name | Purpose |
| --- | --- | --- |
| Game Mode | `BP_DockShieldGameMode` | vertical slice rules |
| Player Controller | `BP_DockShieldPlayerController` | input, targeting, command mode |
| Base Hero | `BP_HeroBase` | shared movement and ability hooks |
| Reel Hero | `BP_Hero_Reel` | Pull prototype |
| Fly Hero | `BP_Hero_Fly` | Mark prototype |
| Lilly Hero | `BP_Hero_Lilly` | Bind prototype |
| Target Interface | `BPI_DockShieldTargetable` | target actions by hero |
| Target Component | `BP_TargetableComponent` | reusable target metadata |
| Grapple Point | `BP_Target_GrapplePoint` | Pull target |
| Civilian | `BP_Target_Civilian` | rescue target |
| Hazard | `BP_Target_ToxicHazard` | hazard target |
| Enemy | `BP_Enemy_MutantBasic` | first enemy |
| Boss | `BP_Boss_MutatedBass` | first mini boss |
| Command UI | `WBP_CommandMode` | slow-time command overlay |
| HUD | `WBP_DockShieldHUD` | hero, target, weather, objective |

## Shared Target Data

Each target should expose:

- target type
- valid hero actions
- display name
- interaction prompt
- range requirement
- target state
- weak point flag
- rescue flag
- hazard flag

## First Implementation Rule

Do not build all target types first.

Build one target object that supports:

- The Reel Pull
- The Fly Mark as a debug tag
- Lilly Bind as a debug lock

Then split into specialized targets only after the loop works.

## Command Mode v0

Command Mode v0 should:

- slow time
- show selectable heroes
- show valid targets
- preview one queued action
- execute one action

It should not:

- become full turn-based combat
- manage inventory
- manage party AI deeply
- support complex multi-action timelines

