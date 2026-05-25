# DockShield Core Gameplay Bible

## Game Identity

**DockShield** is a tactical supernatural rescue-action game where mutated heroes defend contaminated waterways, storm zones, marinas, lakes, docks, bayous, and hidden spill sites.

The player controls members of **The Depth**, a squad deployed through the DockShield operation system.

## Core Pitch

```text
Weather-driven tactical rescue combat where fishing logic becomes superhero equipment.
```

## World Premise

After contaminated spill events begin mutating wildlife, waterways, civilians, and responders, DockShield is activated as a covert waterway defense initiative.

DockShield is:

- the game title
- the in-world tactical deployment system
- the covert operation program

## Campaign Structure

```text
DockShield
  Campaign 1: The Depth
    Operation 01: Castor Bayou Incident
    Operation 02: Toxic Tide
    Operation 03: Deadwater Signal
    Operation 04: Stormline Breach
```

## Core Pillars

1. Dynamic weather changes the mission.
2. Water is both map and weapon.
3. Every hero uses fishing logic differently.
4. Missions happen at lakes, docks, ponds, rivers, marinas, bayous, and spill sites.
5. The player can switch between field action and overhead tactical command.

## The Three Prototype Verbs

The first playable should prove only three verbs:

| Hero | Verb | Prototype Meaning |
| --- | --- | --- |
| The Reel | Pull | grapple, rescue, yank enemies or objects |
| The Fly | Mark | reveal, tag, weaken, guide team actions |
| Lilly Loch | Bind | snare, restrain, control terrain or hazards |

## Shared Targeting System

Shared targeting is the heart of the prototype.

Target types:

- Grapple Point
- Enemy
- Civilian
- Object
- Weak Point
- Hazard
- Rescue Target

Each target should expose character-specific actions.

| Target Type | The Reel | The Fly | Lilly Loch |
| --- | --- | --- | --- |
| Grapple Point | swing/pull | anchor trap | vine swing |
| Enemy | hook/yank | mark/snare | bind/crush |
| Civilian | rescue reel | guide/extract | vine lift |
| Object | pull/throw | sabotage | root-shift |
| Weak Point | expose | mark | rupture |
| Hazard | redirect | avoid/tag | absorb/control |

## Weather System

Weather is not decoration. It changes gameplay.

First prototype weather should be one state only:

- heavy rain, or
- fog

Do not build the full weather matrix until the shared targeting loop works.

Future states:

- heavy rain
- fog
- lightning
- wind
- flood surge
- toxic rain
- heat haze
- hurricane band

## Squad Command Mode

Command Mode v0 should be a limited slow-time overlay:

- select hero
- choose target
- preview range
- queue one action
- execute

Do not build full turn-based tactics in the first slice.

## Vertical Slice Goal

The first slice should be playable in 8-12 minutes and prove that the DockShield idea works in the player's hands.

It should not try to prove the whole franchise.

