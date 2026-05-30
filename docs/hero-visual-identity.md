# DockShield Hero Visual Identity

This is the current prototype source of truth for separating the three selectable heroes in HUD language, boat styling, and gadget read.

## Theme Rules

| Hero | UI Theme | Boat Identity | Equipment Read |
| --- | --- | --- | --- |
| The Reel | Red, blue, and gold. Public, loud, rescue-forward. | `REEL RESCUE BOAT`, flashy rescue craft, `R // THE REEL` emblem. | Reel-gauntlet, rescue raft, hooks, buoys, bobber tools, visible brass/red-blue hardware. |
| The Fly | Blackwater black, dark teal, sonar cyan. Stealth and surveillance. | `FLY BLACKWATER SKIFF`, low-light covert boat, `FLY // COVERT` emblem. | Fly-line snare, sonar probe, tactical rod, net traps, toxin vials, quiet wet-black gear. |
| Lilly Loch | Pink, lime, mud camo, swamp green. Organic control and healing. | `LILLY SWAMP SKIFF`, pink/lime mud-camo swamp boat, `LILLY // SWAMP QUEEN` emblem. | Vine reel, root staff, healing moss, bloom nodes, living vine/tape/plant gear. |

## Prototype Acceptance

- Switching heroes must change the HUD color language immediately.
- Boat debug identity must expose hero label, emblem text, and loadout text for validation.
- `M_Test_Targeting` should contain a themed boat/gadget lineup for quick identity review: Reel rescue boat, Fly blackwater skiff, Lilly swamp skiff.
- Themed boats must keep the shared water/boarding/towing mechanics unchanged.
- Gadget names should read as character-specific even before final meshes are built.

## Production Notes

- Current boat and HUD theming is code-driven prototype styling.
- Final emblems, decals, boat meshes, and material layers still need authored production assets.
- Do not ship the current debug text as final logo art.
