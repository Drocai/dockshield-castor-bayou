# The Reel Asset Generation Prompts

Use these when generating new 3D assets for DockShield. The goal is **playable asset source**, not key art.

Preferred export order:

```text
FBX if rigged or rig-ready
GLB if textured static/source model
OBJ only if clean mesh plus textures
STL only for sculpt reference
```

## Prompt Rules

- Use neutral A-pose or T-pose for characters.
- Ask for full body, centered, no crop.
- Ask for separate props when possible.
- Ask for PBR texture maps.
- Avoid cinematic scenes, action poses, water splashes, victims, text, logos, and background sets.
- Do not ask the generator to bake storm lighting into the character texture. Unreal handles lighting.

## The Reel - Main Riggable Character

```text
Create a game-ready full-body 3D character model for a cinematic Unreal Engine third-person action game.

Character: "The Reel", a public rescue superhero and tactical fishing-gear bruiser for a dark storm-bayou rescue game called DockShield.

Pose and topology requirements:
- neutral A-pose, arms slightly away from the body, palms relaxed
- full body visible, centered, standing upright
- humanoid adult male proportions, strong rescue-worker build
- clean readable face, hands, boots, belt, shoulders, and back silhouette
- animation-friendly posture, no action pose, no crouch, no twist
- separate or clearly removable hard-surface gear where possible

Visual design:
- wet dark blue and black rescue-fishing tactical outfit
- broad weathered angler hat with subtle lure/bobber details
- heavy waterproof vest, worn leather straps, utility belt, rescue pouches
- red and brass reel hardware accents
- scuffed metal, wet fabric, worn leather, mud stains, rain sheen
- one high-torque tactical reel gauntlet on the forearm
- rope, hooks, fishing lures, rescue floats, and compact line gear attached logically
- heroic, practical, grounded, not fantasy armor, not sci-fi space armor

Material requirements:
- PBR materials
- base color, normal, roughness, metallic/ORM maps if available
- wet cloth, worn leather, painted metal, brass, rubber, mud
- subtle toxic green indicator lights only as small accents

Output:
- GLB or FBX
- include textures
- no background
- no text
- no logo
- no other characters
```

### Negative Prompt

```text
no action pose, no dynamic fishing cast, no water splash, no scene, no dock, no victim, no monster, no text, no logo, no extra limbs, no fused hands, no weapon replacing hands, no oversized fantasy armor, no space suit, no cartoon style, no low-poly toy, no statue base, no merged background, no dramatic baked lighting
```

## The Reel - Front/Side/Back Turnaround Image

Use this if the generator makes images better than riggable 3D meshes.

```text
Create a character turnaround reference sheet for The Reel from DockShield.

Show front view, 3/4 view, side view, and back view on one clean neutral sheet.
Character is a public rescue superhero using tactical fishing gear.
Strong adult male rescue-worker build, broad angler hat, wet dark blue/black waterproof outfit, red/brass reel accents, heavy tactical belt, pouches, bobbers, lures, rope, rescue gear, one tactical reel gauntlet.

Neutral standing pose for all views.
No action scene.
No background environment.
No text except small view labels if unavoidable.
Readable clothing layers, back silhouette, gauntlet, boots, belt, hat, and gear placement.
Dark cinematic game character concept, realistic, gritty, wet, worn, practical.
```

## Tactical Reel Gauntlet Prop

```text
Create a game-ready 3D hard-surface prop: The Reel's high-torque tactical fishing reel gauntlet.

Object only, no character, no hand, no scene.

Design:
- forearm-mounted rescue reel device
- compact heavy-duty fishing reel mechanism
- red painted metal, brass rings, black rubber grips, steel cable spool
- small hook anchor, shock absorber housing, line guide, crank details
- scuffed wet metal, mud, scratches, practical rescue equipment
- readable silhouette for a third-person game

Output GLB or FBX with PBR textures.
No text, no logos, no background.
```

## Rod / Line Launcher Prop

```text
Create a game-ready 3D prop: tactical fishing rod and line launcher for The Reel.

Object only, no character, no hand, no scene.

Design:
- rugged fishing rod crossed with rescue grappling line launcher
- reinforced black carbon rod, red/brass reel details, steel hook tip, cable guide rings
- rescue-tool design, not a gun, not fantasy weapon
- wet worn materials, mud stains, scratches
- practical scale for a human third-person character

Output GLB or FBX with PBR textures.
No text, no logos, no background.
```

## Rescue Gear Prop Pack

```text
Create a compact game-ready 3D prop pack for The Reel's rescue loadout.

Objects only, no character, no scene.

Include:
- bobber bomb
- life ring launcher module
- inflatable rescue raft pack
- buoy line spool
- hook and lure set
- waterproof utility bait canister

Style:
- dark bayou rescue equipment
- red, black, brass, worn rubber, scratched metal
- wet, muddy, practical, cinematic, realistic
- readable shapes for third-person gameplay

Output separate objects if possible.
Output GLB or FBX with PBR textures.
No text, no logos, no background.
```

## Best Current Ask For More Generated Assets

If generating only one thing next, generate this:

```text
The Reel main riggable A-pose character as GLB or FBX, with PBR textures, no scene, no action pose, no background, no text, no logos, separate gear where possible.
```
