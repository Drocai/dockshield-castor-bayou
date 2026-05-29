# Duct Legendary Encounter Plan

## Status

Prototype v0 is scoped to a debug sighting in `M_Test_Targeting`.

`Duct` is the lore-level rubber duck anomaly tied to The Depth. In this version he is not catchable. The player can sight him, bait him, latch him, fight the reel tension, nearly catch him, and lose him. The trophy-case result is evidence, not ownership.

## Design Rules

- Duct is a legendary sighting, not normal loot.
- Duct must never enter a captured, inventory-owned, or completed-catch state in this prototype.
- The reward is evidence: sightings, near-catches, and Duct Tape traces.
- Attempts should use The Reel first, then later expand into bait, net, Fly tracking, Lilly ripple reads, weather, and boat positioning.
- The encounter should be exciting but non-blocking; no mission should require catching Duct.

## Prototype State Machine

```text
Hidden
  -> Sighted
  -> Lured
  -> Latched
  -> Near Catch
  -> Escaped
```

The `Escaped` state is intentional. It is not a failure bug.

## Current v0 Implementation Target

- Add a targetable `DSDuctLegendaryEncounterActor`.
- Place one debug Duct sighting in `M_Test_Targeting`.
- Allow The Reel to latch with LMB/E and build near-catch pressure with held `R`.
- Force Duct to slip away at peak progress or unsafe tension.
- Record:
  - Duct sightings
  - Duct near-catches
  - Duct Tape trace evidence
- Show Trophy Case status in the prototype HUD.
- Validate the loop headlessly with a commandlet smoke test.

## Later Expansion

- Rare map spawn manager with weather, moonlight, flood, and mission-stage weighting.
- Bait economy rows for Bobber Bouncer, Duct Tape Bait, rescue chum, toxic lure, and decoy raft.
- Fly sonar chain that keeps Duct visible longer.
- Lilly swamp-ripple bind read that reveals escape routes without catching him.
- Trophy case page with sighting logs, best near-catch percent, bait used, weather, and location.
- Audio/VFX pass: tiny yellow silhouette, golden wake, squeak ping, shimmer slip, tape residue.

## Acceptance Criteria

- `is_catchable()` always returns false.
- Duct can be sighted, latched, advanced to near-catch, and forced to slip away.
- The HUD says Duct remains `UNCAUGHT`.
- Duct evidence increments without awarding a captured item.
- The prototype validation runner includes the Duct smoke test.
